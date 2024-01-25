brew install libx11
brew install git cmake curl

brew install qt
brew install boost # --> Require for coin
# Coin3D/Coin
git clone --recurse-submodules https://github.com/coin3d/coin coin
cmake -Hcoin -Bcoin_build -G "Unix Makefiles" -DCMAKE_INSTALL_PREFIX=/usr/local -DCMAKE_BUILD_TYPE=Release -DCOIN_BUILD_DOCUMENTATION=OFF
sudo cmake --build coin_build --target all --config Release -- -j4
sudo cmake --build coin_build --target install --config Release -- -j4
# SoQT
git clone --recurse-submodules https://github.com/coin3d/soqt soqt
cmake -Hsoqt -Bsoqt_build -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local -DCMAKE_PREFIX_PATH="/usr/local" -DSOQT_BUILD_DOCUMENTATION=OFF
sudo cmake --build soqt_build --target all --config Release -- -j4
sudo cmake --build soqt_build --target install --config Release -- -j4