/*********************************************************************/
/* vtpeak:     Threshold image between two most sig. hgram peaks     */
/*********************************************************************/

#include "VisXV4.h"          /* VisionX structure include file       */
#include "Vutil.h"           /* VisionX utility header files         */

VXparam_t par[] =            /* command line structure               */
{
{  "if=",   0,   " input file, vtpeak: threshold between hgram peaks"},
{  "of=",   0,   " output file "},
{  "d=",    0,   " min dist between hgram peaks (default 10)"},
{  "-v",    0,   "(verbose) print threshold information"},
{   0,      0,   0} /* list termination */
};
#define  IVAL   par[0].val
#define  OVAL   par[1].val
#define  DVAL   par[2].val
#define  VFLAG  par[3].val

main(argc, argv)
int argc;
char *argv[];
{

    Vfstruct (im);                 /* input image structure          */
    int y,x;                       /* index counters                 */
    int i;

    int hist[256];                 /* histogram bins                 */
    int thresh;                    /* threshold                      */
    int maxbin;                    /* maximum histogram bin          */
    int nxtbin;                    /* second maximum bin             */
    int minbin;                    /* minumim histogram bin          */
    int maxa, maxb;       /* second maximum bin above/below maxbin   */
    int dist;                   /* minimum distance between maxima   */
			     
    VXparse(&argc, &argv, par);    /* parse the command line         */

    dist = 10;                    /* default dist */
    if (DVAL) dist = atoi(DVAL);  /* if d= was specified, get value */
    if (dist < 0 || dist > 255) {
	fprintf(stderr, "d= must be between 0 and 255\nUsing d=10\n");
        dist = 10;
    }

    while ( Vfread( &im, IVAL) ) {
        if ( im.type != VX_PBYTE ) {
              fprintf (stderr, "error: image not byte type\n");
              exit (1);
        }

        /* clear the histogram */
        for (i = 0; i < 256; i++) hist[i] = 0;
 
        /* compute the histogram */
        for (y = im.ylo; y <= im.yhi; y++)
            for (x = im.xlo; x <= im.xhi; x++)
                 hist[im.u[y][x]]++;
  
         /* compute the threshold */
  
         /* find maximum bin for the entire histogram */
         maxbin = 0;
         for (i = 0; i <= 255; i++)
             if (hist[i] > hist[maxbin]) maxbin = i;

         /* find the next maximum bin at dist below maxbin */
         maxb = maxbin-dist;
         for (i = maxbin-dist; i >=   0; i--)
                if (hist[i] > hist[maxb]) maxb = i;
  
         /* find the next maximum bin at dist above maxbin */
        maxa = maxbin+dist;
        for (i = maxbin+dist; i <= 255; i++)
             if (hist[i] > hist[maxa]) maxa = i;
  
        /* if second max found above and below take maximum */
        if (maxa <= 255 && maxb >= 0) {
            if (hist[maxa] > hist[maxb]) nxtbin = maxa;
            else                         nxtbin = maxb;
        }
        /* if only found second max above */
        else if (maxa <= 255) nxtbin = maxa;
        /* if only found second max below */
        else if (maxb >=   0) nxtbin = maxb;
  
        /* find minimum between peaks and use as threshold */
        if (nxtbin < maxbin) {
            minbin = nxtbin;
            for (i = nxtbin; i <= maxbin; i++)
                if (hist[i] < hist[minbin]) minbin = i;
        } else {
            minbin = maxbin;
            for (i = maxbin; i <= nxtbin; i++)
                if (hist[i] < hist[minbin]) minbin = i;
        }
  
        thresh = minbin;
  
        if(VFLAG)
             fprintf(stderr, "maxbin = %d nxtbin = %d thresh = %d\n",
                                  maxbin, nxtbin, thresh);
  
        /* apply the threshold */
        for (y = im.ylo; y <= im.yhi; y++) {
            for (x = im.xlo; x <= im.xhi; x++) {
                 if (im.u[y][x] >= thresh) im.u[y][x] = 255;
                 else                      im.u[y][x] = 0;
            }
        }
  
        Vfwrite( &im, OVAL);
    } /* end of every frame section */
    exit(0);
}
