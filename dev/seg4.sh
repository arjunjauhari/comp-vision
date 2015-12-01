#!/bin/sh
#
# try some built in programs to segement the image
# Input
# 1 filename
# 2 range for vgrow

# region growing -> obj-shadow pair extraction
./vgrow if=$1 r=$2 -p of=$1_vgrow

./vtobjshadow if=$1_vgrow d=7 -v of=$1_vgrow_vtsha of2=$1_vgrow_vtobj

vdview pm=0 $1_vgrow &
vdview pm=0 $1_vgrow_vtsha &
vdview pm=0 $1_vgrow_vtobj &
#./vgrow if=$1 r=$2 -p | ./vtobjshadow -v of=$1_vgrow_vtsha of2=$1_vgrow_vtobj
