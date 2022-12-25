#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

struct NPC {
  glm::vec3 position;
  AABB collisionVolume;
};

struct AABB {
  glm::vec3 min;
  glm::vec3 max;
};

NPC* intersectRayNPC(const glm::vec3& start, const glm::vec3& end, const std::vector<NPC>& npcs)
{
  // Check if the ray intersects any NPCs
  for (const NPC& npc : npcs) {
    // Check if the ray intersects the NPC's AABB
    float tmin = 0.0f;
    float tmax = std::numeric_limits<float>::max();

    for (int i = 0; i < 3; i++) {
      if (std::abs(end[i] - start[i]) < 1e-6) {
        // Ray is parallel to the AABB plane
        if (start[i] < npc.collisionVolume.min[i] || start[i] > npc.collisionVolume.max[i]) {
          // Ray is outside the AABB
          continue;
        }
      } else {
        // Calculate the intersection with the AABB planes
        float ood = 1.0f / (end[i] - start[i]);
        float t1 = (npc.collisionVolume.min[i] - start[i]) * ood;
        float t2 = (npc.collisionVolume.max[i] - start[i]) * ood;
        if (t1 > t2) {
          std::swap(t1, t2);
        }
        tmin = glm::max(tmin, t1);
        tmax = glm::min(tmax, t2);
      }
    }

    if (tmax >= tmin) {
      // Ray intersects the AABB
      return &npc;
    }
  }

  // Ray does not intersect any NPCs
  return nullptr;
}

int main() {
  std::vector<NPC> npcs = {
    { { 0.0f, 0.0f, 0.0f }, { { -1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f } } },
    { { 5.0f, 0.0f, 0.0f }, { { 4.0f, -1.0f, -1.0f }, { 6.0f, 1.0f, 1.0f } } }
  };

  // Check if the ray intersects any NPCs
  NPC* hit = intersectRayNPC({ 0.0f, 0.0f, 0.0f }, { 10.0f, 0.0f, 0.0f }, npcs);
  if (hit) {
    std::cout << "Ray intersects NPC at position: " << hit->position.x << ", " << hit->position.y << ", " << hit->position.z << std::endl;
  } else {
    std::cout << "Ray does not intersect any NPCs" << std::endl;
  }

  return 0;
}