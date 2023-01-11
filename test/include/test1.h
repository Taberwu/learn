/**
****************************************************************************************
 * @FilePath: test1.h
 * @Author: Taber.wu
 * @Date: 2022-11-29 14:29:43
 * @LastEditors: 
 * @LastEditTime: 2022-11-29 14:29:43
 * @Copyright: 2022 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************
*/
#ifndef TEST_1_H_
#define TEST_1_H_


#include <array>
#include <condition_variable>
#include <chrono>
#include <cmath>
#include <deque>
#include <Math.hpp>
#include <map>
#include <memory>
#include <mutex>
#include <vector>
#include <iostream>
#include <random>
#include <shared_mutex>
#include <set>
#include <thread>
#include <utility>
#include <unistd.h>
#include <variant>
#include <vector>

#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Geometry>

#include <pugixml/pugixml.hpp>

using Vec2D = std::array<double, 2>;
using Vec3D = std::array<double, 3>;
using Line3D = std::vector<Vec3D>;

struct XmlNode
{
    pugi::xml_node xml_node;
};

struct RoadGeometry :public XmlNode{
    RoadGeometry(double s, double x, double y, double hdg, double length):s0(s), x0(x), y0(y), hdg0(hdg), length0(length)
    {}
    virtual ~RoadGeometry() = default;
    virtual std::unique_ptr<RoadGeometry> clone() const = 0;
    virtual Vec2D get_xy(double s) const = 0;
    virtual Vec2D get_grad(double s) const = 0;
    virtual std::set<double> approximate_linear(double eps) const = 0;

    double       s0 = 0;
    double       x0 = 0;
    double       y0 = 0;
    double       hdg0 = 0;
    double       length0 = 0;
};

struct Line : public RoadGeometry
{
    Line(double s0, double x0, double y0, double hdg0, double length):RoadGeometry(s0, x0, y0, hdg0, length)
    {}
    std::unique_ptr<RoadGeometry> clone() const override{
        return std::make_unique<Line>(*this);
    }

    Vec2D get_xy(double s) const override
    {
        const double x = (std::cos(hdg0) * (s - s0)) + x0;
        const double y = (std::sin(hdg0) * (s - s0)) + y0;
        return Vec2D{x, y};
    }
    Vec2D get_grad(double s) const override
    { 
        return {{std::cos(hdg0), std::sin(hdg0)}}; 
    }


    std::set<double> approximate_linear(double eps) const override
    { 
        return {s0, s0 + length0}; 
        
    }
};

struct RefLine
{
    RefLine(std::string road_id, double length):road_id_(road_id), length_(length)
    {}
    std::set<const RoadGeometry*> get_geometries() const{
        std::set<const RoadGeometry*> geometries;
        for(const auto &s0_geometry : this->s0_to_geometry){
            geometries.insert(s0_geometry.second.get());
        }
        return geometries;
    }
    double get_geometry_s0(const double s) const{
        if(this->s0_to_geometry.empty())
            return NAN;
        auto target_geom_iter = this->s0_to_geometry.upper_bound(s);
        if(target_geom_iter != s0_to_geometry.begin())
            target_geom_iter--;
        return target_geom_iter->first;
    }
    const RoadGeometry* get_geometry(const double s) const{
        auto gemo_s0 = this->get_geometry_s0(s);
        if(std::isnan(gemo_s0)) 
            return nullptr;
        return this->s0_to_geometry.at(gemo_s0).get();
    }
    RoadGeometry* get_geometry(const double s){
        RoadGeometry* road_geometry = const_cast<RoadGeometry*>(static_cast<const RefLine&>(*this).get_geometry(s));
        return road_geometry;
    }

    Vec3D get_xyz(const double s) const
    {
        const RoadGeometry* geom = this->get_geometry(s);
        Vec2D pt_xy{0, 0};
        if (geom)
            pt_xy = geom->get_xy(s);
        return Vec3D{pt_xy[0], pt_xy[1], 1.0};
    }

    Vec3D get_grad(const double s) const
    {
        const RoadGeometry* geom = this->get_geometry(s);
        Vec2D d_xy{0, 0};
        if (geom)
            d_xy = geom->get_grad(s);
        return Vec3D{d_xy[0], d_xy[1], 1.0};
    }

    double match(const double x, const double y) const
    {
        std::function<double(double)> f_dist = [&](const double s)
        {
            const Vec3D pt = this->get_xyz(s);
            return test::euclDistance(Vec2D{pt[0], pt[1]}, {x, y});
        };
        return test::golden_section_search<double>(f_dist, 0.0, this->length_, 1e-2);
    }

    Line3D get_line(const double s_start, const double s_end, const double eps) const
    {
        std::set<double> s_vals = this->approximate_linear(eps, s_start, s_end);
        Line3D out_line;
        for (const double& s : s_vals)
            out_line.push_back(this->get_xyz(s));
        return out_line;
    }

    std::set<double> approximate_linear(const double eps, const double s_start, const double s_end) const
    {
        if ((s_start == s_end) || this->s0_to_geometry.empty())
            return {};

        auto s_end_geom_iter = this->s0_to_geometry.lower_bound(s_end);
        auto s_start_geom_iter = this->s0_to_geometry.upper_bound(s_start);
        if (s_start_geom_iter != s0_to_geometry.begin())
            s_start_geom_iter--;

        std::vector<double> s_vals{s_start};
        for (auto s0_geom_iter = s_start_geom_iter; s0_geom_iter != s_end_geom_iter; s0_geom_iter++)
        {
            const std::set<double> s_vals_geom = s0_geom_iter->second->approximate_linear(eps);
            if (s_vals_geom.size() < 2)
                throw std::runtime_error("expected at least two sample points");
            for (const double& s : s_vals_geom)
            {
                if (s > s_start && s < s_end)
                    s_vals.push_back(s);
            }
            if (std::next(s0_geom_iter) != s_end_geom_iter)
                s_vals.pop_back();
        }

        // std::set<double> s_vals_elevation = this->elevation_profile.approximate_linear(eps, s_start, s_end);
        // for (const double& s : s_vals_elevation)
        // {
        //     if (s > s_start && s < s_end)
        //         s_vals.push_back(s);
        // }

        s_vals.push_back(s_end);

        std::set<double> s_vals_set(s_vals.begin(), s_vals.end());
        return s_vals_set;
    }



    std::string road_id_ = "";
    double      length_ = 0;
    std::map<double, std::unique_ptr<RoadGeometry>> s0_to_geometry;
};


#endif