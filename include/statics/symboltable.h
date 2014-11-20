/**
 * @file symboltable.h
 *
 * @brief Mapping from symbols to arbitrary information, usually types. This is
 * used for both variable types in a function body, and function and other types
 * at the top level.
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __SYMBOLTABLE_H
#define __SYMBOLTABLE_H

#include <defs.h>

// TODO: It is not clear that this is more useful than a simple map
// TODO: Might want this to be a utility

template<typename T>
class SymbolTable {
private:

    std::unordered_map<std::string, T> table;

public:

    SymbolTable() {
    }

    void set(std::string symbol, T val) {
        table[symbol] = val;
    }

    T get(std::string symbol) {
        return table[symbol];
    }

    bool hasSymbol(std::string symbol) {
        return table.find(symbol) != table.end();
    }

};

#endif
