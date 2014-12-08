
                                   Compiler
                                    README

AUTHORS:

    Sean James <seanjames777@gmail.com>
    Kurt Mueller <kurtmueller42@gmail.com>

411 BACKGROUND:

    We rewrote our compiler from scratch in C++, for several reasons. First,
    we were no longer happy with the organization of our OCaml compiler. We
    wanted to use C++'s object orientation, template system, etc. to improve
    the organization of the compiler, as OCaml's module and functor system
    was getting in the way.

    We also wanted to gain the performance advantage of native C++ code over
    OCaml code. Most importantly, we wanted to use the LLVM IR libraries
    directly. This made working with LLVM much easier, because its built-
    in assertions could catch errors, and we didn't need to worry about
    formatting the IR output. Furthermore, we could use LLVM's optimization
    infrastructure, etc. We did not rewrite our x86_64 backend in C++, because
    we were focused on the LLVM portion. Frank Pfenning approved this on
    Piazza, so our compiler simply invokes llc to compile our LLVM output for
    x86_64.

    Note that this compiler has also been used as a starting point for a
    project for another class (mostly retargeting the compiler to CUDA). The
    changes have been merged back into the main compiler to ease the
    maintenence effort, so there are some portions of the compiler and
    infrastructure which are not related to 411.

    Note that the code makes use of a few patterns. The first is the use of
    shared pointers in most cases, to avoid memory management problems. Second,
    std::dynamic_pointer_cast<>() is used as a substitute for basic pattern
    matching. This looks somewhat strange, but is fairly idiomatic in other
    projects like LLVM itself, although it also uses its own RTTI system.

    Third, we have chosen not to implement the bulk of the compiler the way
    "traditional" OOP would suggest. Specifically, we do not, for example,
    place type checking, code generation, printing, etc. code for every type of
    AST node in the AST classes themselves. This is because we wanted to keep
    concerns such as type checking, code generation, etc. organized by the
    logical progression of the compiler, rather than mixing those concerns in
    the AST classes. This means that we need to do some heavy pattern matching,
    but also means that, for example, all of the type checking code is in
    the same file.

    We also make fairly heavy use of assertions and exceptions, rather than
    propgating error codes. Most of these are caught at the top level and
    printed as errors to stdout. This simplifies the code, but adds the
    overhead of exception handling code to the executable.

ORGANIZATION:

    The source tree, as far as 411 is concerned, is organized as follows:

    include/       Header files, corresponding to the files in src/
    src/           Source C++ files
        ast/       Abstract syntax tree class heirarchy
        codegen/   Code generator, translates AST to LLVM IR
        parser/    Flex/Bison parser and lexer files
        statics/   Type checker and other statics rules
    driver/
        driver.py  Our own test driver, not up to date
    runtime/
        host_rt.c  Our runtime, which is copied to l4lib.c
    tests/         Our own tests which test extended compiler capabilities
    411_wrapper.py Python wrapper around the compiler, which translates
                   411 command line arguments to our own arguments.
    Makefile       411 makefile which does not require CMake

411 BUILD:

    The 411 compiler can be built directly with the provided makefile, through:

        make l4c

    The general build process is described below. The CUDA backend is not
    currently up to date.

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
