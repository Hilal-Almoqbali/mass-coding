#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Initialize the crosshair size and color
const float CROSSHAIR_SIZE = 0.1f;
const float CROSSHAIR_R = 1.0f;
const float CROSSHAIR_G = 1.0f;
const float CROSSHAIR_B = 1.0f;

void renderCrosshair(GLFWwindow* window) {
  // Get the window size
  int windowWidth, windowHeight;
  glfwGetWindowSize(window, &windowWidth, &windowHeight);

  // Calculate the crosshair position and size
  float x = (float) windowWidth / 2.0f;
  float y = (float) windowHeight / 2.0f;
  float size = CROSSHAIR_SIZE * (float) windowHeight;

  // Set the crosshair color
  glColor3f(CROSSHAIR_R, CROSSHAIR_G, CROSSHAIR_B);

  // Draw the crosshair quad
  glBegin(GL_QUADS);
  glVertex2f(x - size, y - size);
  glVertex2f(x + size, y - size);
  glVertex2f(x + size, y + size);
  glVertex2f(x - size, y + size);
  glEnd();
}