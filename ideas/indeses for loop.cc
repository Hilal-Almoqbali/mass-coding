#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

struct Vertex
{
  glm::vec3 position;
  glm::vec3 color;
};

std::vector<Vertex> vertices;

// Fill the vertices vector with your vertex data

const GLuint indices[] = {
    0, 1, 2, // First triangle
    2, 3, 0  // Second triangle
};

int main(int argc, char *argv[])
{
  // Initialize GLFW and create a window
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  GLFWwindow *window = glfwCreateWindow(800, 600, "Object", nullptr, nullptr);
  glfwMakeContextCurrent(window);

  // Initialize GLEW
  glewExperimental = GL_TRUE;
  glewInit();

  // Define the viewport dimensions
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);

  // Create a Vertex Array Object (VAO) and a Vertex Buffer Object (VBO)
  GLuint VAO, VBO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  // Bind the VAO and VBO
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

  // Bind the EBO
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // Set the vertex attribute pointers
  GLsizei stride = sizeof(Vertex);
  for (int i = 0; i < 2; i++)
  {
    GLvoid *offset = (GLvoid *)(i * sizeof(GLfloat));
    glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, stride, offset);
    glEnableVertexAttribArray(i);
  }

  // Unbind the VAO
  glBindVertexArray(0);
}