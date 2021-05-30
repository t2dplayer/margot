#include <iostream>
#include <cmath>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <vector>
#include <set>
#include "window.h"
#include "point.h"
#include "vector.h"
#include "matrix.h"
#include "matrix-utils.h"
#include "rect.h"
#include "transformations.h"

using Matrix1x3 = Margot::Matrix<double, 1, 3>;
using Matrix3x3 = Margot::Matrix<double, 3, 3>;
using Vector = std::vector<Matrix1x3>;


Vector make_triangle() {
    Vector r = {
        {200., 200., 1.},
        {300., 50., 1.},
        {400., 200., 1.},
        {200., 200.f, 1.},
    };
    return r;
}

template <typename T>
auto window_to_viewport(const Margot::Rect<T>& window,
                        const Margot::Rect<T>& viewport) {
    auto xmin = window.left();
    auto ymin = window.top();
    auto umin = viewport.left();
    auto vmin = viewport.top();
    auto sx = viewport.width() / window.width();
    auto sy = viewport.height() / window.height();
    auto t1 = Margot::Transformations::translate_matrix<T, 3>({-xmin, -ymin, 1.});
    auto s = Margot::Transformations::scale_matrix<T, 3>({sx, sy, 1.});
    auto t2 = Margot::Transformations::translate_matrix<T, 3>({umin, vmin, 1.});        
    return t1 * s * t2;
}


Vector make_viewport_points(Margot::Rect<double>& window,
                            Margot::Rect<double>& viewport,
                            Vector vertices) {
    if (vertices.size() == 0) return {};
    Vector r; 
    r.reserve(vertices.size());
    auto N = window_to_viewport(window, viewport);
    for (auto const& pw: vertices) {
        Matrix1x3 U = {pw[0][0], pw[0][1], 1.};
        auto V = U * N;
        r.push_back({V[0][0], V[0][1], 1.});
    }    
    return r;
}


// Vector make_arc(const Margot::Point<double, 2>& p1,
//                 const Margot::Point<double, 2>& p2,
//                 const Margot::Point<double, 2>& p3) {
//     Vector r;
//     r.reserve(1. / 0.01);
//     for (double t = 0.0; t < 1; t += 0.01) {
//         double a1 = pow((1. - t), 2.);
//         double a2 = 2. * t * (1. - t);
//         double a3 = pow(t, 2.);
//         Margot::Point<double, 2> p = (a1 * p1) + (a2 * p3) + (a3 * p2);
//         r.emplace_back(p);
//     }
//     return r;
// }

// void math01(Display* d, Window& w, GC& g) {
//     Vector v1 = make_triangle();
//     for (std::size_t i = 0; i < v1.size() - 1; ++i) {
//         XDrawLine(d, w, g, v1[i][0], v1[i][1], v1[i + 1][0], v1[i + 1][1]);
//     }
//     Vector v2 = make_arc(v1[0], v1[2], v1[1]);
//     for (const auto& p: v2) {
//         XDrawPoint(d, w, g, p[0], p[1]);
//     }
// }

void draw_rectangle(Display* d, Window& w, GC& g,
    Margot::Rect<double>& rect) {
    XDrawLine(d, w, g, rect.getLeftTop()[0], rect.getLeftTop()[1], rect.getRightTop()[0], rect.getRightTop()[1]);
    XDrawLine(d, w, g, rect.getRightTop()[0], rect.getRightTop()[1], rect.getRightBottom()[0], rect.getRightBottom()[1]);
    XDrawLine(d, w, g, rect.getLeftTop()[0], rect.getLeftTop()[1], rect.getLeftBottom()[0], rect.getLeftBottom()[1]);
    XDrawLine(d, w, g, rect.getLeftBottom()[0], rect.getLeftBottom()[1], rect.getRightBottom()[0], rect.getRightBottom()[1]);
}


