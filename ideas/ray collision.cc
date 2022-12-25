#include <glm/glm.hpp>
#include <vector>

// Define the objects in your scene
struct Cube {
  glm::vec3 position;
  float size;
};

// Initialize the objects in your scene
std::vector<Cube> cubes;

bool intersects(const glm::vec3& origin, const glm::vec3& direction, const Cube& cube) {
  // Check if the ray intersects the cube
  float tmin = -INFINITY;
  float tmax = INFINITY;
  for (int i = 0; i < 3; i++) {
    // Compute the intersection distance for each axis
    float t1 = (cube.position[i] - cube.size - origin[i]) / direction[i];
    float t2 = (cube.position[i] + cube.size - origin[i]) / direction[i];
    if (t1 > t2) std::swap(t1, t2);
    if (t1 > tmin) tmin = t1;
    if (t2 < tmax) tmax = t2;
    if (tmin > tmax) return false;
  }
  return true;
}

int main() {
  // Set up the scene
  cubes.push_back({ glm::vec3(0.0f, 0.0f, 0.0f), 1.0f });
  cubes.push_back({ glm::vec3(2.0f, 0.0f, 0.0f), 1.0f });
  
  // Set up the ray
  glm::vec3 origin(0.0f, 0.0f, 2.0f);
  glm::vec3 direction(0.0f, 0.0f, -1.0f);
  
  // Check if the ray intersects any of the cubes
  for (const auto& cube : cubes) {
    if (intersects(origin, direction, cube)) {
      std::cout << "The ray intersects the cube at position (" << cube.position.x << ", " << cube.position.y << ", " << cube.position.z << ")" << std::endl;
    }
  }
  
  return 0;
}
