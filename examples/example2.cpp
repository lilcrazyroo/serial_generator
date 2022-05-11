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

    std::string bytes = memserial::serialize( article );

    memserial::print( memserial::parse< Header >( bytes ), std::cout );
    std::cout << '\n';

    memserial::trace( bytes, std::cout );
}
