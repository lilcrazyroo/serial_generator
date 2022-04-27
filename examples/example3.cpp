#include "serialization.h"
#include "serial_trace.h"
#include "serial_exception.h"
#include "serial_precompile.h"

int main() {
    Article article { "Article1", {
        Article{ "Article2" },
        Article{ "Article3" }
    } };

    try {
        std::string bytes = memserial::serialize( article );
        memserial::print( memserial::parse< Article >( bytes ), std::cout );
    }
    catch ( memserial::SerialException& ex ) {
        std::cout << ex.what();
    }
}
