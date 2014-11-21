#!/usr/bin/python

import sys
import subprocess
import time
import md5

####### CONFIGURATION #######

target = "host"
run_list = []
dump_temp = False

tests = [
    ("../tests/testBinOps1.cc", 14),
    ("../tests/testBinOps2.cc", 2),
    ("../tests/testCmp1.cc", 1),
    ("../tests/testUndeclared.cc", "undeclared"),
    ("../tests/testUndefined.cc", "undefined"),
    ("../tests/testRedeclared.cc", "redeclared"),
    ("../tests/testId1.cc", 10),
    ("../tests/testId2.cc", 20),
    ("../tests/testId3.cc", "undeclared"),
    ("../tests/testId4.cc", 20),
    ("../tests/testId5.cc", "undeclared"),
    ("../tests/testId6.cc", 100),
    ("../tests/testReturnDeclDef.cc", 5),
    ("../tests/testUnOps1.cc", -1),
    ("../tests/testUnOps2.cc", -1),
    ("../tests/testUnOps3.cc", 2),
    ("../tests/testScope1.cc", 10),
    ("../tests/testScope2.cc", "undeclared"),
    ("../tests/testScope3.cc", 10),
    ("../tests/testScope4.cc", 10),
    ("../tests/testIf1.cc", 10),
    ("../tests/testIf2.cc", 20),
    ("../tests/testIf3.cc", 10),
    ("../tests/testIf4.cc", "undefined"),
    ("../tests/testIf5.cc", 10),
    ("../tests/testIf6.cc", 10),
    ("../tests/testReturnCheck1.cc", "noreturn"),
    ("../tests/testReturnCheck2.cc", 10),
    ("../tests/testReturnCheck3.cc", 10),
    ("../tests/testReturnCheck4.cc", 10),
    ("../tests/testReturnCheck5.cc", "noreturn"),
    ("../tests/testReturnCheck6.cc", 10),
    ("../tests/testCall1.cc", 5),
    ("../tests/testCall2.cc", "illegaltype"),
    ("../tests/testCall3.cc", "illegaltype"),
    ("../tests/testCall4.cc", "illegaltype"),
    ("../tests/testCall5.cc", 5),
    ("../tests/testCall6.cc", "illegaltype"),
    ("../tests/testCall7.cc", "illegaltype"),
    ("../tests/testCall8.cc", 20),
    ("../tests/testCall9.cc", 5),
    ("../tests/testCall10.cc", "illegaltype"),
    ("../tests/testCall11.cc", "illegaltype"),
    ("../tests/testCall12.cc", 5),
    ("../tests/testCall13.cc", 11),
    ("../tests/testCall14.cc", "illegaltype"),
    ("../tests/testCall15.cc", "redeclared"),
    ("../tests/testFib1.cc", 34),
    ("../tests/testFib2.cc", 34),
    ("../tests/testFib3.cc", 34),
]

# TODO: actually test type checking...

#############################

def parse_args():
    global target, input, dump_temp

    for arg in sys.argv:
        if arg == "--host":
            target = "host"
        elif arg == "--device":
            target = "device"
        elif arg == "--tmp":
            dump_temp = True
        elif arg != sys.argv[0]:
            run_list.append(arg)

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

for (name, expected) in tests:
    if len(run_list) > 0 and not(name in run_list):
        continue

    temp = "tmp/" + md5.new(name).hexdigest()

    print "\033[37;1mTest '" + name + "'\033[0m"

    if dump_temp:
        print "    " + temp

    stat = -7
    output = []

    if target == "host":
        (cc_stat, cc_out) = run_shell([ "../out/bin/cc", "-o", temp + "_host.ll", name ])
        if cc_stat == 0:
            run_shell([ "llc-mp-3.5", "-o", temp + "_host.o", "-filetype=obj", temp + "_host.ll" ])
            run_shell([ "clang", "-o", temp + "_host", temp + "_host.o", "../out/bin/libhost_rt.a" ])
            (stat, output) = run_shell([ temp + "_host" ])
        else:
            (stat, output) = (cc_stat, cc_out)
    else:
        (cc_stat, cc_out) = run_shell([ "../out/bin/cc", "-o", temp + "_device.ll", "--emit-device", name ])
        if cc_stat == 0:
            run_shell([ "llc-mp-3.5", "-o", temp + "_device.ptx", temp + "_device.ll" ])
            run_shell([ "nvcc", "-fatbin", "-o", temp + "_device.cubin", temp + "_device.ptx" ])
            run_shell([ "clang", "-o", temp + "_device", "../out/bin/libdevice_rt.a", "-framework", "CUDA",
                "-sectcreate", "__TEXT", "__kernels", temp + "_device.cubin",
                "-sectalign", "__TEXT", "__kernels", "8" ])
            (stat, output) = run_shell([ temp + "_device" ])
        else:
            (stat, output) = (cc_stat, cc_out)

    run = run + 1

    if stat == -1 or len(output) == 0:
        print "\033[31;1m    FAILED: unknown error", "\033[0m"
    elif type(expected) is str:
        if output[0] == expected:
            print "\033[32;1m    PASS:", expected, "\033[0m"
            passed = passed + 1
        else:
            print "\033[31;1m    FAILED: expected", expected, "\033[0m"
    elif type(expected) is int:
        if int(output[0]) != expected:
            print "\033[31;1m    FAILED: expected", expected, "- returned", output[0], "\033[0m"
        else:
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
