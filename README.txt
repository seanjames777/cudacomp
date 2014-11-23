AUTHORS:

    Sean James <seanjames777@gmail.com>
    Kurt Mueller <kurtmueller42@gmail.com>

BUILD:

    Build using CMake. You will need to install some dependencies first:

    - Install CMake
    - Insatll Flex and Bison. These can be installed via, for example, MacPorts.
    - Install LLVM, and tell the build system which LLVM to use. For example,
      if the 'llc' command is available on your path as "llc-mp-3.5", then you
      would configure CMake as follows.
    - The driver requires Python
    - The CUDA backend requires that CUDA is installed. To support a CUDA
      backend, define "PTX_BACKEND" and set the path to CUDA. Otherwise, omit
      these options.

    Then, create a build directory, configure CMake, and build:

      mkdir build
      cd build
      cmake -DCMAKE_INSTALL_PREFIX="../out/" -DLLVM_SUFFIX="-mp-3.5" \
          -DPTX_BACKEND="TRUE" -DCUDA_PATH="/Developer/NVIDIA/CUDA-6.5/" ../
      make && make install

TEST:

    Use the test driver to run tests. The build system will create a copy of
    the test driver script configured with the correct paths in your build
    directory, so you can run tests as follows. Note that you will need to
    use 'make install' above. 'make' alone is insufficient.

      ./driver.py

    To run tests in device mode:

      ./driver.py --device

    To run specific tests, provide the name of the files in tests/:

      ./driver.py testCall1.cc testCall2.cc

    To add more tests, edit the CONFIGURATION section of driver/driver.py,
    and rerun 'make && make install'.

DOCUMENTATION:

    To build documentation with doxygen, simply:

      make doc
