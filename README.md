## KWallet Runner

This plugin allows you to copy/show/add/edit passwords from KWallet. 

This is a fork from https://github.com/jimtendo/kwallet-runner but has extra features added and deprecated code removed.  

### Required Dependencies

<details>
<summary><b>Debian/Ubuntu</b></summary>

Plasma5:  
```bash install-ubuntu-plasma5
sudo apt install git cmake extra-cmake-modules build-essential libkf5runner-dev libkf5i18n-dev libkf5kcmutils-dev libkf5notifications-dev libkf5wallet-dev libkf5dbusaddons-bin
```
Plasma6:  
```bash install-ubuntu-plasma6
sudo apt install git cmake extra-cmake-modules build-essential libkf6runner-dev libkf6i18n-dev libkf6kcmutils-dev libkf6notifications-dev libkf6wallet-dev libkf6dbusaddons-bin
```

</details>

<details>
<summary><b>OpenSUSE</b></summary>

Plasma5:  
```bash install-opensuse-plasma5
sudo zypper install git cmake extra-cmake-modules ki18n-devel krunner-devel kcmutils-devel knotifications-devel kwallet-devel kdbusaddons-tools libQt5Test-devel
```
Plasma6:  
```bash install-opensuse-plasma6
sudo zypper install git cmake kf6-extra-cmake-modules kf6-ki18n-devel kf6-krunner-devel kf6-kcmutils-devel kf6-knotifications-devel kf6-kwallet-devel kf6-kdbusaddons-tools qt6-test-devel
```

</details>

<details>
<summary><b>Fedora</b></summary>

Plasma5:  
```bash install-fedora-plasma5
sudo dnf install git cmake extra-cmake-modules kf5-ki18n-devel kf5-krunner-devel kf5-kcmutils-devel kf5-knotifications-devel kf5-kwallet-devel
```
Plasma6:  
```bash install-fedora-plasma6
sudo dnf install git cmake extra-cmake-modules kf6-ki18n-devel kf6-krunner-devel kf6-kcmutils-devel kf6-knotifications-devel kf6-kwallet-devel
```

</details>

The easiest way to install is:  
`curl https://raw.githubusercontent.com/alex1701c/krunner-kwallet/master/install.sh | bash`  
Or you may clone the repo manually and run the script.

### Screenshots  
You can search for entries and copy the password (default action), 
show the password in a dialog info button(Shift+Return) or edit it.  
To add a new password type kwallet-add  
![Overview](https://raw.githubusercontent.com/alex1701c/Screenshots/master/krunner-kwallet/overview.png)

If the password can not be copied to the clipboard or the user selects the info action the entry is shown in a dialog  
![Show entry](https://raw.githubusercontent.com/alex1701c/Screenshots/master/krunner-kwallet/show_map.png)

Edit normal password, other entry types have to be edited in the KWalletManager tool  
![Edit entry](https://raw.githubusercontent.com/alex1701c/Screenshots/master/krunner-kwallet/edit_normal_password.png)
