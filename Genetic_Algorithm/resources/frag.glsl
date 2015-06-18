uniform sampler2D u_texture;

void main(void)
{	
	gl_FragColor = texture2D(u_texture, gl_TexCoord[0].st);
}
