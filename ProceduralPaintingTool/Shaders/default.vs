#version 330 core
uniform mat4 u_Model;
uniform mat4 u_ViewProjection;

layout(location = 0) in vec3 a_Position;

out vec3 f_Position;

void main()
{
	vec4 pos = u_ViewProjection * u_Model * vec4(a_Position, 1.0);
	gl_Position = pos;
	f_Position = gl_Position.xyz;
}