#ifndef __WINDOW__
#define __WINDOW__

#include <string>
#include <X11/Xlib.h>
#include <functional>

namespace Margot {

    using Func = std::function<void(Display*, Window&, GC&)>;
    using OnKeyPressed = std::function<void(KeyCode)>;

    class MWindow {
        Display* display = nullptr;
        Window window;
        GC gc;
        Func f = nullptr;
        OnKeyPressed k = nullptr;
        std::string title = "";
        int width, height;
    public:
        MWindow(const std::string& title,
                int width, int height);
        ~MWindow();
        void render(Func f = nullptr);
        void onKeyPressed(OnKeyPressed k = nullptr);
        void redraw();
        void show();
    };
}

#endif /* __WINDOW__ */
