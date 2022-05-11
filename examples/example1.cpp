#include "serial_forward.h"

struct Article {
    std::string name;
    std::vector< Article > refs;
};

int main() {
    Article article { "Article1", {
        Article{ "Article2" },
        Article{ "Article3" }
    } };

    std::string bytes = memserial::serialize( article );
    Article value = memserial::parse< Article >( bytes );
    memserial::print( value, std::cout );
}
