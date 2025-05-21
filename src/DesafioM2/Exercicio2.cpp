#include <iostream>
#include <string>
#include <assert.h>
#include <vector>

using namespace std;

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

#include <cmath>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

GLuint createTriangle(float x0, float y0, float x1, float y1, float x2, float y2);
int setupShader();
int setupGeometry();

const GLuint WIDTH = 800, HEIGHT = 600;

const GLchar* vertexShaderSource = "#version 400\n"
"layout (location = 0) in vec3 position;\n"
"uniform mat4 projection;\n"
"uniform mat4 model;\n"
"void main()\n"
"{\n"
"gl_Position = projection * model * vec4(position.x, position.y, position.z, 1.0);\n"
"}\0";

const GLchar* fragmentShaderSource = "#version 400\n"
"uniform vec4 inputColor;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = inputColor;\n"
"}\n\0";

struct Triangle {
    vec3 position;
    vec3 dimensions;
    vec3 color;
};

vector<Triangle> triangles;

vector <vec3> colors;
int iColor = 0;

int main()
{
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ola Triangulo! -- Rossana", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    const GLubyte* renderer = glGetString(GL_RENDERER); 
    const GLubyte* version = glGetString(GL_VERSION);
    cout << "Renderer: " << renderer << endl;
    cout << "OpenGL version supported " << version << endl;

    colors.push_back(vec3(0.969, 0.773, 0.800)); 
    colors.push_back(vec3(0.843, 0.737, 0.910)); 
    colors.push_back(vec3(0.722, 0.851, 0.973));
    colors.push_back(vec3(0.776, 0.945, 0.839));
    colors.push_back(vec3(1.000, 0.953, 0.690));
    colors.push_back(vec3(0.980, 0.831, 0.717));
    colors.push_back(vec3(0.973, 0.922, 0.867));
    colors.push_back(vec3(0.905, 0.847, 0.788));
    colors.push_back(vec3(0.749, 0.867, 0.792));
    colors.push_back(vec3(0.792, 0.816, 0.902));
    colors.push_back(vec3(0.945, 0.804, 0.765)); 
    colors.push_back(vec3(0.980, 0.882, 0.851)); 
    colors.push_back(vec3(0.851, 0.906, 0.702)); 
    colors.push_back(vec3(0.918, 0.922, 0.796));  

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    GLuint shaderID = setupShader();

    
    GLuint VAO = createTriangle(-0.1, -0.1, 0.1, -0.1, 0.0, 0.1);

    
    glUseProgram(shaderID);

    GLint colorLoc = glGetUniformLocation(shaderID, "inputColor");

    mat4 projection = ortho(0.0, 800.0, 600.0, 0.0, -1.0, 1.0);  
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "projection"), 1, GL_FALSE, value_ptr(projection));

    mat4 model = mat4(1);
    
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_FALSE, value_ptr(model));

    glUniform4f(colorLoc, 1.0f, 0.0f, 0.5f, 1.0f);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClearColor(1.0f, 0.8f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glLineWidth(10);
        glPointSize(20);

        glBindVertexArray(VAO);

        for (int i = 0; i < triangles.size(); i++)
		{
			mat4 model = mat4(1); 
			
			model = translate(model,vec3(triangles[i].position.x,triangles[i].position.y,0.0));
			model = rotate(model,radians(180.0f),vec3(0.0,0.0,1.0));
			model = scale(model,vec3(triangles[i].dimensions.x,triangles[i].dimensions.y,1.0));

			glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_FALSE, value_ptr(model));
			glUniform4f(colorLoc, triangles[i].color.r, triangles[i].color.g, triangles[i].color.b, 1.0f); 
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
        
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
       double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        cout << xpos << "  " << ypos << endl;

        Triangle tri;
		tri.position = vec3(xpos,ypos,0.0);
		tri.dimensions = vec3(100.0,100.0,1.0);
		tri.color = vec3(colors[iColor].r, colors[iColor].g, colors[iColor].b);
		iColor = (iColor + 1) % colors.size();
		triangles.push_back(tri);
    }
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
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

int setupGeometry()
{
    GLfloat vertices[] = {
        -0.5 , -0.5 , 0.0,
         0.5 , -0.5 , 0.0,
         0.0 ,  0.5 , 0.0,				  
    };

    GLuint VBO, VAO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 

    return VAO;
}

GLuint createTriangle(float x0, float y0, float x1, float y1, float x2, float y2) {
    GLfloat vertices[] = {
        x0, y0, 0.0, 
        x1, y1, 0.0, 
        x2, y2, 0.0, 				  
    };

    GLuint VBO, VAO;
    
    glGenBuffers(1, &VBO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    
    glGenVertexArrays(1, &VAO);
    
    glBindVertexArray(VAO);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    glBindVertexArray(0); 
    
    return VAO; 
}