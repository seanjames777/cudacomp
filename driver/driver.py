#!/usr/bin/python

import sys
import subprocess
import time
import md5
import os.path

####### CONFIGURATION #######

target = "host"
run_list = []
dump_temp = False
verbose = False

SOURCE_DIR = "@CMAKE_CURRENT_SOURCE_DIR@/"
BINARY_DIR = "@CMAKE_CURRENT_BINARY_DIR@/"
INSTALL_DIR = "@CMAKE_INSTALL_PREFIX@/"
LLC = "@LLVM_LLC@"

tests = [
    (SOURCE_DIR + "tests/testBinOps1.cc", 14),
    (SOURCE_DIR + "tests/testBinOps2.cc", 2),
    (SOURCE_DIR + "tests/testCmp1.cc", 1),
    (SOURCE_DIR + "tests/testUndeclared.cc", "error"),
    (SOURCE_DIR + "tests/testUndefined.cc", "error"),
    (SOURCE_DIR + "tests/testRedeclared.cc", "error"),
    (SOURCE_DIR + "tests/testId1.cc", 10),
    (SOURCE_DIR + "tests/testId2.cc", 20),
    (SOURCE_DIR + "tests/testId3.cc", "error"),
    (SOURCE_DIR + "tests/testId4.cc", 20),
    (SOURCE_DIR + "tests/testId5.cc", "error"),
    (SOURCE_DIR + "tests/testId6.cc", 100),
    (SOURCE_DIR + "tests/testReturnDeclDef.cc", 5),
    (SOURCE_DIR + "tests/testUnOps1.cc", -1),
    (SOURCE_DIR + "tests/testUnOps2.cc", -1),
    (SOURCE_DIR + "tests/testUnOps3.cc", 2),
    (SOURCE_DIR + "tests/testScope1.cc", 10),
    (SOURCE_DIR + "tests/testScope2.cc", "error"),
    (SOURCE_DIR + "tests/testScope3.cc", 10),
    (SOURCE_DIR + "tests/testScope4.cc", 10),
    (SOURCE_DIR + "tests/testScope5.cc", 10),
    (SOURCE_DIR + "tests/testIf1.cc", 10),
    (SOURCE_DIR + "tests/testIf2.cc", 20),
    (SOURCE_DIR + "tests/testIf3.cc", 10),
    (SOURCE_DIR + "tests/testIf4.cc", "error"),
    (SOURCE_DIR + "tests/testIf5.cc", 10),
    (SOURCE_DIR + "tests/testIf6.cc", 10),
    (SOURCE_DIR + "tests/testReturnCheck1.cc", "error"),
    (SOURCE_DIR + "tests/testReturnCheck2.cc", 10),
    (SOURCE_DIR + "tests/testReturnCheck3.cc", 10),
    (SOURCE_DIR + "tests/testReturnCheck4.cc", 10),
    (SOURCE_DIR + "tests/testReturnCheck5.cc", "error"),
    (SOURCE_DIR + "tests/testReturnCheck6.cc", 10),
    (SOURCE_DIR + "tests/testCall1.cc", 5),
    (SOURCE_DIR + "tests/testCall2.cc", "error"),
    (SOURCE_DIR + "tests/testCall3.cc", "error"),
    (SOURCE_DIR + "tests/testCall4.cc", "error"),
    (SOURCE_DIR + "tests/testCall5.cc", 5),
    (SOURCE_DIR + "tests/testCall6.cc", "error"),
    (SOURCE_DIR + "tests/testCall7.cc", "error"),
    (SOURCE_DIR + "tests/testCall8.cc", 20),
    (SOURCE_DIR + "tests/testCall9.cc", 5),
    (SOURCE_DIR + "tests/testCall10.cc", "error"),
    (SOURCE_DIR + "tests/testCall11.cc", "error"),
    (SOURCE_DIR + "tests/testCall12.cc", 5),
    (SOURCE_DIR + "tests/testCall13.cc", 11),
    (SOURCE_DIR + "tests/testCall14.cc", "error"),
    (SOURCE_DIR + "tests/testCall15.cc", "error"),
    (SOURCE_DIR + "tests/testCall16.cc", 0),
    (SOURCE_DIR + "tests/testFib1.cc", 34),
    (SOURCE_DIR + "tests/testFib2.cc", 34),
    (SOURCE_DIR + "tests/testFib3.cc", 34),
    (SOURCE_DIR + "tests/testFib4.cc", 34),
    (SOURCE_DIR + "tests/testWhile1.cc", 1),
    (SOURCE_DIR + "tests/testWhile2.cc", 55),
    (SOURCE_DIR + "tests/testWhile3.cc", 1000),
    (SOURCE_DIR + "tests/testWhile4.cc", "error"),
    (SOURCE_DIR + "tests/testWhile5.cc", "error"),
    (SOURCE_DIR + "tests/testWhile6.cc", "error"),
    (SOURCE_DIR + "tests/testTypeDef1.cc", 5),
    (SOURCE_DIR + "tests/testTypeDef2.cc", "error"),
    (SOURCE_DIR + "tests/testTypeDef3.cc", 5),
    (SOURCE_DIR + "tests/testTypeDef4.cc", "error"),
    (SOURCE_DIR + "tests/testTypeDef5.cc", "error"),
    (SOURCE_DIR + "tests/testTypeDef6.cc", 5),
    (SOURCE_DIR + "tests/testFunDecl1.cc", 5),
    (SOURCE_DIR + "tests/testFunDecl2.cc", 5),
    (SOURCE_DIR + "tests/testFunDecl3.cc", "error"),
    (SOURCE_DIR + "tests/testFunDecl4.cc", "error"),
    (SOURCE_DIR + "tests/testFunDecl5.cc", "error"),
    (SOURCE_DIR + "tests/testFunDecl6.cc", "error"),
    (SOURCE_DIR + "tests/testFunDecl7.cc", "error"),
    (SOURCE_DIR + "tests/testFunDecl8.cc", "error"),
    (SOURCE_DIR + "tests/testFunDecl9.cc", 5),
    (SOURCE_DIR + "tests/testFunDecl10.cc", "error"),
    (SOURCE_DIR + "tests/testFunDecl11.cc", 5),
    (SOURCE_DIR + "tests/testFunDecl12.cc", "error"),
    (SOURCE_DIR + "tests/testFunDecl13.cc", 518),
    (SOURCE_DIR + "tests/testArr1.cc", 5),
    (SOURCE_DIR + "tests/testArr2.cc", 0),
    (SOURCE_DIR + "tests/testArr3.cc", 5),
    (SOURCE_DIR + "tests/testArr4.cc", 19),
    (SOURCE_DIR + "tests/testArr5.cc", 10),
    (SOURCE_DIR + "tests/testArr6.cc", 5),
    (SOURCE_DIR + "tests/testArr7.cc", "error"),
    (SOURCE_DIR + "tests/testArr8.cc", "error"),
    (SOURCE_DIR + "tests/testArr9.cc", "error"),
    (SOURCE_DIR + "tests/testLValue1.cc", "error"),
    (SOURCE_DIR + "tests/testRange1.cc", 33),
    (SOURCE_DIR + "tests/testRange2.cc", 10),
    (SOURCE_DIR + "tests/testRange3.cc", 4950),
    (SOURCE_DIR + "tests/testFloat1.cc", 10),
]

