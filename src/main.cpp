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

    if (!node)
        return -1;

    CodegenCtx ctx(args.emit_device);
    Codegen::codegen_stmt(&ctx, node);
    ctx.emit(args.out_file);

    delete node;
}
