#version 450
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_texCoord;
layout(location = 2) in vec4 a_colorTint;

layout(set = 1, binding = 0) uniform CameraData {
    mat4 viewProj;
} camera;

layout(location = 0) out vec2 v_texCoord;
layout(location = 1) out vec4 v_colorTint;

void main() {
    gl_Position = camera.viewProj * vec4(a_position, 1.0);
    v_texCoord = a_texCoord;
    v_colorTint = a_colorTint;
}
