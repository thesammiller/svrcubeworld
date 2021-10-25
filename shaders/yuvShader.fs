//precision lowp float;
in vec2 TexCoords;


uniform sampler2D samplerY;
uniform sampler2D samplerU;
uniform sampler2D samplerV;


void main() {
    float r, g, b, y, u, v, fYmul;
    
    vec3 rgb, yuv;

    yuv.x = texture2D(samplerY, TexCoords).r;
    yuv.y = texture2D(samplerU, TexCoords).r;
    yuv.z = texture2D(samplerV, TexCoords).r;
    
    const vec3 offset = vec3(-0.0625, -0.5, -0.5);
    // RGB coefficients
    const vec3 Rcoeff = vec3( 1.164, 0.000,  1.596);
    const vec3 Gcoeff = vec3( 1.164, -0.391, -0.813);
    const vec3 Bcoeff = vec3( 1.164, 2.018,  0.000);
    
    yuv = clamp(yuv, 0.0, 1.0);
    yuv += offset;

    rgb.r = dot(yuv, Rcoeff);
    rgb.g = dot(yuv, Gcoeff);
    rgb.b = dot(yuv, Bcoeff);
    gl_FragColor = vec4(rgb, 1.0);
    //gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}