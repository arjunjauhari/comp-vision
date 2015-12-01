/*******************************************************************/
/* vtemp      Compute local max operation on a single byte image   */
/*******************************************************************/

#include "VisXV4.h"           /* VisionX structure include file    */
#include "Vutil.h"            /* VisionX utility header files      */

VXparam_t par[] =             /* command line structure            */
{ /* prefix, value,   description                         */   
{    "if=",    0,   " input file  vtemp: local max filter "},
{    "of=",    0,   " output file "},
{    "r=",     0,   " range value "},
{    "-p",     0,   " labeling scheme information "},
{     0,       0,   0}  /* list termination */
};

#define  IVAL   par[0].val
#define  OVAL   par[1].val
#define  RANGE  par[2].val
#define  PFLAG  par[3].val

void setlabel(int, int, int);
Vfstruct (im);                      /* i/o image structure          */
Vfstruct (tm);                      /* temp image structure         */
int first;
int range;                 /* range for region             */

main(argc, argv)
int argc;
char *argv[];
{
int        y,x;                     /* index counters               */
int        L = 1;                     /* index counters               */
  VXparse(&argc, &argv, par);       /* parse the command line       */

  range = 10;                    /* default dist */
  if (RANGE) range = atoi(RANGE);  /* if d= was specified, get value */

  Vfread(&im, IVAL);                /* read image file              */
  Vfembed(&tm, &im, 1,1,1,1);       /* image structure with border  */
  if ( im.type != VX_PBYTE ) {      /* check image format           */
     fprintf(stderr, "vtemp: no byte image data in input file\n");
     exit(-1);
  }
  //fprintf (stderr, "the value of ylo,yhi xlo,xhi is %d,%d and %d,%d\n", im.ylo,im.yhi,im.xlo,im.xhi);
  //fprintf (stderr, "checking abs %d\n", abs(-5));
  /* Zeroing out the output file pixels im */
  for (y = im.ylo ; y <= im.yhi ; y++) {
     for (x = im.xlo; x <= im.xhi; x++)  {
     	im.u[y][x] = 0;
     }
   }

  for (y = im.ylo ; y <= im.yhi ; y++) {
     for (x = im.xlo; x <= im.xhi; x++)  {
        if (tm.u[y][x] != 0) {
     	if (im.u[y][x] == 0) {
	    first = tm.u[y][x];
	    if (PFLAG) {
	        setlabel(x,y,first);
	    }
	    else {
	        setlabel(x,y,L);
	        L = (L % 255) + 1;
	    }
	}
	}
     }
   }
   Vfwrite(&im, OVAL);             /* write image file                */
   exit(0);
}

/* function to compute the local maximum */
void setlabel(int x, int y, int L) 
{						
    im.u[y][x] = L;
	if (tm.u[y][x+1] != 0 && im.u[y][x+1] == 0 && abs(tm.u[y][x+1] - first) <= range)
		setlabel(x+1,y,L);
	if (tm.u[y][x-1] != 0 && im.u[y][x-1] == 0 && abs(tm.u[y][x-1] - first) <= range)
		setlabel(x-1,y,L);
	if (tm.u[y+1][x] != 0 && im.u[y+1][x] == 0 && abs(tm.u[y+1][x] - first) <= range)
		setlabel(x,y+1,L);
	if (tm.u[y-1][x] != 0 && im.u[y-1][x] == 0 && abs(tm.u[y-1][x] - first) <= range)
		setlabel(x,y-1,L);
}
