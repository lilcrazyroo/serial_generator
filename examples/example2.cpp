#include "serial_forward.h"

struct Header {
    std::string name;
};

struct Article {
    std::string name;
    std::vector< Article > refs;
};

SERIAL_TYPE_INFO( Header )
SERIAL_TYPE_INFO( Article )

int main() {
    Article article { "Article1", {
        Article{ "Article2" },
        Article{ "Article3" }
    } };

    using StringStorage = memserial::StringStorage< memserial::BigEndian >;
    std::string bytes = memserial::serialize< StringStorage >( article );

    using StringWrapper = memserial::StringWrapper< memserial::BigEndian >;
    memserial::print( memserial::parse< Header >( StringWrapper{ bytes } ), std::cout );

    std::cout << '\n';
    memserial::trace( StringWrapper{ bytes }, std::cout );
}
