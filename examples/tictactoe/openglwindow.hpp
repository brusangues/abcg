#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <array>
#include <vector>
#include <fmt/core.h>

#include "abcg.hpp"

class OpenGLWindow : public abcg::OpenGLWindow {
  protected:
    void initializeGL() override; // override optional but recommended
    void paintGL() override;
    void paintUI() override;

  private:
    //Game ttt{Game()};
    std::array<float, 4> m_clearColor{0.906f, 0.910f, 0.918f, 1.0f};
    // 4 position array defining the background color
};

#endif