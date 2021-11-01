#include <fmt/core.h>
#include "abcg.hpp"
#include "openglwindow.hpp"//openglwindow class definition

int main(int argc, char **argv) {
  try {
    // Create application instance
    abcg::Application app(argc, argv); // app management
    
    // Create OpenGL window
    auto window{std::make_unique<OpenGLWindow>()};// smart pointer creation
    window->setWindowSettings({.title = "Tic-Tac-Toe"});//, .width=200, .height=400});// window title definition; 
    
    // Run application
    app.run(std::move(window));// abcg::Application::run is called
  } catch (const abcg::Exception &exception) {
    fmt::print(stderr, "{}\n", exception.what());
    return -1;
  }
  return 0;
}