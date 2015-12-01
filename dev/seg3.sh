#!/bin/sh
#
# try some built in programs to segement the image
# Input
# 1 filename
# 2 range for vgrow
# 3 d for vtpeak

# region growing -> thresholding
./vgrow if=$1 r=$2 -p of=$1_vgrow

./vgrow if=$1 r=$2 -p | ./vtpeak d=$3 -v of=$1_vgrow_vtpeak

./vgrow if=$1 r=$2 -p | ./vits -v of=$1_vgrow_vits
