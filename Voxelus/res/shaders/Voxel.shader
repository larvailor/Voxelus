#shader vertex
#version 330 core

layout(location = 0) in vec4 position;

uniform mat4 u_MVP;
//uniform vec3 u_Center;
//uniform float u_HalfWidth;

void main()
{
	gl_Position = u_MVP * position;
}



#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform vec3 u_LightColor;
uniform vec3 u_Color;

void main()
{
	gl_FragColor = vec4(u_LightColor * u_Color, 1.0f);
}