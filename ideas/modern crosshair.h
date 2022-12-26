#include <string>

class Shader {
public:
  Shader(const std::string& vertexShaderFilepath, const std::string& fragmentShaderFilepath);
  ~Shader();

  void use();

  // Other methods and member variables go here...
};

Shader::Shader(const std::string& vertexShaderFilepath, const std::string& fragmentShaderFilepath) {
  // Load and compile the vertex and fragment shaders from the given filepaths
  // and create a program object from them.
  // Initialize other member variables and perform other necessary setup tasks.
}

Shader::~Shader() {
  // Clean up any resources associated with the Shader object.
}

void Shader::use() {
  // Bind the shader program to the current rendering state.
}


void crossHair(){
Shader crosshairShader("crosshairVertexShader.glsl", "crosshairFragmentShader.glsl");

// Set up the VAO and VBO for the crosshair
unsigned int crosshairVAO, crosshairVBO;
glGenVertexArrays(1, &crosshairVAO);
glGenBuffers(1, &crosshairVBO);
glBindVertexArray(crosshairVAO);
glBindBuffer(GL_ARRAY_BUFFER, crosshairVBO);

// Define the vertices of the crosshair
float crosshairVertices[] = {
  // Top line
  -0.01, 0.05, 0.0,
  0.01, 0.05, 0.0,
  // Bottom line
  -0.01, -0.05, 0.0,
  0.01, -0.05, 0.0,
  // Left line
  -0.05, -0.01, 0.0,
  -0.05, 0.01, 0.0,
  // Right line
  0.05, -0.01, 0.0,
  0.05, 0.01, 0.0
};

// Store the vertices in the VBO
glBufferData(GL_ARRAY_BUFFER, sizeof(crosshairVertices), crosshairVertices, GL_STATIC_DRAW);

// Set up the vertex attributes for the crosshair
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);

// Unbind the VAO and VBO
glBindVertexArray(0);
glBindBuffer(GL_ARRAY_BUFFER, 0);

// Draw the crosshair
crosshairShader.use();
glBindVertexArray(crosshairVAO);
glDrawArrays(GL_LINES, 0, 8);
glBindVertexArray(0);
}