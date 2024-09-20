"""! @brief [description du fichier]
 @file dfs.hpp
 @section libs Librairies/Modules
 @section authors Auteur(s)
  - Créé par [Name] [Last Name] le 2024/9/19 .
"""
/****************************************************************************************
 * @FilePath: dfs.hpp
 * @Author: Taber.wu
 * @Date: 2024-09-09 16:07:55
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2024-09-12 11:22:00
 * @Copyright: 2024 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************/
#ifndef __DEPFISRTSEAR_HPP_
#define __DEPFISRTSEAR_HPP_

#include <algorithm>
#include "vector"
#include "string"
#include "memory"
#include "map"

namespace dfs{
    class Lane{
    public:
        std::string id;
        std::vector<std::shared_ptr<Lane>> next;
        Lane(const std::string name):
         id(name)
         {
            next.clear();
         }
         Lane(const Lane& other){
            id = other.id;
            next.assign(other.next.begin(), other.next.end());
         }
         template<typename T>
         void setNext(T& data){
            next.push_back(data);
         }
         template<typename T,typename ...Args>
         void setNext(T& data, Args&... rest){
            next.push_back(data);
            setNext(rest...);
         }


    };


    class DepthFs{
        public:
        DepthFs(){}
        virtual ~DepthFs() = default;

        void test(){
            auto start = std::make_shared<Lane>("Start");
            auto a = std::make_shared<Lane>("A");
            auto b = std::make_shared<Lane>("B");
            auto c = std::make_shared<Lane>("c");
            auto d = std::make_shared<Lane>("D");
            auto e = std::make_shared<Lane>("E");
            auto f = std::make_shared<Lane>("F");
            auto g = std::make_shared<Lane>("G");
            auto h = std::make_shared<Lane>("H");
            auto i = std::make_shared<Lane>("I");
            auto j = std::make_shared<Lane>("J");
            auto k = std::make_shared<Lane>("K");
            auto l = std::make_shared<Lane>("L");
            auto end = std::make_shared<Lane>("End");

            start->setNext(a, b, c);
            a->setNext(d,e);
            b->setNext(e, f);
            c->setNext(g);
            d->setNext(k);
            e->setNext(h,i);
            f->setNext(i, l);
            g->setNext(j);
            h->setNext(k);
            i->setNext(k);
            j->setNext(l);
            k->setNext(end);
            l->setNext(end);

            std::map<std::string, std::shared_ptr<Lane>> lane_map;
            lane_map[start->id] = start;
            lane_map[end->id] = end;
            lane_map[a->id] = a;lane_map[b->id] = b;lane_map[c->id] = c;lane_map[d->id] = d;
            lane_map[e->id] = e;lane_map[f->id] = f;lane_map[g->id] = g;lane_map[h->id] = h;
            lane_map[i->id] = i;lane_map[j->id] = j;lane_map[k->id] = k;lane_map[l->id] = l;

            std::vector<std::vector<std::string>> paths;
            std::vector<std::string> current_path;
            current_path.push_back(start->id);
            searchPath(lane_map, start->id, end->id, paths, current_path);
           
           int count = 0;
            for(auto& pth : paths){
                count ++;
                printf("lane %d \n", count);
                for(auto id : pth){
                    printf("%s  ", id.c_str());
                }
                printf("\n");
            }
            

        }


        protected:
            void searchPath(std::map<std::string, std::shared_ptr<Lane>>&lane_map, std::string start, const std::string end, std::vector<std::vector<std::string>> &paths, std::vector<std::string> &current_path)
            {
                if(start == end){
                    paths.push_back(current_path);
                    return;
                }
                for(auto next_lane : (lane_map[start]->next)){
                    current_path.push_back(next_lane->id);
                    searchPath(lane_map, next_lane->id, end, paths, current_path);
                    current_path.pop_back();
                }
                return;
                
            }
    };

}


#endif