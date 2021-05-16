#shader vertex
#version 330 core

layout(location = 0) in vec4 position;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * position;
}

//
//
//		GEOMETRY
//
//

#shader geometry
#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

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

layout(location = 0) out vec4 color;

uniform vec3 u_Color;

void main()
{
	gl_FragColor = vec4(u_Color, 1.0f);
}