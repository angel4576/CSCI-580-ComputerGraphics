/* CS580 Homework 3 */

#include	"stdafx.h"
#include	"stdio.h"
#include	"math.h"
#include	"Gz.h"
#include	"rend.h"

#define PI (float) 3.14159265358979323846

int GzRender::GzRotXMat(float degree, GzMatrix mat)
{
/* HW 3.1
// Create rotate matrix : rotate along x axis
// Pass back the matrix using mat value
*/
	float radian = degree * PI / 180.0;
	float sinValue = sin(radian);
	float cosValue = cos(radian);

	GzMatrix Rx = { {1, 0, 0, 0},
					{0, cosValue, -sinValue, 0},
					{0, sinValue, cosValue, 0},
					{0, 0, 0, 1} };

	// pass back to mat
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			mat[i][j] = Rx[i][j];
		}
	}

	return GZ_SUCCESS;
}

int GzRender::GzRotYMat(float degree, GzMatrix mat)
{
/* HW 3.2
// Create rotate matrix : rotate along y axis
// Pass back the matrix using mat value
*/
	float radian = degree * PI / 180.0;
	float sinValue = sin(radian);
	float cosValue = cos(radian);

	GzMatrix Ry = { {cosValue, 0, sinValue, 0},
		{0, 1, 0, 0},
		{-sinValue, 0, cosValue, 0},
		{0, 0, 0, 1} };

	// pass back to 4x4 mat
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			mat[i][j] = Ry[i][j];
		}
	}

	return GZ_SUCCESS;
}

int GzRender::GzRotZMat(float degree, GzMatrix mat)
{
/* HW 3.3
// Create rotate matrix : rotate along z axis
// Pass back the matrix using mat value
*/
	float radian = degree * PI / 180.0;
	float sinValue = sin(radian);
	float cosValue = cos(radian);

	GzMatrix Rz = { {cosValue, -sinValue, 0, 0},
					{sinValue, cosValue, 0, 0},
					{0, 0, 1, 0},
					{0, 0, 0, 1} };

	// pass back to 4x4 mat
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			mat[i][j] = Rz[i][j];
		}
	}

	return GZ_SUCCESS;
}

int GzRender::GzTrxMat(GzCoord translate, GzMatrix mat)
{
/* HW 3.4
// Create translation matrix
// Pass back the matrix using mat value
*/
	float x = translate[0];
	float y = translate[1];
	float z = translate[2];

	GzMatrix Tr = { {1, 0, 0, x},
		{0, 1, 0, y},
		{0, 0, 1, z},
		{0, 0, 0, 1} };

	// pass back to 4x4 mat
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			mat[i][j] = Tr[i][j];
		}
	}

	return GZ_SUCCESS;
}


int GzRender::GzScaleMat(GzCoord scale, GzMatrix mat)
{
/* HW 3.5
// Create scaling matrix
// Pass back the matrix using mat value
*/
	float sx = scale[0];
	float sy = scale[1];
	float sz = scale[2];

	GzMatrix Sc = { {sx, 0, 0, 0},
		{0, sy, 0, 0},
		{0, 0, sz, 0},
		{0, 0, 0, 1} };

	// pass back to 4x4 mat
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			mat[i][j] = Sc[i][j];
		}
	}

	return GZ_SUCCESS;
}


