#include <assert.h>
#include <iostream>
#include <stdio.h>
#include <io.h>
#include <type_traits>

#include "runtime.h"

using namespace Dimitory;

class AATest
{

public:
    AATest( int g ) : intValue( g ), floatValue( 1337 ) {}
    AATest( float g ) : intValue( 1337 ), floatValue( g ) {}
    ~AATest() {}

	int getInteger() const { return intValue; }
    float getFloat() const { return floatValue; }

    void setInteger(int value) { intValue = value; }

    int square( int x ) const { return x*x; }

protected:
    int intValue;
    float floatValue;
};

class ABTest : private AATest
{
public:
    ABTest() : AATest( 1 ) {}
    ABTest( const ABTest& source ) : AATest( source.intValue ) {}
    void foo() const { std::cout << "ABTest::foo()" << std::endl; }
};

struct ACTest
{
    int value;
};

REGISTER_TYPE( int );
REGISTER_TYPE( int64_t );
REGISTER_TYPE( float );
REGISTER_TYPE( double );
REGISTER_TYPE( void );
REGISTER_TYPE( bool );
REGISTER_TYPE( String );
REGISTER_TYPE( AATest );
REGISTER_TYPE( ABTest );

REGISTER_CONSTRUCTOR( AATest, int );
REGISTER_CONSTRUCTOR( AATest, float );
REGISTER_BASECLASSE( ABTest, AATest );

int main()
{
    REGISTER_PROPERTY( AATest, "integer", getInteger, setInteger );
    REGISTER_READONLY_PROPERTY( AATest, "float", getFloat );
    REGISTER_METOD( AATest, "square", square );
    REGISTER_METOD( ABTest, "foo", foo );

    auto obj = Object( 1 );
    assert( obj.getType() == typeof( int ) );
    assert( obj == 1 );
    assert( obj != 1.f );
    assert( obj != nullptr );

    obj = typeof( AATest ).create( "invalidValue" );
    assert( !obj.getType().isValid() );
    assert( obj == nullptr );

    obj = typeof( AATest ).create( 1 );
    assert( obj.getType() == typeof( AATest ) );
    assert( obj != nullptr );

    auto& aaTest = obj.getValue<AATest>();
    assert( aaTest.getInteger() == 1 );

    obj = typeof( AATest ).create( 1.f );
    assert( obj.getType() == typeof( AATest ) );
    assert( obj != nullptr );

    aaTest = obj.getValue<AATest>();
    assert( aaTest.getFloat() == 1.f );

    auto obj_template = Type::createObj<AATest>( 1.f );
    assert( obj_template.getType() == typeof( AATest ) );
    assert( obj_template != nullptr );

    auto copy = obj;
    assert( obj != nullptr );
    assert( copy != nullptr );

    auto aaType = typeof( AATest );
    auto field = aaType.getField( "integer" );
    assert( field.isValid() && !field.isReadOnly() );
    assert( field.getValue( copy ) == 1337 );

    auto readOnlyField = aaType.getField( "float" );
    assert( readOnlyField.isValid() && readOnlyField.isReadOnly() );
    assert( readOnlyField.getValue( copy ) == 1.f );
    
    auto abType = typeof( ABTest );
    assert( abType.isDerivedFrom( aaType ) );
    assert( !aaType.isDerivedFrom( abType ) );

    auto method = aaType.getMethod( "square" );
    assert( method.isValid() && method.isConst() );

    auto ret = method.invoke( obj, Arguments{ 5 } );
    assert( ret == 25 );

    method = abType.getMethod( "foo" );
    assert( method.isValid() && method.isConst() );
    method.invoke( obj, Arguments{} );

    return 0;
}

