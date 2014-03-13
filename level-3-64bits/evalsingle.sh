#!/bin/bash


./cfglp -eval $1
mv $1.eval my.out
./cfglp64 -eval $1
mv $1.eval sir.out
diff -bB sir.out my.out 

