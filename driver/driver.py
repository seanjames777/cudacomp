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
    (SOURCE_DIR + "tests/testBinOps1.cc", 14, "both"),
    (SOURCE_DIR + "tests/testBinOps2.cc", 2, "both"),
    (SOURCE_DIR + "tests/testCmp1.cc", 1, "both"),
    (SOURCE_DIR + "tests/testUndeclared.cc", "error", "both"),
    (SOURCE_DIR + "tests/testUndefined.cc", "error", "both"),
    (SOURCE_DIR + "tests/testRedeclared.cc", "error", "both"),
    (SOURCE_DIR + "tests/testId1.cc", 10, "both"),
    (SOURCE_DIR + "tests/testId2.cc", 20, "both"),
    (SOURCE_DIR + "tests/testId3.cc", "error", "both"),
    (SOURCE_DIR + "tests/testId4.cc", 20, "both"),
    (SOURCE_DIR + "tests/testId5.cc", "error", "both"),
    (SOURCE_DIR + "tests/testId6.cc", 100, "both"),
    (SOURCE_DIR + "tests/testReturnDeclDef.cc", 5, "both"),
    (SOURCE_DIR + "tests/testUnOps1.cc", -1, "both"),
    (SOURCE_DIR + "tests/testUnOps2.cc", -1, "both"),
    (SOURCE_DIR + "tests/testUnOps3.cc", 2, "both"),
    (SOURCE_DIR + "tests/testScope1.cc", 10, "both"),
    (SOURCE_DIR + "tests/testScope2.cc", "error", "both"),
    (SOURCE_DIR + "tests/testScope3.cc", 10, "both"),
    (SOURCE_DIR + "tests/testScope4.cc", 10, "both"),
    (SOURCE_DIR + "tests/testScope5.cc", 10, "both"),
    (SOURCE_DIR + "tests/testIf1.cc", 10, "both"),
    (SOURCE_DIR + "tests/testIf2.cc", 20, "both"),
    (SOURCE_DIR + "tests/testIf3.cc", 10, "both"),
    (SOURCE_DIR + "tests/testIf4.cc", "error", "both"),
    (SOURCE_DIR + "tests/testIf5.cc", 10, "both"),
    (SOURCE_DIR + "tests/testIf6.cc", 10, "both"),
    (SOURCE_DIR + "tests/testReturnCheck1.cc", "error", "both"),
    (SOURCE_DIR + "tests/testReturnCheck2.cc", 10, "both"),
    (SOURCE_DIR + "tests/testReturnCheck3.cc", 10, "both"),
    (SOURCE_DIR + "tests/testReturnCheck4.cc", 10, "both"),
    (SOURCE_DIR + "tests/testReturnCheck5.cc", "error", "both"),
    (SOURCE_DIR + "tests/testReturnCheck6.cc", 10, "both"),
    (SOURCE_DIR + "tests/testCall1.cc", 5, "both"),
    (SOURCE_DIR + "tests/testCall2.cc", "error", "both"),
    (SOURCE_DIR + "tests/testCall3.cc", "error", "both"),
    (SOURCE_DIR + "tests/testCall4.cc", "error", "both"),
    (SOURCE_DIR + "tests/testCall5.cc", 5, "both"),
    (SOURCE_DIR + "tests/testCall6.cc", "error", "both"),
    (SOURCE_DIR + "tests/testCall7.cc", "error", "both"),
    (SOURCE_DIR + "tests/testCall8.cc", 20, "both"),
    (SOURCE_DIR + "tests/testCall9.cc", 5, "both"),
    (SOURCE_DIR + "tests/testCall10.cc", "error", "both"),
    (SOURCE_DIR + "tests/testCall11.cc", "error", "both"),
    (SOURCE_DIR + "tests/testCall12.cc", 5, "both"),
    (SOURCE_DIR + "tests/testCall13.cc", 11, "both"),
    (SOURCE_DIR + "tests/testCall14.cc", "error", "both"),
    (SOURCE_DIR + "tests/testCall15.cc", "error", "both"),
    (SOURCE_DIR + "tests/testCall16.cc", 0, "both"),
    (SOURCE_DIR + "tests/testFib1.cc", 34, "both"),
    (SOURCE_DIR + "tests/testFib2.cc", 34, "both"),
    (SOURCE_DIR + "tests/testFib3.cc", 34, "both"),
    (SOURCE_DIR + "tests/testFib4.cc", 34, "both"),
    (SOURCE_DIR + "tests/testWhile1.cc", 1, "both"),
    (SOURCE_DIR + "tests/testWhile2.cc", 55, "both"),
    (SOURCE_DIR + "tests/testWhile3.cc", 1000, "both"),
    (SOURCE_DIR + "tests/testWhile4.cc", "error", "both"),
    (SOURCE_DIR + "tests/testWhile5.cc", "error", "both"),
    (SOURCE_DIR + "tests/testWhile6.cc", "error", "both"),
    (SOURCE_DIR + "tests/testWhile7.cc", 0, "both"),
    (SOURCE_DIR + "tests/testFor1.cc", 10, "both"),
    (SOURCE_DIR + "tests/testFor2.cc", 10, "both"),
    (SOURCE_DIR + "tests/testFor3.cc", 0, "both"),
    (SOURCE_DIR + "tests/testFor4.cc", 63, "both"),
    (SOURCE_DIR + "tests/testFor5.cc", 900, "both"),
    (SOURCE_DIR + "tests/testFor6.cc", 0, "both"),
    (SOURCE_DIR + "tests/testFor7.cc", "error", "both"),
    (SOURCE_DIR + "tests/testTypeDef1.cc", 5, "both"),
    (SOURCE_DIR + "tests/testTypeDef2.cc", "error", "both"),
    (SOURCE_DIR + "tests/testTypeDef3.cc", 5, "both"),
    (SOURCE_DIR + "tests/testTypeDef4.cc", "error", "both"),
    (SOURCE_DIR + "tests/testTypeDef5.cc", "error", "both"),
    (SOURCE_DIR + "tests/testTypeDef6.cc", 5, "both"),
    (SOURCE_DIR + "tests/testFunDecl1.cc", 5, "both"),
    (SOURCE_DIR + "tests/testFunDecl2.cc", 5, "both"),
    (SOURCE_DIR + "tests/testFunDecl3.cc", "error", "both"),
    (SOURCE_DIR + "tests/testFunDecl4.cc", "error", "both"),
    (SOURCE_DIR + "tests/testFunDecl5.cc", "error", "both"),
    (SOURCE_DIR + "tests/testFunDecl6.cc", "error", "both"),
    (SOURCE_DIR + "tests/testFunDecl7.cc", "error", "both"),
    (SOURCE_DIR + "tests/testFunDecl8.cc", "error", "both"),
    (SOURCE_DIR + "tests/testFunDecl9.cc", 5, "both"),
    (SOURCE_DIR + "tests/testFunDecl10.cc", "error", "both"),
    (SOURCE_DIR + "tests/testFunDecl11.cc", 5, "both"),
    (SOURCE_DIR + "tests/testFunDecl12.cc", "error", "both"),
    (SOURCE_DIR + "tests/testFunDecl13.cc", 518, "both"),
    (SOURCE_DIR + "tests/testArr1.cc", 5, "both"),
    (SOURCE_DIR + "tests/testArr2.cc", 0, "both"),
    (SOURCE_DIR + "tests/testArr3.cc", 5, "both"),
    (SOURCE_DIR + "tests/testArr4.cc", 19, "both"),
    (SOURCE_DIR + "tests/testArr5.cc", 10, "both"),
    (SOURCE_DIR + "tests/testArr6.cc", 5, "both"),
    (SOURCE_DIR + "tests/testArr7.cc", "error", "both"),
    (SOURCE_DIR + "tests/testArr8.cc", "error", "both"),
    (SOURCE_DIR + "tests/testArr9.cc", "error", "both"),
    (SOURCE_DIR + "tests/testLValue1.cc", "error", "both"),
    (SOURCE_DIR + "tests/testFloat1.cc", 10, "both"),
    (SOURCE_DIR + "tests/testPointers1.cc", 5, "both"),
    (SOURCE_DIR + "tests/testPointers2.cc", 0, "both"),
    (SOURCE_DIR + "tests/testPointers3.cc", 20, "both"),
    (SOURCE_DIR + "tests/testPointers4.cc", 0, "both"),
    (SOURCE_DIR + "tests/testPointers5.cc", 0, "both"),
    (SOURCE_DIR + "tests/testPointers6.cc", "error", "both"),
    (SOURCE_DIR + "tests/testPointers7.cc", 10, "both"),
    (SOURCE_DIR + "tests/testPointers8.cc", 20, "both"),
    (SOURCE_DIR + "tests/testPointers9.cc", 0, "both"),
    (SOURCE_DIR + "tests/testPointers10.cc", "error", "both"),
    (SOURCE_DIR + "tests/testPointers11.cc", 1, "both"),
    (SOURCE_DIR + "tests/testStructs1.cc", 0, "both"),
    (SOURCE_DIR + "tests/testStructs2.cc", 5, "both"),
    (SOURCE_DIR + "tests/testStructs3.cc", 1, "both"),
    (SOURCE_DIR + "tests/testStructs4.cc", 5, "both"),
    (SOURCE_DIR + "tests/testStructs5.cc", 5, "both"),
    (SOURCE_DIR + "tests/testStructs6.cc", 10, "both"),
    (SOURCE_DIR + "tests/testStructs7.cc", 13, "both"),
    (SOURCE_DIR + "tests/testStructs8.cc", 10, "both"),
    (SOURCE_DIR + "tests/testStructs9.cc", 1, "both"),
    (SOURCE_DIR + "tests/testStructs10.cc", 1, "both"),
    (SOURCE_DIR + "tests/testStructs11.cc", 1, "both"),
    (SOURCE_DIR + "tests/testStructs12.cc", 1, "both"),
    (SOURCE_DIR + "tests/testStructs13.cc", 1, "both"),
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

