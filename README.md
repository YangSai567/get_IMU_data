# get_IMU_data
GUI ON getting_IMU_data  under QT
not for development, but save for myself


安装 opengl:
sudo apt install build-essential libgl1-mesa-dev freeglut3-dev
libglew-dev libsdl2-dev libsdl2-image-dev libglm-dev libfreetype6-dev
安装 freetype2:
cd ~
wget
http://download.savannah.gnu.org/releases/freetype/freetype-2.4.10.ta
r.gz
解压:
tar zxvf ./freetype-2.4.10.tar.gz
安装:
cd ./freetype-2.4.10/
./configure --prefix=/usr/local/freetype
make
sudo make install
安装 LpSensor
cd ~
https://bitbucket.org/lpresearch/openmat/downloads/LpSensor-1.3.5-Lin
ux-x86-64.tar.gz
tar -zxvf ~/LpSensor-1.3.5-Linux-x86-64.tar.gz
cd ./LpSensor-1.3.5-Linux-x86-64/
sudo dpkg -i ./liblpsensor-1.3.5-Linux.deb