void example_window_to_viewport(Display* d, Window& w, GC& g,
                                Margot::Rect<double>& window,
                                Margot::Rect<double>& viewport) {
    // Draw Window and Viewport labels and rectangles
    draw_rectangle(d, w, g, window);
    draw_rectangle(d, w, g, viewport);
    XDrawString(d, w, g, window.left() + 2, window.top() + 18, "window", 6);
    XDrawString(d, w, g, viewport.left() + 2, viewport.top() + 18, "viewport", 8);
    Vector window_triangle = make_triangle();
    Vector viewport_triangle = make_viewport_points(window, viewport, window_triangle);
    // Draw in window
    for (std::size_t i = 0; i < window_triangle.size() - 1; ++i) {
        XDrawLine(d, w, g, 
            window_triangle[i][0][0], window_triangle[i][0][1], 
            window_triangle[i + 1][0][0], window_triangle[i + 1][0][1]);
    }
    // Draw in viewport
    for (std::size_t i = 0; i < viewport_triangle.size() - 1; ++i) {
        XDrawLine(d, w, g, viewport_triangle[i][0][0], viewport_triangle[i][0][1], 
            viewport_triangle[i + 1][0][0], viewport_triangle[i + 1][0][1]);
    }
}

Matrix1x3 onTranslationKey(const KeyCode& key) {
    static const double speed = 4.;
    Matrix1x3 T{0., 0., 1.};
    switch (key) {
        case 0x6f: // up
            T = {0., -speed, 1.}; 
            break;
        case 0x74: // down
            T = {0., speed, 1.};
            break;
        case 0x71: // left
            T = {-speed, 0., 1.};
            break;
        case 0x72: // right
            T = {speed, 0., 1.};
            break;
    }
    return T;
}

Matrix1x3 onScalingKey(const KeyCode& key) {
    Matrix1x3 T{0., 0., 1.};
    switch (key) {
        case 86: // plus
            T = {1.01, 1.01, 1.}; 
            break;
        case 82: // minus
            T = {0.99, 0.99, 1.};
            break;
    }
    return T;
}

Matrix1x3 onRotatingKey(const KeyCode& key) {
    static const double radians = 0.01;
    Matrix1x3 T{0., 0., 1.};
    switch (key) {
        case 106: // division
            T = {-radians, -radians, 1.}; 
            break;
        case 63: // multiplication
            T = {radians, radians, 1.};
            break;
    }
    return T;
}

int main(int argc, char** argv) {
    std::set<KeyCode> translatingKeys = {
        0x71, 0x72, 0x74, 0x6f
    };
    std::set<KeyCode> scalingKeys = {
        82, 86
    };
    std::set<KeyCode> rotatingKeys = {
       106, 63
    };    
    Margot::Rect<double> window({50., 50.}, {250., 250.}, {50., 250.}, {250., 50.});
    Margot::Rect<double> viewport({1024. - 500., 768. - 500.}, 500., 500.);
    Margot::MWindow win("Margot 2D/3D Engine", 1024, 768);
    win.render([&](Display* d, Window& w, GC& g){
        example_window_to_viewport(d, w, g, window, viewport);
    });
    win.onKeyPressed([&](KeyCode key) {           
        Matrix1x3 LT{window.getLeftTop()[0], window.getLeftTop()[1], 1.};
        Matrix1x3 RT{window.getRightTop()[0], window.getRightTop()[1], 1.};
        Matrix1x3 RB{window.getRightBottom()[0], window.getRightBottom()[1], 1.};
        Matrix1x3 LB{window.getLeftBottom()[0], window.getLeftBottom()[1], 1.};
        Matrix1x3 T = {0., 0., 1.};
        Matrix3x3 U = Margot::MatrixUtils::Identity<double, 3, 3>();
        if (rotatingKeys.find(key) != rotatingKeys.end()) {
            T = onRotatingKey(key);
            U = Margot::Transformations::rotate<double, 3>(LT, T);
        }    
        if (translatingKeys.find(key) != translatingKeys.end()) {
            T = onTranslationKey(key);
            U = Margot::Transformations::translate<double, 3>(T);
        } else if (scalingKeys.find(key) != scalingKeys.end()) {
            T = onScalingKey(key);
            U = Margot::Transformations::scale<double, 3>(LT, T);
        }
        auto LeftTop = LT * U;
        auto RightTop = RT * U;
        auto LeftBottom = LB * U;
        auto RightBottom = RB * U;
        window = Margot::Rect<double>(
            {LeftTop[0][0], LeftTop[0][1]},
            {RightBottom[0][0], RightBottom[0][1]},
            {LeftBottom[0][0], LeftBottom[0][1]},
            {RightTop[0][0], RightTop[0][1]}
        );
        win.redraw();
    });
    win.show();
    return EXIT_SUCCESS;
}
