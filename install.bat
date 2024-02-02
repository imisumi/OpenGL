@echo off
cd dep\openexr
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=install-dir -DBUILD_TESTING=OFF -DBUILD_SHARED_LIBS=OFF -DOPENEXR_BUILD_TOOLS=OFF -DOPENEXR_INSTALL_TOOLS=OFF -DOPENEXR_INSTALL_EXAMPLES=OFF
cmake --build . --target install --config Release
