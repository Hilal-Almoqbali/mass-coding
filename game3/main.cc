#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
};

struct Mesh {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
};

struct Model {
  std::vector<Mesh> meshes;
};

struct NPC {
  Model model;
  glm::vec3 position;
};

struct Player {
  Model model;
  glm::vec3 position;
  glm::vec3 direction;
};

struct World {
  Model model;
  std::vector<NPC> npcs;
};

// Reads a Model from a .obj file
Model readModelFromOBJ(const std::string& filepath) {
  std::ifstream file(filepath);
  if (!file.is_open()) {
    std::cerr << "Error: Could not open file: " << filepath << std::endl;
    exit(EXIT_FAILURE);
  }

  Model model;
  Mesh mesh;

  std::string line;
  while (std::getline(file, line)) {
    std::istringstream iss(line);
    std::string token;
    iss >> token;

    if (token == "v") {
      Vertex vertex;
      iss >> vertex.position.x >> vertex.position.y >> vertex.position.z;
      mesh.vertices.push_back(vertex);
    } else if (token == "vn") {
      glm::vec3 normal;
      iss >> normal.x >> normal.y >> normal.z;
      for (Vertex& vertex : mesh.vertices) {
        vertex.normal = normal;
      }
    } else if (token == "f") {
      std::string v1, v2, v3;
      iss >> v1 >> v2 >> v3;
      mesh.indices.push_back(std::stoi(v1.substr(0, v1.find('/'))) - 1);
      mesh.indices.push_back(std::stoi(v2.substr(0, v2.find('/'))) - 1);
      mesh.indices.push_back(std::stoi(v3.substr(0, v3.find('/'))) - 1);
    }
  }

  model.meshes.push_back(mesh);
  return model;
}

// Reads a World from a .txt file
World readWorldFromTXT(const std::string& filepath) {
  World world;

  // Open the world file
  std::ifstream file(filepath);
  if (!file.is_open()) {
    std::cerr << "Error: Failed to open file " << filepath << std::endl;
    return world;
  }

  // Read the file line by line
  std::string line;
  while (std::getline(file, line)) {
    // Split the line into tokens
    std::istringstream iss(line);
    std::vector<std::string> tokens;
    std::copy(std::istream_iterator<std::string>(iss),
              std::istream_iterator<std::string>(),
              std::back_inserter(tokens));

    // Process the tokens based on their type
    if (tokens.empty()) continue;
    if (tokens[0] == "v") {
      // Add a vertex to the world
      if (tokens.size() < 4) continue;
      world.vertices.emplace_back(std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]));
    } else if (tokens[0] == "f") {
      // Add a triangle to the world
      if (tokens.size() < 4) continue;
      world.triangles.emplace_back(std::stoul(tokens[1]), std::stoul(tokens[2]), std::stoul(tokens[3]));
    }
  }

  // Close the file
  file.close();

  return world;
}

void renderWorld(World &world) {
  // Render the ground model
  glBindVertexArray(world.model.vao);
  glDrawArrays(GL_TRIANGLES, 0, world.model.numVertices);
  glBindVertexArray(0);

  // Render the NPC models
  for (const NPC &npc : world.npcs) {
    glBindVertexArray(npc.model.vao);
    glDrawArrays(GL_TRIANGLES, 0, npc.model.numVertices);
    glBindVertexArray(0);
  }
}


// Update player position based on input
if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
  player.position += player.forward * player.speed * deltaTime;
}
if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
  player.position -= player.forward * player.speed * deltaTime;
}
if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
  player.position -= glm::normalize(glm::cross(player.forward, player.up)) * player.speed * deltaTime;
}
if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
  player.position += glm::normalize(glm::cross(player.forward, player.up)) * player.speed * deltaTime;
}

// Update player orientation based on mouse input
double xpos, ypos;
glfwGetCursorPos(window, &xpos, &ypos);
double dx = xpos - player.lastX;
double dy = ypos - player.lastY;
player.yaw += dx * player.sensitivity;
player.pitch += dy * player.sensitivity;
player.lastX = xpos;
player.lastY = ypos;

// Clamp pitch to avoid gimbal lock
if (player.pitch > 89.0) {
  player.pitch = 89.0;
}
if (player.pitch < -89.0) {
  player.pitch = -89.0;
}

// Update player forward and up vectors based on yaw and pitch
glm::vec3 front;
front.x = cos(glm::radians(player.yaw)) * cos(glm::radians(player.pitch));
front.y = sin(glm::radians(player.pitch));
front.z = sin(glm::radians(player.yaw)) * cos(glm::radians(player.pitch));
player.forward = glm::normalize(front);
player.up = glm::normalize(glm::cross(player.forward, glm::vec3(-1.0, 0.0, 0.0)));

for (NPC &npc : world.npcs) {
  // Calculate distance between player and NPC
  glm::vec3 d = player.position - npc.position;
  float distance = glm::length(d);

  // If the distance is less than the collision threshold, there is a collision
  if (distance < COLLISION_THRESHOLD) {
    // Handle the collision (e.g. reduce player health, trigger NPC animation, etc.)
  }
}

