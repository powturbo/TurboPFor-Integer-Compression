HDF5_VERSION=$1

# Download and install HDF5 $HDF5_VERSION from source for building wheels
curl https://support.hdfgroup.org/ftp/HDF5/releases/hdf5-${HDF5_VERSION%.*}/hdf5-$HDF5_VERSION/src/hdf5-$HDF5_VERSION.tar.gz -O -s
tar -xzf hdf5-$HDF5_VERSION.tar.gz
cd hdf5-$HDF5_VERSION
./configure --prefix=/usr/local
make -j 2
make install
cd ..
