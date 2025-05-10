#define GLFW_INCLUDE_NONE
#include <cmath>
#include <iostream>
#include <ostream>
#include <GLFW/glfw3.h>
#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>

#include "shader.h"

using namespace gl;


static void ErrorCallback(int error, const char* description) {
    std::cout << "GLFW Error: " << description << std::endl;
}


static void ProcessInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

GLFWwindow* window = nullptr;

int main() {
    if (!glfwInit()) {
        return -1;
    }

    glfwSetErrorCallback(ErrorCallback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    window = glfwCreateWindow(1600, 1200, "Rendering", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -2;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glbinding::initialize(glfwGetProcAddress);
    glViewport(0, 0, 1600, 1200);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);


    // OpenGL stuff

    constexpr float vertices1[] = {
        -0.5f, -0.5f, 0.0f, 1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 0.0f,  1.0f, 0.0f,
         0.0f,  0.5f, 0.0f, 0.0f,  0.0f, 1.0f
    };

    unsigned int indices[] {
        0, 1, 2, // first triangle vertices index
        //3, 1, 4  // second triangle vertices index
    };

    unsigned int vbo;
    unsigned int vao;
    unsigned int ebo;

    // VBO 1
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

    // VAO 1
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // This index (element) buffer is automatically connected to the vao buffer bound when this buffer was
    // bound; meaning if the vao is unbound and rebound, it will automatically use this EBO again without the ebo
    // needing to be explicitly rebound. This also works for the vbo
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Shader stuff

    Shader shader("vertex.glsl", "fragment.glsl");
    shader.Use();

    // Loop
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Comment this out to disable wireframe

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ProcessInput(window);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw the vertices using the indices specified by the bound index buffer
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