GzRender::GzRender(int xRes, int yRes)
{
/* HW1.1 create a framebuffer for MS Windows display:
 -- set display resolution
 -- allocate memory for framebuffer : 3 bytes(b, g, r) x width x height
 -- allocate memory for pixel buffer
 */
	xres = xRes;
	yres = yRes;

	framebuffer = (char*)malloc(3 * sizeof(char) * xRes * yRes);
	// framebuffer = (char*)malloc(3 * xres * yres);
	pixelbuffer = (GzPixel*)malloc(xres * yres * sizeof(GzPixel));

/* HW 3.6
- setup Xsp and anything only done once 
- init default camera 
*/ 
	matlevel = -1;
	// Setup Xsp
	GzMatrix XspTemp = { {xres / 2.0, 0, 0, xres / 2.0},
		{0, -yres / 2.0, 0, yres / 2.0},
		{0, 0, MAXINT, 0},
		{0, 0, 0, 1} };

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			Xsp[i][j] = XspTemp[i][j];
		}
	}

	// Init default camera
	m_camera.position[X] = DEFAULT_IM_X;
	m_camera.position[Y] = DEFAULT_IM_Y;
	m_camera.position[Z] = DEFAULT_IM_Z;

	m_camera.worldup[X] = 0;
	m_camera.worldup[Y] = 1;
	m_camera.worldup[Z] = 0;

	m_camera.FOV = DEFAULT_FOV;

	m_camera.lookat[X] = 0;
	m_camera.lookat[Y] = 0;
	m_camera.lookat[Z] = 0;
}

GzRender::~GzRender()
{
/* HW1.2 clean up, free buffer memory */
	free(framebuffer);
	free(pixelbuffer);

}

int GzRender::GzDefault()
{
/* HW1.3 set pixel buffer to some default values - start a new frame */
	GzPixel pixel = { 200, 200, 255, 1, MAXINT };

	for (int i = 0; i < xres * yres; i++) {
		pixelbuffer[i] = pixel;
	}

	return GZ_SUCCESS;
}

float DotProduct(GzCoord a, GzCoord b) {
	float result = 0.0f;
	for (int i = 0; i < 3/*sizeof(a) / sizeof(a[0])*/; i++) {
		result += a[i] * b[i];
	}

	return result;
}

float* CrossProduct(GzCoord a, GzCoord b/*, GzCoord res*/) {
	float resultX = a[Y] * b[Z] - a[Z] * b[Y];
	float resultY = a[Z] * b[X] - a[X] * b[Z];
	float resultZ = a[X] * b[Y] - a[Y] * b[X];

	GzCoord res = { resultX, resultY, resultZ };
	/*res[0] = resultX;
	res[1] = resultY;
	res[2] = resultZ;*/

	return res;
}

// Calculate magnitude of a vector
// sqrt(x^2 + y^2 + z^2)
float CalculateMag(GzCoord v) {
	return (sqrt(v[X] * v[X] + v[Y] * v[Y] + v[Z] * v[Z]));
}

int GzRender::GzBeginRender()
{
/* HW 3.7 
- setup for start of each frame - init frame buffer color,alpha,z
- compute Xiw and projection xform Xpi from camera definition 
- init Ximage - put Xsp at base of stack, push on Xpi and Xiw 
- now stack contains Xsw and app can push model Xforms when needed 
*/ 
// Build Xpi
	float FOVRadians = m_camera.FOV * PI / 180.0;
	float reciprocalD = tan(FOVRadians / 2.0); // 1 / d
	GzMatrix Xpi = { {1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, reciprocalD, 0},
		{0, 0, reciprocalD, 1} };

	// Copy to camera Xpi
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m_camera.Xpi[i][j] = Xpi[i][j];
		}
	}

	//Build Xiw

	// cl = l - c
	GzCoord cl = { m_camera.lookat[X] - m_camera.position[X],
	m_camera.lookat[Y] - m_camera.position[Y],
	m_camera.lookat[Z] - m_camera.position[Z] };
	float magCl = CalculateMag(cl);

	// Get Z-Axis
	// Z = cl / || cl ||
	GzCoord zAxis = { cl[X] / magCl, cl[Y] / magCl, cl[Z] / magCl };

	// Get Y-Axis
	// up' = up - (up . Z) * Z
	// Y = up' / || up' ||
	float upDotz = DotProduct(m_camera.worldup, zAxis);
	GzCoord zTemp = { zAxis[X] * upDotz, zAxis[Y] * upDotz, zAxis[Z] * upDotz };
	GzCoord upPri = { m_camera.worldup[X] - zTemp[X],
	m_camera.worldup[Y] - zTemp[Y],
	m_camera.worldup[Z] - zTemp[Z] };

	float magUpPri = CalculateMag(upPri);
	GzCoord yAxis = { upPri[X] / magUpPri, upPri[Y] / magUpPri, upPri[Z] / magUpPri };

	// Get X-Axis (perpendicular to Y and Z)
	GzCoord res = { 0, 0, 0 };
	float* xproduct = CrossProduct(yAxis, zAxis/*, res*/);
	GzCoord xAxis = { xproduct[X], xproduct[Y], xproduct[Z] };

	// Build Xiw
	// float xdp = -DotProduct(xAxis, m_camera.position);
	GzMatrix Xiw = { { xAxis[X], xAxis[Y], xAxis[Z], -DotProduct(xAxis, m_camera.position) },
	{ yAxis[X], yAxis[Y], yAxis[Z], -DotProduct(yAxis, m_camera.position) },
	{ zAxis[X], zAxis[Y], zAxis[Z], -DotProduct(zAxis, m_camera.position) },
	{ 0, 0, 0, 1 } };

	// Copy to m_camera Xiw
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m_camera.Xiw[i][j] = Xiw[i][j];
		}
	}

	// Stack
	// init Ximage - put Xsp at base of stack, push on Xpi and Xiw 
	
	GzPushMatrix(Xsp);
	GzPushMatrix(Xpi);
	GzPushMatrix(Xiw);


	return GZ_SUCCESS;
}

