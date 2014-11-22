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

    std::shared_ptr<ASTTopSeqNode> node = Parser::parse(args.in_file);

    if (!node)
        return -1;

    std::shared_ptr<ModuleInfo> moduleInfo;

    try {
        moduleInfo = Statics::run(node);
    }
    catch (Statics::UndefinedException *except) {
        std::cout << "undefined" << std::endl;
        std::cout << except->what() << std::endl;
        return -2;
    }
    catch (Statics::UndeclaredException *except) {
        std::cout << "undeclared" << std::endl;
        std::cout << except->what() << std::endl;
        return -2;
    }
    catch (Statics::RedeclaredException *except) {
        std::cout << "redeclared" << std::endl;
        std::cout << except->what() << std::endl;
        return -2;
    }
    catch (Statics::IllegalTypeException *except) {
        std::cout << "illegaltype" << std::endl;
        std::cout << except->what() << std::endl;
        return -2;
    }
    catch (Statics::NoReturnException *except) {
        std::cout << "noreturn" << std::endl;
        std::cout << except->what() << std::endl;
        return -2;
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
