

#ifndef OPERATION_MANAGER_HPP
#define OPERATION_MANAGER_HPP

#include <map>
#include <string>
#include <functional>

#include <boost/any.hpp>

#include "operation_caller.hpp"


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
        const std::string& _name,
        types_t&&...       _t ) {
        typedef operation_caller<ret_t, types_t...> caller_t;
        caller_t& fcn = boost::any_cast< caller_t& >( op_map_[ _name ] );
        return fcn.call( std::forward<types_t>(_t)...);
    }

    template< typename ret_t >
    ret_t call(
        const std::string& _name ) {
        typedef operation_caller<ret_t> caller_t;
        caller_t& fcn = boost::any_cast< caller_t& >( op_map_[ _name ] );
        return fcn.call( );
    }

    void call(
        const std::string& _name ) {
        typedef operation_caller<void,void> caller_t;
        caller_t& fcn = boost::any_cast< caller_t& >( op_map_[ _name ] );
        return fcn.call( );
    }

    private:
    std::map<std::string,boost::any> op_map_;

}; // class operation_manager

#endif // OPERATION_MANAGER_HPP



