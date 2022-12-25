#include <iostream>
#include <string>
#include <LuaBridge/LuaBridge.h>

int main() {
  // Create a Lua state and open the Lua interpreter
  luabridge::LuaRef luaState = luabridge::getGlobal(luabridge::newState());

  // Load and execute the game logic written in the Lua file
  try {
    luaState.doFile("game_logic.lua");
  } catch (luabridge::LuaException const& e) {
    std::cerr << "Error loading game logic: " << e.what() << std::endl;
    return 1;
  }

  // Call the game logic function from C++
  luaState["gameLogic"]();

  return 0;
}
