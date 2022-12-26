#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

const char* compute_shader_source = R"(
    #version 450
    layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
    layout (rgba32f, binding = 0) writeonly uniform image2D output_image;
    void main() {
        ivec2 store_pos = ivec2(gl_GlobalInvocationID.xy);
        imageStore(output_image, store_pos, vec4(0.0, 1.0, 0.0, 1.0));
    }
)";

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Create window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    GLFWwindow* window = glfwCreateWindow(800, 600, "Compute Shader", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Create compute shader
    GLuint compute_shader = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(compute_shader, 1, &compute_shader_source, nullptr);
    glCompileShader(compute_shader);

    // Check for shader compilation errors
    GLint success;
    GLchar info_log[512];
    glGetShaderiv(compute_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(compute_shader, 512, nullptr, info_log);
        std::cerr << "Failed to compile compute shader: " << info_log << std::endl;
        glfwTerminate();
        return -1;
    }

    // Create compute program
    GLuint compute_program = glCreateProgram();
   glAttachShader(compute_program, compute_shader);
glLinkProgram(compute_program);

// Check for program linking errors
glGetProgramiv(compute_program, GL_LINK_STATUS, &success);
if (!success) {
    glGetProgramInfoLog(compute_program, 512, nullptr, info_log);
    std::cerr << "Failed to link compute program: " << info_log << std::endl;
    glfwTerminate();
    return -1;
}

glUseProgram(compute_program);
glDispatchCompute(800 / 16, 600 / 16, 1);

// Create output texture
GLuint output_texture;
glGenTextures(1, &output_texture);
glBindTexture(GL_TEXTURE_2D, output_texture);
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 800, 600, 0, GL_RGBA, GL_FLOAT, nullptr);
glBindImageTexture(0, output_texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

// Read output texture
std::vector<float> pixels(800 * 600 * 4);
glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, pixels.data());

// Display output texture
// ...
