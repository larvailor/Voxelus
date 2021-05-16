#shader vertex
#version 330 core

layout(location = 0) in vec3 i_Position;
layout(location = 1) in vec4 i_Color;

uniform mat4 u_ViewProj;
uniform mat4 u_Transform;

out vertexData
{
	vec3 color;
	vec3 fragPos;
} vs_out;

void main()
{
	vs_out.color = i_Color.xyz;
	vs_out.fragPos = i_Position;
	gl_Position = u_ViewProj * u_Transform * vec4(i_Position, 1.0f);
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

in vertexData
{
	vec3 color;
	vec3 fragPos;
} gs_in[];

out vec3 v_Color;
out vec3 v_Normal;
out vec3 v_FragPos;

vec3 GetNormal()
{
	vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
	vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
	return normalize(cross(a, b));
}

void main() {
	v_Color = gs_in[0].color;
	v_FragPos = gs_in[0].fragPos;
	v_Normal = GetNormal();
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();

	v_Color = gs_in[1].color;
	v_FragPos = gs_in[1].fragPos;
	v_Normal = GetNormal();
	gl_Position = gl_in[1].gl_Position;
	EmitVertex();

	v_Color = gs_in[2].color;
	v_FragPos = gs_in[2].fragPos;
	v_Normal = GetNormal();
	gl_Position = gl_in[2].gl_Position;
	EmitVertex();

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

uniform vec3 u_LightPos;
uniform vec3 u_LightColor;

in vec3 v_Color;
in vec3 v_Normal;
in vec3 v_FragPos;

void main()
{
	float ambientStrength = 0.4f;
	vec3 ambientColor = vec3(1.0f, 1.0f, 1.0f) * ambientStrength;

	float diffuseStrength = 0.8f;
	vec3 lightDir = normalize(u_LightPos - v_FragPos);
	vec3 diffuseColor = u_LightColor * max(dot(v_Normal, lightDir), 0.0) * diffuseStrength;

	vec3 resultColor = (ambientColor + diffuseColor) * v_Color;
	color = vec4(resultColor, 1.0);
}