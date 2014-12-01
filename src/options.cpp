/**
 * @file options.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <options.h>

struct CCArgs args;

struct CCArgs *getOptions() {
    return &args;
}

void printHelp(char *argv[]) {
    printf(
        "Usage: %s [-h] [--print-ast] [--emit-device] [-o <file>] [--symbol-prefix <pfx>] [<file>]\n"
        "    [ --mem-safe ] [ --oper-safe ] [ -S ]\n"
        "\n"
        "Options:\n"
        "    -h                    Print this help message and exit\n"
        "    --print-ast           Print a description of the parsed abstract syntax tree\n"
        "    --emit-device         Emit the entire program as PTX code, if possible\n"
        "    -o <file>             Output assembly to a file instead of standard out\n"
        "    --symbol-prefix <pfx> String to prefix internal function names with\n"
        "    --mem-safe            Insert memory safety checks\n"
        "    --oper-safe           Insert operator safety checks (division by 0, etc.)\n"
        "    -S                    Emit LLVM IR as text instead of bitcode\n"
        "    <file>                File to parse, instead of standard in\n"
        , argv[0]);
}

void parseArgs(int argc, char *argv[]) {
    memset(&args, 0, sizeof(args));
    args.symbol_prefix = "";

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            printHelp(argv);
            exit(0);
        }
        else if (strcmp(argv[i], "--emit-device") == 0)
            args.emit_device = true;
        else if (strcmp(argv[i], "--print-ast") == 0)
            args.print_ast = true;
        else if (strcmp(argv[i], "-o") == 0)
            args.out_file = argv[++i];
        else if (strcmp(argv[i], "--verbose") == 0)
            args.verbose = true;
        else if (strcmp(argv[i], "--symbol-prefix") == 0)
            args.symbol_prefix = argv[++i];
        else if (strcmp(argv[i], "--mem-safe") == 0)
            args.mem_safe = true;
        else if (strcmp(argv[i], "--oper-safe") == 0)
            args.opr_safe = true;
        else if (strcmp(argv[i], "-S") == 0)
            args.emit_text = true;
        else
            args.in_file = argv[i];
    }
}