#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoordinates;

out vec2 outTextureCoordinates;
uniform mat4 transform;

void main() {
    outTextureCoordinates = textureCoordinates;
    gl_Position = transform * vec4(position, 1.0);
}