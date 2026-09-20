#version 450
layout(location = 0) in vec2 v_texCoord;
layout(location = 1) in vec4 v_colorTint;

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 0) uniform sampler2D texSampler;

void main() {
    vec4 texColor = texture(texSampler, v_texCoord);
    if (texColor.a < 0.5) {
        discard;
    }
    outColor = texColor * v_colorTint;
}