int GzRender::GzPutCamera(GzCamera camera)
{
/* HW 3.8 
/*- overwrite renderer camera structure with new camera definition
*/
	m_camera.position[X] = camera.position[X];
	m_camera.position[Y] = camera.position[Y];
	m_camera.position[Z] = camera.position[Z];

	m_camera.lookat[X] = camera.lookat[X];
	m_camera.lookat[Y] = camera.lookat[Y];
	m_camera.lookat[Z] = camera.lookat[Z];

	m_camera.worldup[X] = camera.worldup[X];
	m_camera.worldup[Y] = camera.worldup[Y];
	m_camera.worldup[Z] = camera.worldup[Z];

	m_camera.FOV = camera.FOV;

	return GZ_SUCCESS;	
}

float** MatrixMultiply(GzMatrix a, GzMatrix b) {
	float** result = new float* [4];
	for (int i = 0; i < 4; i++) {
		result[i] = new float[4];
	}

	/*for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result[i][j] = 0.0f;
		}
	}*/

	for (int i = 0; i < 4 /*numRowsA*/; i++) {
		for (int j = 0; j < 4 /*numColB*/; j++) {
			result[i][j] = 0.0f;
			for (int k = 0; k < 4 /*numColA/numRowB*/; k++) {
				result[i][j] += a[i][k] * b[k][j];
			}
		}
	}

	return result;
}

int GzRender::GzPushMatrix(GzMatrix	matrix)
{
/* HW 3.9 
- push a matrix onto the Ximage stack
- check for stack overflow
*/
	if (matlevel >= MATLEVELS) {
		return GZ_FAILURE;
	}

	if (matlevel < 0) { // empty stack, push a stack on top
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				Ximage[matlevel + 1][i][j] = matrix[i][j];
			}
		}

	}
	else { // Multiply the top of stack by new matrix
		// float** newMat = MatrixMultiply(Ximage[matlevel], matrix);
		/*float newMat[4][4];*/
		GzMatrix newMat;
		for (int i = 0; i < 4 /*numRowsA*/; i++) {
			for (int j = 0; j < 4 /*numColB*/; j++) {
				newMat[i][j] = 0.0f;
				for (int k = 0; k < 4 /*numColA/numRowB*/; k++) {
					newMat[i][j] += Ximage[matlevel][i][k] * matrix[k][j];
				}
			}
		}
		// push to stack
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				Ximage[matlevel + 1][i][j] = newMat[i][j];
			}
		}
	}

	matlevel++;

	return GZ_SUCCESS;
}

