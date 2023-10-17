/* Texture functions for cs580 GzLib	*/
#include    "stdafx.h" 
#include	"stdio.h"
#include	"Gz.h"

GzColor	*image=NULL;
int xs, ys;
int reset = 1;

int FindIndex(int x, int y) {
    return (x + y * xs);
}

/* Image texture function */
int tex_fun(float u, float v, GzColor color)
{
  unsigned char		pixel[3];
  unsigned char     dummy;
  char  		foo[8];
  int   		i, j;
  FILE			*fd;

  if (reset) {          /* open and load texture file */
    fd = fopen ("texture", "rb");
    if (fd == NULL) {
      fprintf (stderr, "texture file not found\n");
      exit(-1);
    }
    fscanf (fd, "%s %d %d %c", foo, &xs, &ys, &dummy);
    image = (GzColor*)malloc(sizeof(GzColor)*(xs+1)*(ys+1));
    if (image == NULL) {
      fprintf (stderr, "malloc for texture image failed\n");
      exit(-1);
    }

    for (i = 0; i < xs*ys; i++) {	/* create array of GzColor values */
      fread(pixel, sizeof(pixel), 1, fd);
      image[i][RED] = (float)((int)pixel[RED]) * (1.0 / 255.0);
      image[i][GREEN] = (float)((int)pixel[GREEN]) * (1.0 / 255.0);
      image[i][BLUE] = (float)((int)pixel[BLUE]) * (1.0 / 255.0);
      }

    reset = 0;          /* init is done */
	fclose(fd);
  }

/* bounds-test u,v to make sure nothing will overflow image array bounds */
/* determine texture cell corner values and perform bilinear interpolation */
/* set color to interpolated GzColor value and return */

    // bound check u, v
    if (u < 0 || u >= 1 || v < 0 || v >= 1) {
        return GZ_FAILURE;
    }

    // Scale to texture image 
    float sampleX = u * (xs - 1);
    float sampleY = v * (ys - 1);

    // Bilinear Interpolation
    float s = sampleX - floor(sampleX);
    float t = sampleY - floor(sampleY);

    // Color(p) = s t C + (1-s) t D + s (1-t) B + (1-s) (1-t) A 
    // A B C D (square clockwise) are pixel RGB colors at neighbouring texels
    int iA = FindIndex(floor(sampleX), floor(sampleY));
    GzColor colorA = { image[iA][RED], image[iA][GREEN], image[iA][BLUE] };

    int iB = FindIndex(ceil(sampleX), floor(sampleY));
    GzColor colorB = { image[iB][RED], image[iB][GREEN], image[iB][BLUE] };

    int iC = FindIndex(floor(sampleX), ceil(sampleY));
    GzColor colorC = { image[iC][RED], image[iC][GREEN], image[iC][BLUE] };

    int iD = FindIndex(ceil(sampleX), ceil(sampleY));
    GzColor colorD = { image[iD][RED], image[iD][GREEN], image[iD][BLUE] };

    for (int i = 0; i < 3; i++) {
        color[i] = s * t * colorC[i] + (1 - s) * t * colorD[i] + s * (1 - t) * colorB[i]
            + (1 - s) * (1 - t) * colorA[i];
    }
    
	return GZ_SUCCESS;
}

/* Procedural texture function */
int ptex_fun(float u, float v, GzColor color)
{
    /*
        if odd interval -> white
        else if even interval -> black
    */

    // bound check u, v
    if (u < 0 || u >= 1 || v < 0 || v >= 1) {
        return GZ_FAILURE;
    }
   
    float totalInterval = 6; 
    float oneInterval = 1 / totalInterval;

    float numIntvlU = u / oneInterval; // how many intervals between 0 - u/v
    float numIntvlV = v / oneInterval;

    int uI = ceil(numIntvlU);
    int vI = ceil(numIntvlV);

    if ((uI % 2 == 0 && vI % 2 == 0) || (uI % 2 != 0 && vI % 2 != 0)) { // even or odd at the same time
        for (int i = 0; i < 3; i++) {
            color[i] = 100;
        }
    }
    else if ((uI % 2 == 0 && vI % 2 != 0) || (uI % 2 != 0 && vI % 2 == 0)) { // one is even the other is odd
        for (int i = 0; i < 3; i++) {
            color[i] = 0;
        }
    }

	return GZ_SUCCESS;
}

/* Free texture memory */
int GzFreeTexture()
{
	if(image!=NULL)
		free(image);
	return GZ_SUCCESS;
}

