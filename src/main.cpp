#define GLFW_INCLUDE_NONE
#include "shader.h"
#include "stb_image/stb_image.h"
#include "types.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>
#include <algorithm>
#include <cmath>
#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>
#include <iostream>
#include <ostream>

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

    constexpr float vertices[] {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  // bottom left
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f,   // bottom right
         0.5f,  0.5f, 0.0f, 1.0f, 1.0f,     // top right
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f   // top left
    };

    constexpr unsigned int indices[] {
            0, 1, 2,  // first triangle vertices index
            2, 3, 0   // second triangle vertices index
    };

    unsigned int vbo;
    unsigned int vao;
    unsigned int ebo;

    // VBO
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    // Texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // This index (element) buffer is automatically connected to the vao buffer bound when this buffer was
    // bound; meaning if the vao is unbound and rebound, it will automatically use this EBO again without the ebo
    // needing to be explicitly rebound. This also works for the vbo
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Shader stuff
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    const Shader shader("vertex.glsl", "fragment.glsl");
    shader.Use();

    // Texture loading
    unsigned int textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glActiveTexture(GL_TEXTURE0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_set_flip_vertically_on_load(true);
    int width;
    int height;
    int channels;
    const unsigned char* textureData = stbi_load("wall.png", &width, &height, &channels, 0);

    if(!textureData) {
        std::cout << "Failed to load texture" << std::endl;
        return -1;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, channels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, textureData);
    stbi_image_free((void*)textureData);


    const uint32 transformLocation = glGetUniformLocation(shader.ID, "transform");

    // Loop
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Comment this out to disable wireframe
    float currentOpacity = 1.0f;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ProcessInput(window);
        glClear(GL_COLOR_BUFFER_BIT);

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            currentOpacity += 0.01f;
            shader.SetFloat("opacity", currentOpacity);
        }

        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            currentOpacity -= 0.01f;
            shader.SetFloat("opacity", currentOpacity);
        }

        Matrix4 transform(1.0);
        transform = glm::scale(transform, glm::vec3(0.5f, 0.5f, 0.5f));
        transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transform));

        // Draw the vertices using the indices specified by the bound index buffer
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        Matrix4 transform2(1.0);
        float scale = std::sin(glfwGetTime());
        transform2 = glm::translate(transform2, glm::vec3(-0.5f, 0.5f, 0.0f));
        transform2 = glm::scale(transform2, glm::vec3(scale, scale, scale));

        glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transform2));

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
