#include <algorithm>
#include <array>
#include <cmath>
#include <random>
#include <string>

#include <GLFW/glfw3.h>

#ifdef _MSC_VER
#pragma warning(push, 0)
#endif
#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#undef GLAD_GL_IMPLEMENTATION
#ifdef _MSC_VER
#pragma warning(pop)
#endif

#include "context_manager.h"

namespace {
constexpr int MAX_EDGES = 100;
bool isPaused = false;
}  // namespace

void drawCircle(int edges, const std::array<float, MAX_EDGES * 3>& colors);

void keyCallback(GLFWwindow* window, int key, int, int action, int) {
  // There are three actions: press, release, hold
  if (action != GLFW_PRESS) return;
  switch (key) {
    case GLFW_KEY_ESCAPE:
      // Esc
      glfwSetWindowShouldClose(window, GL_TRUE);
      break;
    case GLFW_KEY_SPACE:
      // Space
      isPaused = !isPaused;
      break;
  }
}

int main() {
  // Generate some random float for colors
  std::random_device rd;
  std::mt19937 mte(rd());
  std::uniform_real_distribution<float> distribution(0.25f, 1.0f);
  std::array<float, MAX_EDGES * 3> colors;
  std::generate(colors.begin(), colors.end(), [&]() { return distribution(mte); });
  // Initialize OpenGL context, details are wrapped in OpenGLContext class.
#ifdef __APPLE__
  // MacOS need explicit request legacy support
  OpenGLContext::createContext(21, GLFW_OPENGL_ANY_PROFILE);
#else
  OpenGLContext::createContext(41, GLFW_OPENGL_COMPAT_PROFILE);
#endif
  GLFWwindow* window = OpenGLContext::getWindow();
  glfwSetFramebufferSizeCallback(window, OpenGLContext::framebufferResizeCallback);
  glfwSetKeyCallback(window, keyCallback);
  glfwSetWindowSize(window, 800, 800);
#ifndef NDEBUG
  OpenGLContext::printSystemInfo();
  // Slow, but very useful for debugging.
  OpenGLContext::enableDebugCallback();
#endif
  // More obvious line.
  glLineWidth(3);
  // Speed depends on refresh rate to give similar results
  int speed = OpenGLContext::getRefreshRate() / 10;
  int i = 0;
  // Main rendering loop
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    // GL_XXX_BIT can simply "OR" together to use.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Change edge count over time.
    if (!isPaused) {
      // These two line can be merged together
      // (++i) %= (speed * (MAX_EDGES - 2))
      ++i;
      i %= (speed * (MAX_EDGES - 2));
    }
    drawCircle(i / speed + 3, colors);
    // Change window title based on current edges
    std::string title = "Edges: " + std::to_string(i / speed + 3);
    glfwSetWindowTitle(window, title.c_str());
    // Some platform need explicit glFlush
    glFlush();
    glfwSwapBuffers(window);
  }
  return 0;
}

void drawCircle(int edges, const std::array<float, MAX_EDGES * 3>& colors) {
  edges = std::max(edges, 3);
  // Draw a regular polygon
  glBegin(GL_LINE_LOOP);
  for (int i = 0; i <= edges; ++i) {
    float angle = static_cast<float>(2 * M_PI * i) / edges;
    float x = 0.75f * std::cos(angle);
    float y = 0.75f * std::sin(angle);
    glColor3fv(colors.data() + 3LL * i);
    glVertex2f(x, y);
  }
  glEnd();
}
