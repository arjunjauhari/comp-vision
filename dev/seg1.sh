#!/bin/sh
#
# try some built in programs to segement the image
# Input
# 1 filename
# 2 range for vgrow

# try edge detection -> region growing -> thresholding
vrcross $1 | ./vgrow r=$2 -p | ./vits -v of=$1_vrcedge_vgrow_vits

