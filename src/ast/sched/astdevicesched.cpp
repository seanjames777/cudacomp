/**
 * @file astdevicesched.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/sched/astdevicesched.h>

std::shared_ptr<ASTDeviceSched> ASTDeviceSched::instance = nullptr;

ASTDeviceSched::ASTDeviceSched() {
}

std::shared_ptr<ASTDeviceSched> ASTDeviceSched::get() {
    if (!instance)
        instance = std::make_shared<ASTDeviceSched>();

    return instance;
}

void ASTDeviceSched::print(std::ostream & out) {
}
