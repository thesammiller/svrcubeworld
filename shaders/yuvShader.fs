#version 330 core

out vec4 FragColor;
in vec2 TexCoords;
uniform sampler2D textureY;
uniform sampler2D textureU;
uniform sampler2D textureV;

int clamp(int value)
{
	return value<0 ? 0 : (value>255 ? 255 : value);
}


void main()
{

    int ystride = 1088;
    int uvstride = 544;
    int rgbstride = 1024 * 3;

    int y1 = int ((TexCoords.y * 1024) * 1024 * ystride);
    //int y2 = int (((TexCoords.y * 1024 )+ 1) * ystride);
    int u = int ((TexCoords.y/2 * 1024) * 1024 * 544);
    int v = int ((TexCoords.y/2 * 1024) * 1024 * 544);

    int rgb_point = int(TexCoords.y * 1024 * rgbstride);
    int rgb2_point = int(TexCoords.y * 1024 * rgbstride);

    int u_tmp = u-128;
    int v_tmp = v-128;

    //0.2126, 0.0722, 16.0, 235.0, 224.0
    float Rf = 0.2126;
    float Bf = 0.0722; 
    float YMin = 16.0;
    float YMax = 235.0;
    float CbCrRange = 224.0;

    int cb_factor= int((((255.0*(2.0*(1-Bf))/CbCrRange)*(1<<6))+0.5));
    int cr_factor= int((((255.0*(2.0*(1-Rf))/CbCrRange)*(1<<6))+0.5));
    int g_cb_factor= int((((Bf/(1.0-Bf-Rf)*255.0*(2.0*(1-Bf))/CbCrRange)*(1<<7))+0.5));
    int g_cr_factor= int((((Rf/(1.0-Bf-Rf)*255.0*(2.0*(1-Rf))/CbCrRange)*(1<<7))+0.5));
    int y_factor= int(((255.0/(YMax-YMin)*(1<<7))+0.5));
    int y_offset=int(YMin);
    
    int b_cb_offset, r_cr_offset, g_cbcr_offset;
    b_cb_offset = (cb_factor * u_tmp) >> 6;
    r_cr_offset = (cr_factor * v_tmp) >> 6;
    g_cbcr_offset = (g_cb_factor* u_tmp + g_cr_factor * v_tmp) >> 7;

    int y_tmp;
    vec3 rgb;
    y_tmp = int(y_factor*(y1-y_offset))>>7;
    rgb.r = float(clamp(y_tmp + r_cr_offset))/255.0;
    rgb.g = float(clamp(y_tmp - g_cbcr_offset))/255.0;
    rgb.b = float(clamp(y_tmp + b_cb_offset))/255.0;

                   
    FragColor = vec4(rgb, 1.0);  

}