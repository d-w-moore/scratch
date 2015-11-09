

#ifndef OPERATION_CALLER_HPP
#define OPERATION_CALLER_HPP

#include <functional>


template< typename ret_t, typename... types_t>
class operation_caller {
    public:
    typedef std::function<ret_t( types_t... )> fcn_t;
    operation_caller( fcn_t _f ) : operation_(_f) {
    }
    ret_t call( types_t&& ..._t ) {
        return operation_( std::forward<types_t>(_t)...);
    }
    private:
    fcn_t operation_;
}; // class operation_caller

template< typename ret_t >
class operation_caller<ret_t,void> {
    public:
    typedef std::function<ret_t( void )> fcn_t;
    operation_caller( fcn_t _f ) : operation_(_f) {}
    ret_t call( void ) {
        return operation_( );
    }
    private: 
    fcn_t operation_;
}; // class operation_caller

template<>
class operation_caller<void,void> {
    public:
    typedef std::function<void( void )> fcn_t;
    operation_caller( fcn_t _f ) : operation_(_f) {}
    void call( void ) {
        operation_( );
    }
    private: 
    fcn_t operation_;
}; // class operation_caller

#endif // OPERATION_CALLER_HPP


