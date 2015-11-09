



// =-=-=-=-=-=-=-
// STL Includes
#include <iostream>
#include <string>
#include <functional>

// =-=-=-=-=-=-=-
// dlopen, etc
#include <dlfcn.h>

#include "operation_manager.hpp"

// =-=-=-=-=-=-=-
// symbol to be read from loaded library
std::string SYM_IN_EXEC( "i am a symbol in the main executable." );

// =-=-=-=-=-=-=-
// main
int main( int _argc, char* _argv[] ) {
    std::cout << "From Main [" << SYM_IN_EXEC << "]" << std::endl;
    // =-=-=-=-=-=-=-
    // load the dynamic library
    void* handle = dlopen( "./dynamic_loader_example.so", RTLD_LAZY );
    if( !handle ) {
        std::cout << "failed to load ./dynamic_loader_example.so[" << dlerror() << "]" << std::endl;
        return -1;
    }

    // =-=-=-=-=-=-=-
    // fcn ptr typdef
    typedef void (*factory_t)( operation_manager& );

    // =-=-=-=-=-=-=-
    // read a fcn ptr symbol from the library
    factory_t fac_ptr = reinterpret_cast< factory_t >( dlsym( handle, "factory_function" ) );
    if( !fac_ptr ) {
        std::cout << "failed to load the library_function [" << dlerror() << "]" << std::endl;
        return -1;
    }

    {   // scope block to release the references to the library functions
        // before the shared object is closed
        operation_manager op_mgr;
        fac_ptr( op_mgr );

        op_mgr.call<double,int,float>( "fcn2", 666, 777.888 );
        op_mgr.call( "fcn1" );
        op_mgr.call<double>( "fcn3" );
    }

    // =-=-=-=-=-=-=-
    // close the library
    dlclose( handle );

    return 0;

} // main


