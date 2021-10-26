#version 330 core

//Original Fragment Shader
//https://github.com/moonyl/yuv-player/blob/master/player/colorSpaceConversion.fs


out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D textureY;
uniform sampler2D textureU;
uniform sampler2D textureV;


/*
https://github.com/diederickh/video_capture/blob/master/src/opengl_example.cpp
const vec3 R_cf = vec3(1.164383,  0.000000,  1.596027);
const vec3 G_cf = vec3(1.164383, -0.391762, -0.812968);
const vec3 B_cf = vec3(1.164383,  2.017232,  0.000000);

//const vec3 R_cf = vec3(1,  0.000000,  1.13983);
//const vec3 G_cf = vec3(1, -0.39465, -0.58060);
//const vec3 B_cf = vec3(1,  2.03211,  0.000000);

const vec3 offset = vec3(-0.0625, -0.5, -0.5);

void main() {
  float y = texture(textureY, TexCoords).r;
  float u = texture(textureU, TexCoords).r;
    float v = texture(textureV, TexCoords).r;
 vec3 yuv = vec3(y,u,v);
  yuv += offset;
  FragColor = vec4(0.0, 0.0, 0.0, 1.0);
  FragColor.r = dot(yuv, R_cf);
  FragColor.g = dot(yuv, G_cf);
  FragColor.b = dot(yuv, B_cf);
  
}



https://dsp.stackexchange.com/questions/365/yv12-to-rgb-what-is-wrong-with-my-algorithm 
void main(void) {
  float nx,ny,r,g,b,y,u,v;
  vec4 txl,ux,vx;
  nx=TexCoords.x;
    ny=1088.0-TexCoords.y;
   y=texture(textureY,vec2(nx,ny)).r;
   u=texture(textureU,vec2(nx,ny)).r;
   v=texture(textureV,vec2(nx,ny)).r;
   y=1.1643*(y-0.0625);
   u=u-0.5;
   v=v-0.5;

   r=y+1.5958*v;
   g=y-0.39173*u-0.81290*v;
   b=y+2.017*u;

    FragColor=vec4(r,g,b,1.0);
  }


void main()
{
	float r,g,b,y,u,v;
	y=texture(textureY, TexCoords).r;
	u=texture(textureU, TexCoords).r;
	v=texture(textureV, TexCoords).r;

	y=1.1643*(y-0.0625);
	u=u-0.5;
	v=v-0.5;

	r=y+1.5958*v ;
	g=y-0.39173*u-0.81290*v;
	b=y+2.017*u;

	FragColor = vec4(r, g, b, 1.0);
}


https://community.khronos.org/t/extracting-y-from-gl-luminance/65001/2

When you say why the math, do you mean why Y is scaled, and U and V are offset by 0.5? 
The Y is being scaled there because the code is assuming that video levels are being used, 
where 16 is picture black (so ‘super-black’ is supported), and picture white is 235. 
That math (I didn’t check the numbers, but you can) converts to a more normal 0.0 - 1.0. 
Depending on what your write the result to, it will be clamped.

U and V are color difference signals, so they represent a signed number with a range of approx -0.5 to 0.5. It’s most common to offset the signed value to make an unsigned integer in the 0-255 range, representing -127 to 127. The math converts it back to a signed floating point difference value.
Acha! 16/255 = 0.0625. So, if you have a value of 16, then 1.1643 * 16 - 0.0625 = 0. 
Conversely, 235/256 = 0.91796 and (0.91796 * 1.1643) - 0.0625 ~= 1.

*/

const mat3 yuv2rgb = mat3( 1,              1,      1,    
                            0,       -0.18732, 1.8556,
                            1.57481, -0.46813,      0);

const mat3 otherrgb = mat3(      1,       1,       1,
                0, -.21482, 2.12798,
                1.28033, -.38059,       0) * yuv;

void main() { 
    
    vec3 yuv = vec3(
                    1.1643 * (texture(textureY, TexCoords).r - 0.0625),
                    texture(textureU, vec2(TexCoords.x/2.0, TexCoords.y/2.0)).r - 0.5,
                    texture(textureV, vec2(TexCoords.x/2.0, TexCoords.y/2.0)).r - 0.5
                    );
    vec3 rgb = otherrgb * yuv;
    
    FragColor = vec4(rgb, 1.0);
}


/*

//uniform sampler2D ;
//uniform sampler2D u_texture_cr;
//uniform sampler2D u_texture_cb;

https://stackoverflow.com/questions/55930123/converting-yuv-ycbcr420p-to-rgb-in-glsl-fragment-shader

const float yScale = 255.0 / (235.0 - 16.0); //(BT709_YMax-BT709_YMin)
const float uvScale = 255.0 / (240.0 - 16.0); //(BT709_UVMax-BT709_UVMin)

float BT709_nonLinearNormToLinear(float normV) {
    if (normV < 0.081) {
        normV *= (1.0 / 4.5);
    } else {
        float a = 0.099;
        float gamma = 1.0 / 0.45;
        normV = (normV + a) * (1.0 / (1.0 + a));
        normV = pow(normV, gamma);
    }
    return normV;
}

void main() {
    float y = texture(textureY, TexCoords).r;
    float u = texture(textureU, TexCoords).r - 0.5;
    float v = texture(textureV, TexCoords).r - 0.5;

    y = y - 16.0/255.0;

    float r = y*yScale +                          v*uvScale*1.5748;
    float g = y*yScale - u*uvScale*1.8556*0.101 - v*uvScale*1.5748*0.2973;
    float b = y*yScale + u*uvScale*1.8556;

    r = clamp(r, 0.0, 1.0);
    g = clamp(g, 0.0, 1.0);
    b = clamp(b, 0.0, 1.0);

    r = BT709_nonLinearNormToLinear(r);
    g = BT709_nonLinearNormToLinear(g);
    b = BT709_nonLinearNormToLinear(b);

    FragColor = vec4(r, g, b, 1.0);
}

*/