int GzRender::GzPopMatrix()
{
/* HW 3.10
- pop a matrix off the Ximage stack
- check for stack underflow
*/
	if (matlevel <= -1) { // -1 means empty
		return GZ_FAILURE;
	}

	matlevel--;

	return GZ_SUCCESS;
}

GzIntensity Clamp(GzIntensity color) {
	if (color < 0) {
		color = 0;
	}
	else if (color > 4095) {
		color = 4095;
	}

	return color;
}

int GzRender::GzPut(int i, int j, GzIntensity r, GzIntensity g, GzIntensity b, GzIntensity a, GzDepth z)
{
/* HW1.4 write pixel values into the buffer */

	// Clamp color
	r = Clamp(r);
	g = Clamp(g);
	b = Clamp(b);

	/*r = r >> 4;
	g = g >> 4;
	b = b >> 4;
	a = a >> 4;*/

	if (i >= 0 && i < yres && j >= 0 && j < xres) { // bound check pixel coordinate
		int index = ARRAY(i, j);
		pixelbuffer[index].red = r >> 4;
		pixelbuffer[index].green = g >> 4;
		pixelbuffer[index].blue = b >> 4;
		pixelbuffer[index].alpha = a >> 4;
		pixelbuffer[index].z = z;
	}

	return GZ_SUCCESS;
}


int GzRender::GzGet(int i, int j, GzIntensity *r, GzIntensity *g, GzIntensity *b, GzIntensity *a, GzDepth *z)
{
/* HW1.5 retrieve a pixel information from the pixel buffer */

	int index = ARRAY(i, j);
	*r = pixelbuffer[index].red;
	*g = pixelbuffer[index].green;
	*b = pixelbuffer[index].blue;
	*a = pixelbuffer[index].alpha;
	*z = pixelbuffer[index].z;

	return GZ_SUCCESS;
}


int GzRender::GzFlushDisplay2File(FILE* outfile)
{
/* HW1.6 write image to ppm file -- "P6 %d %d 255\r" */
	
	int result = fprintf(outfile, "P6 %d %d 255\r", xres, yres);
	for (int i = 0; i < xres * yres; i++) {
		fwrite(&pixelbuffer[i].red, 1, 1, outfile);// red
		fwrite(&pixelbuffer[i].green, 1, 1, outfile);// green
		fwrite(&pixelbuffer[i].blue, 1, 1, outfile);// blue
	}

	return GZ_SUCCESS;
}

int GzRender::GzFlushDisplay2FrameBuffer()
{
/* HW1.7 write pixels to framebuffer: 
	- put the pixels into the frame buffer
	- CAUTION: when storing the pixels into the frame buffer, the order is blue, green, and red 
	- NOT red, green, and blue !!!
*/

	int j = 0;
	for (int i = 0; i < xres * yres; i++) {

		framebuffer[j] = (char)pixelbuffer[i].blue;
		framebuffer[j + 1] = (char)pixelbuffer[i].green;
		framebuffer[j + 2] = (char)pixelbuffer[i].red;

		j += 3;
	}

	return GZ_SUCCESS;
}


/***********************************************/
/* HW2 methods: implement from here */

