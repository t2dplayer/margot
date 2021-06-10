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
            R[i][Cols - 1] = T[0][i];
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
    template <typename T, std::size_t Cols>
    static auto transform(const Margot::Rect<T>& from,
                          const Margot::Rect<T>& to,
                          const Margot::Matrix<T, 1, Cols>& scale,
                          const Margot::Matrix<T, 1, Cols>& rotation) {
        auto x_min = from.left() + from.width() / 2.;
        auto y_min = from.top() + from.height() / 2.;
        auto T1 = translate_matrix<double, 3>({-x_min, -y_min, 1.});
        auto sx = scale[0][0];
        auto sy = scale[0][1];
        auto S = scale_matrix<double, 3>({sx, sy, 1.});
        auto u_min = to.left() + to.width() / 2.;
        auto v_min = to.top() + to.height() / 2;
        auto T2 = translate_matrix<double, 3>({u_min, v_min, 1.});
        auto ax = rotation[0][0];
        auto ay = rotation[0][1];
        auto R = rotate_matrix<double, 3>({ax, ay, 1.});
        auto M = T2 * R * S * T1;
        return M;
    }
};

} /* namespace Margot */

#endif /* __TRANSFORMATIONS__ */