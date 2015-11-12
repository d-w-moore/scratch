


#ifndef KEY_VAL_PAIR_HPP
#define KEY_VAL_PAIR_HPP

#include "avro_dictionary.hpp"

/* keyValPair_t - str key, str value pair */

typedef struct KeyValPair {
    int len;
    char **keyWord;     /* array of keywords */
    char **value;       /* pointer to an array of values */
} keyValPair_t;

static int
clearKeyVal( keyValPair_t *condInput ) {

    if ( condInput == NULL || condInput->len < 1 ) {
        return 0;
    }

    for ( int i = 0; i < condInput->len; i++ ) {
        if ( condInput->keyWord != NULL ) {
            free( condInput->keyWord[i] );
        }
        if ( condInput->value != NULL ) {
            free( condInput->value[i] );
        }
    }

    free( condInput->keyWord );
    free( condInput->value );
    memset( condInput, 0, sizeof( keyValPair_t ) );
    return 0;
}

#define PTR_ARRAY_MALLOC_LEN    10  /* the len to allocate each time */

static int
addKeyVal( keyValPair_t *condInput, const char *keyWord, const char *value ) {
    if ( condInput == NULL ) {
        return -1;
    }
    if ( condInput->keyWord == NULL || condInput->value == NULL ) {
        condInput->len = 0;
    }

    /* check if the keyword exists */
    for ( int i = 0; i < condInput->len; i++ ) {
        if ( condInput->keyWord[i] == NULL || strlen( condInput->keyWord[i] ) == 0 ) {
            free( condInput->keyWord[i] );
            free( condInput->value[i] );
            condInput->keyWord[i] = strdup( keyWord );
            condInput->value[i] = value ? strdup( value ) : NULL;
            return 0;
        }
        else if ( strcmp( keyWord, condInput->keyWord[i] ) == 0 ) {
            free( condInput->value[i] );
            condInput->value[i] = value ? strdup( value ) : NULL;
            return 0;
        }
    }

    if ( ( condInput->len % PTR_ARRAY_MALLOC_LEN ) == 0 ) {
        condInput->keyWord = ( char ** )realloc( condInput->keyWord,
                             ( condInput->len + PTR_ARRAY_MALLOC_LEN ) * sizeof( *condInput->keyWord ) );
        condInput->value = ( char ** )realloc( condInput->value,
                                               ( condInput->len + PTR_ARRAY_MALLOC_LEN ) * sizeof( *condInput->value ) );
        memset( condInput->keyWord + condInput->len, 0, PTR_ARRAY_MALLOC_LEN * sizeof( *condInput->keyWord ) );
        memset( condInput->value + condInput->len, 0, PTR_ARRAY_MALLOC_LEN * sizeof( *condInput->value ) );
    }

    condInput->keyWord[condInput->len] = strdup( keyWord );
    condInput->value[condInput->len] = value ? strdup( value ) : NULL;
    condInput->len++;

    return 0;
}



typedef boost::shared_ptr< std::vector< uint8_t > > serialized_t;
serialized_t serialize( keyValPair_t&& _kvp ) {
    dictionary d;
    d.len = _kvp.len;

    for( size_t i = 0; i < d.len; ++i ) {
        if( _kvp.keyWord[i] && _kvp.value[i] ) {
            d.keyVal[ _kvp.keyWord[ i ] ] = _kvp.value[ i ];
        } else {
        }
    }

    std::auto_ptr< avro::OutputStream > out = avro::memoryOutputStream();
    avro::EncoderPtr e = avro::binaryEncoder();
    e->init( *out );
    avro::encode( *e, d );
    return avro::snapshot( *out );

} // serialize keyValPair_t



#endif // KEY_VAL_PAIR_HPP



