#!/bin/bash
cd cfg
for f in *
do
echo $f
../cfglp -eval $f
mv $f.eval my.out
../cfglp64 -eval $f
mv $f.eval sir.out
diff -bB sir.out my.out 
    rm *out
    echo ================================================================================== 
done
