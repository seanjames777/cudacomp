/**
 * @file symboltable.h
 *
 * @brief Symbol tables
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __SYMBOLTABLE_H
#define __SYMBOLTABLE_H

#include <defs.h>

/**
 * @brief A symbol, which represents a function, variable, etc.
 */
typedef std::string Symbol;

/**
 * @brief An ordered set of symbol names
 */
typedef std::set<Symbol> SymbolSet;

/**
 * @brief A map from symbols to arbitrary information, usually types. This is
 * used for both variable types in a function body, and function and other types
 * at the top level.
 *
 * @tparam T Type to map symbols to
 */
template<typename T>
class SymbolTable {
private:

    std::unordered_map<Symbol, T> table;

public:

    /**
     * @brief Iterator type
     */
    typedef typename std::unordered_map<Symbol, T>::iterator iterator;

    /**
     * @brief Constructor
     */
    SymbolTable() {
    }

    /**
     * @brief Set the value a symbol maps to
     *
     * @param[in] symbol Symbol to map from
     * @param[in] val    Symbol to map to
     */
    void set(Symbol symbol, T val) {
        table[symbol] = val;
    }

    /**
     * @brief Get the value a symbol is mapped to, if it exists
     *
     * @param[in] symbol Symbol mapped from
     *
     * @return Value mapped to
     */
    T get(Symbol symbol) {
        return table[symbol];
    }

    /**
     * @brief Check if the symbol table contains a mapping for a symbol
     *
     * @param[in] symbol Symbol mapped from
     *
     * @return Whether there is a mapping
     */
    bool hasSymbol(Symbol symbol) {
        return table.find(symbol) != table.end();
    }

    /**
     * @brief Remove all entries from the symbol table
     */
    void clear() {
        table.clear();
    }

    /**
     * @brief Get an iterator to the beginning of the table
     */
    iterator begin() {
        return table.begin();
    }

    /**
     * @brief Get an iterator to the end of the table
     */
    iterator end() {
        return table.end();
    }

    /**
     * @brief Erase an element from the table
     */
    void erase(iterator it) {
        table.erase(it);
    }

};

#endif
