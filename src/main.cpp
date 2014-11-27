/**
 * @file main.cpp
 *
 * @brief Executable entrypoint
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <defs.h>
#include <parser/parse.h>
#include <codegen/codegen.h>
#include <statics/statics.h>
#include <ast/astprint.h>

struct CCArgs {
    bool  emit_device;
    bool  print_ast;
    char *in_file;
    char *out_file;
    bool  verbose;
} args;

void printHelp(char *argv[]) {
    printf(
        "Usage: %s [-h] [--print-ast] [--emit-device] [-o <file>] [<file>]\n"
        "\n"
        "Options:\n"
        "    -h            Print this help message and exit\n"
        "    --print-ast   Print a description of the parsed abstract syntax tree\n"
        "    --emit-device Emit the entire program as PTX code, if possible\n"
        "    -o <file>     Output assembly to a file instead of standard out\n"
        "    <file>        File to parse, instead of standard in\n"
        , argv[0]);
}

void parseArgs(int argc, char *argv[]) {
    memset(&args, 0, sizeof(args));

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
            args.out_file = argv[i++ + 1];
        else if (strcmp(argv[i], "--verbose") == 0)
            args.verbose = true;
        else
            args.in_file = argv[i];
    }
}

int main(int argc, char *argv[]) {
    parseArgs(argc, argv);

    std::shared_ptr<ASTDeclSeqNode> node;

    try {
        node = Parser::parse(args.in_file);
    }
    catch (Parser::ParseException & except) {
        std::cout << "\033[31;1m" << except.what() << "\033[0m" << std::endl;
        return -1;
    }

    if (args.print_ast) {
        ASTPrint print(std::cout, true);
        print.run(node);
    }

    std::shared_ptr<ModuleInfo> moduleInfo;

    try {
        moduleInfo = Statics::run(node, args.verbose);
    }
    catch (Statics::StaticsException & except) {
        std::cout << "\033[31;1m" << except.what() << "\033[0m" << std::endl;
        return -1;
    }

    if (args.out_file) {
        std::ofstream out(args.out_file, std::ios::out);

        if (!out) {
            std::cout << "\033[31;1m" << "Error opening " << args.out_file << "\033[0m" << std::endl;
            return -1;
        }

        Codegen::codegen_tops(moduleInfo, node, args.emit_device, out);

        out.close();
    }
    else
        Codegen::codegen_tops(moduleInfo, node, args.emit_device, std::cout);

    return 0;
}
