#include	"stdafx.h"
#include	"stdio.h"
#include	"math.h"
#include	"Gz.h"
#include	"rend.h"
/*   CS580 HW   */
#include    "stdafx.h"  
#include	"Gz.h"

GzRender::GzRender(int xRes, int yRes)
{
/* HW1.1 create a framebuffer for MS Windows display:
 -- set display resolution
 -- allocate memory for framebuffer : 3 bytes(b, g, r) x width x height
 -- allocate memory for pixel buffer
 */
	xres = xRes;
	yres = yRes;

	framebuffer = (char*) malloc(3 * xres * yres);
	pixelbuffer = (GzPixel*) malloc(xres * yres * sizeof(GzPixel));
	
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
	GzPixel pixel = { 255, 255, 255, 1, 0 };

	for (int i = 0; i < xres * yres; i++) {
		pixelbuffer[i] = pixel;
	}

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
	
	 // fclose(outfile);
	
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

		framebuffer[j] = (char) pixelbuffer[i].blue;
		framebuffer[j+1] = (char) pixelbuffer[i].green;
		framebuffer[j+2] = (char) pixelbuffer[i].red;

		j += 3;
	}

	return GZ_SUCCESS;
}