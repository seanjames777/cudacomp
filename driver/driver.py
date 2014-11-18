#!/usr/bin/python

import sys
import subprocess

####### CONFIGURATION #######

target = "host"
input = ""

#############################

def parse_args():
    global target, input

    for arg in sys.argv:
        if arg == "--host":
            target = "host"
        elif arg == "--device":
            target = "device"
        else:
            input = arg

def run_shell(command):
    proc = subprocess.Popen (command, stdout=subprocess.PIPE)

    while True:
        line = proc.stdout.readline()
        if not line:
            break
        sys.stdout.write(line)
        sys.stdout.flush()

########### ENTRY ###########

parse_args()

if input == "":
    print "Error: no input file"
    exit(-1)

print "Compile", input, "for", target

run_shell([ "make" ])
run_shell([ "make", "install" ])

if target == "host":
    run_shell([ "../out/bin/cc", "-o", "host.ll", input ])
    run_shell([ "llc-mp-3.5", "-o", "host.o", "-filetype=obj", "host.ll" ])
    run_shell([ "clang", "-o", "host", "host.o", "../out/bin/libhost_rt.a" ])
    run_shell([ "./host" ])
else:
    run_shell([ "../out/bin/cc", "-o", "device.ll", "--emit-device", input ])
    run_shell([ "llc-mp-3.5", "-o", "device.ptx", "device.ll" ])
    run_shell([ "clang", "-o", "device", "../out/bin/libdevice_rt.a", "-framework", "CUDA" ])
    run_shell([ "./device" ])
