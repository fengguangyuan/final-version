#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormal_modelspace;

out vec3 nor_WorldSpace;

uniform mat4 MVP;
uniform mat4 M;

void main(){

	// Output position of the vertex, in clip space : MVP * position
	vec4 clip_pos =  MVP * vec4(vertexPosition_modelspace,1);
	gl_Position = clip_pos;
	nor_WorldSpace = (M * vec4(vertexNormal_modelspace,1)).xyz;
}

