#include <iostream>
#include <cmath>
#include <X11/Xlib.h>
#include "window.h"
#include "point.h"
#include "vector.h"
#include "matrix.h"

void math01(Display* d, Window& w, GC& g) {
    Margot::Point<double, 2> p1 = {200.f, 200.f};
    Margot::Point<double, 2> p2 = {400.f, 200.f};
    Margot::Point<double, 2> p3 = {300.f, 50.f};
    XDrawLine(d, w, g, p1[0], p1[1], p2[0], p2[1]);
    XDrawLine(d, w, g, p1[0], p1[1], p3[0], p3[1]);
    XDrawLine(d, w, g, p3[0], p3[1], p2[0], p2[1]);
    for (double t = 0.0; t < 1; t += 0.01) {
        double a1 = pow((1. - t), 2.);
        double a2 = 2. * t * (1. - t);
        double a3 = pow(t, 2.);
        Margot::Point<double, 2> p = (a1 * p1) + (a2 * p3) + (a3 * p2);
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
    std::cout << m * n << '\n';
}

int main(int argc, char** argv) {
    Margot::MWindow win("Margot 2D/3D Engine", 1024, 768);
    win.render([&](Display* d, Window& w, GC& g){
        math01(d, w, g);
        matrix(d, w, g);
    });
    win.show();
    return EXIT_SUCCESS;
}
