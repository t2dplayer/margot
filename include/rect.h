#ifndef __RECT__
#define __RECT__

namespace Margot {

#include "point.h"

template <typename T, std::size_t Dim = 2>
class Rect {
public:
    Rect(const Margot::Point<T, Dim>& leftTop, 
         const Margot::Point<T, Dim>& rightBottom):
         leftTop(leftTop), rightBottom(rightBottom) {}
    Rect(const Margot::Point<T, Dim>& leftTop,
         T width, T height) {
        this->leftTop = leftTop;
        this->rightBottom = {leftTop[0] + width, leftTop[1] + height};
    }    
    T left() const {
        return leftTop[0];
    }
    T right() const {
        return rightBottom[0];
    }
    T top() const {
        return leftTop[1];
    }
    T bottom() const {
        return leftTop[1];
    }
    T width() const {
        return std::abs(leftTop[0] - rightBottom[0]);
    }
    T height() const {
        return std::abs(leftTop[1] - rightBottom[1]);
    }
    Margot::Point<T, Dim> getLeftTop() const {
        return leftTop;
    }
    Margot::Point<T, Dim> getRightBottom() const {
        return rightBottom;
    }
    friend std::ostream& operator<<(std::ostream& os, const Rect<T>& rect) {
        os << "[";
        os << rect.leftTop << ", ";
        os << rect.rightBottom;
        os << "]";
        return os;
    }
protected:
    Margot::Point<T, Dim> leftTop, rightBottom;
};

} /* namespace Margor */



#endif /* __RECT__ */