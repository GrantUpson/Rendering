#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoordinates;

out vec2 outTextureCoordinates;

void main() {
    outTextureCoordinates = textureCoordinates;
    gl_Position = vec4(position.x, position.y, position.z, 1.0);
}