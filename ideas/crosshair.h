#include <GLFW/glfw3.h>

// Set up crosshair geometry (two perpendicular lines)
float vertices[] = {
  -0.05f,  0.0f, 0.0f,
   0.05f,  0.0f, 0.0f,
   0.0f, -0.05f, 0.0f,
   0.0f,  0.05f, 0.0f
};

// Vertex shader source code
const char* vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";

// Fragment shader source code
const char* fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                   "}\0";

// Create and compile the vertex shader
unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
glCompileShader(vertexShader);

// Create and compile the fragment shader
unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
glCompileShader(fragmentShader);

GLuint shaderProgram;

shaderProgram = glCreateProgram();

// Attach shaders to the program
glAttachShader(shaderProgram, vertexShader);
glAttachShader(shaderProgram, fragmentShader);

// Link the program
glLinkProgram(shaderProgram);

// Check for errors
GLint success;
glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
if (!success) {
  // Print error message and clean up
  GLchar infoLog[512];
  glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
  std::cerr << "Error linking shader program: " << infoLog << std::endl;
  glDeleteProgram(shaderProgram);
}

// Create a VBO and store our crosshair vertices
GLuint vbo;
glGenBuffers(1, &vbo);
glBindBuffer(GL_ARRAY_BUFFER, vbo);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

// Create a VAO to specify the layout of our vertices
GLuint vao;
glGenVertexArrays(1, &vao);
glBindVertexArray(vao);
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);

// Set up a projection matrix to position our crosshair in the center of the window
int width, height;
glfwGetFramebufferSize(window, &width, &height);
glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
glm::mat4 model = glm::mat4(1.0f);
glm::mat4 mvp = projection * view * model;

// In your rendering loop:
glUseProgram(shaderProgram);
glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, &mvp[0][0]);
glBindVertexArray(vao);