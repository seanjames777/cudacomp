/**
 * @file exceptions.h
 *
 * @brief Static analysis exceptions
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __STATICS_EXCEPTIONS_H
#define __STATICS_EXCEPTIONS_H

#include <defs.h>

namespace Statics {

/**
 * @brief Base class for all statics exceptions
 */
class StaticsException : public std::exception {
private:

    std::string msg;

protected:

    /**
     * @brief Set the 'what' message
     */
    void setMsg(std::string msg);

public:

    /**
     * @brief Constructor, intializes message to empty string
     */
    StaticsException();

    /**
     * @brief Constructor, initializes message
     */
    StaticsException(std::string msg);

    /**
     * @brief Get a message describing the exception
     */
    virtual const char *what() const noexcept override;
};

/**
 * @brief A non-void function was missing a return statement along a control
 * flow path.
 */
class NoReturnException : public StaticsException {
public:
    NoReturnException();
};

/**
 * @brief A function returned a value of the wrong type, or a void function
 * returned a value.
 */
class IllegalReturnTypeException : public StaticsException {
public:
    IllegalReturnTypeException();
};

/**
 * @brief A variable was used before being defined
 */
class UndefinedIdentifierException : public StaticsException {
public:
    UndefinedIdentifierException(std::string id);
};

/**
 * @brief A variable was used before being declared
 */
class UndeclaredIdentifierException : public StaticsException {
public:
    UndeclaredIdentifierException(std::string id);
};

/**
 * @brief A variable was declared more than once
 */
class RedeclaredIdentifierException : public StaticsException {
public:
    RedeclaredIdentifierException(std::string id);
};

/**
 * @brief A function was called but never defined
 */
class UndefinedFunctionException : public StaticsException {
public:
    UndefinedFunctionException(std::string id);
};

/**
 * @brief A function was called before being declared
 */
class UndeclaredFunctionException : public StaticsException {
public:
    UndeclaredFunctionException(std::string id);
};

/**
 * @brief A function was defined more than once
 */
class RedefinedFunctionException : public StaticsException {
public:
    RedefinedFunctionException(std::string id);
};

/**
 * @brief A type was used before being declared
 */
class UndeclaredTypeException : public StaticsException {
public:
    UndeclaredTypeException(std::string id);
};

/**
 * @brief A type was declared more than once
 */
class RedeclaredTypeException : public StaticsException {
public:
    RedeclaredTypeException(std::string id);
};

/**
 * @brief An operation was performed on an illegal type
 */
class IllegalTypeException : public StaticsException {
public:
    IllegalTypeException();
};

/**
 * @brief A function was declared more than once with different signatures
 */
class IncorrectSignatureException : public StaticsException {
public:
    IncorrectSignatureException(std::string id);
};

/**
 * @brief A function was declared more than once with different linkage
 */
class IncorrectLinkageException : public StaticsException {
public:
    IncorrectLinkageException(std::string id);
};

/**
 * @brief An external function was defined
 */
class ExternalFunctionDefinedException : public StaticsException {
public:
    ExternalFunctionDefinedException(std::string id);
};

/**
 * @brief Attempted to assign to an illegal lvalue
 */
class IllegalLValueException : public StaticsException {
public:
    IllegalLValueException();
};

/**
 * @brief Attempted to assign to an illegal lvalue
 */
class InvalidEntrypointException : public StaticsException {
public:
    InvalidEntrypointException();
};

}

#endif
