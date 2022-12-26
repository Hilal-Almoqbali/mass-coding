#include <fstream>
#include <sstream>
#include <vector>

struct Node {
  std::string mesh;
  std::string texture;
  glm::vec3 position;
  glm::vec3 rotation;
  glm::vec3 scale;
};

std::vector<Node> readNodesFromFile(const std::string& filepath) {
  std::vector<Node> nodes;

  std::ifstream file(filepath);
  std::string line;
  while (std::getline(file, line)) {
    if (line.empty() || line[0] == '#') continue;

    std::istringstream iss(line);
    std::string type;
    iss >> type;

    if (type == "player" || type == "npc") {
      Node node;
      while (iss >> line) {
        size_t separator = line.find(':');
        std::string key = line.substr(0, separator);
        std::string value = line.substr(separator + 2, line.size() - separator - 3); // +2 and -3 to skip the ": " and ")" characters

        if (key == "mesh") node.mesh = value;
        else if (key == "texture") node.texture = value;
        else if (key == "position") {
          std::istringstream(value) >> node.position.x >> node.position.y >> node.position.z;
        }
        else if (key == "rotation") {
          std::istringstream(value) >> node.rotation.x >> node.rotation.y >> node.rotation.z;
        }
        else if (key == "scale") {
          std::istringstream(value) >> node.scale.x >> node.scale.y >> node.scale.z;
        }
      }
      nodes.push_back(node);
    }
  }

  return nodes;
}

/* the file:
# this is a comment lol
player
{
  mesh: "player.obj"
  texture: "player.png"
  position: (0.0, 0.0, 0.0)
  rotation: (0.0, 0.0, 0.0)
  scale: (1.0, 1.0, 1.0)
}
npc
{
  mesh: "npc.obj"
  texture: "npc.png"
  position: (5.0, 9.0, 6.0)
  rotation: (1.0, 0.0, 0.0)
  scale: (1.0, 1.0, 1.0)
}

*/