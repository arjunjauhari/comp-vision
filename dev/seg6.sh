#!/bin/sh
#
# try some built in programs to segement the image
# Input
# 1 filename
# 2 range for vgrow

# region growing -> obj-shadow pair extraction
./vgrow if=$1 r=$2 -p of=$1_vgrow

./vtobjshadow if=$1_vgrow d=7 p=8 -v -s of=$1_vgrow_vtboth

vmedian xy=15 $1_vgrow_vtboth of=$1_vmed

vdview pm=0 $1_vgrow &
vdview pm=0 $1_vgrow_vtboth &
vdview pm=0 $1_vmed &
