#include "window.h"
#include <cstdlib>
#include <X11/Xutil.h>
#include <X11/Xos.h>

Margot::MWindow::MWindow(const std::string& title,
    int width, int height): title(title),
    width(width), height(height) {
    this->display = XOpenDisplay(NULL);
    if (this->display == NULL)  {
        exit(1);
    }    
    int s = DefaultScreen(this->display);
    this->window = XCreateSimpleWindow(this->display, 
        DefaultRootWindow(this->display), 
        0, 0, this->width, this->height, 1,
        BlackPixel(this->display, s), 
        WhitePixel(this->display, s));   
    XSelectInput(this->display, this->window, ExposureMask | KeyPressMask);
    this->gc = XCreateGC(this->display, this->window, 0, 0);
    XMapWindow(this->display, this->window); 
    XStoreName(this->display, this->window, this->title.c_str());
}

Margot::MWindow::~MWindow() {
    XDestroyWindow(this->display, this->window);
    XCloseDisplay(this->display);
}

void Margot::MWindow::render(Margot::Func f = nullptr) {
    this->f = f;
}

void Margot::MWindow::show() {
    XEvent e;
    while (1)  {
        XNextEvent(this->display, &e);
        if (e.type == KeyPress) {
            char buf[128] = {0};
            KeySym keysym;
            if (keysym == XK_Escape) break;
        }
        if (f != nullptr) f(this->display, this->window, this->gc);
    }  
}