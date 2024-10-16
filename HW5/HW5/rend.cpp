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
	// GzPixel pixel = { 0, 0, 0, 1, MAXINT };

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

float* Normalize(float* vector, float* ans) {
	float mag = CalculateMag(vector);
	ans[0] = vector[0] / mag;
	ans[1] = vector[1] / mag;
	ans[2] = vector[2] / mag;

	return ans;
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

	// Build Xiwi
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

	GzMatrix identityMat = { {1, 0, 0, 0},
							 {0, 1, 0, 0},
							 {0, 0, 1, 0},
							 {0, 0, 0, 1} };

	GzMatrix unitaryMat = { {1, 0, 0, 0},
							{0, 1, 0, 0},
							{0, 0, 1, 0},
							{0, 0, 0, 1} };
	// Scale factor K = (a^2 + b^2 + c^2)^1/2 
	for (int i = 0; i < 3; i++) {
		/*float K = sqrt(matrix[i][0] * matrix[i][0] + matrix[i][1] * matrix[i][1]
			+ matrix[i][2] * matrix[i][2] + matrix[i][3] * matrix[i][3]);*/
		float K = sqrt(matrix[i][0] * matrix[i][0] + matrix[i][1] * matrix[i][1]
			+ matrix[i][2] * matrix[i][2]);
		for (int j = 0; j < 3; j++) {
			unitaryMat[i][j] = matrix[i][j] / K;
		}
	}


	if (matlevel < 0) { // empty stack, push a stack on top
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				Ximage[matlevel + 1][i][j] = matrix[i][j];
				Xnorm[matlevel + 1][i][j] = identityMat[i][j];
			}
		}

	}
	else { // Multiply the top of stack by new matrix
		// float** newMat = MatrixMultiply(Ximage[matlevel], matrix);
		GzMatrix imageNewMat;
		GzMatrix normNewMat;
		for (int i = 0; i < 4 /*numRowsA*/; i++) {
			for (int j = 0; j < 4 /*numColB*/; j++) {
				imageNewMat[i][j] = 0.0f;
				normNewMat[i][j] = 0.0f;
				for (int k = 0; k < 4 /*numColA/numRowB*/; k++) {
					imageNewMat[i][j] += Ximage[matlevel][i][k] * matrix[k][j];
					// Xnorm 
					if (matlevel + 1 < 2 || matlevel + 1 == 3) { // skip Xsp, Xpi, scaling
						normNewMat[i][j] += Xnorm[matlevel][i][k] * identityMat[k][j];
					}
					else {
						normNewMat[i][j] += Xnorm[matlevel][i][k] * unitaryMat[k][j];
					}
				}
			}
		}
		// push to stack
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				Ximage[matlevel + 1][i][j] = imageNewMat[i][j];
				Xnorm[matlevel + 1][i][j] = normNewMat[i][j];
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

/*
- GzPutAttribute() must accept the following tokens/values:

- GZ_RGB_COLOR					GzColor		default flat-shade color
- GZ_INTERPOLATE				int			shader interpolation mode
- GZ_DIRECTIONAL_LIGHT			GzLight
- GZ_AMBIENT_LIGHT            	GzLight		(ignore direction)
- GZ_AMBIENT_COEFFICIENT		GzColor		Ka reflectance
- GZ_DIFFUSE_COEFFICIENT		GzColor		Kd reflectance
- GZ_SPECULAR_COEFFICIENT       GzColor		Ks coef's
- GZ_DISTRIBUTION_COEFFICIENT   float		spec power
*/
	for (int i = 0; i < numAttributes; i++) {
		if (nameList[i] == GZ_RGB_COLOR) {
			GzColor* colorPtr = (GzColor*)valueList[i];
			flatcolor[0] = (*colorPtr)[0];
			flatcolor[1] = (*colorPtr)[1];
			flatcolor[2] = (*colorPtr)[2];
		}
		// Lighting attributes
		else if (nameList[i] == GZ_DIRECTIONAL_LIGHT) { // directional light
			numlights = numAttributes;
			GzLight* lightPtr = (GzLight*)valueList[i];
			lights[i] = *lightPtr;
		}
		else if (nameList[i] == GZ_AMBIENT_LIGHT) {
			GzLight* ambientPtr = (GzLight*)valueList[i];
			ambientlight = *ambientPtr;
		}
		// Shading attributes
		else if (nameList[i] == GZ_DIFFUSE_COEFFICIENT) {
			GzColor* KdPtr = (GzColor*)valueList[i];
			Kd[0] = (*KdPtr)[0];
			Kd[1] = (*KdPtr)[1];
			Kd[2] = (*KdPtr)[2];
		}
		else if (nameList[i] == GZ_INTERPOLATE) {
			int* interpModePtr = (int*)valueList[i];
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
		else if (nameList[i] == GZ_TEXTURE_MAP) {
			tex_fun = (GzTexture) valueList[i];
		}

	}

	return GZ_SUCCESS;
}

float* SpanLine(float* cur, float* left, float* right, float deltaX, float* result) {
	float slopez = (right[2] - left[2]) / (right[0] - left[0]); // dz/dx 

	// GzCoord spanCur = { curx, cury, curz };
	result[0] = cur[0] + deltaX; // LX + dX
	result[1] = cur[1];
	result[2] = cur[2] + deltaX * slopez; // LZ + dX * slopez

	return result;
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
	C = (Ks SigmaL[le(R，E)^s]) + (Kd SigmaL[le(N，L)]) + (Ka la)
*/
float* ShadingEquation(GzLight* lights, GzColor Ka, GzColor Kd, GzColor Ks,
	GzLight ambientLight, float spec, GzCoord norm, int numLights, GzColor resultColor) {
	// R = 2(N，L)N - L
	GzCoord Rs[3];
	for (int i = 0; i < numLights; i++) {
		float twoNDotL = DotProduct(norm, lights[i].direction) * 2; // max ?

		Rs[i][X] = norm[X] * twoNDotL - lights[i].direction[X];
		Rs[i][Y] = norm[Y] * twoNDotL - lights[i].direction[Y];
		Rs[i][Z] = norm[Z] * twoNDotL - lights[i].direction[Z];
	}

	GzCoord E = { 0.0f, 0.0f, -1.0f };

	// Specular
	GzColor totalSpec = { 0.0f, 0.0f, 0.0f };
	for (int i = 0; i < numLights; i++) {
		float rDotE = DotProduct(Rs[i], E);
		if (rDotE < 0)
			rDotE = 0;
		else if (rDotE > 1)
			rDotE = 1;

		float powRDotE = pow(rDotE, spec);

		/*float nDotL = DotProduct(norm, lights[i].direction);
		float nDotE = DotProduct(norm, lights[i].direction);*/

		/*if (nDotL * nDotE < 0) {

		}*/
		totalSpec[0] += lights[i].color[0] * powRDotE;
		totalSpec[1] += lights[i].color[1] * powRDotE;
		totalSpec[2] += lights[i].color[2] * powRDotE;
	}

	GzColor specMag = { Ks[0] * totalSpec[0], Ks[1] * totalSpec[1], Ks[2] * totalSpec[2] };

	// Diffuse
	GzColor totalDiff = { 0.0f, 0.0f, 0.0f };
	for (int i = 0; i < numLights; i++) {
		float nDotL = DotProduct(norm, lights[i].direction);
		// nDotL = max(0, nDotL);

		// Orientation
		// Sign of N，L and N，E
		float nDotE = DotProduct(norm, E);
		if (nDotL > 0 && nDotE > 0) { // both positive
			totalDiff[0] += lights[i].color[0] * nDotL;
			totalDiff[1] += lights[i].color[1] * nDotL;
			totalDiff[2] += lights[i].color[2] * nDotL;
		}
		else if (nDotL < 0 && nDotE < 0) { // both negative
			// flip normal
			GzCoord negNorm = { -norm[0], -norm[1], -norm[2] };
			float negNDotL = DotProduct(negNorm, lights[i].direction);

			totalDiff[0] += lights[i].color[0] * negNDotL;
			totalDiff[1] += lights[i].color[1] * negNDotL;
			totalDiff[2] += lights[i].color[2] * negNDotL;
		}
		else if (nDotL * nDotE < 0) { // different signs -> skip calculation
			continue;
		}

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

float PlaneEquation(float* v1, float* v2, float* v3, float* planeN) {
	GzCoord v1v2 = { v2[0] - v1[0], v2[1] - v1[1], v2[2] - v1[2] };
	GzCoord v1v3 = { v3[0] - v1[0], v3[1] - v1[1], v3[2] - v1[2] };

	float* norm = CrossProduct(v1v2, v1v3); // (A, B, C)
	/*float normMag = CalculateMag(norm);
	GzCoord normalizedN = { norm[0] / normMag, norm[1] / normMag, norm[2] / normMag };*/
	planeN[0] = norm[0];
	planeN[1] = norm[1];
	planeN[2] = norm[2];

	// D = (A * x1 + B * y1 + C * z1)
	float D = norm[0] * v1[0] + norm[1] * v1[1] + norm[2] * v1[2];

	return D;
}

float* InterpolateColor(float* normR, float* normG, float* normB,
	float DR, float DG, float DB, int px, int py, float* resultColor) {
	// Normal[0/1/2] = (A, B, C)
	// Plan equation: Ax + By + Cz = D
	// z = (D - Ax - By) / C
	float zR = (DR - normR[0] * px - normR[1] * py) / normR[2];
	float zG = (DG - normG[0] * px - normG[1] * py) / normG[2];
	float zB = (DB - normB[0] * px - normB[1] * py) / normB[2];

	resultColor[0] = zR;
	resultColor[1] = zG;
	resultColor[2] = zB;

	return resultColor;
}

float* InterpolateUV(float* norm1, float* norm2, float D1, float D2, 
	int px, int py, float* result) {
	// Normal[0/1/2] = (A, B, C)
	// Plan equation: Ax + By + Cz = D
	// z = (D - Ax - By) / C
	float zU = (D1 - norm1[0] * px - norm1[1] * py) / norm1[2];
	float zV = (D2 - norm2[0] * px - norm2[1] * py) / norm2[2];

	result[0] = zU;
	result[1] = zV;

	return result;
}



int GzRender::GzPutTriangle(int numParts, GzToken *nameList, GzPointer *valueList)
/* numParts - how many names and values */
{
/* HW 2.2
-- Pass in a triangle description with tokens and values corresponding to
      GZ_NULL_TOKEN:		do nothing - no values
      GZ_POSITION:		3 vert positions in model space
-- Return error code
*/
/*
-- Xform positions of verts using matrix on top of stack 
-- Clip - just discard any triangle with any vert(s) behind view plane 
		- optional: test for triangles with all three verts off-screen (trivial frustum cull)
-- invoke triangle rasterizer  
*/
	GzCoord* vertexListPtr;
	GzCoord* normalListPtr;
	GzTextureIndex* uvListPtr;
	for (int i = 0; i < numParts; i++) {
		if (nameList[i] == GZ_POSITION) {
			vertexListPtr = (GzCoord*) valueList[i];
		}
		else if (nameList[i] == GZ_NORMAL) {
			normalListPtr = (GzCoord*) valueList[i];
		}
		else if (nameList[i] == GZ_TEXTURE_INDEX) {
			uvListPtr = (GzTextureIndex*) valueList[i];
		}
	}

	// 3 vertices of tri
	float* v1m = vertexListPtr[0]; // GzCoord (float Array) 
	float* v2m = vertexListPtr[1];
	float* v3m = vertexListPtr[2];

	// Copy (C++ problem)
	GzCoord nV1 = { 0.0f, 0.0f, 0.0f };
	GzCoord nV2 = { 0.0f, 0.0f, 0.0f };
	GzCoord nV3 = { 0.0f, 0.0f, 0.0f };

	// Transform vertex
	float* v1 = VectorMultiplyMatrix(v1m, Ximage[matlevel], nV1);
	float* v2 = VectorMultiplyMatrix(v2m, Ximage[matlevel], nV2);
	float* v3 = VectorMultiplyMatrix(v3m, Ximage[matlevel], nV3);

	// Normal for every vertex of tri 
	float* norm1m = normalListPtr[0];
	float* norm2m = normalListPtr[1];
	float* norm3m = normalListPtr[2];

	// Transform normal
	GzCoord nN1 = { 0.0f, 0.0f, 0.0f };
	GzCoord nN2 = { 0.0f, 0.0f, 0.0f };
	GzCoord nN3 = { 0.0f, 0.0f, 0.0f };

	float* norm1 = VectorMultiplyMatrix(norm1m, Xnorm[matlevel], nN1);
	float* norm2 = VectorMultiplyMatrix(norm2m, Xnorm[matlevel], nN2);
	float* norm3 = VectorMultiplyMatrix(norm3m, Xnorm[matlevel], nN3);

	// uv for tri vertices
	float* uv1 = uvListPtr[0];
	float* uv2 = uvListPtr[1];
	float* uv3 = uvListPtr[2];

	/*
		Perspective correction for uv
	*/ 
	// V'z = Vzs / (Zmax - Vzs) 
	// Vzs: screen space Z
	float Vpz1 = v1[Z] / (MAXINT - v1[Z]);
	// Transform to perspective space 
	// Ps = P / (V'z + 1) 
	float u1P = uv1[U] / (Vpz1 + 1);
	float v1P = uv1[V] / (Vpz1 + 1);

	float Vpz2 = v2[Z] / (MAXINT - v2[Z]);
	float u2P = uv2[U] / (Vpz2 + 1);
	float v2P = uv2[V] / (Vpz2 + 1);

	float Vpz3 = v3[Z] / (MAXINT - v3[Z]);
	float u3P = uv3[U] / (Vpz3 + 1);
	float v3P = uv3[V] / (Vpz3 + 1);

	GzColor txColorV1 = { 0.0f, 0.0f, 0.0f };
	tex_fun(uv1[U], uv1[V], txColorV1);

	GzColor txColorV2 = { 0.0f, 0.0f, 0.0f };
	tex_fun(uv2[U], uv2[V], txColorV2);

	GzColor txColorV3 = { 0.0f, 0.0f, 0.0f };
	tex_fun(uv3[U], uv3[V], txColorV3);


	/*
		Shading
		Calculating Color
	*/

	/*
		Gouraud shading
		GZ_COLOR
		HW5: Replace Ks, Ka, Kd
	*/
	
	GzColor nColor1 = { 0.0f, 0.0f, 0.0f };
	GzColor nColor2 = { 0.0f, 0.0f, 0.0f };
	GzColor nColor3 = { 0.0f, 0.0f, 0.0f };

	// Ignore K coefficient when calculating vertex color in Gouraud shading (then use this to interpolate to pixel)
	// then multiply by texture color
	GzColor K1 = { 1, 1, 1 };
	float* c1 = ShadingEquation(lights, K1, K1, K1, ambientlight, spec, norm1, numlights, nColor1);
	float* c2 = ShadingEquation(lights, K1, K1, K1, ambientlight, spec, norm2, numlights, nColor2);
	float* c3 = ShadingEquation(lights, K1, K1, K1, ambientlight, spec, norm3, numlights, nColor3);
	
	
		// Hw4 color
	/*float* c1 = ShadingEquation(lights, Ka, Kd, Ks, ambientlight, spec, norm1, numlights, nColor1);
	float* c2 = ShadingEquation(lights, Ka, Kd, Ks, ambientlight, spec, norm2, numlights, nColor2);
	float* c3 = ShadingEquation(lights, Ka, Kd, Ks, ambientlight, spec, norm3, numlights, nColor3);
	*/

	// Red
	GzCoord v1R = { v1[X], v1[Y], c1[0] };
	GzCoord v2R = { v2[X], v2[Y], c2[0] };
	GzCoord v3R = { v3[X], v3[Y], c3[0] };

	GzCoord nR = { 0.0f, 0.0f, 0.0f }; // plane normal (A B C)
	float DR = PlaneEquation(v1R, v2R, v3R, nR);

	// Green
	GzCoord v1G = { v1[X], v1[Y], c1[1] };
	GzCoord v2G = { v2[X], v2[Y], c2[1] };
	GzCoord v3G = { v3[X], v3[Y], c3[1] };

	GzCoord nG = { 0.0f, 0.0f, 0.0f };
	float DG = PlaneEquation(v1G, v2G, v3G, nG);

	// Blue
	GzCoord v1B = { v1[X], v1[Y], c1[2] };
	GzCoord v2B = { v2[X], v2[Y], c2[2] };
	GzCoord v3B = { v3[X], v3[Y], c3[2] };

	GzCoord nB = { 0.0f, 0.0f, 0.0f };
	float DB = PlaneEquation(v1B, v2B, v3B, nB);

	/*
		Phong shading
		GZ_NORMALS
	*/
	GzCoord v1Nx1 = { v1[X], v1[Y], norm1[X] };
	GzCoord v2Nx2 = { v2[X], v2[Y], norm2[X] };
	GzCoord v3Nx3 = { v3[X], v3[Y], norm3[X] };

	GzCoord nNx = { 0.0f, 0.0f, 0.0f };
	float Dx = PlaneEquation(v1Nx1, v2Nx2, v3Nx3, nNx);

	GzCoord v1Ny1 = { v1[X], v1[Y], norm1[Y] };
	GzCoord v2Ny2 = { v2[X], v2[Y], norm2[Y] };
	GzCoord v3Ny3 = { v3[X], v3[Y], norm3[Y] };

	GzCoord nNy = { 0.0f, 0.0f, 0.0f };
	float Dy = PlaneEquation(v1Ny1, v2Ny2, v3Ny3, nNy);

	GzCoord v1Nz1 = { v1[X], v1[Y], norm1[Z] };
	GzCoord v2Nz2 = { v2[X], v2[Y], norm2[Z] };
	GzCoord v3Nz3 = { v3[X], v3[Y], norm3[Z] };

	GzCoord nNz = { 0.0f, 0.0f, 0.0f };
	float Dz = PlaneEquation(v1Nz1, v2Nz2, v3Nz3, nNz);


	/*
		Interpolate u, v
		Plane equation
		same as interpolating colors/normals
	*/

	//// U
	//GzCoord v1U = { v1[X], v1[Y], uv1[U] };
	//GzCoord v2U = { v2[X], v2[Y], uv2[U] };
	//GzCoord v3U = { v3[X], v3[Y], uv3[U] };

	//GzCoord nU = { 0.0f, 0.0f, 0.0f };
	//float DU = PlaneEquation(v1U, v2U, v3U, nU);

	//// V
	//GzCoord v1V = { v1[X], v1[Y], uv1[V] };
	//GzCoord v2V = { v2[X], v2[Y], uv2[V] };
	//GzCoord v3V = { v3[X], v3[Y], uv3[V] };

	//GzCoord nV = { 0.0f, 0.0f, 0.0f };
	//float DV = PlaneEquation(v1V, v2V, v3V, nV);

	/* Interpolate in perspective space */
	// U
	GzCoord v1U = { v1[X], v1[Y], u1P };
	GzCoord v2U = { v2[X], v2[Y], u2P };
	GzCoord v3U = { v3[X], v3[Y], u3P };

	GzCoord nU = { 0.0f, 0.0f, 0.0f };
	float DU = PlaneEquation(v1U, v2U, v3U, nU);

	// V
	GzCoord v1V = { v1[X], v1[Y], v1P };
	GzCoord v2V = { v2[X], v2[Y], v2P };
	GzCoord v3V = { v3[X], v3[Y], v3P };

	GzCoord nV = { 0.0f, 0.0f, 0.0f };
	float DV = PlaneEquation(v1V, v2V, v3V, nV);

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


	// Initial Left/Right
	float* left = top;
	float* right = top;

	float dy = ceil(top[1]) - top[1]; // dY = ceil(V1(Y)) - V1(Y) 

	GzCoord leftCur = { 0.0f, 0.0f, 0.0f };
	GzCoord rightCur = { 0.0f, 0.0f, 0.0f };

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

		GzCoord spanCurVer = { 0.0f, 0.0f, 0.0f };

		float dx = ceil(left[0]) - left[0]; // dX = ceil(LX) - LX

		for (int x = ceil(left[0]); x < ceil(right[0]); x++) {
			// Calculate current vertex on spanline
			spanCur = SpanLine(spanCur, left, right, dx, spanCurVer);

			GzColor gColor = { 0.0f, 0.0f, 0.0f }; // for Gouraud shading
			GzColor resColor = { 0.0f, 0.0f, 0.0f };

			GzCoord resNorm = { 0.0f, 0.0f, 0.0f };
			GzTextureIndex resUV = { 0.0f, 0.0f };
			// Gouraud shading
			if (interp_mode == GZ_COLOR) {
				InterpolateColor(nR, nG, nB, DR, DG, DB, spanCur[0], spanCur[1], gColor);
				InterpolateUV(nU, nV, DU, DV, spanCur[0], spanCur[1], resUV);
				
				// Transform interpolated parameters back to affine space after interpolation	
				// P = Ps (V'z + 1) 
				float vpz = spanCur[Z] / (MAXINT - spanCur[Z]);
				resUV[U] = resUV[U] * (vpz + 1);
				resUV[V] = resUV[V] * (vpz + 1);

				tex_fun(resUV[U], resUV[V], resColor);
				
				// use tex_fun color as K to multiply interpolated color
				for (int i = 0; i < 3; i++) {
					resColor[i] *= gColor[i];
				}

			}
			else if (interp_mode == GZ_NORMALS) {
				// Interpolate Normals
				InterpolateColor(nNx, nNy, nNz, Dx, Dy, Dz, spanCur[0], spanCur[1], resNorm);
				Normalize(resNorm, resNorm);

				// Perspective correction
				// Interpolate uv in perspective space
				InterpolateUV(nU, nV, DU, DV, spanCur[0], spanCur[1], resUV);

				// Transform interpolated parameters back to affine space after interpolation	
				// P = Ps (V'z + 1) 
				float vpz = spanCur[Z] / (MAXINT - spanCur[Z]);
				resUV[U] = resUV[U] * (vpz + 1);
				resUV[V] = resUV[V] * (vpz + 1);

				tex_fun(resUV[U], resUV[V], resColor);

				// Interpolate Color based on normals on each pixel
				ShadingEquation(lights, resColor, resColor, Ks, ambientlight, spec, resNorm, numlights, resColor);
			}

			if (ZBuffering(spanCur, xres, yres, pixelbuffer, ARRAY(spanCur[0], spanCur[1]))) {
				GzPut((int)spanCur[0], (int)spanCur[1], ctoi(resColor[0]), ctoi(resColor[1]),
					ctoi(resColor[2]), 1, (GzDepth)spanCur[2]);
			}

			dx = 1;
		}

		dy = 1;

	}

	/*
		Lower part of Triangle
	*/

	if (SortEdges(top, mid, bottom) == 1) { // counter-clockwise
		right = Interpolate(right, top, bottom, mid[1] - left[1], rightCur);
		left = mid;
	}
	else {
		left = Interpolate(left, top, bottom, mid[1] - right[1], leftCur);
		right = mid;
	}

	dy = ceil(mid[1]) - mid[1];
	for (int y = ceil(left[1]); y < ceil(bottom[1]); y++) {

		// Differentiate Left and Right edges
		if (SortEdges(top, mid, bottom) == 1) {
			left = Interpolate(left, mid, bottom, dy, leftCur);
			right = Interpolate(right, top, bottom, dy, rightCur);
		}
		else if (SortEdges(top, mid, bottom) == 0) {
			left = Interpolate(left, top, bottom, dy, leftCur);
			right = Interpolate(right, mid, bottom, dy, rightCur);
		}

		float* spanCur = left;

		GzCoord spanCurVer = { 0.0f, 0.0f, 0.0f };

		float dx = ceil(left[0]) - left[0];

		for (int x = ceil(left[0]); x < ceil(right[0]); x++) {
			spanCur = SpanLine(spanCur, left, right, dx, spanCurVer);

			GzColor gColor = { 0.0f, 0.0f, 0.0f }; // for Gouraud shading
			GzColor resColor = { 0.0f, 0.0f, 0.0f };

			GzColor resNorm = { 0.0f, 0.0f, 0.0f };
			GzTextureIndex resUV = { 0.0f, 0.0f };
			// Gouraud shading
			if (interp_mode == GZ_COLOR) {
				InterpolateColor(nR, nG, nB, DR, DG, DB, spanCur[0], spanCur[1], gColor);
				InterpolateUV(nU, nV, DU, DV, spanCur[0], spanCur[1], resUV);
				
				// Transform interpolated parameters back to affine space after interpolation	
				// P = Ps (V'z + 1) 
				float vpz = spanCur[Z] / (MAXINT - spanCur[Z]);
				resUV[U] = resUV[U] * (vpz + 1);
				resUV[V] = resUV[V] * (vpz + 1);

				tex_fun(resUV[U], resUV[V], resColor);

				for (int i = 0; i < 3; i++) {
					resColor[i] *= gColor[i];
				}
			}
			// Phong shading
			else if (interp_mode == GZ_NORMALS) {
				// Interpolate Normals
				InterpolateColor(nNx, nNy, nNz, Dx, Dy, Dz, spanCur[0], spanCur[1], resNorm);
				Normalize(resNorm, resNorm);

				InterpolateUV(nU, nV, DU, DV, spanCur[0], spanCur[1], resUV);
				
				// Transform back to image space
				float vpz = spanCur[Z] / (MAXINT - spanCur[Z]);
				resUV[U] = resUV[U] * (vpz + 1);
				resUV[V] = resUV[V] * (vpz + 1);
				
				tex_fun(resUV[U], resUV[V], resColor);

				// Interpolate Color based on normals on each pixel
				ShadingEquation(lights, resColor, resColor, Ks, ambientlight, spec, resNorm, numlights, resColor);	
			}

			if (ZBuffering(spanCur, xres, yres, pixelbuffer, ARRAY(spanCur[0], spanCur[1]))) {
				GzPut((int)spanCur[0], (int)spanCur[1], ctoi(resColor[0]),
					ctoi(resColor[1]), ctoi(resColor[2]), 1, (GzDepth)spanCur[2]);
			}

			dx = 1;
		}

		dy = 1;

	}

	return GZ_SUCCESS;
}

