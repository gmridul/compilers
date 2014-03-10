#!/bin/bash


./cfglp -tokens $1
mv $1.toks my.out
./cfglp64 -tokens $1
mv $1.toks sir.out
diff -bB my.out sir.out

