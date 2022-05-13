// Copyright 2019 Ilya Kiselev.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#if defined( SERIAL_FORWARD_HEADER )
#include SERIAL_FORWARD_HEADER
#endif

#include <fstream>

namespace memserial {
namespace detail {

#define GENERATED_PREFIX \
"// This file was generated automatically.\n" \
"// All changes will be overwritten.\n" \
"\n" \
"namespace memserial {\n" \
"namespace detail {\n" \
"\n" \
"#define GENERATED_STUB( Stub ) \\\n" \
"struct Stub {};\n" \
"\n" \
"#define GENERATED_HEAD( Head, Size ) \\\n" \
"template<> \\\n" \
"struct rebind_serial< Head, std::true_type > { \\\n" \
"    using ValueType = Head; \\\n" \
"    static constexpr std::size_t tuple_size = Size; \\\n" \
"    template< std::size_t Index > struct tuple_field; \\\n" \
"};\n" \
"\n" \
"#define GENERATED_FIELD( Head, Field, Index, Offset ) \\\n" \
"template<> \\\n" \
"struct rebind_serial< Head, std::true_type >::tuple_field< Index > { \\\n" \
"    using FieldType = Field; \\\n" \
"    static constexpr Field& get( Head& head ) { \\\n" \
"        char* field = reinterpret_cast< char* >( &head ) + Offset; \\\n" \
"        return reinterpret_cast< Field& >( *field ); \\\n" \
"    } \\\n" \
"};\n" \

#define GENERATED_STUBS \
"\n" \
"// --- Generated stub types\n"

#define GENERATED_HEADS \
"\n" \
"// --- Generated head bindings\n"

#define GENERATED_FIELDS \
"\n" \
"// --- Generated field bindings\n"

#define GENERATED_SUFFIX \
"\n" \
"}} // --- namespace\n"

/**
 *
 */
template< typename T >
using is_alias = std::integral_constant< bool,
        std::is_same< T, string >::value ||
        std::is_same< T, wstring >::value ||
        std::is_same< T, u16string >::value ||
        std::is_same< T, u32string >::value ||
        std::is_same< T, system_clock >::value ||
        std::is_same< T, steady_clock >::value ||
        std::is_same< T, nanoseconds >::value ||
        std::is_same< T, microseconds >::value ||
        std::is_same< T, milliseconds >::value ||
        std::is_same< T, seconds >::value ||
        std::is_same< T, minutes >::value ||
        std::is_same< T, hours >::value ||
        std::is_same< T, atto >::value ||
        std::is_same< T, femto >::value ||
        std::is_same< T, pico >::value ||
        std::is_same< T, nano >::value ||
        std::is_same< T, micro >::value ||
        std::is_same< T, milli >::value ||
        std::is_same< T, centi >::value ||
        std::is_same< T, deci >::value ||
        std::is_same< T, deca >::value ||
        std::is_same< T, hecto >::value ||
        std::is_same< T, kilo >::value ||
        std::is_same< T, mega >::value ||
        std::is_same< T, giga >::value ||
        std::is_same< T, tera >::value ||
        std::is_same< T, peta >::value ||
        std::is_same< T, exa >::value >;

template< typename T >
using is_head = std::integral_constant< bool,
        is_serial< T >::value && SerialMetatype< T >::serial() >;

template< typename T >
using is_stub = std::integral_constant< bool,
        is_serial< T >::value && !SerialMetatype< T >::serial() >;

template< typename T >
using is_true = std::integral_constant< bool, !is_alias< T >::value >;

/**
 *
 */
template< typename Type, typename I = std::true_type >
struct SerialData;

/**
 *
 */
struct GeneratorData {
    std::vector< uint64_t > hashes;
    std::vector< std::string > stubs;
    std::vector< std::string > heads;
    std::vector< std::string > fields;

    static GeneratorData create() {
        return {};
    }

    bool recursion( uint64_t hash ) {
        if ( std::find( begin( hashes ), end( hashes ), hash ) != std::end( hashes ) )
            return true;
        hashes.push_back( hash );
        return false;
    }

    void head( const std::string& head, std::size_t size ) {
        std::string generated;
        generated += "GENERATED_HEAD( PACK( ";
        generated += head;
        generated += " ), ";
        generated += std::to_string( size );
        generated += " )\n";
        heads.push_back( generated );
    }

    void stub( const std::string& stub ) {
        std::string generated;
        generated += "GENERATED_STUB( ";
        generated += stub;
        generated += " )\n";
        stubs.push_back( generated );
    }

