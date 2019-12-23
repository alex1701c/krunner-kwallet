#!/bin/bash

# Exit if something fails
set -e

if [[ $(basename "$PWD") != "krunner-kwallet"* ]];then
    git clone https://github.com/alex1701c/krunner-kwallet
    cd krunner-kwallet/
fi

mkdir -p build
cd build

cmake -DQT_PLUGIN_INSTALL_DIR=`kf5-config --qt-plugins` -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)
sudo make install

quitapp5 krunner 2> /dev/null
kstart5 --windowclass krunner krunner > /dev/null 2>&1 &

echo "Installation finished !";