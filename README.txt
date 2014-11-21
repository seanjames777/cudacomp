AUTHOR:

    Sean James <seanjames777@gmail.com>

BUILD:

    Build using CMake. You will need Flex and Bison. These can be installed via,
    for example, MacPorts.

    mkdir build
    cd build
    cmake -DCMAKE_INSTALL_PREFIX=../out/ ../
    make && make install

TEST:

    Use the test driver to run tests. The build system will create a copy of
    the test driver script configured with the correct paths in your build
    directory, so you can run tests as follows. Note that you will need to
    use 'make install' above. 'make' alone is insufficient.

    ./driver.py

    To run tests in device mode:

    ./driver.py --device

DOCUMENTATION:

    To build documentation with doxygen, simply:

    make doc
