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
        "Usage: %s [-S] [-i <file>] [-o[d] <file>] [<file>]\n"
        "\n"
        "Basic Options:\n"
        "    -h                     Print this help message and exit\n"
        "    -S                     Emit LLVM IR as text instead of bitcode\n"
        "    -o <file>              Output (host) assembly to a file\n"
        "    -od <file>             Output (device) assembly to a file\n"
        "    -ix <file>             Include a header of external declarations\n"
        "    <file>                 File to parse, instead of standard in\n"
        "\n"
        "Additional Options:\n"
        "    --print-ast            Print a description of the parsed abstract syntax tree\n"
        "    --emit-device          Emit the entire program as PTX code, if possible\n"
        "    --symbol-prefix <pfx>  String to prefix internal function names with\n"
        "    --mem-safe             Insert memory safety checks\n"
        "    --oper-safe            Insert operator safety checks (division by 0, etc.)\n"
        "    --require-entry <name> Require an entrypoint called <name> \n"
        , argv[0]);
}

void parseArgs(int argc, char *argv[]) {
    args.emit_device   = false;
    args.print_ast     = false;
    args.in_file       = "";
    args.out_hfile     = "";
    args.out_dfile     = "";
    args.verbose       = false;
    args.symbol_prefix = "";
    args.mem_safe      = false;
    args.opr_safe      = false;
    args.entrypoint    = "_cc_main";

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
            args.out_hfile = std::string(argv[++i]);
        else if (strcmp(argv[i], "-od") == 0)
            args.out_dfile = std::string(argv[++i]);
        else if (strcmp(argv[i], "--verbose") == 0)
            args.verbose = true;
        else if (strcmp(argv[i], "--symbol-prefix") == 0)
            args.symbol_prefix = std::string(argv[++i]);
        else if (strcmp(argv[i], "--mem-safe") == 0)
            args.mem_safe = true;
        else if (strcmp(argv[i], "--oper-safe") == 0)
            args.opr_safe = true;
        else if (strcmp(argv[i], "-S") == 0)
            args.emit_text = true;
        else if (strcmp(argv[i], "--require-entry") == 0)
            args.entrypoint = std::string(argv[++i]);
        else if (strcmp(argv[i], "-ix") == 0)
            args.headers.push_back(std::string(argv[++i]));
        else
            args.in_file = std::string(argv[i]);
    }

    if (args.out_hfile.empty())
        args.out_hfile = args.emit_text ? "host.ll" : "host.bc";

    if (args.out_dfile.empty())
        args.out_dfile = args.emit_text ? "device.ll" : "device.bc";
}
