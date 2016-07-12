#!/bin/bash

sed 's/[^ ][^ ]*/"&"/g' flags.txt > newflags.txt
tr --delete '\n' < newflags.txt > newnewflags.txt
tr -s '[:space:]' < newnewflags.txt > newnewnewflags.txt
tr ' ' ',' < newnewnewflags.txt > newnewnewnewflags.txt