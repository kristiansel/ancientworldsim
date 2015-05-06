#!/bin/sh

echo "building from directory '${GEDIT_CURRENT_DOCUMENT_DIR}'"

cd build
cmake ..
make && make install && cd dist/bin && ./OgreApp && echo "" && echo "program returned $?" && cd ../../..
