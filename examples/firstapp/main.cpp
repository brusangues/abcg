#include "abcg.hpp"
#include "openglwindow.hpp"//openglwindow class definition

int main(int argc, char **argv) {
  // Create application instance
  abcg::Application app(argc, argv); // app management
  
  // Create OpenGL window
  auto window{std::make_unique<OpenGLWindow>()};// smart pointer creation
  window->setWindowSettings({.title = "First App"});// window title definition; 
  
  // Run application
  app.run(std::move(window));// abcg::Application::run is called
  
  return 0;
}