    void field( const std::string& head, const std::string& field,
            std::size_t index, std::size_t offset ) {
        std::string generated;
        generated += "GENERATED_FIELD( PACK( ";
        generated += head;
        generated += " ), PACK( ";
        generated += field;
        generated += " ), ";
        generated += std::to_string( index );
        generated += ", ";
        generated += std::to_string( offset );
        generated += " )\n";
        fields.push_back( generated );
    }

    template< typename Type >
    void generate( SerialData< Type >& data );
};

/**
 *
 */
template< typename Type >
struct SerialData< Type, is_alias< Type > > {
    using Self = SerialData< Type, is_alias< Type > >;

    Type& ref;
    std::string name;

    static Self create( Type& ref ) {
        return { ref, alias() };
    }

    static std::string alias() {
        return serial_alias< Type >();
    }

    template< typename HeadType >
    void generate( GeneratorData& generator, SerialData< HeadType >& head ) {}
};

/**
 *
 */
template< typename Type >
struct SerialData< Type, is_head< Type > > {
    using Self = SerialData< Type, is_head< Type > >;

    Type& ref;
    std::string name;

    static Self create( Type& ref ) {
        return { ref, alias() };
    }

    static std::string alias() {
        return serial_alias< Type >();
    }

    template< typename HeadType >
    void generate( GeneratorData& generator, SerialData< HeadType >& head ) {
        if ( !generator.recursion( serial_hash< Type >() ) ) {
            generator.generate( *this );
        }
    }
};

/**
 *
 */
template< typename Type >
struct SerialData< Type, is_stub< Type > > {
    using Self = SerialData< Type, is_stub< Type > >;

    Type& ref;
    std::string name;

    static Self create( Type& ref ) {
        return { ref, alias() };
    }

    static std::string alias() {
        uint64_t hash = SerialType< Type >::hash();
        return "SerialStub" + std::to_string( hash );
    }

    template< typename HeadType >
    void generate( GeneratorData& generator, SerialData< HeadType >& head ) {
        if ( !generator.recursion( SerialType< Type >::hash() ) ) {
            generator.stub( name );
            generator.generate( *this );
        }
    }
};

/**
 *
 */
template< typename Type >
struct SerialData< Type, is_primitive< Type > > {
    using Self = SerialData< Type, is_primitive< Type > >;
    using InternalType = typename rebind_primitive< Type >::InternalType;

    Type& ref;
    std::string name;

    static Self create( Type& ref ) {
        return { ref, alias() };
    }

    static std::string alias() {
        return serial_alias< InternalType >();
    }

    template< typename HeadType >
    void generate( GeneratorData& generator, SerialData< HeadType >& head ) {}
};

/**
 *
 */
template< typename ... Args >
struct SerialData< basic_string< Args... >, is_true< basic_string< Args... > > > {
    using Self = SerialData< basic_string< Args... >, is_true< basic_string< Args... > > >;
    using Type = basic_string< Args... >;
    using DataType = typename Type::value_type;

    Type& ref;
    std::string name;

    static Self create( Type& ref ) {
        return { ref, alias() };
    }

    static std::string alias() {
        return serial_alias< Type >() + "< " + SerialData< DataType >::alias() + " >";
    }

    template< typename HeadType >
    void generate( GeneratorData& generator, SerialData< HeadType >& head ) {}
};

/**
 *
 */
template< typename ... Args >
struct SerialData< vector< Args... >, is_true< vector< Args... > > > {
    using Self = SerialData< vector< Args... >, is_true< vector< Args... > > >;
    using Type = vector< Args... >;
    using DataType = typename Type::value_type;

    Type& ref;
    std::string name;

    static Self create( Type& ref ) {
        return { ref, alias() };
    }

    static std::string alias() {
        return serial_alias< Type >() + "< " + SerialData< DataType >::alias() + " >";
    }

    template< typename HeadType >
    void generate( GeneratorData& generator, SerialData< HeadType >& head ) {
        DataType data_ref{};
        auto serial_data = SerialData< DataType >::create( data_ref );
        serial_data.generate( generator, *this );
    }
};

/**
 *
 */
template< typename ... Args >
struct SerialData< tuple< Args... >, is_true< tuple< Args... > > > {
    using Self = SerialData< tuple< Args... >, is_true< tuple< Args... > > >;
    using Type = tuple< Args... >;

