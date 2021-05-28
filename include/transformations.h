#ifndef __TRANSFORMATIONS__
#define __TRANSFORMATIONS__

#include <cmath>
#include "point.h"
#include "matrix.h"
#include "matrix-utils.h"


namespace Margot {

class Transformations {
public:
    template <typename T, std::size_t Rows, std::size_t Cols, std::size_t Dim = 2>
    static auto translate_matrix(const Margot::Point<T, Dim>& p) {
        auto r = Margot::MatrixUtils::Identity<T, Rows, Cols>();
        for (std::size_t i = 0; i < Dim; ++i) {
            r[i][Cols - 1] = p[i];
        }
        return r;
    }
    template <typename T, std::size_t Rows, std::size_t Cols, std::size_t Dim = 2>
    static auto scale_matrix(const Margot::Point<T, Dim>& p) {
        auto r = Margot::MatrixUtils::Identity<T, Rows, Cols>();
        for (std::size_t i = 0; i < Dim; ++i) {
            r[i][i] = p[i];
        }
        return r;
    }
    template <typename T, std::size_t Rows, std::size_t Cols, std::size_t Dim = 2>
    static auto rotate_matrix(const Margot::Point<T, Dim>& p, T radians) {
        auto r = Margot::MatrixUtils::Identity<T, Rows, Cols>();
        static_assert(Dim > 1, "Invalid dimension length");
        r[0][0] = cos(static_cast<double>(radians));
        r[0][1] = -sin(static_cast<double>(radians));
        r[1][0] = sin(static_cast<double>(radians));
        r[1][1] = cos(static_cast<double>(radians));
        return r;
    }
};

} /* namespace Margot */

#endif /* __TRANSFORMATIONS__ */