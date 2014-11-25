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
} args;

void parseArgs(int argc, char *argv[]) {
    memset(&args, 0, sizeof(args));

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--emit-device") == 0)
            args.emit_device = true;
        else if (strcmp(argv[i], "--print-ast") == 0)
            args.print_ast = true;
        else if (strcmp(argv[i], "-o") == 0)
            args.out_file = argv[i++ + 1];
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
        std::cout << except.what() << std::endl;
        return -1;
    }

    if (args.print_ast) {
        ASTPrint print(std::cout, true);
        print.run(node);
    }

    std::shared_ptr<ModuleInfo> moduleInfo;

    try {
        moduleInfo = Statics::run(node);
    }
    catch (Statics::StaticsException & except) {
        std::cout << except.what() << std::endl;
        return -1;
    }

    if (args.out_file) {
        std::ofstream out(args.out_file, std::ios::out);

        if (!out) {
            std::cout << "Error opening " << args.out_file << std::endl;
            return -1;
        }

        Codegen::codegen_tops(moduleInfo, node, args.emit_device, out);

        out.close();
    }
    else
        Codegen::codegen_tops(moduleInfo, node, args.emit_device, std::cout);

    return 0;
}
