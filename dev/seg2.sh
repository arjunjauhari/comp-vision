#!/bin/sh
#
# try some built in programs to segement the image

# edge detection + cleanup
vsobel $1 -d | vedgex of=$1_sobeledge_cleanup

