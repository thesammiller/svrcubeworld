#version 330 core

//Original Fragment Shader
//https://github.com/moonyl/yuv-player/blob/master/player/colorSpaceConversion.fs


out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D textureY;
uniform sampler2D textureU;
uniform sampler2D textureV;

const vec3 R_cf = vec3(1.164383,  0.000000,  1.596027);
const vec3 G_cf = vec3(1.164383, -0.391762, -0.812968);
const vec3 B_cf = vec3(1.164383,  2.017232,  0.000000);
const vec3 offset = vec3(-1.0 * ( 1.0 - (1088.0/1024.0)), -1.0 * (544.0/1024.0), -1.0 * (544.0/1024.0));

void main() {
  float y = texture(textureY, TexCoords+vec2(offset.x, offset.x)).r;
  float u = texture(textureU, TexCoords+vec2(offset.y, offset.y)).r;
    float v = texture(textureV, TexCoords+vec2(offset.y, offset.y)).r;
 vec3 yuv = vec3(y,u,v);
  yuv += offset;
  FragColor = vec4(0.0, 0.0, 0.0, 1.0);
  FragColor.r = dot(yuv, R_cf);
  FragColor.g = dot(yuv, G_cf);
  FragColor.b = dot(yuv, B_cf);
  
}
 