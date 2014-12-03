#!/usr/bin/python

import sys, subprocess, os, getpass

emit_llvm = False
safe_mode = True
optimize = 2
print_ast = False

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
    elif arg == "--print-ast":
        print_ast = True
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

# If we are not the autograder we need to compile the runtime library ourself...
autograder = getpass.getuser() == "autograder"
link_runtime = not(emit_llvm)

# Process each source file
for source in sources:
    compiler_args = [ "./cc", "--symbol-prefix", "_c0_", "--require-entry", "main", "--no-float" ]

    if print_ast:
        compiler_args.append("--print-ast")

    outfile_bc = source.rsplit('.', 1)[0] + ".bc"
    outfile_ll = source.rsplit('.', 1)[0] + ".ll"

    for header in headers:
        compiler_args.append("-ix")
        compiler_args.append(header)

    if not(link_runtime):
        compiler_args.append("-S")
        compiler_args.append("-o")
        compiler_args.append(outfile_ll)
    else:
        compiler_args.append("-o")
        compiler_args.append(outfile_bc)

    if safe_mode:
        compiler_args.append("--mem-safe")
        compiler_args.append("--oper-safe")

    compiler_args.append(source)

    stat = run_shell(compiler_args)

    if stat != 0:
        print "Compile error"
        exit(stat)

    if link_runtime:
        # Link with the compiled C0
        stat = run_shell([ "llvm-link", "-S", "-o", outfile_ll, "l4lib.bc", outfile_bc ])

        if stat != 0:
            print "llvm-link error"
            exit(stat)

    # We don't have an x86 backend, so if we're compiling to x86, use llc
    if not(emit_llvm):
        outfile = source.rsplit('.', 1)[0] + ".s"

        stat = run_shell([ "llc", "-o", outfile, outfile_ll ])

        if stat != 0:
            print "llc error"
            exit(stat)

    if link_runtime:
        run_shell([ "rm", outfile_bc ])

    if not(emit_llvm):
        run_shell([ "rm", outfile_ll ])

exit(0)
