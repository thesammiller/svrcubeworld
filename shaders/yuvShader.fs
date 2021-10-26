#version 330 core

//Original Fragment Shader
//https://github.com/moonyl/yuv-player/blob/master/player/colorSpaceConversion.fs


out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D textureY;
uniform sampler2D textureU;
uniform sampler2D textureV;


//https://www.libsdl.org/tmp/SDL/src/render/opengl/SDL_shaders_gl.c
// YUV offset
//const vec3 offset = vec3(-0.0627451017, -0.501960814, -0.501960814);
const vec3 offset = vec3(-0.0625, -0.5, -0.5);

// RGB coefficients \n"                           
const vec3 Rcoeff = vec3(1.1644,  0.000,  1.7927);
const vec3 Gcoeff = vec3(1.1644, -0.2132, -0.5329);
const vec3 Bcoeff = vec3(1.1644,  2.1124,  0.000);

//const vec3 Rcoeff = vec3(1.1644,  0.000,  1.596);           \
//const vec3 Gcoeff = vec3(1.1644, -0.3918, -0.813);          \
//const vec3 Bcoeff = vec3(1.1644,  2.0172,  0.000);


void main()
{
    vec2 tcoord;
    vec3 yuv, rgb;
                                                            
    // Get the Y value \n"                                     
    tcoord = TexCoords;
    yuv.x = texture(textureY, tcoord).r;
                                                            
  // Get the U and V values \n"                             
   //tcoord -= vec2(0.5, 0.5);
    yuv.y = texture(textureU, tcoord).r;
    yuv.z = texture(textureV, tcoord).r;
                             
   // Do the color transform 
    yuv += offset;
    rgb.r = dot(yuv, Rcoeff);
    rgb.g = dot(yuv, Gcoeff);
    rgb.b = dot(yuv, Bcoeff);
                                            
    // That was easy. :) \n"                
    FragColor = vec4(rgb, 1.0);
}