    template< std::size_t Index >
    using DataType = typename std::tuple_element< Index, Type >::type;
    static constexpr std::size_t tuple_size = std::tuple_size< Type >::value;

    Type& ref;
    std::string name;

    static Self create( Type& ref ) {
        return { ref, alias() };
    }

    static std::string alias() {
        AliasFunctor functor{ serial_alias< Type >() + "< " };
        foreach_sequence( functor, size_t_< 0 >{}, size_t_< tuple_size - 1 >{} );
        return functor.alias + SerialData< DataType< tuple_size - 1 > >::alias() + " >";
    }

    template< typename HeadType >
    void generate( GeneratorData& generator, SerialData< HeadType >& head ){
        GenerationFunctor functor{ generator, *this };
        foreach_sequence( functor, size_t_< 0 >{}, size_t_< tuple_size >{} );
    }

    struct AliasFunctor {
        std::string alias;

        template< std::size_t Index >
        constexpr void operator()( size_t_< Index > ) {
            alias += SerialData< DataType< Index > >::alias() + ", ";
        }
    };

    struct GenerationFunctor {
        GeneratorData& generator;
        Self& head;

        template< std::size_t Index >
        constexpr void operator()( size_t_< Index > ) {
            DataType< Index > data_ref{};
            auto serial_data = SerialData< DataType< Index > >::create( data_ref );
            serial_data.generate( generator, head );
        }
    };
};

/**
 *
 */
template< typename Arg, std::size_t Dim >
struct SerialData< array< Arg, Dim >, is_true< array< Arg, Dim > > > {
    using Self = SerialData< array< Arg, Dim >, is_true< array< Arg, Dim > > >;
    using Type = array< Arg, Dim >;
    using DataType = typename Type::value_type;

    Type& ref;
    std::string name;

    static Self create( Type& ref ) {
        return { ref, alias() };
    }

    static std::string alias() {
        return serial_alias< Type >() + "< " +
                SerialData< DataType >::alias() + ", " + std::to_string( Dim ) + " >";
    }

    template< typename HeadType >
    void generate( GeneratorData& generator, SerialData< HeadType >& head ) {
        DataType data_ref{};
        auto serial_data = SerialData< DataType >::create( data_ref );
        serial_data.generate( generator, *this );
    }
};

/**
 *
 */
template< std::size_t Bits >
struct SerialData< bitset< Bits >, is_true< bitset< Bits > > > {
    using Self = SerialData< bitset< Bits >, is_true< bitset< Bits > > >;
    using Type = bitset< Bits >;

    Type& ref;
    std::string name;

    static Self create( Type& ref ) {
        return { ref, alias() };
    }

    static std::string alias() {
        return serial_alias< Type >() + "< " + std::to_string( Bits ) + " >";
    }

    template< typename HeadType >
    void generate( GeneratorData& generator, SerialData< HeadType >& head ){}
};

/**
 *
 */
template< typename Clock, typename Duration >
struct SerialData< time_point< Clock, Duration >, is_true< time_point< Clock, Duration > > > {
    using Self = SerialData< time_point< Clock, Duration >, is_true< time_point< Clock, Duration > > >;
    using Type = time_point< Clock, Duration >;
    using DataType = typename Type::duration;
    using ClockType = typename Type::clock;

    Type& ref;
    std::string name;

    static Self create( Type& ref ) {
        return { ref, alias() };
    }

    static std::string alias() {
        return serial_alias< Type >() + "< " +
                SerialData< ClockType >::alias() + ", " + SerialData< DataType >::alias() + " >";
    }

    template< typename HeadType >
    void generate( GeneratorData& generator, SerialData< HeadType >& head ) {
        DataType data_ref{};
        auto serial_data = SerialData< DataType >::create( data_ref );
        serial_data.generate( generator, *this );
    }
};

/**
 *
 */
template< typename Rep, typename Period >
struct SerialData< duration< Rep, Period >, is_true< duration< Rep, Period > > > {
    using Self = SerialData< duration< Rep, Period >, is_true< duration< Rep, Period > > >;
    using Type = duration< Rep, Period >;
    using DataType = typename Type::rep;
    using PeriodType = typename Type::period;

    Type& ref;
    std::string name;

    static Self create( Type& ref ) {
        return { ref, alias() };
    }

    static std::string alias() {
        return serial_alias< Type >() + "< " +
                SerialData< DataType >::alias() + ", " + SerialData< PeriodType >::alias() + " >";
    }

