#ifndef __RECT__
#define __RECT__

namespace Margot {

#include "point.h"

template <typename T, std::size_t Dim = 2>
class Rect {
public:
    Rect(const Margot::Point<T, Dim>& leftTop, 
         const Margot::Point<T, Dim>& rightBottom):
         leftTop(leftTop), rightBottom(rightBottom) {
        rightTop = {rightBottom[0], leftTop[1]};
        leftBottom = {leftTop[0], rightBottom[1]};
    }
    Rect(const Margot::Point<T, Dim>& leftTop, 
         const Margot::Point<T, Dim>& rightBottom,
         const Margot::Point<T, Dim>& leftBottom,
         const Margot::Point<T, Dim>& rightTop):
         leftTop(leftTop), rightBottom(rightBottom), leftBottom(leftBottom), rightTop(rightTop) {
    }
    Rect(const Margot::Point<T, Dim>& leftTop,
         T width, T height) {
        this->leftTop = leftTop;
        this->rightBottom = {leftTop[0] + width, leftTop[1] + height};
        rightTop = {this->rightBottom[0], this->leftTop[1]};
        leftBottom = {this->leftTop[0], this->rightBottom[1]};
    }    
    T left() const {
        return leftTop[0];
    }
    T right() const {
        return rightTop[0];
    }
    T top() const {
        return leftTop[1];
    }
    T bottom() const {
        return leftBottom[1];
    }
    T width() const {
        return std::abs(left() - right());
    }
    T height() const {
        return std::abs(top() - bottom());
    }
    auto getLeftTop() const {
        return leftTop;
    }
    auto getRightTop() const {
        return rightTop;
    }
    auto getLeftBottom() const {
        return leftBottom;
    }
    auto getRightBottom() const {
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
    Margot::Point<T, Dim> rightTop, leftBottom;
};

} /* namespace Margor */



#endif /* __RECT__ */