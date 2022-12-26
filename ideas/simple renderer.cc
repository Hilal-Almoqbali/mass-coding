#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

struct Node {
  glm::vec3 position;
  glm::vec3 color;
};

class Renderer {
public:
  Renderer() {
    // Initialize GLFW and create a window
    glfwInit();
    window_ = glfwCreateWindow(640, 480, "Renderer", nullptr, nullptr);
    glfwMakeContextCurrent(window_);

    // Initialize GLEW
    glewInit();

    // Set up OpenGL state
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
  }

  ~Renderer() {
    // Clean up GLFW and OpenGL resources
    glfwDestroyWindow(window_);
    glfwTerminate();
  }

  void Render(const std::vector<Node>& nodes) {
    // Clear the screen and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set up the projection matrix
    int width, height;
    glfwGetFramebufferSize(window_, &width, &height);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);

    // Set up the view matrix
    glm::mat4 view = glm::lookAt(
      glm::vec3(0.0f, 0.0f, 3.0f), // camera position
      glm::vec3(0.0f, 0.0f, 0.0f), // target position
      glm::vec3(0.0f, 1.0f, 0.0f)  // up vector
    );

    // Set up the model matrix
    glm::mat4 model = glm::mat4(1.0f);

    // Set up the model-view-projection matrix
    glm::mat4 mvp = projection * view * model;

    // Set up the vertex array and buffer objects
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Set up the vertex data
    std::vector<float> vertexData;
    for (const Node& node : nodes) {
      vertexData.push_back(node.position.x);
      vertexData.push_back(node.position.y);
      vertexData.push_back(node.position.z);
      vertexData.push_back(node.color.x);
      vertexData.push_back(node.color.y);
      vertexData.push_back(node.color.z);
    }

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    void Node::draw(Shader& shader) {
  // Bind the model matrix to the shader
  shader.setMat4("model", model);
  
  // Bind the node's texture to the shader
  texture.bind();
  
  // Draw the node's mesh
  mesh.draw();
  
  // Draw the node's children
  for (auto& child : children) {
    child.draw(shader);
  }
}

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texCoords;
};

class Mesh {
 public:
  Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices);
  void draw() const;
  void bind() const;

 private:
  GLuint VAO, VBO, EBO;
};

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices) {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

  // Vertex Positions
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
  // Vertex Normals
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
  // Vertex Texture Coords
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texCoords));

  glBindVertexArray(0);
}

void Mesh::draw(const Camera& camera, const glm::mat4& modelMatrix)
{
    // Use the appropriate shader program
    glUseProgram(shaderProgram);

    // Set the uniform variables for the model matrix and view-projection matrix
    GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    GLint viewProjLoc = glGetUniformLocation(shaderProgram, "viewProj");
    glUniformMatrix4fv(viewProjLoc, 1, GL_FALSE, glm::value_ptr(camera.getViewProjection()));

    // Bind the vertex array object and draw the mesh
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
}

void Mesh::bind() {
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
}