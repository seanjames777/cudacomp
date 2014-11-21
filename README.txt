AUTHOR:

    Sean James <seanjames777@gmail.com>

BUILD:

    Build using CMake:

    mkdir build
    cd build
    cmake -DCMAKE_INSTALL_PREFIX=../out/ ../
    make
    make install

TEST:

    Use the test driver to run tests:

    ../driver/driver.py

    To run tests in device mode:

    ../driver/driver.py --device
