#version 330 core
in vec3 f_World;
in vec3 f_Normal;

uniform vec3 u_DirectionalLight;
uniform vec3 u_EyePosition;

out vec4 o_Color;

const vec3 AMBIENT = vec3(1.0, 1.0, 1.0);
const float AMBIENT_STRENGTH = 0.15;
const float SPEC_EXPONENT = 32.0;
const float SPEC_INTENSITY = 0.8;

vec3 light_func(vec3 light_dir, vec3 color, float intensity)
{
	//Diffuse light 
	float diffuse = dot(f_Normal, -light_dir);
	diffuse = clamp(diffuse, 0.0, 1.0);

	//Ambient light
	vec3 ambient = AMBIENT * AMBIENT_STRENGTH;

	//Specular light (using Blinn - Phong)
	vec3 eye_dir = normalize(u_EyePosition - f_World);
	vec3 middle = normalize(eye_dir + -light_dir);

	float specular = dot(f_Normal, middle);
	specular = max(specular, 0.0);
	specular = pow(specular, SPEC_EXPONENT) * SPEC_INTENSITY;

	return color * (diffuse + specular) * intensity;
}

void main()
{
	vec3 light_clr = vec3(0.0);
	light_clr += light_func(u_DirectionalLight, vec3(1.0), 0.8);

	vec3 ambient = AMBIENT * AMBIENT_STRENGTH;

	o_Color = vec4(ambient + light_clr, 1.0);
}