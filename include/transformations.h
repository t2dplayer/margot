#ifndef __TRANSFORMATIONS__
#define __TRANSFORMATIONS__

#include <cmath>
#include "point.h"
#include "matrix.h"
#include "matrix-utils.h"


namespace Margot {

class Transformations {
public:
    template <typename Type, std::size_t Cols>
    static auto translate_matrix(const Margot::Matrix<Type, 1, Cols>& T) {
        auto R = Margot::MatrixUtils::Identity<Type, Cols, Cols>();
        for (std::size_t i = 0; i < Cols; ++i) {
            R[Cols - 1][i] = T[0][i];
        }
        return R;
    }
    template <typename Type, std::size_t Cols>
    static auto scale_matrix(const Margot::Matrix<Type, 1, Cols>& S) {
        auto R = Margot::MatrixUtils::Identity<Type, Cols, Cols>();
        for (std::size_t i = 0; i < Cols; ++i) {
            R[i][i] = S[0][i];
        }
        return R;
    }
    template <typename Type, std::size_t Cols>
    static auto rotate_matrix(const Margot::Matrix<Type, 1, Cols>& T) {
        auto r = Margot::MatrixUtils::Identity<Type, Cols, Cols>();
        static_assert(Cols > 1, "Invalid dimension length");
        r[0][0] = cos(T[0][0]);
        r[0][1] = -sin(T[0][1]);
        r[1][0] = sin(T[0][0]);
        r[1][1] = cos(T[0][1]);
        return r;
    }
    template <typename Type, std::size_t Cols>
    static auto translate(const Margot::Matrix<Type, 1, Cols>& T) {
        auto U = translate_matrix<Type, Cols>(T);        
        return U;
    }
    template <typename Type, std::size_t Cols>
    static auto scale(const Margot::Matrix<Type, 1, Cols>& P, 
                      const Margot::Matrix<Type, 1, Cols>& S) {
        auto P_ = P;
        for (std::size_t i = 0; i < Cols - 1; ++i) {
            P_[0][i] *= static_cast<Type>(-1.);        
        }
        auto T1 = translate_matrix<Type, Cols>(P_);
        auto U = scale_matrix<Type, Cols>(S);
        auto T2 = translate_matrix<Type, Cols>(P);
        return T1 * U * T2;
    }
    template <typename Type, std::size_t Cols>
    static auto rotate(const Margot::Matrix<Type, 1, Cols>& P, 
                       const Margot::Matrix<Type, 1, Cols>& R) {
        auto P_ = P;
        for (std::size_t i = 0; i < Cols - 1; ++i) {
            P_[0][i] *= static_cast<Type>(-1.);        
        }
        auto T1 = translate_matrix<Type, Cols>(P_);
        auto U = rotate_matrix<Type, Cols>(R);
        auto T2 = translate_matrix<Type, Cols>(P);
        return T1 * U * T2;
    }    
};

} /* namespace Margot */

#endif /* __TRANSFORMATIONS__ */