#include <iostream>
#include <stdexcept>

struct OJException : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

struct UserException : public OJException {
    using OJException::OJException;
};

struct SystemException : public OJException {
    using OJException::OJException;
};

signed main() {
    
    return 0;
}