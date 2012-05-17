/*
 * Some stuff to load texture and images in OpenGL
 *
 * Copyright (C) 2010  Matthieu DANIEL <matth.daniel@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 **/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <jpeglib.h>
#include <jerror.h>
#include <tiffio.h>

#ifdef __APPLE__ 
#include <OpenGL/gl.h> 
#else 
#include <GL/gl.h>
#endif 


#ifdef __APPLE__ 
#include <OpenGL/glu.h>
#else 
#include <GL/glu.h> 
#endif 


#ifdef __APPLE__ 
#include <GLUT/glut.h>
#else 
#include <GL/glut.h>
#endif 


void  puissance2( int largpx, int * ll, int * mm )
{
  int k=largpx;
  int l,m;
  l=1;
  m=0;
  while( k/2 != 0 ) {m++; k/=2; l*=2;}
  *ll=l;
  *mm=m;
}


int  ImagePuissanceDe2(int   largpx,  
					   int   hautpx,  
					   int   nbc1px,  
					   unsigned char *   image,
					   int * largpx2, 
					   int * hautpx2, 
					   int * nbc1px2, 
					   unsigned char **  image2 )
{
  int l, m;
  puissance2( largpx, largpx2, &m );

  int h, n;
  puissance2( hautpx, hautpx2, &n ); 
	
  *image2 = NULL;
  if( *largpx2 <= 0 || *hautpx2 <= 0 || nbc1px  <= 0 ) return 1;

  int k=0;
  int i,j;
  int p ;
  if( nbc1px >= 3 )
    {
      *image2 = (unsigned char *)malloc(sizeof(unsigned char *)*(*largpx2) * (*hautpx2) * nbc1px);
      if( *image2 == NULL ) return 2;
      for (  i=0; i<*largpx2; i++)
	for ( j=0; j<*hautpx2; j++)
	{
	  m = (i*hautpx+j) * nbc1px;
	  for ( p=0; p<nbc1px; p++ )
	    (*image2)[k++] = image[ m + p ];
	}
      *nbc1px2 = nbc1px;
    }
  else
    {
      *image2 = (unsigned char *) malloc(sizeof(unsigned char) * (*largpx2) * (*hautpx2) * 3);
      if( *image2 == NULL ) return 3;
      for ( i=0; i<*largpx2; i++)
	for ( j=0; j<*hautpx2; j++)
	{
	  m = (i*hautpx+j) * nbc1px;
	  for ( p=0; p<3; p++ )
	    (*image2)[k++] = image[ m ];
	}
      *nbc1px2 = 3;
    }
  return 0;
}

int  ImageTiff(char *NomFichierTiff, 
			   int * largpx, 
			   int * hautpx, 
			   int * nbc1px,
			   unsigned char ** image )
{
  *nbc1px  = 3;
  *image = NULL;

  int i, j;
  size_t npixels;
  uint32* raster;
  
  printf("fichier %s \n", NomFichierTiff);
  TIFF* tif = TIFFOpen(NomFichierTiff, "r");
  if (tif)
  {
    TIFFGetField( tif, TIFFTAG_IMAGEWIDTH,  largpx );
    TIFFGetField( tif, TIFFTAG_IMAGELENGTH, hautpx ); 

    *image = ( unsigned char *)malloc( sizeof(unsigned char)*(*largpx) * (*hautpx) * (*nbc1px));
    if( *image == NULL ) return 1;
 
    npixels = (*largpx) * (*hautpx);
    raster = (uint32*) _TIFFmalloc( npixels * sizeof(uint32) ); 
    if (raster != NULL)
    {
      // lecture de l'image
      if( TIFFReadRGBAImage(tif,  *largpx,  *hautpx,  raster,  1) )
      {
	// transfert de l'image vers le tableau 'image'
	int k=0;
	for (i=0;i<*largpx;i++)
	  for (j=0;j<*hautpx;j++)
	  {
	    (*image)[k++]=((unsigned char *)raster)[i*(*hautpx)*4+j*4+0];
	    (*image)[k++]=((unsigned char *)raster)[i*(*hautpx)*4+j*4+1];
	    (*image)[k++]=((unsigned char *)raster)[i*(*hautpx)*4+j*4+2];
	  }
      }
      else
      {
	printf("Erreur de chargement du fichier %s\n", NomFichierTiff);
	return 2;
      }
      _TIFFfree(raster); 
    }
    TIFFClose(tif);
  }
  printf("Fichier Tiff image      %s \n", NomFichierTiff);
  return 0;
}


