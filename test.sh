#!/bin/bash

git submodule init
git submodule update

tar xf src/test/data/rockyou/rockyou.txt.tar.gz -C src/test/data
cd build
./hashdb_test
cd ..