#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

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
    int m_delay{200};
    abcg::ElapsedTimer m_elapsedTimer;

    // Create resorces m_vao and m_vboVertices.
    // Called every time a new triangle needs to be drawn.
    void setupModel(int sides);
};
#endif