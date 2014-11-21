AUTHOR:

    Sean James <seanjames777@gmail.com>

BUILD:

    Build using CMake. Note that the test and driver infrastructure currently
    assume you are using exactly this setup. You will need Flex and Bison. These
    can be installed via, for example, MacPorts.

    mkdir build
    cd build
    cmake -DCMAKE_INSTALL_PREFIX=../out/ ../
    make
    make install

TEST:

    Use the test driver to run tests.

    ./driver.py

    To run tests in device mode:

    ./driver.py --device
