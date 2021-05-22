#ifndef __MATRIX__
#define __MATRIX__

#include <vector>
#include <type_traits>

namespace Margot {

template <typename T, std::size_t Rows, std::size_t Cols>
class Matrix {
public:
    using Data = std::vector<std::vector<T>>;
    Matrix() {
        data.reserve(Rows);
        for (std::size_t i = 0; i < Rows * Cols; ++i) {
            if (i % Cols == 0) {
                data.push_back({});
                data[i / Cols].reserve(Cols);
            }
            data[i / Cols].push_back(static_cast<T>(0));
        }
    }
    template <typename... U>
    Matrix(U... values) {
        static_assert(sizeof...(values) == Rows * Cols, "Invalid number of values");
        std::vector<T> aux = {values...};
        data.reserve(Rows);
        for (std::size_t i = 0; i < aux.size(); ++i) {
            if (i % Cols == 0) {
                data.push_back({});
                data[i / Cols].reserve(Cols);
            }
            data[i / Cols].push_back(aux[i]);
        }
    }
    template <typename Type, std::size_t R, std::size_t C>
    Matrix<T, Rows, C> operator*(const Matrix<Type, R, C>& m) {
        Matrix<T, Rows, C> r;
        for (std::size_t i = 0; i < rows(); ++i) {
            for (std::size_t j = 0; j < m.cols(); ++j) {
                for (std::size_t k = 0; k < cols(); ++k) {
                    r[i][j] += data[i][k] * m[k][j];
                }
            }
        }
        return r;
    }
    auto operator[](std::size_t i) const {
        return data[i];
    }
    auto& operator[](std::size_t i) {
        return data[i];
    }
    constexpr std::size_t rows() const {
        return Rows;
    }
    constexpr std::size_t cols() const {
        return Cols;
    }
    friend std::ostream& operator<<(std::ostream& os,
                                    const Matrix& m) {
        for (std::size_t i = 0; i < m.rows(); ++i) {
            for (std::size_t j = 0; j < m.cols(); ++j) {
                os << m[i][j];
                if (j < m.cols() - 1) os << ", ";
            }
            os << '\n';
        }
        return os;
    }
protected:
    Data data;
};

} /* namespace Margot */

#endif /* __MATRIX__ */