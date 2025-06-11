#include <iostream>
#include <string>
#include <vector>
#include <assert.h>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
int setupShader();
GLuint setupQuadVAO();
GLuint loadTexture(const string& filePath);

const GLuint WIDTH = 800, HEIGHT = 600;

const GLchar *vertexShaderSource = R"(
#version 400
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texc;

uniform mat4 model;
uniform mat4 projection;

out vec3 vColor;
out vec2 tex_coord;

void main() {
    vColor = color;
    tex_coord = texc;
    gl_Position = projection * model * vec4(position, 1.0);
}
)";

const GLchar *fragmentShaderSource = R"(
#version 400
in vec3 vColor;
in vec2 tex_coord;
out vec4 color;
uniform sampler2D tex_buff;

void main() {
    color = texture(tex_buff, tex_coord);
}
)";

class Sprite {
public:
    GLuint vao;
    GLuint textureID;
    glm::vec2 position;
    glm::vec2 scale;
    float rotation;
    GLuint shaderID;

    Sprite(GLuint vao, GLuint textureID, GLuint shaderID)
        : vao(vao), textureID(textureID), shaderID(shaderID),
          position(0.0f), scale(1.0f), rotation(0.0f) {}

    void draw(const glm::mat4 &projection) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(position, 0.0f));
        model = glm::translate(model, glm::vec3(0.5f * scale.x, 0.5f * scale.y, 0.0f));
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 0, 1));
        model = glm::translate(model, glm::vec3(-0.5f * scale.x, -0.5f * scale.y, 0.0f));
        model = glm::scale(model, glm::vec3(scale, 1.0f));

        glUseProgram(shaderID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glBindVertexArray(vao);

        glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shaderID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }
};

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Sprites com Transformações", nullptr, nullptr);
    if (!window)
    {
        cerr << "Falha ao criar a janela GLFW" << endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cerr << "Falha ao inicializar GLAD" << endl;
        return -1;
    }

    glViewport(0, 0, WIDTH, HEIGHT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLuint shaderID = setupShader();
    GLuint quadVAO = setupQuadVAO();
    glm::mat4 projection = glm::ortho(0.0f, float(WIDTH), 0.0f, float(HEIGHT), -1.0f, 1.0f);

    GLuint texBackground = loadTexture("../assets/tex/1.png");
    GLuint texPink = loadTexture("../assets/tex/pink.png");
    GLuint texGreen = loadTexture("../assets/tex/green.png");
    GLuint texWall = loadTexture("../assets/tex/pixelWall.png");
    GLuint texBlue = loadTexture("../assets/tex/blue.png");
    GLuint texRain = loadTexture("../assets/tex/rain.png");
    GLuint texColorful = loadTexture("../assets/tex/colorful.png");
    GLuint texLightColorful = loadTexture("../assets/tex/light-colorful.png");
    GLuint texPurple = loadTexture("../assets/tex/purple.png");

    vector<Sprite> sprites;

    // Fundo cobrindo toda a janela
    sprites.emplace_back(quadVAO, texBackground, shaderID);
    sprites.back().position = glm::vec2(0, 0);
    sprites.back().scale = glm::vec2(WIDTH, HEIGHT);

    // Sprites com tamanhos variados: horizontais e verticais
    sprites.emplace_back(quadVAO, texPink, shaderID);
    sprites.back().position = glm::vec2(50, 400);
    sprites.back().scale = glm::vec2(180, 90); // horizontal

    sprites.emplace_back(quadVAO, texGreen, shaderID);
    sprites.back().position = glm::vec2(250, 400);
    sprites.back().scale = glm::vec2(80, 180); // vertical

    sprites.emplace_back(quadVAO, texWall, shaderID);
    sprites.back().position = glm::vec2(400, 400);
    sprites.back().scale = glm::vec2(200, 100); // horizontal grande

    sprites.emplace_back(quadVAO, texBlue, shaderID);
    sprites.back().position = glm::vec2(650, 400);
    sprites.back().scale = glm::vec2(60, 160); // vertical fino

    sprites.emplace_back(quadVAO, texRain, shaderID);
    sprites.back().position = glm::vec2(100, 200);
    sprites.back().scale = glm::vec2(150, 70); // horizontal pequeno

    sprites.emplace_back(quadVAO, texColorful, shaderID);
    sprites.back().position = glm::vec2(300, 200);
    sprites.back().scale = glm::vec2(90, 150); // vertical médio

    sprites.emplace_back(quadVAO, texLightColorful, shaderID);
    sprites.back().position = glm::vec2(500, 200);
    sprites.back().scale = glm::vec2(130, 120); // quase quadrado

    sprites.emplace_back(quadVAO, texPurple, shaderID);
    sprites.back().position = glm::vec2(650, 150);
    sprites.back().scale = glm::vec2(100, 200); // vertical largo

    glUseProgram(shaderID);
    glUniform1i(glGetUniformLocation(shaderID, "tex_buff"), 0);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        for (auto &sprite : sprites)
            sprite.draw(projection);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &quadVAO);
    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

int setupShader()
{
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        cout << "Erro ao compilar Vertex Shader:\n" << infoLog << endl;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        cout << "Erro ao compilar Fragment Shader:\n" << infoLog << endl;
    }

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        cout << "Erro ao linkar Shader Program:\n" << infoLog << endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;
}

GLuint setupQuadVAO()
{
    float vertices[] = {
        0.0f, 0.0f, 0.0f, 1, 0, 0, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0, 1, 0, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 0, 0, 1, 1.0f, 1.0f,

        0.0f, 0.0f, 0.0f, 1, 0, 0, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 0, 0, 1, 1.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 0, 1, 1, 0.0f, 1.0f
    };

    GLuint VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    return VAO;
}

GLuint loadTexture(const string& filePath)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height, nrChannels;
    unsigned char *data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        cout << "Falha ao carregar a textura: " << filePath << endl;
    }

    stbi_image_free(data);
    return textureID;
}