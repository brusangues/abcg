#include "openglwindow.hpp"

#include <fmt/core.h>
#include <imgui.h>

#include <chrono>

void OpenGLWindow::initializeGL() {
  // Start pseudo-random number generator
  auto seed{std::chrono::steady_clock::now().time_since_epoch().count()};
  m_randomEngine.seed(seed);

  // Randomly choose a pair of coordinates in the interval [-1; 1]
  std::uniform_real_distribution<float> realDistribution(-1.0f, 1.0f);
  m_P.x = realDistribution(m_randomEngine);
  m_P.y = realDistribution(m_randomEngine);
}

void OpenGLWindow::paintGL() {
  // Randomly choose a triangle vertex index
  std::uniform_int_distribution<int> intDistribution(0, m_points.size() - 1);
  int index{intDistribution(m_randomEngine)};

  // The new position is the midpoint between the current position and the
  // chosen vertex
  m_P = (m_P + m_points.at(index)) / 2.0f;

  // Print coordinates to the console
  fmt::print("({:+.2f}, {:+.2f})\n", m_P.x, m_P.y);
}