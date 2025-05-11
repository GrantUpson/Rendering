#version 430 core

in vec2 outTextureCoordinates;
out vec4 FragColor;

uniform sampler2D wallTexture;
uniform float opacity = 1.0f;

void main() {
   vec4 colour = texture(wallTexture, outTextureCoordinates);
   FragColor = vec4(colour.rgb, opacity);
}