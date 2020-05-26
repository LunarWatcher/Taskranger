#ifndef TASKRANGER_EXCEPTIONS_EXCEPTIONS_HPP
#define TASKRANGER_EXCEPTIONS_EXCEPTIONS_HPP

#include <stdexcept>
#include <string>

#define DeclareException(className) \
    class className : public std::runtime_error { \
    private: \
        const char* msg; \
\
    public: \
        className(const char* msg) : std::runtime_error(msg), msg(msg) {} \
        const char* what() const noexcept override { \
            return msg; \
        } \
    };

namespace taskranger {

DeclareException(PermissionError)

}

#endif
