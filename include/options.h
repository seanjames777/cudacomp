/**
 * @file options.h
 *
 * @brief Compiler command line arguments and settings
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __OPTIONS_H
#define __OPTIONS_H

#include <defs.h>

/**
 * @brief Compiler command line options
 */
struct CCArgs {
    bool                     emit_device;   //!< Whether to emit PTX code
    bool                     print_ast;     //!< Whether to print the AST
    std::string              in_file;       //!< Input file, possibly null to indicate stdin
    std::string              out_hfile;     //!< Output host code file
    std::string              out_dfile;     //!< Output device code file
    bool                     verbose;       //!< Whether to print verbose output
    std::string              symbol_prefix; //!< String to prefix intern function names with
    bool                     mem_safe;      //!< Whether to insert memory safety checks
    bool                     opr_safe;      //!< Whether to insert operator safety checks
    bool                     emit_text;     //!< Whether to emit LLVM IR as text
    std::string              entrypoint;    //!< Required entrypoint name
    std::vector<std::string> headers;       //!< List of headers
    bool                     no_float;      //!< Disable floating point
};

/**
 * @brief Get the options struct. This is not valid until parseArgs() is called.
 */
struct CCArgs *getOptions();

/**
 * @brief Print help message
 *
 * @param[in] argv Command line arguments
 */
void printHelp(char *argv[]);

/**
 * @brief Parse command line arguments. May terminate the program.
 *
 * @param[in] argc Number of command line arguments
 * @param[in] argv Command line arguments
 */
void parseArgs(int argc, char *argv[]);

#endif
