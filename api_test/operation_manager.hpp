

#ifndef OPERATION_MANAGER_HPP
#define OPERATION_MANAGER_HPP

#include <map>
#include <string>
#include <functional>

#include <boost/any.hpp>
#include <boost/filesystem.hpp>
#include <boost/python/import.hpp>
#include <boost/python/raw_function.hpp>
#include <boost/python/slice.hpp>
#include <boost/shared_ptr.hpp>

#include "operation_caller.hpp"

namespace bp = boost::python;
typedef boost::shared_ptr< std::vector< uint8_t > > serialized_t;

void call_rule_engine(
    bp::object&        _rc,
    const std::string& _name,
    std::vector< serialized_t >& _data ) {

    bp::list rule_arguments;
    for( size_t i = 0; i < _data.size(); ++i ) {
        PyObject* py_buf = PyBuffer_FromReadWriteMemory(
                               &_data[i]->front(),
                               _data[i]->size() * sizeof( uint8_t ) );
        rule_arguments.append( bp::object(bp::handle<>(py_buf)) );

    }

    bp::object rf = _rc.attr(_name.c_str());
    rf( rule_arguments );
}

void call_rule_engine(
    bp::object&        _rc,
    const std::string& _name ) {
    bp::object rf = _rc.attr(_name.c_str());
    rf( );
}

template< typename T0 >
void pack_serialized_parameters(
    std::vector< serialized_t >& _data,
    T0                           _t0 ) {
    _data.push_back( serialize( std::forward<T0>(_t0) ) );
}

template< typename T0, typename... types_t >
void pack_serialized_parameters(
    std::vector< serialized_t >& _data,
    T0                           _t0,
    types_t...                   _tn ) {

    _data.push_back( serialize( std::forward<T0>(_t0) ) );
    pack_serialized_parameters( _data, std::forward<types_t>(_tn)... );
}

class operation_manager {
    public:
    template< typename ret_t, typename... types_t>
    void add_operation(
        const std::string&                  _name,
        operation_caller<ret_t, types_t...> _operation ) {
        op_map_[ _name ] = _operation;
    }

    void add_operation(
        const std::string&           _name,
        operation_caller<void,void>  _operation ) {
        op_map_[ _name ] = _operation;
    }

    template< typename ret_t, typename... types_t>
    ret_t call(
        bp::object& _rc,
        const std::string& _name,
        types_t&&...       _t ) {

        std::vector< serialized_t > data;
        pack_serialized_parameters(
            data,
            std::forward<types_t>(_t)... );

        call_rule_engine( _rc, _name, data );

        typedef operation_caller<ret_t, types_t...> caller_t;
        caller_t& fcn = boost::any_cast< caller_t& >( op_map_[ _name ] );
        return fcn.call( std::forward<types_t>(_t)...);
    }

    template< typename ret_t >
    ret_t call(
        bp::object& _rc,
        const std::string& _name ) {

        call_rule_engine( _rc, _name );

        typedef operation_caller<ret_t> caller_t;
        caller_t& fcn = boost::any_cast< caller_t& >( op_map_[ _name ] );
        return fcn.call( );
    }

    void call(
        bp::object& _rc,
        const std::string& _name ) {

        call_rule_engine( _rc, _name );

        typedef operation_caller<void,void> caller_t;
        caller_t& fcn = boost::any_cast< caller_t& >( op_map_[ _name ] );
        return fcn.call( );
    }

    private:
    std::map<std::string,boost::any> op_map_;

}; // class operation_manager

#endif // OPERATION_MANAGER_HPP



