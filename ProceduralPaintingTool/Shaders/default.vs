#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in vec4 a_Color;

uniform mat4 u_Model;
uniform mat4 u_ViewProjection;

out vec3 f_World;
out vec3 f_Normal;
out vec4 f_Color;

void main()
{
	gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);

	f_World = (u_Model * vec4(a_Position, 1.0)).xyz;
	f_Normal = normalize((u_Model * vec4(a_Normal, 0.0)).xyz);

	f_Color = a_Color;
}