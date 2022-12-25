#include <glm/glm.hpp>
#include <vector>

// Define the objects in your scene
struct Cube {
  glm::vec3 position;
  float size;
};

// Initialize the objects in your scene
std::vector<Cube> cubes;

bool intersects(const Cube& a, const Cube& b) {
  // Check if the cubes intersect
  for (int i = 0; i < 3; i++) {
    if (fabs(a.position[i] - b.position[i]) > a.size + b.size) return false;
  }
  return true;
}

int main() {
  // Set up the scene
  cubes.push_back({ glm::vec3(0.0f, 0.0f, 0.0f), 1.0f });
  cubes.push_back({ glm::vec3(2.0f, 0.0f, 0.0f), 1.0f });
  
  // Check if any of the cubes intersect
  for (int i = 0; i < cubes.size(); i++) {
    for (int j = i + 1; j < cubes.size(); j++) {
      if (intersects(cubes[i], cubes[j])) {
        std::cout << "The cubes at positions (" << cubes[i].position.x << ", " << cubes[i].position.y << ", " << cubes[i].position.z << ") and (" << cubes[j].position.x << ", " << cubes[j].position.y << ", " << cubes[j].position.z << ") intersect" << std::endl;
      }
    }
  }
  
  return 0;
}
