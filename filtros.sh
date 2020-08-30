cat fort.7 | awk '{ if(($1==5 && $1==6) || ($5<20000 && $5>-20000 && $6<20000 && $6>-20000))  {print ;} }' > cerout.dat

g++ file.cc -o data -lm

./data
