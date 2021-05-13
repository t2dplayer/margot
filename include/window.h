#ifndef __WINDOW__
#define __WINDOW__

#include <string>
#include <X11/Xlib.h>
#include <functional>

namespace Margot {

    using Func = std::function<void(Display*, Window&, GC&)>;

    class MWindow {
        Display* display = nullptr;
        Window window;
        GC gc;
        Func f = nullptr;
        std::string title = "";
        int width, height;
    public:
        MWindow(const std::string& title,
                int width, int height);
        ~MWindow();
        void render(Func f);
        void show();
    };
}

#endif /* __WINDOW__ */
