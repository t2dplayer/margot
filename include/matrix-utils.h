#ifndef __MATRIX_UTILS__
#define __MATRIX_UTILS__

#include "matrix.h"

namespace Margot {

class MatrixUtils {
public:
    template <typename T, std::size_t Rows, std::size_t Cols>
    static Margot::Matrix<T, Rows, Cols> Identity() {
        Margot::Matrix<T, Rows, Cols> r;
        for (std::size_t i = 0; i < Rows; ++i) {
            for (std::size_t j = 0; j < Cols; ++j) {
                if (i == j) r[i][j] = static_cast<T>(1);
            }
        }
        return r;
    }
};

} /* namespace Margot */

#endif /* __MATRIX_UTILS__ */