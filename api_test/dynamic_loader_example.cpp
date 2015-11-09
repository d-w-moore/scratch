

// =-=-=-=-=-=-=-
// STL Includes
#include <iostream>
#include <string>
#include "operation_manager.hpp"


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


extern "C"
void factory_function( operation_manager& _mgr ) {

    _mgr.add_operation(
        "fcn1",
        operation_caller<void,void>( library_function  ) );
    _mgr.add_operation<double,int,float>(
        "fcn2",
        operation_caller<double,int,float>(library_function2));
    _mgr.add_operation<double>(
        "fcn3",
        operation_caller<double>(library_function3));

}


