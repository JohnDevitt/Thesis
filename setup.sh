
#!/usr/bin/env bash

rm -rf ./beebs
git clone https://github.com/craigblackmore/beebs.git
cd beebs
autoreconf
./configure --disable-werror
automake
make
