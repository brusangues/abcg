#include "openglwindow.hpp"

#include <imgui.h>
#include <fmt/core.h>
#include <cppitertools/itertools.hpp>

#include "abcg.hpp"

void OpenGLWindow::initializeGL() {
  // Vertex shader definition.
  // Contains point size definition,
  // color and position is passed through.
  // Uniform variables: defined before each render.
  // These are global variables shared by all vertices in the buffer, 
  // different from 'in' variables.
  const auto *vertexShader{R"gl(
    #version 410
    layout(location = 0) in vec2 inPosition;
    layout(location = 1) in vec4 inColor;

    uniform vec2 translation;
    uniform float scale;

    out vec4 fragColor;

    void main() {
      vec2 newPosition = inPosition * scale + translation;
      gl_Position = vec4(newPosition, 0, 1);
      fragColor = inColor;
    }
  )gl"};
  
  // Fragment(pixel) shader definition.
  // Color is passed through.
  const auto *fragmentShader{R"gl(
    #version 410
    in vec4 fragColor;
    out vec4 outColor;
    void main() { outColor = fragColor; }
  )gl"};

  // Create shader program.
  // m_program is an integer that identifies the shader program composed of vertex/fragment shader.
  // To activate m_program, we call glUseProgram(m_program).
  // To deactivate, se can call it with another program or use glUseProgram(0).
  m_program = createProgramFromString(vertexShader, fragmentShader);

  // Clear window
  abcg::glClearColor(0, 0, 0, 1);
  abcg::glClear(GL_COLOR_BUFFER_BIT);

  // Start pseudo-random number generator
  // seed is based on current processor time.
  auto seed{std::chrono::steady_clock::now().time_since_epoch().count()};
  m_randomEngine.seed(seed);

  // Color blend test
  //glEnable(GL_BLEND);
  //glBlendEquation(GL_FUNC_ADD);
  //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
}

void OpenGLWindow::setupModel(int sides) {
  // Release previous resources, if any
  abcg::glDeleteBuffers(1, &m_vboPositions);
  abcg::glDeleteBuffers(1, &m_vboColors);
  abcg::glDeleteVertexArrays(1, &m_vao);

  // Select random colors for the radial gradient
  std::uniform_real_distribution<float> rd(0.0f, 1.0f);
  const glm::vec3 color1{rd(m_randomEngine), rd(m_randomEngine),
                         rd(m_randomEngine)};
  const glm::vec3 color2{rd(m_randomEngine), rd(m_randomEngine),
                         rd(m_randomEngine)};

  // Minimum number of sides is 3
  sides = std::max(3, sides);       

  // Colors and sides vectors initialization
  std::vector<glm::vec2> positions(0);
  std::vector<glm::vec3> colors(0);

  // Polygon center
  positions.emplace_back(0, 0);
  colors.push_back(color1);

  // Border vertices
  const auto step{M_PI * 2 / sides};
  for (const auto angle : iter::range(0.0, M_PI * 2, step)) {
    positions.emplace_back(std::cos(angle), std::sin(angle));
    colors.push_back(color2);
  }

  // Duplicate second vertex
  positions.push_back(positions.at(1));
  colors.push_back(color2);

  // Generate VBO of positions
  abcg::glGenBuffers(1, &m_vboPositions); // Creates buffer object id
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vboPositions); // Binds buffer object as VBO
  abcg::glBufferData(GL_ARRAY_BUFFER, // Allocates memory and init buffer with contents copied from a cpu resource
    positions.size() * sizeof(glm::vec2), positions.data(), GL_STATIC_DRAW); // Static draw indicates the buffer will be modified only once, but may be used many times
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0); // Not needed anymore
  
  // Generate VBO of colors
  abcg::glGenBuffers(1, &m_vboColors);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vboColors);
  abcg::glBufferData(GL_ARRAY_BUFFER, 
    colors.size() * sizeof(glm::vec3), colors.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  // Could be omitted, but is kept for sanity.
  const auto positionAttribute{abcg::glGetAttribLocation(m_program, "inPosition")}; // returns 0
  const auto colorAttribute{abcg::glGetAttribLocation(m_program, "inColor")}; // returns 1

  // Create VAO
  // Stores specification state of VBOs with the vertex shader.
  abcg::glGenVertexArrays(1, &m_vao);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_vao);

  // The next lines are executed automatically in a later call of glBindVertexArray,
  // this setup will be later made only once th avoid redundancy.
  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vboPositions);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  //void glVertexAttribPointer(GLuint index,
  //                         GLint size, - number of components of attr (vec2, vec3)
  //                         GLenum type, - type of data of each value
  //                         GLboolean normalized, - should ints be normalized to [-1,1] or [0,1]
  //                         GLsizei stride, - number of bytes in between positions.
  //                         const void * pointer); - displacement in bytes for the first component position.
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glEnableVertexAttribArray(colorAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vboColors);
  abcg::glVertexAttribPointer(colorAttribute, 3, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);
}

