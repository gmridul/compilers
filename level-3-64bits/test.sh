#!/bin/bash
cd cfg
for f in *
do
echo $f
../cfglp -ast $f
mv $f.ast my.out
../cfglp64 -ast $f
mv $f.ast sir.out
diff -bB sir.out my.out 
    rm *out
    echo ================================================================================== 
done
