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
#include <options.h>

int main(int argc, char *argv[]) {
    // Parse arguments
    parseArgs(argc, argv);
    struct CCArgs *args = getOptions();

    std::shared_ptr<ASTDeclSeqNode> node;

    try {
        node = Parser::parse(args->in_file);
    }
    catch (Parser::ParseException & except) {
        std::cout << "\033[31;1m" << except.what() << "\033[0m" << std::endl;
        return -1;
    }

    if (args->print_ast) {
        ASTPrint print(std::cout, true);
        print.run(node);
    }

    std::shared_ptr<ModuleInfo> moduleInfo;

    try {
        moduleInfo = Statics::run(node);
    }
    catch (Statics::StaticsException & except) {
        std::cout << "\033[31;1m" << except.what() << "\033[0m" << std::endl;
        return -1;
    }

    if (args->out_file) {
        std::ofstream out(args->out_file, std::ios::out);

        if (!out) {
            std::cout << "\033[31;1m" << "Error opening " << args->out_file << "\033[0m" << std::endl;
            return -1;
        }

        Codegen::codegen_tops(moduleInfo, node, args->emit_device, out);

        out.close();
    }
    else
        Codegen::codegen_tops(moduleInfo, node, args->emit_device, std::cout);

    return 0;
}
