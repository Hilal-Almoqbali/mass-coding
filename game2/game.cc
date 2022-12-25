#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Vertex
{
  glm::vec3 position;
};

struct Object
{
  std::vector<Vertex> vertices;
};

std::map<std::string, Object> objects;

void parseLine(std::stringstream& lineStream)
{
  std::string type;
  lineStream >> type;
  if (type == "v")
  {
    std::string objectName;
    lineStream >> objectName;
    float x, y, z;
    lineStream >> x >> y >> z;
    Vertex vertex = { glm::vec3(x, y, z) };
    objects[objectName].vertices.push_back(vertex);
  }
}

void loadWorld(const char* filename)
{
  std::ifstream file(filename);
  std::string line;
  while (std::getline(file, line))
{
  std::stringstream lineStream(line);
  parseLine(lineStream);
}

  // Set up the character's position and movement speed as uniform variables
  GLint characterPosLoc = glGetUniformLocation(shaderProgram, "characterPos");
  GLint movementSpeedLoc = glGetUniformLocation(shaderProgram, "movementSpeed");
  glm::vec3 characterPos(0.0f, 0.0f, 0.0f);
  float movementSpeed = 0.1f;
  glUniform3f(characterPosLoc, characterPos.x, characterPos.y, characterPos.z);
  glUniform1f(movementSpeedLoc, movementSpeed);

  // Set the clear color and enable depth testing
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glEnable(GL_DEPTH_TEST);

  // Render loop
  while (!glfwWindowShouldClose(window))
  {
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use the shader program
    glUseProgram(shaderProgram);

    // Update the character's position based on the arrow keys
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
      characterPos.z -= movementSpeed;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
      characterPos.z += movementSpeed;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
      characterPos.x -= movementSpeed;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
      characterPos.x += movementSpeed;
    glUniform3f(characterPosLoc, characterPos.x, characterPos.y, characterPos.z);

    // Render the objects
    for (const auto& object : objects)
    {
      // Bind the vertex buffer object and draw the object
      glBindBuffer(GL_ARRAY_BUFFER, VBOs[object.first]);
      glBufferData(GL_ARRAY_BUFFER, object.second.vertices.size() * sizeof(Vertex), object.second.vertices.data(), GL_STATIC_DRAW);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
      glEnableVertexAttribArray(0);
      glDrawArrays(GL_TRIANGLES, 0, object.second.vertices.size());
      glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    // Swap the buffers and poll for events
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Clean up
  glDeleteBuffers(objects.size(), VBOs);
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  glDeleteProgram(shaderProgram);
  // Terminate GLFW and exit
  glfwTerminate();
  return 0;
}