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



	return GZ_SUCCESS;
}

int GzRender::GzRotYMat(float degree, GzMatrix mat)
{
/* HW 3.2
// Create rotate matrix : rotate along y axis
// Pass back the matrix using mat value
*/

	return GZ_SUCCESS;
}

int GzRender::GzRotZMat(float degree, GzMatrix mat)
{
/* HW 3.3
// Create rotate matrix : rotate along z axis
// Pass back the matrix using mat value
*/

	return GZ_SUCCESS;
}

int GzRender::GzTrxMat(GzCoord translate, GzMatrix mat)
{
/* HW 3.4
// Create translation matrix
// Pass back the matrix using mat value
*/

	return GZ_SUCCESS;
}


int GzRender::GzScaleMat(GzCoord scale, GzMatrix mat)
{
/* HW 3.5
// Create scaling matrix
// Pass back the matrix using mat value
*/

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

	framebuffer = (char*) malloc (3 * sizeof(char) * xRes * yRes);
	// framebuffer = (char*)malloc(3 * xres * yres);
	pixelbuffer = (GzPixel*)malloc(xres * yres * sizeof(GzPixel));

/* HW 3.6
- setup Xsp and anything only done once 
- init default camera 
*/ 
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

int GzRender::GzBeginRender()
{
/* HW 3.7 
- setup for start of each frame - init frame buffer color,alpha,z
- compute Xiw and projection xform Xpi from camera definition 
- init Ximage - put Xsp at base of stack, push on Xpi and Xiw 
- now stack contains Xsw and app can push model Xforms when needed 
*/ 

	return GZ_SUCCESS;
}

int GzRender::GzPutCamera(GzCamera camera)
{
/* HW 3.8 
/*- overwrite renderer camera structure with new camera definition
*/

	return GZ_SUCCESS;	
}

int GzRender::GzPushMatrix(GzMatrix	matrix)
{
/* HW 3.9 
- push a matrix onto the Ximage stack
- check for stack overflow
*/
	
	return GZ_SUCCESS;
}

int GzRender::GzPopMatrix()
{
/* HW 3.10
- pop a matrix off the Ximage stack
- check for stack underflow
*/

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
	GzColor* colorPtr;
	for (int i = 0; i < numAttributes; i++) {
		if (nameList[i] == GZ_RGB_COLOR) {
			colorPtr = (GzColor*)valueList[i];

		}
	}

	flatcolor[0] = (*colorPtr)[0];
	flatcolor[1] = (*colorPtr)[1];
	flatcolor[2] = (*colorPtr)[2];

	return GZ_SUCCESS;
}

float* SpanLine(float* cur, float* left, float* right, float deltaX) {
	float slopez = (right[2] - left[2]) / (right[0] - left[0]); // dz/dx 

	// float deltaX = ceil(left[0]) - left[0]; // ¦¤X = ceil(LX) - LX
	// float deltaX = ceil(cur[0]) - cur[0]; // ¦¤X = ceil(LX) - LX

	float curx = cur[0] + deltaX; // LX + ¦¤X
	float cury = cur[1];
	float curz = cur[2] + deltaX * slopez; // LZ + ¦¤X * slopez

	GzCoord spanCur = { curx, cury, curz };

	return spanCur;
}

float* Interpolate(float* cur, float* start, float* end, float deltaY, float* newCur) {
	float slopex = (end[0] - start[0]) / (end[1] - start[1]); // dx/dy
	float slopez = (end[2] - start[2]) / (end[1] - start[1]); // dz/dy

	// float deltaY = y - cur[1];
	//float deltaY = ceil(start[1]) - start[1]; // ¦¤Y = ceil(V1(Y)) ¨C V1(Y) 
	// float deltaY = ceil(cur[1]) - cur[1];

	newCur[0] = cur[0] + slopex * deltaY; // X + SLOPEx * delta Y
	newCur[1] = cur[1] + deltaY; // Y + delta Y
	newCur[2] = cur[2] + slopez * deltaY; // Z + SLOPEz * delta Y

	//float curx = cur[0] + slopex * deltaY; // X + SLOPEx * delta Y
	//float cury = cur[1] + deltaY; // Y + delta Y
	//float curz = cur[2] + slopez * deltaY; // Z + SLOPEz * delta Y

	//GzCoord current = { curx, cury, curz };

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

float CalculateLine(float* p1, float* p2, float y) {
	float slope = (p2[1] - p1[1]) / (p2[0] - p1[0]);

	float x = (y - p1[1]) / slope + p1[0];
	return x;
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
*/
	GzCoord* vertexListPtr; // GzCoord Pointer
	for (int i = 0; i < numParts; i++) {
		if (nameList[i] == GZ_POSITION) {
			vertexListPtr = (GzCoord*)valueList[i];
		}
	}

	// 3 vertices of tri
	float* v1 = vertexListPtr[0]; // GzCoord (float Array) 
	float* v2 = vertexListPtr[1];
	float* v3 = vertexListPtr[2];

	/*
		Rasterization
		Scan Line
	*/

	// Set up top, middle, bottom vertex
	// Sort by y
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


	// dy = 0.1f;
	GzCoord leftCur = { 0, 0, 0 };
	GzCoord rightCur = { 0, 0, 0 };


	// Initial Left/Right
	float* left = top;
	float* right = top;

	//if (ceil(top[1]) == ceil(mid[1])) {

	//	if (SortEdges(top, mid, bottom) == 1) { // counter-clockwise
	//		left = mid;
	//		right = top;
	//	}
	//	else
	//	{
	//		left = top;
	//		right = mid;
	//	}

	//}

	float dy = ceil(top[1]) - top[1]; // ¦¤Y = ceil(V1(Y)) ¨C V1(Y) 

	for (int y = ceil(top[1]); y < ceil(mid[1]); y++) {

		/* Differentiate Left and Right edges */
		//if (ceil(top[1]) != ceil(mid[1])) {
		if (SortEdges(top, mid, bottom) == 1) { // mid at the left of top (top-mid-btm counter-clockwise)
			left = Interpolate(left, top, mid, dy, leftCur);
			right = Interpolate(right, top, bottom, dy, rightCur);
		}
		else if (SortEdges(top, mid, bottom) == 0) { // (top-mid-btm clockwise)
			left = Interpolate(left, top, bottom, dy, leftCur);
			right = Interpolate(right, top, mid, dy, rightCur);
		}
		//}


		/* Span from left to right */
		float* spanCur = left;
		float dx = ceil(left[0]) - left[0]; // ¦¤X = ceil(LX) - LX

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

