#version 330 core
in vec3 m_color;
in vec2 m_texcoor;
out vec4 fragColor;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixval;
void main()
{
	fragColor =mix(texture(texture1, m_texcoor), texture(texture2, vec2(1.0f-m_texcoor.x,m_texcoor.y)), mixval);
}
