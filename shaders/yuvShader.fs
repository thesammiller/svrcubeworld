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

int clamp(int value)
{
	return value<0 ? 0 : (value>255 ? 255 : value);
}


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

    vec3 xyz;
    int y_tmp = int(yuv.x * 255);
    int u_tmp =  int(yuv.y * 255) - 128;
    int v_tmp = int(yuv.z * 255) - 128;
                             
   	float Rf = 0.2126;
     float Bf =  0.0722;
     float YMin = 16.0;
     float YMax = 235.0;
     float CbCrRange =  224.0;

    float value;
    int p;
    value = (255.0*(2.0*(1-Bf))/CbCrRange);
    p = 6;
    int cb_factor=int((value*(1<<p))+0.5);

    value = (255.0*(2.0*(1-Rf))/CbCrRange);
    p = 6;
    int cr_factor=int((value*(1<<p))+0.5);
    
    value = Bf/(1.0-Bf-Rf)*255.0*(2.0*(1-Bf))/CbCrRange;
    p = 7;
    int  g_cb_factor= int(((value)*(1<<p))+0.5);

    value = Rf/(1.0-Bf-Rf)*255.0*(2.0*(1-Rf))/CbCrRange;
    p = 7;
    int g_cr_factor=int(((value)*(1<<p))+0.5);

    value = 255.0/(YMax-YMin);
    p = 7;
    int y_factor=int(((value)*(1<<p))+0.5);
    int y_offset=int(YMin);
		
    //compute Cb Cr color offsets, common to four pixels
    int b_cb_offset, r_cr_offset, g_cbcr_offset;
    b_cb_offset = (cb_factor*u_tmp)>>6;
    r_cr_offset = (cr_factor*v_tmp)>>6;
    g_cbcr_offset = (g_cb_factor*u_tmp + g_cr_factor*v_tmp)>>7;
    
    y_tmp = (y_factor*(y_tmp-y_offset))>>7;
    rgb.r = clamp(y_tmp + r_cr_offset);
    rgb.g = clamp(y_tmp - g_cbcr_offset);
    rgb.b = clamp(y_tmp + b_cb_offset);
    
    FragColor = vec4(rgb.r, rgb.g, 0.0, 1.0);
		
	
}