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

using Matrix3x3 = Margot::Matrix<double, 3, 3>;
using Matrix3x1 = Margot::Matrix<double, 3, 1>;
using Vector = std::vector<Matrix3x1>;
using Vector3D = Margot::Vector<double, 3>;
using MT = Margot::Transformations;

Vector make_polygon() {
    Vector r = {
        {0., 0., 1.},
        {100., 200., 1.},
        {200., 100., 1.},
        {300., 200., 1.},
        {400., 0., 1.},
    };
    return r;
}

void draw_polygon(Display* d, Window& w, GC& g, const Vector& v) {
    if (v.size() == 0) return;
    for (std::size_t i = 1; i < v.size(); ++i) {
        auto src = v[i - 1];
        auto dst = v[i];
        XDrawLine(d, w, g, src[0][0], src[1][0], dst[0][0], dst[1][0]);
    }
    auto src = v[0];
    auto dst = v[v.size() - 1];
    XDrawLine(d, w, g, v[0][0][0], src[1][0], dst[0][0], dst[1][0]);
}

Vector update_vertices(const Vector& vertices,
                       const Matrix3x3& M,
                       const Matrix3x3& N) {
    Vector r;
    for (const auto& v: vertices) {
        auto p = N * M * v;
        r.push_back(p);
    }    
    return r;
}

void draw_rectangle(Display* d, Window& w, GC& g, Margot::Rect<double>& rect) {
    XDrawLine(d, w, g, rect.getLeftTop()[0], rect.getLeftTop()[1], rect.getRightTop()[0], rect.getRightTop()[1]);
    XDrawLine(d, w, g, rect.getRightTop()[0], rect.getRightTop()[1], rect.getRightBottom()[0], rect.getRightBottom()[1]);
    XDrawLine(d, w, g, rect.getLeftTop()[0], rect.getLeftTop()[1], rect.getLeftBottom()[0], rect.getLeftBottom()[1]);
    XDrawLine(d, w, g, rect.getLeftBottom()[0], rect.getLeftBottom()[1], rect.getRightBottom()[0], rect.getRightBottom()[1]);
}

Matrix3x1 onTranslationKey(const KeyCode& key) {
    static const double speed = 4.;
    Matrix3x1 T{0., 0., 1.};
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

Matrix3x1 onScalingKey(const KeyCode& key) {
    Matrix3x1 T{0., 0., 1.};
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

Matrix3x1 onRotatingKey(const KeyCode& key) {
    static const double radians = 0.01;
    Matrix3x1 T{0., 0., 1.};
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
    static const double kScreenWidth = 1024.;
    static const double kScreenHeight = 768.;
    const double vpSize = 500.;
    std::set<KeyCode> translatingKeys = {
        0x71, 0x72, 0x74, 0x6f
    };
    std::set<KeyCode> scalingKeys = {
        82, 86
    };
    std::set<KeyCode> rotatingKeys = {
       106, 63
    };    
    Margot::Rect<double> window({0., 0.}, {250., 250.});
    Margot::Rect<double> ndc({-1., -1.}, {1., 1.});    
    Margot::Rect<double> viewport({kScreenWidth - vpSize, kScreenHeight - vpSize}, vpSize, vpSize);
    Margot::MWindow win("Margot 2D/3D Engine", 1024, 768);
    double rad = 0.0;
    Vector polygon = make_polygon();
    auto M = MT::transform<double, 2>(window, ndc, {2./window.width(), 2./window.height()}, {0.53, 0.53});
    auto N = MT::transform<double, 2>(ndc, viewport, {viewport.width() / 2., viewport.height() / 2.}, {0., 0.});
    win.render([&](Display* d, Window& w, GC& g){
        draw_rectangle(d, w, g, window);
        draw_rectangle(d, w, g, viewport);
        draw_polygon(d, w, g, polygon);
        // convert all point of polygon from window to ndc, and after that, from ndc to viewport
        Vector vpolygon = update_vertices(polygon, M, N);
        draw_polygon(d, w, g, vpolygon);
    });
    win.onKeyPressed([&](KeyCode key) {     
        rad += 0.01;
        M = MT::transform<double, 2>(window, ndc, {2./window.width(), 2./window.height()}, {rad, rad});
        win.redraw();
    });
    win.show();
    return EXIT_SUCCESS;
}