int  chargeTextureTiff(int numtex, 
					   char *NomFichierTiff,
					   int * largpx2, 
					   int * hautpx2, 
					   int * nbc1px2, 
					   unsigned char ** image2 )
{
  int largpx=0, hautpx=0, nbc1px=0;
  unsigned char * image  = NULL;
  *image2 = NULL;

  if( ImageTiff( NomFichierTiff, &largpx, &hautpx, &nbc1px, &image ) ) return 1;
  
  if( ImagePuissanceDe2( largpx,  hautpx,  nbc1px,  image,
			 largpx2, hautpx2, nbc1px2, image2 ) ) return 2;
  if( *image2 == NULL ) return 3;

  printf("Fichier Tiff image      %s \n", NomFichierTiff);
  printf("largeur pixels=%d hauteur pixels= %d  bytes par pixel= %d \n", *largpx2,*hautpx2, *nbc1px2);
  
  return 0;
}


int  ImageJpeg(char * NomFichierJpeg,
			   int  * largpx, 
			   int * hautpx, 
			   int  * nbc1px,
			   unsigned char **  image )
{
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;

  // Initialize the JPEG decompression object with personal error handling
  cinfo.err = jpeg_std_error( &jerr );

  // Allocate and initialize a JPEG decompression object
  jpeg_create_decompress( &cinfo );

  FILE * file;
  if ( (file=fopen( NomFichierJpeg, "rb" ) )==NULL )
    {
      printf("Erreur: Impossible ouvrir le fichier %s \n", NomFichierJpeg);
      return 1;
    }

  // Specify data source for decompression
  jpeg_stdio_src(   &cinfo, file );

  // Read file header, set default decompression parameters
  jpeg_read_header( &cinfo, TRUE );
  /*
  if (cinfo.jpeg_color_space==JCS_GRAYSCALE)
  { cerr << "Erreur: l'image doit etre de type RGB\n"; return 2; }
  */
  // Start decompressor
  jpeg_start_decompress( &cinfo );

  *largpx  = cinfo.image_width;  //image pixel width
  *hautpx  = cinfo.image_height; //image pixel height
  *nbc1px = cinfo.output_components;
  printf("Fichier Jpeg image en MC %s \n", NomFichierJpeg);
  printf("largeur pixels= %d hauteur pixels = %d octets par pixel = %d \n",*largpx,*hautpx,*nbc1px);

  //Process data
  *image = (unsigned char *)malloc(sizeof(unsigned char)*(*largpx) * (*hautpx) * (*nbc1px));
  if( *image == NULL ) return 2;
  unsigned char * ligne = (unsigned char * )malloc(sizeof(unsigned char)*(*largpx) * (*nbc1px));
  if( ligne == NULL ) return 2;
  int m;
  int j,k;
  //Transposition car les pixels sont lus par ligne
  //et le stockage de image est par colonne
  while ( cinfo.output_scanline < cinfo.output_height )
  {  
    jpeg_read_scanlines( &cinfo, &ligne, 1 );  //Reading line by line

    for ( j=0; j<*largpx; j++ )
    {
      m = ( cinfo.output_scanline * (*hautpx) + j ) * (*nbc1px);
      for ( k=0; k<*nbc1px; k++ )
	(*image)[ (cinfo.output_scanline*(*hautpx) + j) * (*nbc1px) + k] = ligne[j*(*nbc1px)+k];
    }
  }

  // Finish decompression and release memory
  jpeg_finish_decompress(  &cinfo );
  jpeg_destroy_decompress( &cinfo );
  fclose(file);

  return 0;
}


int  chargeTextureJpeg( int numtex, char * NomFichierJpeg,
			int  * largpx2, int * hautpx2, int *  nbc1px2, unsigned char **  image2 )
