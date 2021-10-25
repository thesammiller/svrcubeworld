in vec2 TexCoords;
 uniform sampler2DRect Ytex;
 uniform sampler2DRect Utex,Vtex;

void main(void) {
    float nx,ny,r,g,b,y,u,v;
    vec4 txl,ux,vx;
    nx=TexCoords.x;
    ny=1024.0-TexCoords.y;
    y=texture2DRect(Ytex,vec2(nx,ny)).r;
    u=texture2DRect(Utex,vec2(nx/2.0,ny/2.0)).r;
    v=texture2DRect(Vtex,vec2(nx/2.0,ny/2.0)).r;

    y=1.1643*(y-0.0625);
    u=u-0.5;
    v=v-0.5;

    r=y+1.5958*v;
    g=y-0.39173*u-0.81290*v;
    b=y+2.017*u;

    gl_FragColor=vec4(r,g,b,1.0);
  }