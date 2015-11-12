


#ifndef DUMMY_TYPE_HPP
#define DUMMY_TYPE_HPP

#include<string>

class dummy_type {
    public:

        dummy_type() : 
            int_val( 1234 ),
            float_val( 0.1234 ),
            string_val( "[this is the dummy type]" ) {}
    private:
        int int_val;
        float float_val;
        string string_val;

}; // class dummy_type

typedef boost::shared_ptr< std::vector< uint8_t > > serialized_t;
serialized_t serialize( dummy_type&& _d ) {
    dummy d;

    d.int_val = _d.int_val;
    d.float_val = _d.float_val;
    d.string_val = _d.string_val;

    std::auto_ptr< avro::OutputStream > out = avro::memoryOutputStream();
    avro::EncoderPtr e = avro::binaryEncoder();
    e->init( *out );
    avro::encode( *e, d );
    return avro::snapshot( *out );

} // serialize dummy_type




#endif // DUMMY_TYPE_HPP


