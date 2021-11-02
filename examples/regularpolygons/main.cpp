#include <fmt/core.h>
#include "abcg.hpp"
#include "openglwindow.hpp"//openglwindow class definition

int main(int argc, char **argv) {
  try {
    // Create application instance
    abcg::Application app(argc, argv); // app management
    
    // Create OpenGL window
    auto window{std::make_unique<OpenGLWindow>()};// smart pointer creation
    window->setOpenGLSettings(
      {.samples = 2, //to support antialiasing
       .vsync = true, // enable vsync
       .preserveWebGLDrawingBuffer = true // framebuffer not erased between frames
    });
    //used by SDL in the opengl context creation - which represents a framebuffer linked to the window

    window->setWindowSettings(
      {.width = 600,
       .height = 600,
       .title = "Regular Polygons"
    });
    
    // Run application
    app.run(std::move(window));// abcg::Application::run is called
  } catch (const abcg::Exception &exception) {
    fmt::print(stderr, "{}\n", exception.what());
    return -1;
  }
  return 0;
}