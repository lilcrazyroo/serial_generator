#include "serial_forward.h"

#include <ctime>

struct Head {
    uint64_t size;
    uint64_t type;
};

struct [[gnu::packed]] Meta {
#if __BYTE_ORDER == __LITTLE_ENDIAN
    int32_t v0 : 7;
    int32_t v1 : 28;
    int32_t v2 : 29;
#elif __BYTE_ORDER == __BIG_ENDIAN
    int32_t v2 : 29;
    int32_t v1 : 28;
    int32_t v0 : 7;
#endif
};

std::ostream& operator<<( std::ostream& out, const Meta& meta ) {
    return out << meta.v0 << " " << meta.v1 << " " << meta.v2;
}

struct Payload {
    std::chrono::system_clock::time_point time;
    std::vector< std::complex< double > > quads;

    char* ctime() const {
        std::time_t tp = std::chrono::system_clock::to_time_t( time );
        return std::ctime( &tp );
    }
};

std::ostream& operator<<( std::ostream& out, const Payload& payload ) {
    out << payload.ctime();
    memserial::print( payload, out );
    return out;
}

struct Package {
    Head m_head;
    std::array< char, sizeof( Meta ) > m_meta;
    std::string m_payload;

    Head& head() {
        return m_head;
    }

    Meta& meta() {
        return reinterpret_cast< Meta& >( *m_meta.data() );
    }

    Payload payload() {
        using Endian = memserial::StringWrapper< memserial::BigEndian >;
        return memserial::parse< Payload >( static_cast< Endian& >( m_payload ) );
    }
};

SERIAL_TYPE_INFO( Package )
SERIAL_TYPE_INFO( Payload )

int main() {
    char bytes[ 1024 ];

    {
        Payload payload;
        payload.time = std::chrono::system_clock::now();
        payload.quads = { { 0, 1 }, { 1, 0 }, { 1, 1 } };

        Package package;
        package.meta() = { 42, 69, 777 };
        package.m_payload = memserial::serialize< memserial::StringWrapper< memserial::BigEndian > >( payload );
        package.m_head.type = memserial::ident< Payload >();
        package.m_head.size = memserial::size( package );

        memserial::SerialView view( bytes, package.m_head.size );
        memserial::serialize( view, package );
    }

    try {
        Head head = memserial::parse< Head >( memserial::SerialView( bytes, memserial::size< Head >() ) );
        std::cout << "Type: " << memserial::alias( head.type ) << "\n";

        Package package = memserial::parse< Package >( memserial::SerialView( bytes, head.size ) );
        std::cout << "Meta: " << package.meta() << "\n";

        if ( head.type == memserial::ident< Payload >() )
            std::cout << package.payload() << "\n";
    }

    catch ( memserial::SerialException& ex ) {
        std::cerr << ex.what();
    }


}
