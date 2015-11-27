#!/bin/sh
pname=projEval

eval `vshparse if= -v - with $0 $*`
#evaluate a "-" argument
if [ "x"$OPT = "x-" ] ; then
echo "$pname:  evaluate a segmented image with annotation" 1>&2
echo "[if=]  image to be evaluated without extension, must have .vxa annotation with same name " 1>&2
exit
fi

vrdiff if=$if.1.vxa bf=$if_seg.vx -ci of=$if_eval.vx ig=$if.vx os=$if.txt
exit

