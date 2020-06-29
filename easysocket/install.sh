#!/bin/sh

rm -rf /usr/local/include/easysocket
mkdir /usr/local/include/easysocket
cp -f ./bin/release/libeasysocket.so /usr/lib/x86_64-linux-gnu/libeasysocket.so
cp -fR ./include/* /usr/local/include/easysocket/
