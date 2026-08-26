#version 410 core
out vec4 FragColor;

uniform sampler2D tex;

void main()
{
    ivec2 texSize = textureSize(tex, 0);
    vec2 uv = gl_FragCoord.xy / vec2(texSize);
    FragColor = texture(tex, uv);
}
