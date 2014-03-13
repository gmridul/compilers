#!/bin/bash


./cfglp -ast $1
mv $1.ast my.out
./cfglp64 -ast $1
mv $1.ast sir.out
diff -bB sir.out my.out 

