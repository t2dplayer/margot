#include "window.h"
#include <cstdlib>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xutil.h>
#include <iostream>

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
    XFontStruct* myFont = XLoadQueryFont(this->display, "lucidasanstypewriter-bold-14");
    XSetFont(this->display, this->gc, myFont->fid);
}

Margot::MWindow::~MWindow() {
    XDestroyWindow(this->display, this->window);
    XCloseDisplay(this->display);
}

void Margot::MWindow::render(Margot::Func f) {
    this->f = f;
}

void Margot::MWindow::onKeyPressed(OnKeyPressed k) {
    this-> k = k;
}

void Margot::MWindow::show() {
    XEvent e;
    while (1)  {
        XNextEvent(this->display, &e);
        if (e.type == Expose) {
            XClearWindow(this->display, this->window);
            if (f != nullptr) f(this->display, this->window, this->gc);
        } else if (e.type == KeyPress) {
            KeyCode code = e.xkey.keycode;
            if (code == 0x09) break;
            else if (k != nullptr) k(code);
        }        
    }  
}

void Margot::MWindow::redraw() { // Called by any control which needs redrawing
  XEvent event;
  memset(&event, 0, sizeof(event));
  event.type = Expose;
  event.xexpose.display = display;
  XSendEvent(display, window, False, ExposureMask, &event);
}