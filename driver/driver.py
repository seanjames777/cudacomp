#!/usr/bin/python

import sys
import subprocess
import time

####### CONFIGURATION #######

target = "host"

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
    ("../tests/testFib1.cc", 34),
]

#############################

def parse_args():
    global target, input

    for arg in sys.argv:
        if arg == "--host":
            target = "host"
        elif arg == "--device":
            target = "device"

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

for (name, expected) in tests:
    print "\033[37;1mTest '", name, "'\033[0m"

    stat = -7
    output = []

    if target == "host":
        (cc_stat, cc_out) = run_shell([ "../out/bin/cc", "-o", "host.ll", name ])
        if cc_stat == 0:
            run_shell([ "llc-mp-3.5", "-o", "host.o", "-filetype=obj", "host.ll" ])
            run_shell([ "clang", "-o", "host", "host.o", "../out/bin/libhost_rt.a" ])
            (stat, output) = run_shell([ "./host" ])
        else:
            (stat, output) = (cc_stat, cc_out)
        run_shell([ "rm", "-f", "host.ll", "host.o", "host" ])
    else:
        (cc_stat, cc_out) = run_shell([ "../out/bin/cc", "-o", "device.ll", "--emit-device", name ])
        if cc_stat == 0:
            run_shell([ "llc-mp-3.5", "-o", "device.ptx", "device.ll" ])
            run_shell([ "clang", "-o", "device", "../out/bin/libdevice_rt.a", "-framework", "CUDA" ])
            (stat, output) = run_shell([ "./device" ])
        else:
            (stat, output) = (cc_stat, cc_out)
        run_shell([ "rm", "-f", "device.ll", "device.ptx", "device" ])
        time.sleep(1)

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