    template< typename HeadType >
    void generate( GeneratorData& generator, SerialData< HeadType >& head ) {
        DataType data_ref{};
        auto serial_data = SerialData< DataType >::create( data_ref );
        serial_data.generate( generator, *this );
    }
};

/**
 *
 */
template< std::intmax_t Num, std::intmax_t Den >
struct SerialData< ratio< Num, Den >, is_true< ratio< Num, Den > > > {
    using Self = SerialData< ratio< Num, Den >, is_true< ratio< Num, Den > > >;
    using Type = ratio< Num, Den >;

    Type& ref;
    std::string name;

    static Self create( Type& ref ) {
        return { ref, alias() };
    }

    static std::string alias() {
        return serial_alias< Type >() + "< " +
                std::to_string( Num ) + ", " + std::to_string( Den ) + " >";
    }

    template< typename HeadType >
    void generate( GeneratorData& generator, SerialData< HeadType >& head ) {}
};

/**
 *
 */
template< typename Arg >
struct SerialData< complex< Arg >, is_true< complex< Arg > > > {
    using Self = SerialData< complex< Arg >, is_true< complex< Arg > > >;
    using Type = complex< Arg >;
    using DataType = typename Type::value_type;

    Type& ref;
    std::string name;

    static Self create( Type& ref ) {
        return { ref, alias() };
    }

    static std::string alias() {
        return serial_alias< Type >() + "< " + SerialData< DataType >::alias() + " >";
    }

    template< typename HeadType >
    void generate( GeneratorData& generator, SerialData< HeadType >& head ) {
        DataType data_ref{};
        auto serial_data = SerialData< DataType >::create( data_ref );
        serial_data.generate( generator, *this );
    }
};

/**
 *
 */
template< typename Type >
struct SequenceFunctor {
    GeneratorData& generator;
    SerialData< Type >& head_data;

    template< std::size_t Index >
    void operator()( size_t_< Index > );
};

/**
 *
 */
struct SerialFunctor {
    GeneratorData& generator;

    template< std::size_t Index >
    void operator()( size_t_< Index > );
};

/**
 *
 */
template< typename Type >
void GeneratorData::generate( SerialData< Type >& data ) {
    head( data.name, rebind_serial< Type >::tuple_size );
    SequenceFunctor< Type > functor{ *this, data };
    foreach_sequence( functor, size_t_< 0 >{}, size_t_< rebind_serial< Type >::tuple_size >{} );
}

/**
 *
 */
template< typename Type >
template< std::size_t Index >
void SequenceFunctor< Type >::operator()( size_t_< Index > ) {
    using tuple_field = typename rebind_serial< Type >::template tuple_field< Index >;
    using DataType = typename tuple_field::FieldType;
    DataType& data_ref = tuple_field::get( head_data.ref );
    auto serial_data = SerialData< DataType >::create( data_ref );
    generator.field( head_data.name, serial_data.name, Index, ( char* ) &serial_data.ref - ( char* ) &head_data.ref );
    serial_data.generate( generator, head_data );
}

/**
 *
 */
template< std::size_t Index >
void SerialFunctor::operator()( size_t_< Index > ) {
    using DataType = typename SerialIdentity< Index >::ValueType;
    if ( !generator.recursion( serial_hash< Type >() ) ) {
        DataType data_ref{};
        auto serial_data = SerialData< DataType >::create( data_ref );
        generator.generate( serial_data );
    }
}

template<>
void SerialFunctor::operator()( size_t_< 0 > ) {}

}} // --- namespace

/**
 *
 */
int main( int argc, char **argv ) {

    if ( argc != 2 )
        return 1;

    std::ofstream stream( argv[ 1 ] );
    if ( !stream.is_open() )
        return 1;

    std::string generated;
    {
        auto generator = memserial::detail::GeneratorData::create();
        memserial::detail::SerialFunctor functor{ generator };
        memserial::detail::foreach_serial( functor );

        generated += GENERATED_PREFIX;
        generated += GENERATED_STUBS;
        for ( const auto& stub : generator.stubs )
            generated += stub;
        generated += GENERATED_HEADS;
        for ( const auto& head : generator.heads )
            generated += head;
        generated += GENERATED_FIELDS;
        for ( const auto& field : generator.fields )
            generated += field;
        generated += GENERATED_SUFFIX;
    }

    stream.write( generated.data(), generated.size() );
    stream.close();
    return 0;
}
