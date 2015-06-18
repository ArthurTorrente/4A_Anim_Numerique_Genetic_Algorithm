attribute vec3 a_position;
uniform sampler2D u_heightMap;

vec2 verticalBlurTextCoords[14];
vec2 horizontalBlurTextCoords[14];

float g_zAxis = 0.0;

void getBlurCoord()
{
	verticalBlurTextCoords[ 0] = gl_MultiTexCoord0.st + vec2(0.0, -0.028);
    verticalBlurTextCoords[ 1] = gl_MultiTexCoord0.st + vec2(0.0, -0.024);
    verticalBlurTextCoords[ 2] = gl_MultiTexCoord0.st + vec2(0.0, -0.020);
    verticalBlurTextCoords[ 3] = gl_MultiTexCoord0.st + vec2(0.0, -0.016);
    verticalBlurTextCoords[ 4] = gl_MultiTexCoord0.st + vec2(0.0, -0.012);
    verticalBlurTextCoords[ 5] = gl_MultiTexCoord0.st + vec2(0.0, -0.008);
    verticalBlurTextCoords[ 6] = gl_MultiTexCoord0.st + vec2(0.0, -0.004);
    verticalBlurTextCoords[ 7] = gl_MultiTexCoord0.st + vec2(0.0,  0.004);
    verticalBlurTextCoords[ 8] = gl_MultiTexCoord0.st + vec2(0.0,  0.008);
    verticalBlurTextCoords[ 9] = gl_MultiTexCoord0.st + vec2(0.0,  0.012);
    verticalBlurTextCoords[10] = gl_MultiTexCoord0.st + vec2(0.0,  0.016);
    verticalBlurTextCoords[11] = gl_MultiTexCoord0.st + vec2(0.0,  0.020);
    verticalBlurTextCoords[12] = gl_MultiTexCoord0.st + vec2(0.0,  0.024);
    verticalBlurTextCoords[13] = gl_MultiTexCoord0.st + vec2(0.0,  0.028);

	horizontalBlurTextCoords[ 0] = gl_MultiTexCoord0.st + vec2(-0.028, 0.0);
    horizontalBlurTextCoords[ 1] = gl_MultiTexCoord0.st + vec2(-0.024, 0.0);
    horizontalBlurTextCoords[ 2] = gl_MultiTexCoord0.st + vec2(-0.020, 0.0);
    horizontalBlurTextCoords[ 3] = gl_MultiTexCoord0.st + vec2(-0.016, 0.0);
    horizontalBlurTextCoords[ 4] = gl_MultiTexCoord0.st + vec2(-0.012, 0.0);
    horizontalBlurTextCoords[ 5] = gl_MultiTexCoord0.st + vec2(-0.008, 0.0);
    horizontalBlurTextCoords[ 6] = gl_MultiTexCoord0.st + vec2(-0.004, 0.0);
    horizontalBlurTextCoords[ 7] = gl_MultiTexCoord0.st + vec2( 0.004, 0.0);
    horizontalBlurTextCoords[ 8] = gl_MultiTexCoord0.st + vec2( 0.008, 0.0);
    horizontalBlurTextCoords[ 9] = gl_MultiTexCoord0.st + vec2( 0.012, 0.0);
    horizontalBlurTextCoords[10] = gl_MultiTexCoord0.st + vec2( 0.016, 0.0);
    horizontalBlurTextCoords[11] = gl_MultiTexCoord0.st + vec2( 0.020, 0.0);
    horizontalBlurTextCoords[12] = gl_MultiTexCoord0.st + vec2( 0.024, 0.0);
    horizontalBlurTextCoords[13] = gl_MultiTexCoord0.st + vec2( 0.028, 0.0);
}