int GzRender::GzPutAttribute(int numAttributes, GzToken	*nameList, GzPointer *valueList) 
{
/* HW 2.1
-- Set renderer attribute states (e.g.: GZ_RGB_COLOR default color)
-- In later homeworks set shaders, interpolaters, texture maps, and lights
*/

	for (int i = 0; i < numAttributes; i++) {
		if (nameList[i] == GZ_RGB_COLOR) {
			/*GzColor* colorPtr = (GzColor*) valueList[i];
			flatcolor[0] = (*colorPtr)[0];
			flatcolor[1] = (*colorPtr)[1];
			flatcolor[2] = (*colorPtr)[2];*/
		}
		// Lighting attributes
		else if (nameList[i] == GZ_DIRECTIONAL_LIGHT) { // directional light
			numlights = numAttributes;
			GzLight* lightPtr = (GzLight*) valueList[i];
			lights[i] = *lightPtr;
		}
		else if (nameList[i] == GZ_AMBIENT_LIGHT) {
			GzLight* ambientPtr = (GzLight*) valueList[i];
			ambientlight = *ambientPtr;
		}
		// Shading attributes
		else if (nameList[i] == GZ_DIFFUSE_COEFFICIENT) {
			GzColor* KdPtr = (GzColor*) valueList[i];
			Kd[0] = (*KdPtr)[0];
			Kd[1] = (*KdPtr)[1];
			Kd[2] = (*KdPtr)[2];
		}
		else if (nameList[i] == GZ_INTERPOLATE) {
			int* interpModePtr = (int*) valueList[i];
			interp_mode = *interpModePtr;
		}
		else if (nameList[i] == GZ_AMBIENT_COEFFICIENT) {
			GzColor* KaPtr = (GzColor*) valueList[i];
			Ka[0] = (*KaPtr)[0];
			Ka[1] = (*KaPtr)[1];
			Ka[2] = (*KaPtr)[2];
		}
		else if (nameList[i] == GZ_SPECULAR_COEFFICIENT) {
			GzColor* KsPtr = (GzColor*) valueList[i];
			Ks[0] = (*KsPtr)[0];
			Ks[1] = (*KsPtr)[1];
			Ks[2] = (*KsPtr)[2];
		}
		else if (nameList[i] == GZ_DISTRIBUTION_COEFFICIENT) {
			float* specPtr = (float*) valueList[i];
			spec = *specPtr;
		}
		
		
	}

	

	return GZ_SUCCESS;
}

float* SpanLine(float* cur, float* left, float* right, float deltaX) {
	float slopez = (right[2] - left[2]) / (right[0] - left[0]); // dz/dx 

	float curx = cur[0] + deltaX; // LX + ��X
	float cury = cur[1];
	float curz = cur[2] + deltaX * slopez; // LZ + ��X * slopez

	GzCoord spanCur = { curx, cury, curz };

	return spanCur;
}

float* Interpolate(float* cur, float* start, float* end, float deltaY, float* newCur) {
	float slopex = (end[0] - start[0]) / (end[1] - start[1]); // dx/dy
	float slopez = (end[2] - start[2]) / (end[1] - start[1]); // dz/dy

	newCur[0] = cur[0] + slopex * deltaY; // X + SLOPEx * delta Y
	newCur[1] = cur[1] + deltaY; // Y + delta Y
	newCur[2] = cur[2] + slopez * deltaY; // Z + SLOPEz * delta Y

	return newCur;
}

/*
	Use y-intercept to determine L/R edges
*/
int SortEdges(float* top, float* mid, float* bottom) { // y1: top
	float slope = (bottom[1] - top[1]) / (bottom[0] - top[0]); // V2 - V1

	// Take mid-y, get x-coord on top-bottom
	float x = (mid[1] - top[1]) / slope + top[0]; // x - x1 = (y - y1) / slope

	// determine L/R
	if (x < mid[0]) { // x on top-bottom < mid-x -> mid is R -> top-mid-bottom is clockwise
		return 0;
	}
	else if (x > mid[0]) { // mid is R -> counter-clockwise
		return 1;
	}
	else {

		//return 2;
	}
}

boolean ZBuffering(float* cur, short xres, short yres, GzPixel* pixelbuffer, int i) {
	if (cur[0] < 0 || cur[0] >= yres || cur[1] < 0 || cur[1] >= xres) {
		return false;
	}

	if (cur[2] < pixelbuffer[i].z) {
		return true;
	}

	return false;
}

