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
const vec3 offset = vec3(-0.0627451017, -0.501960814, -0.501960814);
//const vec3 offset = vec3(-0.0625, -0.5, -0.5);

//BT709
// RGB coefficients \n"                           
const vec3 Rcoeff = vec3(1.1644,  0.000,  1.7927);
const vec3 Gcoeff = vec3(1.1644, -0.2132, -0.5329);
const vec3 Bcoeff = vec3(1.1644,  2.1124,  0.000);


//BT601
//const vec3 Rcoeff = vec3(1.1644,  0.000,  1.596);           
//const vec3 Gcoeff = vec3(1.1644, -0.3918, -0.813);          
//const vec3 Bcoeff = vec3(1.1644,  2.0172,  0.000);


//JPEG
//const vec3 Rcoeff = vec3(1,  0.000,  1.402);
//const vec3 Gcoeff = vec3(1, -0.3441, -0.7141);
//const vec3 Bcoeff = vec3(1,  1.772,  0.000);


void main()
{
    vec2 tcoord;
    vec3 yuv, rgb;
                                                            
    // Get the Y value \n"                                     
    tcoord = TexCoords;
    yuv.x = texture(textureY, tcoord).r;
                                                            
  // Get the U and V values \n"                             
   tcoord -= vec2(0.5, 0.5);
    yuv.y = texture(textureU, tcoord).r;
    yuv.z = texture(textureV, tcoord).r;
                             
   // Do the color transform 
    //yuv += offset;
    //rgb.r = dot(yuv, Rcoeff);
    //rgb.g = dot(yuv, Gcoeff);
    //rgb.b = dot(yuv, Bcoeff);

    rgb = mat3(1.0,1.0,1.0,
                     0.0,-0.39425,2.03211,
                     1.13983,-0.5806,0.0)*yuv;
       //Output pixel color
        FragColor = vec4(rgb.r, 0.0, 0.0,1.0);
    // That was easy. :) \n"                
    //FragColor = vec4(0.0, rgb.g, 0.0, 1.0);
}