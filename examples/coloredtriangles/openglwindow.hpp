#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <array>
#include <glm/vec2.hpp>
#include <random>

#include "abcg.hpp"

class OpenGLWindow : public abcg::OpenGLWindow {
  protected:
    // called only once
    void initializeGL() override;
    // called every frame
    void paintGL() override;
    // called every time paintGL is called
    void paintUI() override;
    // called by abcg every time the window size is changed
    void resizeGL(int width, int height) override;
    // called by abcg when the window is destroyed.
    void terminateGL() override;

  private:
    // OpenGL allocated resources

    // Vertex Array Object - vbo configs
    GLuint m_vao{};
    // Vertex Buffer Object - stores vertexes attributes
    GLuint m_vboPositions{};
    // Vertex Buffer Object - stores vertexes attributes
    GLuint m_vboColors{};
    // Stores shader
    GLuint m_program{};

    // Window size informed by resizeGL
    int m_viewportWidth{};
    // Window size informed by resizeGL
    int m_viewportHeight{};

    // Random number generator for position and color choices
    std::default_random_engine m_randomEngine;

    // Array containing colors that will be later copied to m_vertexColors
    std::array<glm::vec4, 3> m_vertexColors{
      glm::vec4{0.36f, 0.83f, 1.00f, 1.0f},
      glm::vec4{0.63f, 0.00f, 0.61f, 1.0f},
      glm::vec4{1.00f, 0.69f, 0.30f, 1.0f}};

    // Create resorces m_vao and m_vboVertices.
    // Called every time a new point P needs to be drawn.
    void setupModel();
};
#endif