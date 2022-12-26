#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Global variables for the window and the GUI elements
GLFWwindow* window;
int buttonState = 0;
float sliderValue = 0.5f;
char textInput[256] = "";

// Function prototypes
void drawButton();
void drawSlider();
void drawTextBox();

int main()
{
    // Initialize GLFW and create the window
    glfwInit();
    window = glfwCreateWindow(800, 600, "My GUI", NULL, NULL);
    glfwMakeContextCurrent(window);
    glewInit();

    // Set up the viewport and projection matrix
    glViewport(0, 0, 800, 600);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 800, 600, 0, -1, 1);

    // Set up the GUI elements
    drawButton();
    drawSlider();
    drawTextBox();

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Poll for events and update the button and slider states if necessary
        glfwPollEvents();
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
            buttonState = 1;
        else
            buttonState = 0;
        sliderValue = glfwGetCursorPosX(window) / 800.0f;

        // Clear the screen and draw the GUI elements
        glClear(GL_COLOR_BUFFER_BIT);
        drawButton();
        drawSlider();
        drawTextBox();
        glfwSwapBuffers(window);
    }

    // Clean up and exit
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

// Function definitions
void drawButton()
{
    // Set the color of the button based on its state
    if (buttonState == 0)
        glColor3f(1.0f, 1.0f, 1.0f);
    else
        glColor3f(0.5f, 0.5f, 0.5f);

    // Draw the button
    glBegin(GL_QUADS);
    glVertex2f(100, 100);
    glVertex2f(200, 100);
    glVertex2f(200, 200);
    glVertex2f(100, 200);
    glEnd();
}

void drawButton(float x, float y, float width, float height, const std::string& label, const glm::vec4& bgColor, const glm::vec4& textColor) {
  // Set the color of the button background
  glColor4f(bgColor.r, bgColor.g, bgColor.b, bgColor.a);
  // Draw the button background
  glBegin(GL_QUADS);
  glVertex2f(x, y);
  glVertex2f(x + width, y);
  glVertex2f(x + width, y + height);
  glVertex2f(x, y + height);
  glEnd();

  // Set the color of the label text
  glColor4f(textColor.r, textColor.g, textColor.b, textColor.a);
  // Draw the label text using GLFW
  int textWidth = 0, textHeight = 0;
  glfwGetTextSize(font, label.c_str(), &textWidth, &textHeight);
  glfwDrawText(font, label.c_str(), x + (width - textWidth) / 2, y + (height - textHeight) / 2, textColor.r, textColor.g, textColor.b);
}



void drawSliderThumb(float thumbPercent, int sliderLength, int thumbSize, float thumbColor[4]) {
    // Draw the slider thumb
    int thumbPosX = (int)(thumbPercent * (sliderLength - thumbSize));
    drawRect(thumbPosX, 0, thumbSize, thumbSize, thumbColor);
}

void drawTextBox(float x, float y, int width, int height, const std::string& text, float textColor[4], float bgColor[4]) {
    // Draw the text box background
    drawRect(x, y, width, height, bgColor);

    // Draw the text
    drawText(x, y, text, textColor);
}

void drawRect(float x, float y, int width, int height, float color[4]) {
    // Set the color of the rectangle
    glColor4fv(color);

    // Draw the rectangle
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

void drawText(float x, float y, const std::string& text, const float textColor[4]) {
    // Set the color of the text
    glColor4fv(textColor);

    // Set the position of the text
    glRasterPos2f(x, y);

    // Draw the text
    for (const char& c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
    }
}

void drawText(float x, float y, const char* text, const glm::vec3& textColor) {
// Set up a buffer to hold the text
char buffer[256];
int length = sprintf(buffer, text);

// Create a font atlas and a font shader
stbtt_bakedchar cdata[96];
GLuint ftex;
glGenTextures(1, &ftex);
glBindTexture(GL_TEXTURE_2D, ftex);
glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 512,512, 0, GL_ALPHA, GL_UNSIGNED_BYTE, 0);
stbtt_BakeFontBitmap(stbtt_font_data,0, 32.0f, (unsigned char*)buffer,512,512, 32, 96, cdata);

// Bind the font atlas and draw the text
glBindTexture(GL_TEXTURE_2D, ftex);
for (int i = 0; i < length; ++i) {
stbtt_bakedchar* b = cdata + buffer[i] - 32;
float xpos = x + b->xoff;
float ypos = y + b->yoff;
float w = b->x1 - b->x0;
float h = b->y1 - b->y0;
GLfloat box[4][4] = {
{xpos, ypos + h, 0, 0},
{xpos + w, ypos, 1, 0},
{xpos, ypos, 0, 1},
{xpos + w, ypos + h, 1, 1},
};
glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
}