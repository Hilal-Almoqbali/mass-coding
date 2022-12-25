#include "imgui.h"
#include "imguifilesystem.h"
#include <fstream>

int main() {
  // Initialize dear imgui
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  ImGui::StyleColorsDark();

  // Main loop
  while (true) {
    // Start the ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Create the main window
    ImGui::Begin("Test Editor");

    // Add a text input field
    static char inputText[256] = "Hello, World!";
    ImGui::InputText("Text Input", inputText, 256);

    // Add a slider
    static float sliderValue = 0.5f;
    ImGui::SliderFloat("Slider", &sliderValue, 0.0f, 1.0f);

    // Add a save button
    if (ImGui::Button("Save")) {
      std::ofstream file("test.txt");
      file << inputText << std::endl << sliderValue << std::endl;
      file.close();
    }

    // Add a file explorer
    static char path[256] = "";
    if (ImGuiFileDialog::FileDialog("Open File", ".txt\0.png\0.jpg\0\0", path, 256, ".txt")) {
      std::ifstream file(path);
      file >> inputText >> sliderValue;
      file.close();
    }

    // End the main window
    ImGui::End();

    // Render the GUI
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Swap buffers and poll events
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Clean up dear imgui
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  return 0;
}
