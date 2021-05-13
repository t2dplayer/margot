#ifndef __VECTOR__
#define __VECTOR__

#include <array>
#include <ostream>
#include <cmath>

namespace Margot {

template<typename T, std::size_t S>
class Vector {
public:
    template<typename... U>
    Vector(U... values) {
        data = {values...};
    }
    T operator[](std::size_t i) const {
        return data[i];
    }
    T& operator[](std::size_t i) {
        return data[i];
    }
    friend std::ostream& operator<<(std::ostream& os,
        const Vector& v) {
        os << "<";
        for (std::size_t i = 0; i < v.data.size(); ++i) {
            os << v.data[i];
            if (i < v.data.size() - 1) os << ", ";
        }
        os << ">";
        return os;
    }
    Vector operator+(const Vector& v) {
        if (v.data.size() != data.size()) return {};
        Vector r(data);
        for (std::size_t i = 0; i < v.data.size(); ++i) {
            r[i] += v.data[i];
        }
        return r;
    }
    Vector operator-(const Vector& v) {
        if (v.data.size() != data.size()) return {};
        Vector r(data);
        for (std::size_t i = 0; i < v.data.size(); ++i) {
            r[i] -= v.data[i];
        }
        return r;
    }
    friend Vector operator*(const T& value, const Vector& v) {
        Vector r(v);
        for (std::size_t i = 0; i < r.data.size(); ++i) {
            r[i] *= value;
        }
        return r;    
    }
    Vector operator*(const T& v) {
        Vector r(data);
        for (std::size_t i = 0; i < r.data.size(); ++i) {
            r[i] *= v;
        }
        return r;
    }
    Vector operator/(const T& v) {
        Vector r(data);
        for (std::size_t i = 0; i < r.data.size(); ++i) {
            r[i] /= v;
        }
        return r;
    }
    T dot(const Vector& v) {
        T r = 0;
        for (std::size_t i = 0; i < v.data.size(); ++i) {
            r += data[i] * v.data[i];
        }
        return r;
    }
    T len() {
        T r = 0;
        for (std::size_t i = 0; i < data.size(); ++i) {
            r += data[i] * data[i];
        }
        return std::sqrt(r);
    }
protected:
    std::array<T, S> data;
};


}
#endif /* __VECTOR__ */
