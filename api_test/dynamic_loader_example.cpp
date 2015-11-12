

// =-=-=-=-=-=-=-
// STL Includes
#include <iostream>
#include <string>
#include "operation_manager.hpp"

#include "keyValPair.hpp"

extern std::string SYM_IN_EXEC;
extern "C" void library_function(void) {
    std::cout << "message from the the library_function [" << SYM_IN_EXEC << "]" << std::endl;
} // library_function

extern "C" double library_function2( int _a, float _b ) {
    std::cout << "message from the the library_function2 [" << SYM_IN_EXEC << "] and " << _a << ", " << _b << std::endl;
    return 2.3;
}

extern "C" double library_function3(  ) {
    std::cout << "message from the the library_function3 [" << SYM_IN_EXEC << std::endl;
    return 7.11;
}

int serialize_function( keyValPair_t _kvp, keyValPair_t _kvp2, keyValPair_t _kvp3 ) {
    std::cout << "message from the serialize_function" << std::endl;
    return 666;
}

extern "C"
void factory_function( operation_manager& _mgr ) {
    _mgr.add_operation<int,keyValPair_t,keyValPair_t,keyValPair_t>(
        "serialize_function",
        operation_caller<int,keyValPair_t,keyValPair_t,keyValPair_t>(serialize_function));
    _mgr.add_operation<double>(
        "library_function3",
        operation_caller<double>(library_function3));
    _mgr.add_operation<void,void>(
        "library_function",
        operation_caller<void,void>(library_function));

}