// Chargement de l'image jpeg du fichier NomFichierJpeg
// Ajustement a une puissance  de 2 de la largeur et hauteur en pixels de l'image
// Construction d'une texture numtex a partir de l'image ajustee
{
  int largpx=0, hautpx=0, nbc1px=0;
  unsigned char * image  = NULL;
  *image2 = NULL;

  if( ImageJpeg( NomFichierJpeg,
		 &largpx, &hautpx, &nbc1px, &image ) ) return 1;
  //image RGB en memoire de largpx x hautpx pixels

  //Pour glTexImage2D: largpx = 2**m + 2*border obligatoirement! >= 64 ou 66
  //Pour glTexImage2D: hautpx = 2**n + 2*border obligatoirement! >= 64 ou 66
  if( ImagePuissanceDe2( largpx,  hautpx,  nbc1px,  image,
			 largpx2, hautpx2, nbc1px2, image2 )) return 1;
  if( *image2 == NULL ) return 2;
  printf("Fichier Jpeg image en MC %s \n", NomFichierJpeg);
  printf( "  largeur pixels= %d hauteur pixels = %d octets par pixel = %d \n",*largpx2,*hautpx2,*nbc1px2);

  //Parametrage de la texture avec largpx = 2**m et hautpx = 2**n
 /*  glBindTexture(   GL_TEXTURE_2D, numtex ); */
/*   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );  //magnify */
/*   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST ); */

/*   glTexImage2D( GL_TEXTURE_2D, 0, *nbc1px2, *largpx2, *hautpx2, 0, */
/* 	        GL_RGB, GL_UNSIGNED_BYTE, (GLvoid *) (*image2) ); */
/*   glEnable( GL_TEXTURE_2D ); */

  return 0;
}

void
bwtorgba(unsigned char *b,unsigned char *l,int n) {
    while(n--) {
	l[0] = *b;
	l[1] = *b;
	l[2] = *b;
	l[3] = 0xff;
	l += 4; b++;
    }
}

void
latorgba(unsigned char *b, unsigned char *a,unsigned char *l,int n) {
    while(n--) {
	l[0] = *b;
	l[1] = *b;
	l[2] = *b;
	l[3] = *a;
	l += 4; b++; a++;
    }
}

void
rgbtorgba(unsigned char *r,unsigned char *g,unsigned char *b,unsigned char *l,int n) {
    while(n--) {
	l[0] = r[0];
	l[1] = g[0];
	l[2] = b[0];
	l[3] = 0xff;
	l += 4; r++; g++; b++;
    }
}

void
rgbatorgba(unsigned char *r,unsigned char *g,unsigned char *b,unsigned char *a,unsigned char *l,int n) {
    while(n--) {
	l[0] = r[0];
	l[1] = g[0];
	l[2] = b[0];
	l[3] = a[0];
        l += 4; r++; g++; b++; a++;
    }
}

typedef struct _ImageRec {
    unsigned short imagic;
    unsigned short type;
    unsigned short dim;
    unsigned short xsize, ysize, zsize;
    unsigned int min, max;
    unsigned int wasteBytes;
    char name[80];
    unsigned long colorMap;
    FILE *file;
    unsigned char *tmp, *tmpR, *tmpG, *tmpB;
    unsigned long rleEnd;
    unsigned int *rowStart;
    int *rowSize;
} ImageRec;

static void
ConvertShort(unsigned short *array, long length) {
    unsigned b1, b2;
    unsigned char *ptr;

    ptr = (unsigned char *)array;
    while (length--) {
	b1 = *ptr++;
	b2 = *ptr++;
	*array++ = (b1 << 8) | (b2);
    }
}

static void
ConvertLong(unsigned *array, long length) {
    unsigned b1, b2, b3, b4;
    unsigned char *ptr;

    ptr = (unsigned char *)array;
    while (length--) {
	b1 = *ptr++;
	b2 = *ptr++;
	b3 = *ptr++;
	b4 = *ptr++;
	*array++ = (b1 << 24) | (b2 << 16) | (b3 << 8) | (b4);
    }
}

static ImageRec *ImageOpen(const char *fileName)
{
    union {
	int testWord;
	char testByte[4];
    } endianTest;
    ImageRec *image;
    int swapFlag;
    int x;

    endianTest.testWord = 1;
    if (endianTest.testByte[0] == 1) {
	swapFlag = 1;
    } else {
	swapFlag = 0;
    }

    image = (ImageRec *)malloc(sizeof(ImageRec));
    if (image == NULL) {
	fprintf(stderr, "Out of memory!\n");
	exit(1);
    }
    if ((image->file = fopen(fileName, "rb")) == NULL) {
	perror(fileName);
	exit(1);
    }

    fread(image, 1, 12, image->file);

    if (swapFlag) {
	ConvertShort(&image->imagic, 6);
    }

    image->tmp = (unsigned char *)malloc(image->xsize*256);
    image->tmpR = (unsigned char *)malloc(image->xsize*256);
    image->tmpG = (unsigned char *)malloc(image->xsize*256);
    image->tmpB = (unsigned char *)malloc(image->xsize*256);
    if (image->tmp == NULL || image->tmpR == NULL || image->tmpG == NULL ||
	image->tmpB == NULL) {
	fprintf(stderr, "Out of memory!\n");
	exit(1);
    }

    if ((image->type & 0xFF00) == 0x0100) {
	x = image->ysize * image->zsize * sizeof(unsigned);
	image->rowStart = (unsigned *)malloc(x);
	image->rowSize = (int *)malloc(x);
	if (image->rowStart == NULL || image->rowSize == NULL) {
	    fprintf(stderr, "Out of memory!\n");
	    exit(1);
	}
	image->rleEnd = 512 + (2 * x);
	fseek(image->file, 512, SEEK_SET);
	fread(image->rowStart, 1, x, image->file);
	fread(image->rowSize, 1, x, image->file);
	if (swapFlag) {
	    ConvertLong(image->rowStart, x/sizeof(unsigned));
	    ConvertLong((unsigned *)image->rowSize, x/sizeof(int));
	}
    }
    return image;
}

