/**
 * @file main.cpp
 *
 * @brief Executable entrypoint
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <defs.h>
#include <parser/parse.h>
#include <statics/typecheck.h>
#include <codegen/codegen.h>
#include <statics/returncheck.h>

struct CCArgs {
    bool  emit_device;
    char *in_file;
    char *out_file;
} args;

void parseArgs(int argc, char *argv[]) {
    memset(&args, 0, sizeof(args));

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--emit-device") == 0)
            args.emit_device = true;
        else if (strcmp(argv[i], "-o") == 0)
            args.out_file = argv[i++ + 1];
        else
            args.in_file = argv[i];
    }
}

int main(int argc, char *argv[]) {
    parseArgs(argc, argv);

    ASTStmtNode *node = Parser::parse(args.in_file);

    //node->print(std::cout);

    if (!node)
        return -1;

    TypeCtx typeCtx;

    try {
        Statics::idset decl;
        Statics::idset def;
        Statics::typecheck_stmt(&typeCtx, decl, def, node);
    }
    catch (Statics::UndefinedException *except) {
        std::cout << "undefined" << std::endl;
        return -2;
    }
    catch (Statics::UndeclaredException *except) {
        std::cout << "undeclared" << std::endl;
        return -2;
    }
    catch (Statics::RedeclaredException *except) {
        std::cout << "redeclared" << std::endl;
        return -2;
    }
    catch (Statics::IllegalTypeException *except) {
        std::cout << "illegaltype" << std::endl;
        return -2;
    }

    if (!Statics::returncheck_stmt(node)) {
        std::cout << "noreturn" << std::endl;
        return -2;
    }

    CodegenCtx cgCtx(args.emit_device, &typeCtx);
    Codegen::codegen_stmt(&cgCtx, node);

    cgCtx.emit(args.out_file);

    delete node;

    return 0;
}
