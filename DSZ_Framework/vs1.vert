#version 440 core

uniform mat4 World, View, Projection;

layout(location = 0) in vec3 VertexPosition;

layout(location = 1) in vec3 VertexColor;

layout(location = 2) in vec3 VertexNormal;

layout(location = 3) in vec2 UVCoord;

out vec4 Color;
out vec3 Normal;
out vec2 UV;

void main()
{
	gl_Position = Projection * View * World * vec4(VertexPosition, 1.0);

	Color = vec4(VertexColor, 1.0);

	vec4 n = World * vec4(VertexNormal, 0.0f);
	Normal = normalize(n.xyz);

	UV = UVCoord;
}