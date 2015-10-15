#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormal_modelspace;

varying vec2 UV;
varying vec3 Position_worldspace;
varying vec3 Normal_worldspace;

uniform mat4 MVP;
uniform mat4 M;


void main(){

	// Output position of the vertex, in clip space : MVP * position
	vec4 clip_pos =  MVP * vec4(vertexPosition_modelspace,1);
	vec4 world_pos = M * vec4(vertexPosition_modelspace,1);
	Position_worldspace = world_pos.xyz;
	Normal_worldspace = ( M * vec4(vertexNormal_modelspace,1.0)).xyz;
	gl_Position = clip_pos;
	UV = vertexUV;
}

