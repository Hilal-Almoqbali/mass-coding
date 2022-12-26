#include <iostream>
#include <cstdlib>
#include <ctime>

const int NPC_SPEED = 50; // NPC speed in units per second
const int NPC_FIRE_RATE = 500; // NPC fire rate in milliseconds

// NPC state
enum NPCState {
    IDLE,
    CHASING,
    SHOOTING
};

struct NPC {
    glm::vec3 position; // NPC position
    glm::vec3 direction; // NPC direction
    NPCState state; // NPC state
    int health; // NPC health
    int lastShotTime; // Time when the NPC last shot
};

// Update NPC AI logic
void updateNPC(NPC& npc, glm::vec3 playerPosition, int deltaTime)
{
    // Calculate distance to player
    float distance = glm::length(npc.position - playerPosition);

    // If the NPC is too far away, switch to idle state
    if (distance > 100.0f) {
        npc.state = IDLE;
        return;
    }

    // If the NPC is close enough, switch to chasing state
    if (distance < 30.0f) {
        npc.state = CHASING;
    }

    // If the NPC is in the chasing state, move towards the player
    if (npc.state == CHASING) {
        npc.direction = glm::normalize(playerPosition - npc.position);
        npc.position += npc.direction * NPC_SPEED * (deltaTime / 1000.0f);
    }

    // If the NPC is close enough and has a clear line of sight to the player, switch to shooting state
    if (distance < 20.0f && hasClearLineOfSight(npc.position, playerPosition)) {
        npc.state = SHOOTING;
    }

    // If the NPC is in the shooting state, shoot at the player if the fire rate has been reached
    if (npc.state == SHOOTING) {
        int currentTime = getCurrentTime();
        if (currentTime - npc.lastShotTime > NPC_FIRE_RATE) {
            shoot(npc.position, npc.direction);
            npc.lastShotTime = currentTime;
        }
    }
}
