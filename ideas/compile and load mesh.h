#include <fstream>
#include <iostream>
#include <vector>

struct Mesh {
  std::vector<glm::vec3> vertices;
  std::vector<glm::vec3> normals;
  std::vector<glm::uvec3> triangles;
};

void saveMeshToBIN(const Mesh& mesh, const std::string& filepath) {
  // Open the output file
  std::ofstream file(filepath, std::ios::binary);
  if (!file.is_open()) {
    std::cerr << "Error: Failed to open file " << filepath << " for writing" << std::endl;
    return;
  }

  // Write the number of vertices and triangles
  uint32_t numVertices = mesh.vertices.size();
  uint32_t numTriangles = mesh.triangles.size();
  file.write(reinterpret_cast<char*>(&numVertices), sizeof(numVertices));
  file.write(reinterpret_cast<char*>(&numTriangles), sizeof(numTriangles));

  // Write the vertex and normal data
  for (const auto& vertex : mesh.vertices) {
    file.write(reinterpret_cast<const char*>(&vertex), sizeof(vertex));
  }
  for (const auto& normal : mesh.normals) {
    file.write(reinterpret_cast<const char*>(&normal), sizeof(normal));
  }

  // Write the triangle indices
  for (const auto& triangle : mesh.triangles) {
    file.write(reinterpret_cast<const char*>(&triangle), sizeof(triangle));
  }

  // Close the file
  file.close();
}


Mesh loadMeshFromBIN(const std::string& filepath) {
  Mesh mesh;

  // Open the input file
  std::ifstream file(filepath, std::ios::binary);
  if (!file.is_open()) {
    std::cerr << "Error: Failed to open file " << filepath << " for reading" << std::endl;
    return mesh;
  }

  // Read the number of vertices and triangles
  uint32_t numVertices, numTriangles;
  file.read(reinterpret_cast<char*>(&numVertices), sizeof(numVertices));
  file.read(reinterpret_cast<char*>(&numTriangles), sizeof(numTriangles));

  // Resize the vertex and normal vectors
  mesh.vertices.resize(numVertices);
  mesh.normals.resize(numVertices);

  // Read the vertex and normal data
  for (auto& vertex : mesh.vertices) {
    file.read(reinterpret_cast<char*>(&vertex), sizeof(vertex));
  }
  for (auto& normal : mesh.normals) {
    file.read(reinterpret_cast<char*>(&normal), sizeof(normal));
  }

  // Read the triangle indices
  mesh.triangles.resize(numTriangles);
  for (auto& triangle : mesh.triangles) {
    file.read(reinterpret_cast<char*>(&triangle), sizeof(triangle));
  }

  // Close the file
  file.close();

  return mesh;
}
