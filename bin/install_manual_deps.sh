BOOST_VERSION=1.63.0
BOOST_DIR=boost_1_63_0
BOOST_ARCHIVE=$BOOST_DIR.tar.gz

mkdir -p tmp
cd tmp

if [ ! -d $BOOST_DIR ]; then

  # Download boost
  if [ ! -f $BOOST_ARCHIVE ]; then
    wget https://sourceforge.net/projects/boost/files/boost/$BOOST_VERSION/$BOOST_ARCHIVE/download
    mv download $BOOST_ARCHIVE
  fi

  # Create boost install folder
  tar -xf $BOOST_ARCHIVE
fi

if [ ! -d "/usr/local/include/boost" ]; then
  # Compile boost
  cd $BOOST_DIR

  if [ ! -f "b2" ]; then
    sh bootstrap.sh
  fi

  ./b2 threading=multi
  sudo ./b2 install

  echo "Boost installed."
fi



if [ ! -d "./glip-lib" ]; then
  git clone https://github.com/ronan-kerviche/glip-lib.git
fi

cd ./glip-lib/GLIP-Lib

if [ ! -f "Makefile" ]; then
cmake .
fi

if [ ! -f "/usr/local/lib/libglip.so" ] || [ ! -d "/usr/local/include/glip-lib" ]; then
  make -j3
  sudo make install

  echo "GLIP-Lib installed."
fi

echo "Done."
