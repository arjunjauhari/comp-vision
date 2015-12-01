#!/bin/sh
#
# try some built in programs to segement the image
# Input
# 1 filename

# obj-shadow pair extraction

./vtobjshadow if=$1 -v of=$1_vtsha of2=$1_vtobj

vdview pm=0 $1_vgrow_vtsha &
vdview pm=0 $1_vgrow_vtobj &
#./vgrow if=$1 r=$2 -p | ./vtobjshadow -v of=$1_vgrow_vtsha of2=$1_vgrow_vtobj
