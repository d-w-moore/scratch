



// =-=-=-=-=-=-=-
// STL Includes
#include <iostream>
#include <string>
#include <functional>

// =-=-=-=-=-=-=-
// dlopen, etc
#include <dlfcn.h>

#include "operation_manager.hpp"

#include "keyValPair.hpp"

void init_rule_engine(
    const std::string& _rule_file,
    bp::object&        _rule_context ) {
    
    try {
        Py_Initialize();
    } catch( bp::error_already_set const& ) {
        PyErr_Print();
        return;
    }

    namespace bfs = boost::filesystem; 
    bfs::path workingDir = bfs::absolute("./").normalize();

    char path[] = { "path" };
    PyObject* sysPath = PySys_GetObject( path );
    PyList_Insert( sysPath, 0, PyString_FromString(workingDir.string().c_str()));

    _rule_context = bp::import( _rule_file.c_str() );

} // init_rule_engine

// =-=-=-=-=-=-=-
// symbol to be read from loaded library
std::string SYM_IN_EXEC( "i am a symbol in the main executable." );

// =-=-=-=-=-=-=-
// main
int main( int _argc, char* _argv[] ) {

    bp::object rule_context;
    init_rule_engine( "test_avro", rule_context );
    
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

        keyValPair_t kvp;
        memset(&kvp,0,sizeof(kvp));

        addKeyVal( &kvp, "KEY_1", "VAL_1" );
        addKeyVal( &kvp, "KEY_2", "VAL_2" );
        addKeyVal( &kvp, "KEY_3", "VAL_3" );
        addKeyVal( &kvp, "KEY_4", "VAL_4" );
        op_mgr.call<int,keyValPair_t,keyValPair_t,keyValPair_t>(
                rule_context,
                "serialize_function",
                std::move(kvp),
                std::move(kvp),
                std::move(kvp) );
        op_mgr.call<double>( rule_context, "library_function3");
        op_mgr.call( rule_context, "library_function");
    }

    // =-=-=-=-=-=-=-
    // close the library
    dlclose( handle );

    return 0;

} // main


