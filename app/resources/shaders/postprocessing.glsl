//#shader vertex
#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
}

//#shader fragment
#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

const float offset = 1.0 / 300.0;

void main(){
    vec2 offsets[9] = vec2[](
       vec2(-offset,  offset),
       vec2( 0.0f,    offset),
       vec2( offset,  offset),
       vec2(-offset,  0.0f),
       vec2( 0.0f,    0.0f),
       vec2( offset,  0.0f),
       vec2(-offset, -offset),
       vec2( 0.0f,   -offset),
       vec2( offset, -offset)
    );

    float kernel[9] = float[](
        -0.02, -0.05, -0.02,
         -0.05,  1.24, -0.05,
         -0.02, -0.05, -0.02
    );

    vec3 sampleTex[9];
    for (int i = 0; i < 9; ++i) {
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }

    vec3 col = vec3(0.0);
    for (int i = 0; i < 9; ++i) {
        col += sampleTex[i] * kernel[i];
    }

    FragColor = vec4(col, 1.0);
}