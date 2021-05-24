#include <iostream>
#include <cmath>
#include <X11/Xlib.h>
#include <vector>
#include "window.h"
#include "point.h"
#include "vector.h"
#include "matrix.h"
#include "matrix-utils.h"
#include "rect.h"
#include "transformations.h"

using Vector = std::vector<Margot::Point<double, 2>>;

Vector make_triangle() {
    Vector r = {
        Margot::Point<double, 2>{200.f, 200.f},
        Margot::Point<double, 2>{300.f, 50.f},
        Margot::Point<double, 2>{400.f, 200.f},
        Margot::Point<double, 2>{200.f, 200.f},
    };
    return r;
}

Vector make_arc(const Margot::Point<double, 2>& p1,
                const Margot::Point<double, 2>& p2,
                const Margot::Point<double, 2>& p3) {
    Vector r;
    r.reserve(1. / 0.01);
    for (double t = 0.0; t < 1; t += 0.01) {
        double a1 = pow((1. - t), 2.);
        double a2 = 2. * t * (1. - t);
        double a3 = pow(t, 2.);
        Margot::Point<double, 2> p = (a1 * p1) + (a2 * p3) + (a3 * p2);
        r.emplace_back(p);
    }
    return r;
}

void math01(Display* d, Window& w, GC& g) {
    Vector v1 = make_triangle();
    for (std::size_t i = 0; i < v1.size() - 1; ++i) {
        XDrawLine(d, w, g, v1[i][0], v1[i][1], v1[i + 1][0], v1[i + 1][1]);
    }
    Vector v2 = make_arc(v1[0], v1[2], v1[1]);
    for (const auto& p: v2) {
        XDrawPoint(d, w, g, p[0], p[1]);
    }
}

void matrix(Display* d, Window& w, GC& g) {
    Margot::Matrix<int, 3, 2> m = {
        3, 1,
        2, -1,
        0, 4
    };
    Margot::Matrix<int, 2, 3> n = {
        1, -1, 2,
        3, 0, 5
    };
    Margot::Matrix<int, 3, 3> i = Margot::MatrixUtils::Identity<int, 3, 3>();
}

void rect(Display* d, Window& w, GC& g) {
    Margot::Rect<int> world({0, 0}, {1024, 768});
    Margot::Rect<int> window({100, 25}, 200, 200);
    Margot::Rect<int> viewport({0, 300}, 400, 400);
    XDrawRectangle(d, w, g, window.left(), window.top(), window.width(), window.height());
    XDrawRectangle(d, w, g, viewport.left(), viewport.top(), viewport.width(), viewport.height());
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
    auto t2 = Margot::Transformations::translate<T, 3, 3>({umin, vmin});
    auto s = Margot::Transformations::scale<T, 3, 3>({sx, sy});
    auto t1 = Margot::Transformations::translate<T, 3, 3>({-xmin, -ymin});
    /*
    std::cout << "t1\n" << t1 << '\n';
    std::cout << "s\n" << s << '\n';
    std::cout << "t2\n" << t2 << '\n';
    */
    return t2 * s * t1;
}

void transformation(Display* d, Window& w, GC& g) {    
    Margot::Rect<double> world({0., 0.}, {1024., 768.});    
    Margot::Rect<double> window({25., 0.}, 200, 200);
    Margot::Rect<double> viewport({25., 205.}, 200, 500);
    XDrawString(d, w, g, window.left() + 2, window.top() + 18, "window", 6);
    XDrawString(d, w, g, viewport.left() + 2, viewport.top() + 18, "viewport", 8);     
    
    XDrawRectangle(d, w, g, window.left(), window.top(), window.width(), window.height());
    XDrawRectangle(d, w, g, viewport.left(), viewport.top(), viewport.width(), viewport.height());
    auto N = window_to_viewport(window, viewport);
    std::cout << N << '\n';
    Vector v1 = make_triangle();
    Vector vv; vv.reserve(v1.size());
    for (auto const& pw: v1) {
        Margot::Matrix<double, 3, 1> U = {
            pw[0],
            pw[1],
            1.
        };
        auto V = N * U;
        vv.push_back({V[0][0], V[1][0]});
    }
    for (std::size_t i = 0; i < vv.size() - 1; ++i) {
        XDrawLine(d, w, g, vv[i][0], vv[i][1], vv[i + 1][0], vv[i + 1][1]);
    }
    Vector v2 = make_arc(v1[0], v1[2], v1[1]);
    for (auto const& pw: v2) {
        Margot::Matrix<double, 3, 1> U = {
            pw[0],
            pw[1],
            1.
        };
        auto V = N * U;
        XDrawPoint(d, w, g, V[0][0], V[1][0]);
    }
}

int main(int argc, char** argv) {
    Margot::MWindow win("Margot 2D/3D Engine", 1024, 768);
    win.render([&](Display* d, Window& w, GC& g){
        math01(d, w, g);
        //matrix(d, w, g);
        //rect(d, w, g);
        transformation(d, w, g);
    });
    win.show();
    return EXIT_SUCCESS;
}