// 4d vector muliply by 4d matrix
float* VectorMultiplyMatrix(GzCoord v, GzMatrix mat, GzCoord newV) {
	float v4d[4] = { v[X], v[Y], v[Z], 1 };
	float result4d[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	for (int i = 0; i < 4 /*numRowsMat*/; i++) {
		//for (int j = 0; j < 1 /*colsV*/; j++) { // only one col for vector
		for (int k = 0; k < 4 /*numColsMat/rowsV*/; k++) {
			result4d[i] += mat[i][k] * v4d[k];
		}
		//}
	}

	// convert from 4d to 3d
	float w = result4d[3];
	newV[X] = result4d[X] / w;
	newV[Y] = result4d[Y] / w;
	newV[Z] = result4d[Z] / w;

	return newV;
}

/*
	C = (Ks SigmaL[le(R·E)^s]) + (Kd SigmaL[le(N·L)]) + (Ka la)
*/
float* ShadingEquation(GzLight* lights, GzColor Ka, GzColor Kd, GzColor Ks,
	GzLight ambientLight, float spec, GzCoord norm, int numLights, GzColor resultColor) {
	// R = 2(N·L)N - L
	GzCoord Rs[3];
	for (int i = 0; i < numLights; i++) {
		float twoNDotL = DotProduct(norm, lights[i].direction) * 2; // max ?

		Rs[i][X] = norm[X] * twoNDotL - lights[i].direction[X];
		Rs[i][Y] = norm[Y] * twoNDotL - lights[i].direction[Y];
		Rs[i][Z] = norm[Z] * twoNDotL - lights[i].direction[Z];
	}

	GzCoord E = { 0.0f, 0.0f - 1.0f };

	// Specular
	GzColor totalSpec = { 0.0f, 0.0f, 0.0f };
	for (int i = 0; i < numLights; i++) {
		float rDotE = max(0, DotProduct(Rs[i], E));
		float powRDotE = pow(rDotE, spec);

		totalSpec[0] += lights[i].color[0] * powRDotE;
		totalSpec[1] += lights[i].color[1] * powRDotE;
		totalSpec[2] += lights[i].color[2] * powRDotE;
	}

	GzColor specMag = { Ks[0] * totalSpec[0], Ks[1] * totalSpec[1], Ks[2] * totalSpec[2] };

	// Diffuse
	GzColor totalDiff = { 0.0f, 0.0f, 0.0f };
	for (int i = 0; i < numLights; i++) {
		float nDotL = DotProduct(norm, lights[i].direction);
		nDotL = max(0, nDotL);

		totalDiff[0] += lights[i].color[0] * nDotL;
		totalDiff[1] += lights[i].color[1] * nDotL;
		totalDiff[2] += lights[i].color[2] * nDotL;
	}

	GzColor diffMag = { Kd[0] * totalDiff[0], Kd[1] * totalDiff[1], Kd[2] * totalDiff[2] };

	// Ambient
	GzColor ambMag = { Ka[0] * ambientLight.color[0], Ka[1] * ambientLight.color[1], Ka[2] * ambientLight.color[2] };

	GzColor finalColor = { specMag[0] + diffMag[0] + ambMag[0],
						  specMag[1] + diffMag[1] + ambMag[1],
						  specMag[2] + diffMag[2] + ambMag[2] };

	resultColor[0] = specMag[0] + diffMag[0] + ambMag[0];
	resultColor[1] = specMag[1] + diffMag[1] + ambMag[1];
	resultColor[2] = specMag[2] + diffMag[2] + ambMag[2];

	return resultColor;
}

int GzRender::GzPutTriangle(int numParts, GzToken *nameList, GzPointer *valueList)
/* numParts - how many names and values */
{
/* HW 2.2
-- Pass in a triangle description with tokens and values corresponding to
      GZ_NULL_TOKEN:		do nothing - no values
      GZ_POSITION:		3 vert positions in model space
-- Invoke the rastrizer/scanline framework
-- Return error code

	Hw4
--	Get Normal
*/
	
	GzCoord* vertexListPtr;
	GzCoord* normalListPtr; 
	for (int i = 0; i < numParts; i++) {
		if (nameList[i] == GZ_POSITION) {
			vertexListPtr = (GzCoord*) valueList[i];
		}
		else if (nameList[i] == GZ_NORMAL) {
			normalListPtr = (GzCoord*) valueList[i];
		}
	}

	// 3 vertices of tri
	float* v1B = vertexListPtr[0]; // GzCoord (float Array) 
	float* v2B = vertexListPtr[1];
	float* v3B = vertexListPtr[2];

	// Copy (C++ problem)
	GzCoord nV1 = { 0.0f, 0.0f, 0.0f };
	GzCoord nV2 = { 0.0f, 0.0f, 0.0f };
	GzCoord nV3 = { 0.0f, 0.0f, 0.0f };

	float* v1 = VectorMultiplyMatrix(v1B, Ximage[matlevel], nV1);
	float* v2 = VectorMultiplyMatrix(v2B, Ximage[matlevel], nV2);
	float* v3 = VectorMultiplyMatrix(v3B, Ximage[matlevel], nV3);

	// Normal for every vertex of tri 
	float* norm1 = normalListPtr[0];
	float* norm2 = normalListPtr[1];
	float* norm3 = normalListPtr[2];

	/*
		Shading 
		Calculating Color
	*/

	GzCoord c1 = { 0.0f, 0.0f, 0.0f };
	GzCoord c2 = { 0.0f, 0.0f, 0.0f };
	GzCoord c3 = { 0.0f, 0.0f, 0.0f };  

	float* nColor1 = ShadingEquation(lights, Ka, Kd, Ks, ambientlight, spec, norm1, numlights, c1);
	float* nColor2 = ShadingEquation(lights, Ka, Kd, Ks, ambientlight, spec, norm2, numlights, c2);
	float* nColor3 = ShadingEquation(lights, Ka, Kd, Ks, ambientlight, spec, norm3, numlights, c3);

	
	//flatcolor[0] = nColor[0];
	//flatcolor[1] = nColor[1];
	//flatcolor[2] = nColor[2];

	/*
		Rasterization
		Scan Line
	*/

	/*
		Set up top, middle, bottom vertex
		Sort by y
	*/
	GzCoord top, mid, bottom;
	if (v1[1] <= v2[1] && v1[1] <= v3[1]) { // v1 is the top
		top[0] = v1[0];
		top[1] = v1[1];
		top[2] = v1[2];

		if (v2[1] <= v3[1]) {
			// v2 is the mid
			mid[0] = v2[0];
			mid[1] = v2[1];
			mid[2] = v2[2];

			//v3 is the bottom
			bottom[0] = v3[0];
			bottom[1] = v3[1];
			bottom[2] = v3[2];
		}
		else {
			// v3 is the mid
			mid[0] = v3[0];
			mid[1] = v3[1];
			mid[2] = v3[2];

			// v2 is the bottom
			bottom[0] = v2[0];
			bottom[1] = v2[1];
			bottom[2] = v2[2];
		}

	}
	else if (v2[1] <= v1[1] && v2[1] <= v3[1]) { // v2 is top
		top[0] = v2[0];
		top[1] = v2[1];
		top[2] = v2[2];

		if (v1[1] <= v3[1]) {
			// v1 is mid
			mid[0] = v1[0];
			mid[1] = v1[1];
			mid[2] = v1[2];

			// v3 is bottom
			bottom[0] = v3[0];
			bottom[1] = v3[1];
			bottom[2] = v3[2];
		}
		else {
			// v3 is mid
			mid[0] = v3[0];
			mid[1] = v3[1];
			mid[2] = v3[2];

			// v1 is bottom
			bottom[0] = v1[0];
			bottom[1] = v1[1];
			bottom[2] = v1[2];
		}

	}
	else { // v3 is the top
		top[0] = v3[0];
		top[1] = v3[1];
		top[2] = v3[2];

		if (v1[1] <= v2[1]) {
			// v1 is mid
			mid[0] = v1[0];
			mid[1] = v1[1];
			mid[2] = v1[2];

			// v2 is bottom
			bottom[0] = v2[0];
			bottom[1] = v2[1];
			bottom[2] = v2[2];
		}
		else {
			// v2 is mid
			mid[0] = v2[0];
			mid[1] = v2[1];
			mid[2] = v2[2];

			// v1 is bottom
			bottom[0] = v1[0];
			bottom[1] = v1[1];
			bottom[2] = v1[2];
		}
	}


	GzCoord leftCur = { 0, 0, 0 };
	GzCoord rightCur = { 0, 0, 0 };

	// Initial Left/Right
	float* left = top;
	float* right = top;

	float dy = ceil(top[1]) - top[1]; // ��Y = ceil(V1(Y)) �C V1(Y) 

	for (int y = ceil(top[1]); y < ceil(mid[1]); y++) {

		/* Differentiate Left and Right edges */
		if (SortEdges(top, mid, bottom) == 1) { // mid at the left of top (top-mid-btm counter-clockwise)
			left = Interpolate(left, top, mid, dy, leftCur);
			right = Interpolate(right, top, bottom, dy, rightCur);
		}
		else if (SortEdges(top, mid, bottom) == 0) { // (top-mid-btm clockwise)
			left = Interpolate(left, top, bottom, dy, leftCur);
			right = Interpolate(right, top, mid, dy, rightCur);
		}


		/* Span from left to right */
		float* spanCur = left;
		float dx = ceil(left[0]) - left[0]; // ��X = ceil(LX) - LX

		for (int x = ceil(left[0]); x < ceil(right[0]); x++) {
			spanCur = SpanLine(spanCur, left, right, dx);

			GzCoord copy = { spanCur[0], spanCur[1], spanCur[2] };
			if (ZBuffering(spanCur, xres, yres, pixelbuffer, ARRAY(spanCur[0], spanCur[1]))) {
				GzPut((int)spanCur[0], (int)spanCur[1], ctoi(flatcolor[0]), ctoi(flatcolor[1]),
					ctoi(flatcolor[2]), 1, (GzDepth)spanCur[2]);
			}
			spanCur = copy;

			dx = 1;
		}

		dy = 1;

	}

	/*
		Lower part of Triangle
	*/

	if (SortEdges(top, mid, bottom) == 1) { // counter-clockwise
		//if (left[0] >= mid[0]) {
		right = Interpolate(right, top, bottom, mid[1] - left[1], rightCur);
		left = mid;

		//}
	}
	else {
		//if (right[0] <= mid[0]) {
		left = Interpolate(left, top, bottom, mid[1] - right[1], leftCur);
		right = mid;
		//}
	}

	dy = ceil(mid[1]) - mid[1];
	for (int y = ceil(left[1]); y < ceil(bottom[1]); y++) {

		//		if (ceil(top[1]) != ceil(mid[1])) {
		// Differentiate Left and Right edges
		if (SortEdges(top, mid, bottom) == 1) {
			left = Interpolate(left, mid, bottom, dy, leftCur);
			right = Interpolate(right, top, bottom, dy, rightCur);
		}
		else if (SortEdges(top, mid, bottom) == 0) {
			left = Interpolate(left, top, bottom, dy, leftCur);
			right = Interpolate(right, mid, bottom, dy, rightCur);
		}
		//		}


		float* spanCur = left;
		float dx = ceil(left[0]) - left[0];

		for (int x = ceil(left[0]); x < ceil(right[0]); x++) {
			spanCur = SpanLine(spanCur, left, right, dx);
			GzCoord copy = { spanCur[0], spanCur[1], spanCur[2] };

			if (ZBuffering(spanCur, xres, yres, pixelbuffer, ARRAY(spanCur[0], spanCur[1]))) {
				GzPut((int)spanCur[0], (int)spanCur[1], ctoi(flatcolor[0]),
					ctoi(flatcolor[1]), ctoi(flatcolor[2]), 1, (GzDepth)spanCur[2]);
			}

			spanCur = copy;

			dx = 1;
		}

		dy = 1;

	}

	return GZ_SUCCESS;
}

