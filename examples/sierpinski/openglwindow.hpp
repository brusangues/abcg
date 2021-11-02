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
    GLuint m_vboVertices{};
    // Stores shader
    GLuint m_program{};

    // Window size informed by resizeGL
    int m_viewportWidth{};
    // Window size informed by resizeGL
    int m_viewportHeight{};

    // Random number generator for initial P position, and ABC choice.
    std::default_random_engine m_randomEngine;

    // Array containing positions of points A,B and C
    const std::array<glm::vec2, 3> m_points{glm::vec2( 0,  1), 
                                            glm::vec2(-1, -1),
                                            glm::vec2( 1, -1)};
    // Point P position
    glm::vec2 m_P{};

    // Create resorces m_vao and m_vboVertices.
    // Called every time a new point P needs to be drawn.
    void setupModel();
};
#endif