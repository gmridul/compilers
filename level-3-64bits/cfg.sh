#!/bin/bash
cd test_files
for f in *.c
do
cd ..
make FILE=$f -f Makefile.cfg
cd test_files
done
