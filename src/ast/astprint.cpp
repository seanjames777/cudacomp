/**
 * @file astprint.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/astprint.h>
#include <cxxabi.h>

ASTPrint::ASTPrint(std::ostream & out, bool colorize)
    : tabDepth(0),
      out(out),
      colorize(colorize)
{
}

void ASTPrint::color(int color) {
    if (!colorize)
        return;

    out << "\033[" << (30 + color) << ";1m";
}

void ASTPrint::uncolor() {
    if (!colorize)
        return;

    out << "\033[0m";
}

void ASTPrint::run(std::shared_ptr<ASTDeclSeqNode> ast) {
    ASTVisitor::visitNode(ast);
}

void ASTPrint::enter(std::shared_ptr<ASTNode> node) {
    std::string nodeName = typeid(*(node.get())).name();
    int stat;
    nodeName = abi::__cxa_demangle(nodeName.c_str(), 0, 0, &stat);
    nodeName = nodeName.substr(3, nodeName.length() - 3);

    for (int i = 0; i < tabDepth; i++)
        out << "  ";

    color(7);
    out << "+ ";

    color(6);
    out << nodeName << " ";
    color(0);
    out << "<" << node << "> ";
    color(1);
    out << "(";
    color(7);
    node->print(out);
    color(1);
    out << ")";
    uncolor();
    out << std::endl;
    tabDepth++;
}

void ASTPrint::leave() {
    tabDepth--;
}

void ASTPrint::visitNode(std::shared_ptr<ASTNode> node) {
    if (std::shared_ptr<ASTDeclSeqNode> seq = std::dynamic_pointer_cast<ASTDeclSeqNode>(node))
        ASTVisitor::visitDeclSeqNode(seq);
    else if (std::shared_ptr<ASTExpSeqNode> seq = std::dynamic_pointer_cast<ASTExpSeqNode>(node))
        ASTVisitor::visitExpSeqNode(seq);
    else if (std::shared_ptr<ASTStmtSeqNode> seq = std::dynamic_pointer_cast<ASTStmtSeqNode>(node))
        ASTVisitor::visitStmtSeqNode(seq);
    else if (std::shared_ptr<ASTArgSeqNode> seq = std::dynamic_pointer_cast<ASTArgSeqNode>(node))
        ASTVisitor::visitArgSeqNode(seq);
    else {
        enter(node);
        ASTVisitor::visitNode(node);
        leave();
    }
}
