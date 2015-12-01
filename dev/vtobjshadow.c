/*********************************************************************/
/* vtpeak:     Threshold image between two most sig. hgram peaks     */
/*********************************************************************/

#include "VisXV4.h"          /* VisionX structure include file       */
#include "Vutil.h"           /* VisionX utility header files         */

VXparam_t par[] =            /* command line structure               */
{
{  "if=",   0,   " input file, vtpeak: threshold between hgram peaks"},
{  "of=",   0,   " output file "},
{  "of2=",  0,   " output file "},
{  "d=",    0,   " min mindist between hgram peaks (default 10)"},
{  "-v",    0,   "(verbose) print threshold information"},
{   0,      0,   0} /* list termination */
};
#define  IVAL   par[0].val
#define  OVAL   par[1].val
#define  OVAL2  par[2].val
#define  DVAL   par[3].val
#define  VFLAG  par[4].val

main(argc, argv)
int argc;
char *argv[];
{

    Vfstruct (im);                 /* input image structure          */
    Vfstruct (im2);                 /* input image structure          */
    int y,x;                       /* index counters                 */
    int i,j;

    int hist[256];                 /* histogram bins                 */
    int thresh;                    /* threshold                      */
    int maxbin;                    /* maximum histogram bin          */
    int nxtbin;                    /* second maximum bin             */
    int minbin;                    /* minumim histogram bin          */
    int maxa, maxb;       /* second maximum bin above/below maxbin   */
    int npixels=0;                /* total number of pixels */
    int normthresh;
    int niter;
    int npeaks;
    int peaks[256];             /* peaks array                 */
    int flag;
    int rerun;
    int dist;
    int mindist;
    int shadowth, objth;

			     
    VXparse(&argc, &argv, par);    /* parse the command line         */

    mindist = 7;                    /* default mindist */
    if (DVAL) mindist = atoi(DVAL);  /* if d= was specified, get value */
    if (mindist < 0 || mindist > 255) {
	fprintf(stderr, "d= must be between 0 and 255\nUsing d=10\n");
        mindist = 7;
    }

    while ( Vfread( &im, IVAL) ) {
        if ( im.type != VX_PBYTE ) {
              fprintf (stderr, "error: image not byte type\n");
              exit (1);
        }

        /* make a copy */
        Vfread( &im2, IVAL);

        /* clear the histogram */
        for (i = 0; i < 256; i++) hist[i] = 0;
 
        /* compute the histogram */
        for (y = im.ylo; y <= im.yhi; y++) {
            for (x = im.xlo; x <= im.xhi; x++) {
                 hist[im.u[y][x]]++;
                 npixels++;
            }
        }
  
         /* compute the threshold */
         normthresh = floor(0.001*npixels);    // 0.1% of total pixels
         flag = 1;
         rerun = 0;
         niter = 0;
         while (flag) {
            niter++;
            /* Find peaks above threshold */
            npeaks = 0;
            for (i = 0; i <= 255; i++) {
                if (hist[i] > normthresh) {
                    peaks[npeaks] = i;
                    npeaks++;
                }
            }

            if (npeaks < 3) {
                /* reduce thresh and continue */
                flag = 1;
                normthresh /= 2;    // reduce 50%
                continue;
            } else if (npeaks > 8) {
                /* increase thresh and continue */
                flag = 1;
                normthresh *= 1.1;
                continue;
            }

            for (j = 1; j<npeaks; j++) {
                dist = peaks[j] - peaks[j-1];
                if (dist<mindist) {
                    /* inc threshold and continue */
                    flag = 1;
                    normthresh *= 1.1;  // increase 10%
                    rerun = 1;
                    break;
                }
            }

            if (rerun) {
                rerun = 0;
                continue;
            }
            
            flag = 0;
         }

         if(VFLAG)
              fprintf(stderr, "niter = %d normth = %d npeaks = %d\n",
                                  niter, normthresh, npeaks);
         
         /* Pick leftmost(shadow) rightmost(object) peaks */
         shadowth = floor((peaks[0] + peaks[1])/2);
         objth = floor((peaks[npeaks-1] + peaks[npeaks-2])/2);
         
         /* Adding Non-linearity: dont want too low or too high thresh */
         if (shadowth < 15)
             shadowth += 10;
         if (objth > 240)
             objth -= 10;
  
         if(VFLAG)
              fprintf(stderr, "shadowth = %d objth = %d\n",
                                  shadowth, objth);
  
        /* apply the threshold(shadow) */
        for (y = im.ylo; y <= im.yhi; y++) {
            for (x = im.xlo; x <= im.xhi; x++) {
                 if (im.u[y][x] >= shadowth) im.u[y][x] = 0;
                 else                        im.u[y][x] = 255;
            }
        }
  
        /* apply the threshold(obj) */
        for (y = im2.ylo; y <= im2.yhi; y++) {
            for (x = im2.xlo; x <= im2.xhi; x++) {
                 if (im2.u[y][x] >= objth) im2.u[y][x] = 255;
                 else                         im2.u[y][x] = 0;
            }
        }
        Vfwrite( &im,  OVAL);
        Vfwrite( &im2, OVAL2);
    } /* end of every frame section */
    exit(0);
}
