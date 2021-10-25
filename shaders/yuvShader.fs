#version 330 core

out vec4 FragColor;
in vec2 TexCoords;
uniform sampler2D textureY;
uniform sampler2D textureU;
uniform sampler2D textureV;

void main()
{
    /*
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
    */

    vec3 yuv;                                           
    yuv.x = texture(textureY, TexCoords).r;             
    yuv.y = texture(textureU, TexCoords).r - 0.5f;      
    yuv.z = texture(textureV, TexCoords).r - 0.5f;      
                                                        
    mat3 trans = mat3(1, 1 ,1,                          
                      0, -0.34414, 1.772,               
                      1.402, -0.71414, 0                
                      );                                
                                                        
    FragColor = vec4(trans*yuv, 1.0);  

}