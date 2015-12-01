/*********************************************************************/
/* vits:     Threshold image between two regions with equal avg grey value */
/*********************************************************************/

#include "VisXV4.h"          /* VisionX structure include file       */
#include "Vutil.h"           /* VisionX utility header files         */

VXparam_t par[] =            /* command line structure               */
{
{  "if=",   0,   " input file "},
{  "of=",   0,   " output file "},
{  "it=",   0,   " intial thresh value "},
{  "-v",    0,   "(verbose) print threshold information"},
{   0,      0,   0} /* list termination */
};
#define  IVAL   par[0].val
#define  OVAL   par[1].val
#define  ITVAL  par[2].val
#define  VFLAG  par[3].val

main(argc, argv)
int argc;
char *argv[];
{

    Vfstruct (im);                 /* input image structure          */
    int y,x;                       /* index counters                 */

    int ithresh;                   /* minimum distance between maxima   */
			     
    VXparse(&argc, &argv, par);    /* parse the command line         */

    ithresh = 100;                    /* default dist */
    if (ITVAL) ithresh = atoi(ITVAL);  /* if d= was specified, get value */

    while ( Vfread( &im, IVAL) ) {
        if ( im.type != VX_PBYTE ) {
              fprintf (stderr, "error: image not byte type\n");
              exit (1);
        }

        /* apply the threshold */
        for (y = im.ylo; y <= im.yhi; y++) {
            for (x = im.xlo; x <= im.xhi; x++) {
                 if (im.u[y][x] >= ithresh) im.u[y][x] = 255;
                 else                       im.u[y][x] = 0;
            }
        }
  
        Vfwrite( &im, OVAL);
    } /* end of every frame section */
    exit(0);
}