static void
ImageClose(ImageRec *image) {
    fclose(image->file);
    free(image->tmp);
    free(image->tmpR);
    free(image->tmpG);
    free(image->tmpB);
    free(image);
}

static void
ImageGetRow(ImageRec *image, unsigned char *buf, int y, int z) {
    unsigned char *iPtr, *oPtr, pixel;
    int count;

    if ((image->type & 0xFF00) == 0x0100) {
	fseek(image->file, image->rowStart[y+z*image->ysize], SEEK_SET);
	fread(image->tmp, 1, (unsigned int)image->rowSize[y+z*image->ysize],
	      image->file);

	iPtr = image->tmp;
	oPtr = buf;
	while (1) {
	    pixel = *iPtr++;
	    count = (int)(pixel & 0x7F);
	    if (!count) {
		return;
	    }
	    if (pixel & 0x80) {
		while (count--) {
		    *oPtr++ = *iPtr++;
		}
	    } else {
		pixel = *iPtr++;
		while (count--) {
		    *oPtr++ = pixel;
		}
	    }
	}
    } else {
	fseek(image->file, 512+(y*image->xsize)+(z*image->xsize*image->ysize),
	      SEEK_SET);
	fread(buf, 1, image->xsize, image->file);
    }
}

unsigned *
LoadTextureRGBA (char *name, int *width, int *height, int *components) {
    unsigned *base, *lptr;
    unsigned char *rbuf, *gbuf, *bbuf, *abuf;
    ImageRec *image;
    int y;

    image = ImageOpen(name);
    
    if(!image)
	return NULL;
    (*width)=image->xsize;
    (*height)=image->ysize;
    (*components)=image->zsize;
    base = (unsigned *)malloc(image->xsize*image->ysize*sizeof(unsigned));
    rbuf = (unsigned char *)malloc(image->xsize*sizeof(unsigned char));
    gbuf = (unsigned char *)malloc(image->xsize*sizeof(unsigned char));
    bbuf = (unsigned char *)malloc(image->xsize*sizeof(unsigned char));
    abuf = (unsigned char *)malloc(image->xsize*sizeof(unsigned char));
    if(!base || !rbuf || !gbuf || !bbuf)
      return NULL;
    lptr = base;
    for(y=0; y<image->ysize; y++) {
	if(image->zsize>=4) {
	    ImageGetRow(image,rbuf,y,0);
	    ImageGetRow(image,gbuf,y,1);
	    ImageGetRow(image,bbuf,y,2);
	    ImageGetRow(image,abuf,y,3);
	    rgbatorgba(rbuf,gbuf,bbuf,abuf,(unsigned char *)lptr,image->xsize);
	    lptr += image->xsize;
	} else if(image->zsize==3) {
	    ImageGetRow(image,rbuf,y,0);
	    ImageGetRow(image,gbuf,y,1);
	    ImageGetRow(image,bbuf,y,2);
	    rgbtorgba(rbuf,gbuf,bbuf,(unsigned char *)lptr,image->xsize);
	    lptr += image->xsize;
	} else if(image->zsize==2) {
	    ImageGetRow(image,rbuf,y,0);
	    ImageGetRow(image,abuf,y,1);
	    latorgba(rbuf,abuf,(unsigned char *)lptr,image->xsize);
	    lptr += image->xsize;
	} else {
	    ImageGetRow(image,rbuf,y,0);
	    bwtorgba(rbuf,(unsigned char *)lptr,image->xsize);
	    lptr += image->xsize;
	}
    }
    ImageClose(image);
    free(rbuf);
    free(gbuf);
    free(bbuf);
    free(abuf);

    return (unsigned *) base;
}
