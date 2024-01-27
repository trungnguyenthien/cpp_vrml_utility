PORT_VERSION='2.9.0'
cd /usr
sudo curl -O https://distfiles.macports.org/MacPorts/MacPorts-$PORT_VERSION.tar.bz2
sudo tar xjvf MacPorts-$PORT_VERSION.tar.bz2
cd MacPorts-$PORT_VERSION
./configure && make && sudo make install
export PATH=/opt/local/bin:/opt/local/sbin:$PATH
sudo port -v selfupdate
sudo port install cairo