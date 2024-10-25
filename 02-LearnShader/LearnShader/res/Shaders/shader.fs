#version 330 core
in vec3 m_color;
out vec4 fragColor;
void main()
{
	fragColor = vec4(m_color,1.0f);
}
