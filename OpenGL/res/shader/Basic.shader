#shader vertex

#version 330 core
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_Texcoord;

uniform mat4 u_MVP;

void main()
{
	gl_Position = position * u_MVP;
	v_Texcoord = texCoord;
};

#shader pixel

#version 330 core
layout(location = 0) out vec4 color;

in vec2 v_Texcoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
	vec4 ucolor = texture(u_Texture, v_Texcoord);
	color = ucolor;
};