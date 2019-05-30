#!/bin/bash
set -e
cd /tmp
git clone https://bitbucket.org/sol_prog/raspberry-pi-gcc-binary
cd raspberry-pi-gcc-binary
tar xf gcc-9.1.0-armhf-raspbian.tar.bz2
mv gcc-9.1.0 /opt
echo 'PATH="/opt/gcc-9.1.0/bin:$PATH"' >> ~/.zshrc
echo 'LD_LIBRARY_PATH="/opt/gcc-9.1.0/lib:$LD_LIBRARY_PATH"' >> ~/.zshrc
#source ~/.zshrc
update-alternatives --install /usr/bin/gcc gcc /opt/gcc-9.1.0/bin/gcc-9.1 90 --slave /usr/bin/g++ g++ /opt/gcc-9.1.0/bin/g++-9.1
