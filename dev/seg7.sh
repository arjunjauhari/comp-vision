#!/bin/sh
#
# try some built in programs to segement the image
# Input
# 1 filename
# 2 range for vgrow

# region growing -> obj-shadow pair extraction
./vgrow if=$1.vx r=$2 -p of=$1.vx_vgrow

./vtobjshadow if=$1.vx_vgrow d=7 p=8 -v -s of=$1.vx_vgrow_vtboth

vmedian xy=5 $1.vx_vgrow_vtboth of=$1.vx_vmed

vrdiff if=$1.1.vxa bf=$1.vx_vmed -ci of=$1_eval ig=$1.vx os=$1.txt