void computePosition()
{
	vec4 g_pos = vec4(0.0, 0.0, 0.0, 0.0);

	g_pos += texture2D(u_heightMap, verticalBlurTextCoords[ 0]) * 0.0044299121055113265;
	g_pos += texture2D(u_heightMap, verticalBlurTextCoords[ 1]) * 0.00895781211794;
	g_pos += texture2D(u_heightMap, verticalBlurTextCoords[ 2]) * 0.0215963866053;
	g_pos += texture2D(u_heightMap, verticalBlurTextCoords[ 3]) * 0.0443683338718;
	g_pos += texture2D(u_heightMap, verticalBlurTextCoords[ 4]) * 0.0776744219933;
	g_pos += texture2D(u_heightMap, verticalBlurTextCoords[ 5]) * 0.115876621105;
	g_pos += texture2D(u_heightMap, verticalBlurTextCoords[ 6]) * 0.147308056121;
	g_pos += texture2D(u_heightMap, gl_MultiTexCoord0.st)		  * 0.159576912161;
	g_pos += texture2D(u_heightMap, verticalBlurTextCoords[ 7]) * 0.147308056121;
	g_pos += texture2D(u_heightMap, verticalBlurTextCoords[ 8]) * 0.115876621105;
	g_pos += texture2D(u_heightMap, verticalBlurTextCoords[ 9]) * 0.0776744219933;
	g_pos += texture2D(u_heightMap, verticalBlurTextCoords[10]) * 0.0443683338718;
	g_pos += texture2D(u_heightMap, verticalBlurTextCoords[11]) * 0.0215963866053;
	g_pos += texture2D(u_heightMap, verticalBlurTextCoords[12]) * 0.00895781211794;
	g_pos += texture2D(u_heightMap, verticalBlurTextCoords[13]) * 0.0044299121055113265;

	g_pos += texture2D(u_heightMap, horizontalBlurTextCoords[ 0]) * 0.0044299121055113265;
	g_pos += texture2D(u_heightMap, horizontalBlurTextCoords[ 1]) * 0.00895781211794;
	g_pos += texture2D(u_heightMap, horizontalBlurTextCoords[ 2]) * 0.0215963866053;
	g_pos += texture2D(u_heightMap, horizontalBlurTextCoords[ 3]) * 0.0443683338718;
	g_pos += texture2D(u_heightMap, horizontalBlurTextCoords[ 4]) * 0.0776744219933;
	g_pos += texture2D(u_heightMap, horizontalBlurTextCoords[ 5]) * 0.115876621105;
	g_pos += texture2D(u_heightMap, horizontalBlurTextCoords[ 6]) * 0.147308056121;
	g_pos += texture2D(u_heightMap, gl_MultiTexCoord0.st)			* 0.159576912161;
	g_pos += texture2D(u_heightMap, horizontalBlurTextCoords[ 7]) * 0.147308056121;
	g_pos += texture2D(u_heightMap, horizontalBlurTextCoords[ 8]) * 0.115876621105;
	g_pos += texture2D(u_heightMap, horizontalBlurTextCoords[ 9]) * 0.0776744219933;
	g_pos += texture2D(u_heightMap, horizontalBlurTextCoords[10]) * 0.0443683338718;
	g_pos += texture2D(u_heightMap, horizontalBlurTextCoords[11]) * 0.0215963866053;
	g_pos += texture2D(u_heightMap, horizontalBlurTextCoords[12]) * 0.00895781211794;
	g_pos += texture2D(u_heightMap, horizontalBlurTextCoords[13]) * 0.0044299121055113265;

	g_zAxis = g_pos.z;
}

void main(void)
{
	gl_TexCoord[0] = gl_MultiTexCoord0;
	
	/*
	vec4 pos = gl_Vertex;
	computePosition();
	pos.z = g_zAxis;
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * pos;
	*/

	vec4 pos = gl_Vertex;
	vec4 height = texture2D(u_heightMap, gl_MultiTexCoord0.st);
	pos.z += (height.r * 0.3) + (height.g * 0.59) + (height.b * 0.11);

	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * pos;
}
