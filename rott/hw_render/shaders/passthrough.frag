#version 450
layout(location = 0) in vec2 v_texCoord;
layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 0) uniform sampler2D texSampler;

void main() {
    vec4 color = texture(texSampler, v_texCoord);
    if (color.r < 0.01 && color.g < 0.01 && color.b < 0.01) {
        discard;
    }
    outColor = color;
}
