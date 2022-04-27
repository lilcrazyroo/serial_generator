// This file was generated automatically.
// All changes will be overwritten.

namespace memserial {
namespace detail {

#define GENERATED_STUB( Stub ) \
struct Stub {};

#define GENERATED_HEAD( Head, Size ) \
template<> \
struct rebind_serial< Head, std::true_type > { \
    using ValueType = Head; \
    static constexpr std::size_t tuple_size = Size; \
    template< std::size_t Index > struct tuple_field; \
};

#define GENERATED_FIELD( Head, Field, Index, Offset ) \
template<> \
struct rebind_serial< Head, std::true_type >::tuple_field< Index > { \
    using FieldType = Field; \
    static constexpr Field& get( Head& head ) { \
        char* field = reinterpret_cast< char* >( &head ) + Offset; \
        return reinterpret_cast< Field& >( *field ); \
    } \
};

// --- Generated stub types

// --- Generated head bindings
GENERATED_HEAD( PACK( Article ), 2 )

// --- Generated field bindings
GENERATED_FIELD( PACK( Article ), PACK( string ), 0, 0 )
GENERATED_FIELD( PACK( Article ), PACK( vector< Article > ), 1, 32 )

}} // --- namespace
