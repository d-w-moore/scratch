
import io
import avro.schema
import avro.io

def serialize_function(args):
    print "hello from the python serialize_function"
    schema = avro.schema.parse(open("avro_dictionary.json").read())
    for buf in args:
        bytes_reader = io.BytesIO( buf )
        decoder = avro.io.BinaryDecoder(bytes_reader)
        reader = avro.io.DatumReader(schema)
        d = reader.read(decoder)
        print d

    print "done with the python serialize_function"

def library_function3():
    print "hello from the python library_function3"

def library_function():
    print "hello from the python library_function"
