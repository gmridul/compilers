#!/bin/bash
cd cfg
for f in *
do
echo "ksdj"
    echo $f
../cfglp -tokens $f
mv $f.toks my.out
../cfglp64 -tokens $f
mv $f.toks sir.out
diff -bB my.out sir.out
    rm *out
done
