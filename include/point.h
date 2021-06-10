#ifndef __POINT__
#define __POINT__

#include <array>
#include <ostream>
#include "vector.h"

namespace Margot {

template<typename T, std::size_t S>
class Point {
public:
    template<typename... U>
    Point(U... values) {
        data = {values...};
    }
    T operator[](std::size_t i) const {
        return data[i];
    }
    T& operator[](std::size_t i) {
        return data[i];
    }
    Point operator+(const Margot::Vector<T, S>& v) const {
        Point p(data);
        for (std::size_t i = 0; i < S; ++i) {
            p.data[i] += v[i];
        }
        return p;
    }
    Vector<T, S> operator-(const Point& p) {
        Vector<T, S> r;
        for (std::size_t i = 0; i < p.data.size(); ++i) {
            r[0] = data[i] - p[i];
        }
        return r;
    }
    friend std::ostream& operator<<(std::ostream& out,
        const Point& p) {
        out << "[";
        for (std::size_t i = 0; i < p.data.size(); ++i) {
            out << p.data[i];
            if (i < p.data.size() - 1) out << ", ";
        }
        out << "]";
        return out;
    }
protected:
    std::array<T, S> data;
};

}

#endif /* __POINT__ */
