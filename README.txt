
                                 CUDA Compiler
                                    README

AUTHORS:

    Sean James <seanjames777@gmail.com>
    Kurt Mueller <kurtmueller42@gmail.com>

    The source tree, as far as 411 is concerned, is organized as follows:

    include/         Header files, corresponding to the files in src/
    src/             Source C++ files
        ast/         Abstract syntax tree class heirarchy
            decl/    Top-level function, type, and struct declarations
            expr/    Expressions
            stmt/    Statements
            type/    Types
        codegen/     Code generator, translates AST to LLVM IR
        parser/      Flex/Bison parser and lexer files
        statics/     Type checker and other statics rules
    driver/
        driver.py    Our own test driver, not up to date
    runtime/
        host_rt.c    Host runtime
        device_rt.c  Device runtime
    tests/           Our own tests which test extended compiler capabilities
                     411 command line arguments to our own arguments.
    Makefile         411 makefile which does not require CMake

BUILD:

    Build using CMake. You will need to install some dependencies first:

    - Install CMake
    - Insatll Flex and Bison. These can be installed via, for example, MacPorts.
    - Install LLVM, and specify the LLVM_PATH variable
    - The driver requires Python
    - The CUDA backend requires that CUDA is installed. To support a CUDA
      backend, define "PTX_BACKEND" and set the CUDA_PATH variable. Otherwise,
      omit these options.

    Then, create a build directory, configure CMake, and build:

      mkdir build
      cd build
      cmake -DCMAKE_INSTALL_PREFIX="../out/" \
          -DLLVM_PATH="/opt/local/libexec/llvm-3.5/bin/" \
          -DPTX_BACKEND="TRUE" -DCUDA_PATH="/Developer/NVIDIA/CUDA-6.5/" ../
      make && make install

411 TESTING:

    The ./driver.pl infrastructure works as expected. A secondary test system
    is described below, although it is not currently up to date.

TESTING:

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
