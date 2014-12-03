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

std::shared_ptr<ASTDeclSeqNode> append(
    std::shared_ptr<ASTDeclSeqNode> to,
    std::shared_ptr<ASTDeclSeqNode> from)
{
    if (to == nullptr)
        return from;

    std::shared_ptr<ASTDeclSeqNode> decl = to;

    while (decl->getTail() != nullptr)
        decl = decl->getTail();

    decl->setTail(from);

    return to;
}

int main(int argc, char *argv[]) {
    try {
        // Parse arguments
        parseArgs(argc, argv);
    }
    catch (std::runtime_error & except) {
        std::cout << "\033[31;1m" << except.what() << "\033[0m" << std::endl;
        return -1;
    }

    struct CCArgs *args = getOptions();

    std::shared_ptr<ASTDeclSeqNode> ast = nullptr;

    for (auto header : args->headers) {
        if (args->verbose)
            std::cout << "Parse '" << header << "'" << std::endl;

        try {
            std::shared_ptr<ASTDeclSeqNode> parsed = Parser::parse(header, true);
            ast = append(ast, parsed);
        }
        catch (Parser::ParseException & except) {
            std::cout << "\033[31;1m" << except.what() << "\033[0m" << std::endl;
            return -1;
        }
    }

    if (args->verbose)
            std::cout << "Parse '" << args->in_file << "'" << std::endl;

    try {
        std::shared_ptr<ASTDeclSeqNode> parsed = Parser::parse(args->in_file, false);
        ast = append(ast, parsed);
    }
    catch (Parser::ParseException & except) {
        std::cout << "\033[31;1m" << except.what() << "\033[0m" << std::endl;
        return -1;
    }

    if (args->print_ast) {
        ASTPrint print(std::cout, true);
        print.run(ast);
    }

    std::shared_ptr<ModuleInfo> moduleInfo;

    try {
        moduleInfo = Statics::run(ast);
    }
    catch (Statics::StaticsException & except) {
        std::cout << "\033[31;1m" << except.what() << "\033[0m" << std::endl;
        return -1;
    }

    if (!args->emit_device) {
        std::ofstream out(args->out_hfile, std::ios::out);

        if (!out) {
            std::cout << "\033[31;1m" << "Error opening " << args->out_dfile << "\033[0m" << std::endl;
            return -1;
        }

        Codegen::codegen_tops(moduleInfo, ast, false, out);

        out.close();
    }

    if (moduleInfo->hasCudaFunctions()) {
        std::ofstream out(args->out_dfile, std::ios::out);

        if (!out) {
            std::cout << "\033[31;1m" << "Error opening " << args->out_dfile << "\033[0m" << std::endl;
            return -1;
        }

        Codegen::codegen_tops(moduleInfo, ast, true, out);

        out.close();
    }

    return 0;
}
