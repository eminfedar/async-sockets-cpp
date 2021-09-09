#!/bin/bash
if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root" 
   exit 1
fi

rm -rf /usr/local/include/async-sockets
mkdir /usr/local/include/async-sockets
cp -fR ./include/* /usr/local/include/async-sockets/

echo "Info: The library has been copied to -> /usr/local/include";
echo ""
echo "Example usage:"
echo "#include <async-sockets/tcp-client.hpp>"