# TODO: actually test type checking...

#############################

def parse_args():
    global target, input, dump_temp, verbose

    for arg in sys.argv:
        if arg == "--host":
            target = "host"
        elif arg == "--device":
            target = "device"
        elif arg == "--tmp":
            dump_temp = True
        elif arg == "--verbose":
            verbose = True
        elif arg != sys.argv[0]:
            run_list.append(os.path.basename(arg))

def run_shell(command):
    proc = subprocess.Popen (command, stdout=subprocess.PIPE)
    stat = proc.wait()

    output = []

    while True:
        line = proc.stdout.readline()
        if not line:
            break
        output.append(line.strip())
        #sys.stdout.write(line)
        #sys.stdout.flush()

    return (stat, output)

########### ENTRY ###########

parse_args()

(buildstat, _) = run_shell([ "make" ])

if buildstat != 0:
    exit(-1)

run_shell([ "make", "install" ])

print "\033[34;1m***************************************************\033[0m"
print "\033[34;1m* Running Tests...                                *\033[0m"
print "\033[34;1m***************************************************\033[0m"

run = 0
passed = 0

run_shell([ "mkdir", "-p", "tmp" ])

def print_verbose(s):
    if verbose:
        print "\033[37;1m    " + s + "\033[0m"

for (name, expected) in tests:
    if len(run_list) > 0 and not(os.path.basename(name) in run_list):
        continue

    temp = "tmp/" + md5.new(name).hexdigest()

    print "\033[37;1mTest '" + name + "'\033[0m"

    if dump_temp:
        print "    " + temp

    stat = -1
    output = []

    if target == "host":
        print_verbose("Compiling...")
        (cc_stat, cc_out) = run_shell([ INSTALL_DIR + "bin/cc", "-o", temp + "_host.bc", name ])
        if cc_stat == 0:
            print_verbose("Assembling...")
            run_shell([ LLC, "-o", temp + "_host.o", "-filetype=obj", temp + "_host.bc" ])
            print_verbose("Linking...")
            run_shell([ "clang", "-o", temp + "_host", temp + "_host.o", INSTALL_DIR + "lib/libhost_rt.a" ])
            print_verbose("Executing...")
            (stat, output) = run_shell([ temp + "_host" ])
        else:
            (stat, output) = (cc_stat, cc_out)
    else:
        (cc_stat, cc_out) = run_shell([ INSTALL_DIR + "bin/cc", "-o", temp + "_device.bc", "--emit-device", name ])
        if cc_stat == 0:
            run_shell([ LLC, "-o", temp + "_device.ptx", temp + "_device.bc" ])
            run_shell([ "nvcc", "-arch=sm_30", "-cubin", "-dc", "-o", temp + "_device.cubin", temp + "_device.ptx" ])
            run_shell([ "nvcc", "-arch=sm_30", "-cubin", "-dlink", "-o", temp + "_link.cubin", temp + "_device.cubin", BINARY_DIR + "device_rt_kernel.cubin" ])
            run_shell([ "clang", "-o", temp + "_device", INSTALL_DIR + "lib/libdevice_rt.a", "-framework", "CUDA",
                "-sectcreate", "__TEXT", "__kernels", temp + "_link.cubin",
                "-sectalign", "__TEXT", "__kernels", "8" ])
            (stat, output) = run_shell([ temp + "_device" ])
        else:
            (stat, output) = (cc_stat, cc_out)

    run = run + 1

    # stat == 0 => compiled OK, -1 => compiler caught an error
    if stat != 0 and stat != 255:
        print "\033[31;1m    FAILED: compiler crashed (" + str(stat) + ")\033[0m"
    elif type(expected) is int:
        try:
            outval = int(output[0])

            if outval == expected:
                print "\033[32;1m    PASS:", expected, "\033[0m"
                passed = passed + 1
            else:
                print "\033[31;1m    FAILED: expected", expected, "- returned", outval, "\033[0m"
        except ValueError:
            print "\033[31;1m    FAILED: expected", expected, "- returned", output[0], "\033[0m"
    else:
        try:
            output = int(output[0])

            print "\033[31;1m    FAILED: expected", expected, "\033[0m"
        except:
            print "\033[32;1m    PASS:", expected, "\033[0m"
            passed = passed + 1

if not(dump_temp):
    run_shell([ "rm", "-rf", "tmp" ])

print "\033[34;1m***************************************************\033[0m"
print "\033[34;1m* Summary                                         *\033[0m"
print "\033[34;1m***************************************************\033[0m"
print "\033[37;1mRan:   ", run, "tests\033[0m"
if passed == run:
    print "\033[32;1mPassed:", passed, "tests\033[0m"
    print "\033[32;1mFailed:", (run - passed), "tests\033[0m"
else:
    print "\033[31;1mPassed:", passed, "tests\033[0m"
    print "\033[31;1mFailed:", (run - passed), "tests\033[0m"
