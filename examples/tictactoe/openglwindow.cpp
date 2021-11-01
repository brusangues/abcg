#include <fmt/core.h>

#include "openglwindow.hpp"

#include <imgui.h>

#include <cppitertools/itertools.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <gsl/gsl>
#include <string>

void OpenGLWindow::initializeGL() {
  // called only once
  auto windowSettings{getWindowSettings()};
  fmt::print("Initial window size: {}x{}\n", windowSettings.width,
             windowSettings.height);
  ttt.printBoard();
}

void OpenGLWindow::paintGL() {
  // Called once per frame update

  // Set the clear color - used to clear the window every frame. RGBA
  abcg::glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2],
                     m_clearColor[3]);
  // Clear the color buffer
  abcg::glClear(GL_COLOR_BUFFER_BIT); // uses constant
}

void OpenGLWindow::paintUI() {
  // Called every time paintGL is called

  // Parent class will show fullscreen button and FPS meter
  //abcg::OpenGLWindow::paintUI();

  // Our own ImGui widgets go below
  {
    // If this is the first frame, set initial position of our window
    static bool firstTime{true};
    if (firstTime) {
      ImGui::SetNextWindowPos(ImVec2(5, 75));
      ImGui::SetNextWindowSize(ImVec2(300,300));
      firstTime = false;
    }

    // Window begin
    ImGui::Begin("Tit-Tac-Toe!");

    // Static text
    ImGui::Text("%c turn", ttt.getPlayer());

    // 100x50 button
    if (ImGui::Button("Print board")) {
      fmt::print("Board:\n");
      ttt.printBoard();
    }
    
    // FOR LOOP
    for(auto i{0u}; i<3; i++){
      ImGui::Columns(3);

      for(auto j{0u}; j<3; j++){
        char pos = ttt.getPosition(i, j);
        std::string label = fmt::format("{}##{}{}", pos, i, j);
        bool press = ImGui::Button(label.c_str(), ImVec2(50, 50));
        if( press ) ttt.play(i,j);

        ImGui::NextColumn();
      }
      ImGui::Separator();
    }
    
    // Window end
    ImGui::End();
  }
}