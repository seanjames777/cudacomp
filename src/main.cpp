/**
 * @file main.cpp
 *
 * @brief Executable entrypoint
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <defs.h>
#include <parser/parse.h>

int main(int argc, char *argv[]) {
    ASTNode *node = Parser::parse(argc > 1 ? argv[1] : NULL);

    if (!node) {
        std::cout << "Error parsing " << argv[1] << std::endl;
        return -1;
    }

    CodegenCtx ctx;
    ctx.finish(node->codegen(&ctx));

    delete node;
}
