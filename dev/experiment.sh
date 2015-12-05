#!/bin/sh
# script to evaluate vtpeak with different patch parameters
## Syntax:  patcheval <image-file>
## Returns: the processed images in <image-file>.sq
oname=$1.sq
tmp=tmp.$$.sq
if [ "$#" != "1" ]
then
echo "Error: a single file name argument is required"
exit
fi

rm -f $oname
#looping over different region growing threshold values
for i in 15 20 25 30
do
  #looping over max number of peaks
  for j in 7 8 9 10
  do
      #looping over distance between peaks
      for k in 5 8 11 14
      do
        #param="Patch size: $i  Overlap: $j"
        param="Range: $i  MaxPeak: $j MinDist: $k"
        echo $param
        ./vgrow if=$1 r=$i -p of=$tmp_vgrow
        
        ./vtobjshadow if=$tmp_vgrow d=$k p=$j -v -s of=$tmp_vgrow_vtboth
        
        vmedian xy=5 $tmp_vgrow_vtboth of=$tmp

        vrdiff if=$1.1.vxa bf=$tmp -ci of=$1_eval ig=$1.vx os=$1.txt

        vcapt $tmp c="$param" | vdim -c |  vxfile  of=$oname
        rm -f $tmp
      done
  done
done