run_shell([ "rm", "-rf", "tmp" ])
run_shell([ "mkdir", "-p", "tmp" ])

def print_verbose(s):
    if verbose:
        print "\033[37;1m    " + s + "\033[0m"

for (name, expected, runfor) in tests:
    if len(run_list) > 0 and not(os.path.basename(name) in run_list):
        continue

    if target != runfor and runfor != "both":
        continue

    temp = "tmp/" + md5.new(name).hexdigest()

    print "\033[37;1mTest '" + name + "'\033[0m"

    if dump_temp:
        print "    " + temp

    stat = -1
    output = []

    run = run + 1

    # Compile host and possibly device code to LLVM bitcode
    print_verbose("Compiling...")

    compile_flags = [ INSTALL_DIR + "bin/cc", "-o", temp + "_host.bc", "-od", temp + "_device.bc" ]

    if target == "device":
        compile_flags.append("--emit-device")

    compile_flags.append(name)

    (cc_stat, cc_out) = run_shell(compile_flags)

    # If the compiler crashed, fail
    if cc_stat != 0 and cc_stat != 255:
        print "\033[31;1m    FAILED: compiler crashed (" + str(cc_stat) + ")\033[0m"
        continue

    # If we expected an error, check
    if not(type(expected) is int):
        if cc_stat == 0:
            print "\033[31;1m    FAILED: expected", expected, "\033[0m"
        else:
            print "\033[32;1m    PASS:", expected, "\033[0m"
            passed = passed + 1
        continue

    has_host = os.path.isfile(temp + "_host.bc")
    has_device = os.path.isfile(temp + "_device.bc")

    print_verbose("Assembling...")

    # Assemble host code
    if has_host:
        run_shell([ LLC, "-o", temp + "_host.o", "-filetype=obj", temp + "_host.bc" ])

    # Assemble device code, link with device runtime kernels
    if has_device:
        run_shell([ LLC, "-o", temp + "_device.ptx", temp + "_device.bc" ])
        run_shell([ "nvcc", "-arch=sm_30", "-cubin", "-dc", "-o", temp + "_device.cubin", temp + "_device.ptx" ])
        run_shell([ "nvcc", "-arch=sm_30", "-cubin", "-dlink", "-o", temp + "_link.cubin", temp + "_device.cubin", BINARY_DIR + "device_rt_kernel.cubin" ])

    print_verbose("Linking...")

    # Link host/device code
    link_args = [ "clang", "-o", temp ]

    # The host/device runtime has already been compiled and linked
    if target == "host":
        link_args.append(INSTALL_DIR + "lib/libhost_rt.a")
    else:
        link_args.append(INSTALL_DIR + "lib/libdevice_rt.a")

    # Link host code
    if has_host:
        link_args.append(temp + "_host.o")

    # Embed kernels in executable
    if has_device:
        link_args.append("-framework")
        link_args.append("CUDA")
        link_args.append("-sectcreate")
        link_args.append("__TEXT")
        link_args.append("__kernels")
        link_args.append(temp + "_link.cubin")

    run_shell(link_args)

    # Execute
    (stat, output) = run_shell([ temp ])

    if stat != 0:
        print "\033[31;1m    FAILED: executable returned non-zero exit status\033[0m"
        continue

    outval = int(output[0])

    if outval == expected:
        print "\033[32;1m    PASS:", expected, "\033[0m"
        passed = passed + 1
    else:
        print "\033[31;1m    FAILED: expected", expected, "- returned", outval, "\033[0m"

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
