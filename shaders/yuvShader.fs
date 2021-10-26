#version 330 core

//Original Fragment Shader
//https://github.com/moonyl/yuv-player/blob/master/player/colorSpaceConversion.fs


out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D textureY;
uniform sampler2D textureU;
uniform sampler2D textureV;


/*
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
*/

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
 