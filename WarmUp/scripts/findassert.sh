#!/bin/bash

for f in $(find -follow); do 
    if [ -f $f ]; then 
        echo "File: $f"
        cat $f | grep "assert";
    fi
done
