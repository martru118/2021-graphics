/*****************************************************
 *
 *                texture
 *
 *  Convert a single variable file from the
 *  hurricane Isable data into a texture.  Produces
 *  both a full resolution texture and a quarter
 *  resolution texture.
 *
 ***************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define XDIM	500
#define YDIM	500
#define ZDIM	100

float field[ZDIM][YDIM][XDIM];

unsigned char texture[ZDIM][YDIM][XDIM][3];
unsigned char texture2[ZDIM/2][YDIM/2][XDIM/2][3];

float vmin;
float vmax;

extern int errno; 

int Space = 2;

/*
 *  Read the variable data file
 */
void read_data(char *filename) {
	FILE *infile;
	int n;
	int e;

	infile = fopen(filename,"rb");
	if(infile == NULL) {
		printf("Couldn't open data file: %s\n",filename);
	}
	n = fread(&(field[0][0][0]), sizeof(float), XDIM*YDIM*ZDIM, infile);

	fclose(infile);

}

// from: http://www.cs.rit.edu/~ncs/color/t_convert.html

void HSVtoRGB( float& r, float& g, float& b, float h, float s, float v )
{
	int i;
	float f, p, q, t;
	if( s == 0 ) {
		// achromatic (grey)
		r = g = b = v;
		return;
	}
	h /= 60;			// sector 0 to 5
	i = floor( h );
	f = h - i;			// factorial part of h
	p = v * ( 1 - s );
	q = v * ( 1 - s * f );
	t = v * ( 1 - s * ( 1 - f ) );
	switch( i ) {
		case 0:
			r = v;
			g = t;
			b = p;
			break;
		case 1:
			r = q;
			g = v;
			b = p;
			break;
		case 2:
			r = p;
			g = v;
			b = t;
			break;
		case 3:
			r = p;
			g = q;
			b = v;
			break;
		case 4:
			r = t;
			g = p;
			b = v;
			break;
		default:		// case 5:
			r = v;
			g = p;
			b = q;
			break;
	}
}


void colour(float value, unsigned char& r, unsigned char& g, unsigned char& b) {
	float scale;
	float h, s, v;
	float R, G, B;

	switch(Space) {
	case 1:   // grey scale
		scale = 256.0/(vmax-vmin);
		v = (value-vmin)*scale;
		r = (unsigned char) v;
		g = (unsigned char) v;
		b = (unsigned char) v;
		break;
	case 2:   // HSV
		scale = 360.0/(vmax-vmin);
		h = (value-vmin)*scale;
		s = 0.5;
		v = 0.5;
		HSVtoRGB(R, G, B, h, s, v);
		r = (unsigned char) (R*256.0);
		g = (unsigned char) (G*256.0);
		b = (unsigned char) (B*256.0);
		break;
	}
}

float flip(float x) {
      float result;
      char *in;
      char *out;
      
      in = (char *) &x;
      out = (char *) &result;
      
      out[0] = in[3];
      out[1] = in[2];
      out[2] = in[1];
      out[3] = in[0];
      
      return(result);
      
}


void build_texture() {
	int i,j,k;
	unsigned char r, g, b;
	float value;

	for(i=0; i<ZDIM; i++) {
		for(j=0; j<YDIM; j++) {
			for(k=0; k<XDIM; k++) {
				value = flip(field[i][j][k]);
				if(value > 1e20)
					value = vmin;
				colour(value, r, g, b);
				texture[i][j][k][0] = r;
				texture[i][j][k][1] = g;
				texture[i][j][k][2] = b;
			}
		}
	}
}



void write_data(char *base) {
	char filename[256];
	FILE *outfile;

	sprintf(filename,"%s.tex",base);
	outfile = fopen(filename, "w");
	fwrite(texture, XDIM*YDIM*ZDIM*3, sizeof(unsigned char), outfile);
	fclose(outfile);

}

int main(int argc, char **argv) {
	int type = 1;
	int time = 20;
	char infile[256];
	char base[256];

	if(argc > 1)
		type = atoi(argv[1]);
	if(argc > 2)
		time = atoi(argv[2]);
	if(argc > 3)
		Space = atoi(argv[3]);

	switch(type) {
	case 1:	// temperature
		sprintf(infile,"../TCf%d.bin",time);
		sprintf(base,"../TCf%d",time);
		vmin = -83.00402;
		vmax = 31.51576;
		break;
	case 2: // pressure
		sprintf(infile,"../Pf%d.bin",time);
		sprintf(base,"../Pf%d",time);
		vmin = -5471.85791;
		vmax = 3225.42578;
		break;
	case 3:	// precipitation
		sprintf(infile,"../PRECIPf%d.bin",time);
		sprintf(base,"../PRECIPf%d",time);
		vmin = 0.0;
		vmax = 0.01672;
		break;
	case 4:	// cloud water
		sprintf(infile,"../QCLOUDf%d.bin",time);
		sprintf(base,"../QCLOUD%d",time);
		vmin = 0.0;
		vmax = 0.00332;
		break;
	default:
		printf("unrecognized variable: %d\n", type);
		exit(0);
	}

	printf("load Data: %s\n",infile);
	read_data(infile);
	printf("compute texture\n");
	build_texture();
	printf("write textures\n");
	write_data(base);

	return(0);
}
