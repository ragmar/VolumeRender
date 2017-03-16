uniform sampler2D tex2D;
uniform sampler1D tex1D;

void main(){
	float f=texture2D(tex2D,gl_TexCoord[0].st).r;
	gl_FragColor.rgba = texture1D(tex1D,f).rgba;
}