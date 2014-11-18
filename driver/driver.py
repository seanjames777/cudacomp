#!/usr/bin/python

import sys
import subprocess

####### CONFIGURATION #######

target = "host"

tests = [
    ("../tests/testBinOps.cc", 10),
    ("../tests/testUndeclared.cc", "undeclared"),
    ("../tests/testUndefined.cc", "undefined"),
    ("../tests/testRedeclared.cc", "redeclared"),
    ("../tests/testId1.cc", 10),
    ("../tests/testId2.cc", 20),
    ("../tests/testId3.cc", "undeclared"),
    ("../tests/testId4.cc", 20),
    ("../tests/testId5.cc", "undeclared"),
    ("../tests/testId6.cc", 100)
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

print "\033[34;1m**************************\033[0m"
print "\033[34;1m* Running Tests...       *\033[0m"
print "\033[34;1m**************************\033[0m"

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

    if stat == -1 or len(output) == 0:
        print "\033[31;1m    FAILED: unknown error", "\033[0m"
    elif type(expected) is str:
        if output[0] == expected:
            print "\033[32;1m    PASS:", expected, "\033[0m"
        else:
            print "\033[32;1m    FAILED: expected", expected, "\033[0m"
    elif type(expected) is int:
        if int(output[0]) != expected:
            print "\033[31;1m    FAILED: expected", expected, "- returned", output[0], "\033[0m"
        else:
            print "\033[32;1m    PASS:", expected, "\033[0m"
