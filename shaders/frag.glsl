#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 texCoord;

uniform sampler2D room;
uniform sampler2D face;

void main()
{
    FragColor = mix(texture(room, texCoord), texture(face, vec2(1.0 - texCoord.s, texCoord.t)), 0.2);
}
