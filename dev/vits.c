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
    int i;
    int k;
    int l;
    int avg;
    int avg1;
    int avg2;
    int old1;
    int old2;

    int ithresh;                   /* minimum distance between maxima   */
    int nthresh;                   /* minimum distance between maxima   */
    int flag;                   /* minimum distance between maxima   */
			     
    VXparse(&argc, &argv, par);    /* parse the command line         */

    ithresh = 0;                    /* default dist */
    flag = FALSE;
    if (ITVAL) ithresh = atoi(ITVAL);  /* if d= was specified, get value */

    while ( Vfread( &im, IVAL) ) {
        if ( im.type != VX_PBYTE ) {
              fprintf (stderr, "error: image not byte type\n");
              exit (1);
        }

        /* compute the average */
	avg = 0;
	i = 0;
        for (y = im.ylo; y <= im.yhi; y++) {
            for (x = im.xlo; x <= im.xhi; x++){
                 avg = avg + im.u[y][x];
		 i++;
	    }
	}
	if (i==0)
	    avg = 0;
	else
	    avg = (avg/i);

	if (ithresh == 0){
		ithresh = avg;
		flag = TRUE;
	}
	
        //fprintf(stderr, "ithresh=%d avg=%d i=%d\n",ithresh,avg,i);

	/* main loop of the algorithm*/
	l = 0;
	old1 = 0;
	old2 = 0;
	nthresh = ithresh;
	while(l<100) {
		avg1 = 0;
		avg2 = 0;
		i=0;
		k=0;
		/* average for R1 and R2 separated by nthresh */
        	for (y = im.ylo; y <= im.yhi; y++) {
        	    for (x = im.xlo; x <= im.xhi; x++){
		    	 if (im.u[y][x] <= nthresh) {
			 	// Region 1
        	         	avg1 = avg1 + im.u[y][x];
			 	i++;
			 } else {
			 	// Region 2
        	         	avg2 = avg2 + im.u[y][x];
			 	k++;
			 }
		    }
		}

		if(i==0)
			avg1 = 0;
		else
			avg1 = avg1/i;
		if(k==0)
			avg2 = 0;
		else
			avg2 = avg2/k;

        	//fprintf(stderr, "nthresh = %d avg1 = %d avg2 = %d\n",nthresh,avg1,avg2);
		if ((avg1 != old1) || (avg2 != old2)) {
			old1 = avg1;
			old2 = avg2;
		} else {
			break;
		}
		nthresh = (avg1 + avg2)/2;
		l++;
	}
  
        if(VFLAG)
             fprintf(stderr, "thresh = %d \n",
                                  nthresh);
  
        /* apply the threshold */
        for (y = im.ylo; y <= im.yhi; y++) {
            for (x = im.xlo; x <= im.xhi; x++) {
                 if (im.u[y][x] >= nthresh) im.u[y][x] = 255;
                 else                       im.u[y][x] = 0;
            }
        }
  
        Vfwrite( &im, OVAL);
	if(flag)
	    ithresh = 0;
    } /* end of every frame section */
    exit(0);
}
