#!/usr/bin/python

import sys, subprocess

emit_llvm = False
safe_mode = True
optimize = 2

headers = []
sources = []
next_is_header = False

# Parse arguments
for arg in sys.argv:
    # Skip filename
    if arg == sys.argv[0]:
        continue
    elif arg == "--llvm":
        emit_llvm = True
    elif arg == "--x86_64":
        emit_llvm = False
    elif arg == "--safe":
        safe_mode = True
    elif arg == "--unsafe":
        safe_mode = False
    elif arg == "-O0":
        optimize = 0
    elif arg == "-O1":
        optimize = 1
    elif arg == "-O2":
        optimize = 2
    elif arg == "-l":
        next_is_header = True
    else:
        if next_is_header:
            headers.append(arg)
            next_is_header = False
        else:
            sources.append(arg)

# Runs shell commands
def run_shell(command):
    proc = subprocess.Popen (command, stdout=subprocess.PIPE)
    stat = proc.wait()

    while True:
        line = proc.stdout.readline()
        if not line:
            break
        sys.stdout.write(line)
        sys.stdout.flush()

    return stat

# Process each source file
for source in sources:
    outfile_llvm = source.rsplit('.', 1)[0] + ".ll"

    run_shell([ "./cc", "-o", outfile_llvm, source ])

    if not(emit_llvm):
        outfile = source.rsplit('.')[0] + ".s"
        run_shell([ "llc", "-o", outfile, outfile_llvm ])
