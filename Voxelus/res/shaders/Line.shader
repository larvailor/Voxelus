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


//
//
//		GEOMETRY
//
//

#shader geometry
#version 330 core

layout(lines) in;
layout(line_strip, max_vertices = 2) out;

void main() {

    for (int i = 0; i < gl_in.length(); i++)
    {
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}

//
//
//		FRAGMENT
//
//


#shader fragment
#version 330 core

layout(location = 0) out vec4 o_Color;

in vec4 v_Color;

void main()
{
	o_Color = v_Color;
}