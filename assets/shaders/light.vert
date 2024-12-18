#version 330 core

out Varyings
{
    vec4 color;
    vec2 textureCoordinates;
    vec3 normal;
    vec3 view;
    vertexWorldPosition;
} Vs_outData;

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 textureCoordinates;
layout(location = 3) in vec3 normal;

uniform mat4 M;
uniform mat4 VP;
uniform mat4 M_IT;
uniform vec3 cameraPosition;

void main()
{
    vec3 vertexWorldPosition = (M * vec4(position, 1.0)).xyz; // tranform the vertex position to world space
    gl_Position = VP*vec4(vertexWorldPosition, 1.0); // tranform the vertex position to clip space
    Vs_outData.color = color;
    Vs_outData.textureCoordinates = textureCoordinates;
    Vs_outData.normal = normalize((M_IT * vec4(normal, 0.0)).xyz);
    Vs_outData.view = cameraPosition-vertexWorldPosition; // camera position in world space
    Vs_outData.vertexWorldPosition = vertexWorldPosition;
}