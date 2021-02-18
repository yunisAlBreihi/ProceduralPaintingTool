#version 330 core
in vec3 f_Position;

out vec4 o_Color;

void main()
{
	o_Color = vec4(f_Position, 1.0);
}