void OpenGLWindow::paintGL() {
  // Check whether to render the next polygon
  if (m_elapsedTimer.elapsed() < m_delay / 1000.0) return;
  m_elapsedTimer.restart();

  // Create a regular polygon with a number of sides in the range [3,20]
  std::uniform_int_distribution<int> intDist(3, 20);
  const auto sides{intDist(m_randomEngine)};
  setupModel(sides);

  // Set the viewport
  abcg::glViewport(0, 0, m_viewportWidth, m_viewportHeight);
  // Start using the shader program
  abcg::glUseProgram(m_program);

  // Choose a random xy position from (-1,-1) to (1,1)
  std::uniform_real_distribution<float> rd1(-1.0f, 1.0f);
  const glm::vec2 translation{rd1(m_randomEngine), rd1(m_randomEngine)};
  const GLint translationLocation{
      abcg::glGetUniformLocation(m_program, "translation")};
  abcg::glUniform2fv(translationLocation, 1, &translation.x); // Uniform variable attribution

  // Choose a random scale factor (1% to 25%)
  std::uniform_real_distribution<float> rd2(0.01f, 0.25f);
  const auto scale{rd2(m_randomEngine)};
  const GLint scaleLocation{abcg::glGetUniformLocation(m_program, "scale")};
  abcg::glUniform1f(scaleLocation, scale); // Uniform variable attribution

  // Render
  // Start using VAO
  abcg::glBindVertexArray(m_vao);
  // Draw triangles
  abcg::glDrawArrays(GL_TRIANGLE_FAN, 0, sides + 2);
    //               mode, init index, number of vertices to be processed,...
    // parallel async function
  // End using VAO
  abcg::glBindVertexArray(0);
  // End using the shader program
  abcg::glUseProgram(0);
}

void OpenGLWindow::resizeGL(int width, int height) {
  fmt::print("Resize:w {} h {}\n", width, height);
  // Store new w and h
  m_viewportWidth = width;
  m_viewportHeight = height;

  // Erase color buffer
  abcg::glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLWindow::terminateGL() {
  // Release shader program, VBO and VAO
  abcg::glDeleteProgram(m_program);
  abcg::glDeleteBuffers(1, &m_vboPositions);
  abcg::glDeleteBuffers(1, &m_vboColors);
  abcg::glDeleteVertexArrays(1, &m_vao);
}

void OpenGLWindow::paintUI() {
  abcg::OpenGLWindow::paintUI();

  {
    const auto widgetSize{ImVec2(200, 72)};
    ImGui::SetNextWindowPos(ImVec2(m_viewportWidth - widgetSize.x - 5,
                                   m_viewportHeight - widgetSize.y - 5));
    ImGui::SetNextWindowSize(widgetSize);
    const auto windowFlags{ImGuiWindowFlags_NoResize |
                           ImGuiWindowFlags_NoCollapse |
                           ImGuiWindowFlags_NoTitleBar};
    ImGui::Begin(" ", nullptr, windowFlags);

    ImGui::PushItemWidth(140);
    ImGui::SliderInt("Delay", &m_delay, 0, 200, "%d ms");
    ImGui::PopItemWidth();

    if (ImGui::Button("Clear window", ImVec2(-1, 30))) {
      abcg::glClear(GL_COLOR_BUFFER_BIT);
    }

    ImGui::End();
  }
}