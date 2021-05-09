#shader vertex
#version 330 core

layout(location = 0) in vec3 i_Position;
layout(location = 1) in vec4 i_Color;

uniform mat4 u_ViewProj;
uniform mat4 u_Transform;

out vec4 v_Color;

void main()
{
	v_Color = i_Color;
	gl_Position = u_ViewProj * u_Transform * vec4(i_Position, 1.0f);
}



#shader fragment
#version 330 core

layout(location = 0) out vec4 o_Color;

in vec4 v_Color;

void main()
{
	o_Color = v_Color;
}