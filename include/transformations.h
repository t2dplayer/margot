#ifndef __TRANSFORMATIONS__
#define __TRANSFORMATIONS__

#include "point.h"
#include "matrix.h"
#include "matrix-utils.h"


namespace Margot {

class Transformations {
public:
    template <typename T, std::size_t Rows, std::size_t Cols, std::size_t Dim = 2>
    static auto translate(const Margot::Point<T, Dim>& p) {
        auto r = Margot::MatrixUtils::Identity<T, Rows, Cols>();
        for (std::size_t i = 0; i < Dim; ++i) {
            r[i][Cols - 1] = p[i];
        }
        return r;
    }
    template <typename T, std::size_t Rows, std::size_t Cols, std::size_t Dim = 2>
    static auto scale(const Margot::Point<T, Dim>& p) {
        auto r = Margot::MatrixUtils::Identity<T, Rows, Cols>();
        for (std::size_t i = 0; i < Dim; ++i) {
            r[i][i] = p[i];
        }
        return r;
    }
};

} /* namespace Margot */

#endif /* __TRANSFORMATIONS__ */