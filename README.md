## KWallet Runner

This plugin allows you to copy/show/add/edit passwords from KWallet. 

This is a fork from https://github.com/jimtendo/kwallet-runner but has extra features added and deprecated code removed.  

Required Dependencies
----------------------

Debian/Ubuntu:  
`sudo apt install cmake extra-cmake-modules build-essential libkf5runner-dev libkf5textwidgets-dev qtdeclarative5-dev gettext libkf5wallet-dev`

openSUSE  
`sudo zypper install cmake extra-cmake-modules libQt5Widgets5 libQt5Core5 libqt5-qtlocation-devel ki18n-devel ktextwidgets-devel
 kservice-devel krunner-devel gettext-tools kconfigwidgets-devel kwallet-devel`  

Fedora  
`sudo dnf install cmake extra-cmake-modules kf5-ki18n-devel kf5-kservice-devel kf5-krunner-devel kf5-ktextwidgets-devel gettext kf5-kwallet-devel`  

Arch (Manjaro):  
`sudo pacman -S cmake extra-cmake-modules kwallet`

The easiest way to install is:  
`curl https://raw.githubusercontent.com/alex1701c/JetBrainsRunner/master/install.sh | bash`  

Or you can do it manually (with admin privileges):
```
git clone https://github.com/alex1701c/kwallet-runner
cd kwallet-runner/
mkdir build  
cd build
cmake -DQT_PLUGIN_INSTALL_DIR=`kf5-config --qt-plugins` ..
make
sudo make install
```

Restart krunner to load the runner (in a terminal type: kquitapp5 krunner;kstart5 krunner )