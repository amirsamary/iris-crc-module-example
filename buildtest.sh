
rm -f ./test/libCRC.so
docker run --rm -v "$PWD"/project/CRC:/usr/src/myapp -w /usbr/src/myapp/Release intersystemsdc/irisdemo-base-gcc make clean
docker run --rm -v "$PWD"/project/CRC:/usr/src/myapp -w /usr/src/myapp/Release intersystemsdc/irisdemo-base-gcc make libCRC.so
cp "$PWD"/project/CRC/Release/libCRC.so ./test/
docker build -t amirsamary/iris-crc-module-example:test ./test/