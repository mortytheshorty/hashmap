Build:
mkdir build && cd build && cmake .. && make

Test:
I wrote a little program to test this against the rockyou.txt file (included here)

Run with:
git submodule init
git submodlue update
tar xf src/test/data/rockyou/rockyou.txt.tar.gz -C src/test/data
cd build && ./hashdb_test

