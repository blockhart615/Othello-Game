#ifndef __OTHELLOEXCEPTIONS_H
#define __OTHELLOEXCEPTIONS_H
#include <string>
#include <stdexcept>

// Represents an exception thrown in an othello game. Generic exception class.
class OthelloException : public std::runtime_error {
public:
   // Constructs an exception object with the given message.
   OthelloException(const std::string &m) : std::runtime_error(m) {}
};

#endif