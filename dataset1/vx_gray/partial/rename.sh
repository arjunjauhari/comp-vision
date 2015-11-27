a=18
for i in *.vx; do
  new=$(printf "%02d.vx" "$a") #04 pad to length of 4
  mv -- "$i" "$new"
  let a=a+1
done
