in vec2 TexCoords;
uniform sampler2D s_texture_y;  
uniform sampler2D s_texture_u;  
uniform sampler2D s_texture_v;   
void main() {  
    float y = texture2D(s_texture_y, TexCoords).r;  
    float u = texture2D(s_texture_u, TexCoords).r - 0.5;  
    float v = texture2D(s_texture_v, TexCoords).r - 0.5;  
    float r = y +             1.402 * v;  
    float g = y - 0.344 * u - 0.714 * v;  
    float b = y + 1.772 * u;  
    gl_FragColor = vec4(r,g,b,1.0);  
}