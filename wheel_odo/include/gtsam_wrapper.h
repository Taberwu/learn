#pragma once
#include <gtsam/geometry/Pose3.h>
#include <gtsam/geometry/Rot3.h>
#include <gtsam/navigation/ImuFactor.h>
#include <gtsam/navigation/NavState.h>
#include <gtsam/navigation/PreintegrationBase.h>
#include <gtsam/nonlinear/NonlinearFactor.h>
#include <gtsam/nonlinear/NonlinearFactorGraph.h>
#include <gtsam/nonlinear/NonlinearOptimizer.h>

using namespace gtsam;

namespace gtsam_wrapper
{

    template <class VALUE1, class VALUE2, class VALUE3, class VALUE4, class VALUE5, class VALUE6, class VALUE7>
    class NoiseModelFactor7 : public NoiseModelFactor
    {

    public:
        // typedefs for value types pulled from keys
        typedef VALUE1 X1;
        typedef VALUE2 X2;
        typedef VALUE3 X3;
        typedef VALUE4 X4;
        typedef VALUE5 X5;
        typedef VALUE6 X6;
        typedef VALUE7 X7;

    protected:
        typedef NoiseModelFactor Base;
        typedef NoiseModelFactor7<VALUE1, VALUE2, VALUE3, VALUE4, VALUE5, VALUE6, VALUE7> This;

    public:

        NoiseModelFactor7() {}


        NoiseModelFactor7(const SharedNoiseModel &noiseModel, Key j1, Key j2, Key j3, Key j4, Key j5, Key j6, Key j7) : Base(noiseModel, cref_list_of<7>(j1)(j2)(j3)(j4)(j5)(j6)(j7)) {}

        virtual ~NoiseModelFactor7() {}

        /** methods to retrieve keys */
        inline Key key1() const { return keys_[0]; }
        inline Key key2() const { return keys_[1]; }
        inline Key key3() const { return keys_[2]; }
        inline Key key4() const { return keys_[3]; }
        inline Key key5() const { return keys_[4]; }
        inline Key key6() const { return keys_[5]; }
        inline Key key7() const { return keys_[6]; }

  
        virtual Vector unwhitenedError(const Values &x, boost::optional<std::vector<Matrix> &> H = boost::none) const
        {
            if (this->active(x))
            {
                if (H)
                    return evaluateError(x.at<X1>(keys_[0]), x.at<X2>(keys_[1]), x.at<X3>(keys_[2]), x.at<X4>(keys_[3]), x.at<X5>(keys_[4]), x.at<X6>(keys_[5]), x.at<X7>(keys_[6]), (*H)[0], (*H)[1], (*H)[2], (*H)[3], (*H)[4], (*H)[5], (*H)[6]);
                else
                    return evaluateError(x.at<X1>(keys_[0]), x.at<X2>(keys_[1]), x.at<X3>(keys_[2]), x.at<X4>(keys_[3]), x.at<X5>(keys_[4]), x.at<X6>(keys_[5]), x.at<X7>(keys_[6]));
            }
            else
            {
                return Vector::Zero(this->dim());
            }
        }


        virtual Vector
        evaluateError(const X1 &, const X2 &, const X3 &, const X4 &, const X5 &, const X6 &, const X7 &,
                      boost::optional<Matrix &> H1 = boost::none,
                      boost::optional<Matrix &> H2 = boost::none,
                      boost::optional<Matrix &> H3 = boost::none,
                      boost::optional<Matrix &> H4 = boost::none,
                      boost::optional<Matrix &> H5 = boost::none,
                      boost::optional<Matrix &> H6 = boost::none,
                      boost::optional<Matrix &> H7 = boost::none) const = 0;

    private:
        /** Serialization function */
        friend class boost::serialization::access;
        template <class ARCHIVE>
        void serialize(ARCHIVE &ar, const unsigned int /*version*/)
        {
            ar &boost::serialization::make_nvp("NoiseModelFactor",
                                               boost::serialization::base_object<Base>(*this));
        }
    }; // \class NoiseModelFactor7
};
