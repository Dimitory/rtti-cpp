# Dimitory-rtti-cpp
User friendly cpp rtti engine.
Actually work in progress.

# Usage
## Create instance
```cpp
class AATest
{

public:
    AATest() : intValue( 1337 ) {}
    AATest( int value ) : intValue( value ) {}
    ~AATest() {}
    
    int getInteger() const { return intValue; }
protected:
    int intValue;
};

REGISTER_TYPE( AATest );
REGISTER_CONSTRUCTOR( AATest, int );

int main()
{
  auto aaTestObj = typeof( AATest ).create();
  auto aaTestValue = aaTestObj.getValue<AATest>();
  auto value = aaTestValue->getInteger();
  
  // snd way
  auto aaTestObj = Type::createObj<AATest>();
  auto value = aaTestObj->getInteger();
  
  // non-default constructor
  auto aaTestObj = Type::createObj<AATest>( 1337 );
  auto value = aaTestObj->getInteger();
}

```

## Invoke method
```cpp
class ABTest
{
public:
    ABTest() {}
    int square( int x ) const { return x*x; }
    void foo() const { std::cout << "ABTest::foo()" << std::endl; }
};

int main()
{
    REGISTER_METOD( ABTest, "square", square );
    REGISTER_METOD( ABTest, "foo", foo );

    auto abType = typeof( ABTest );
    auto obj = abType.create( 1.f );
    auto method = abType.getMethod( "foo" );
    method.invoke( obj, Arguments{} );
    
    method = abType.getMethod( "square" );
    auto ret = method.invoke( obj, Arguments{ 5 } );
}
```

## Field
```cpp
class AATest
{

public:
    AATest() : intValue( 1337 ), floatValue( 1337 ) {}
    ~AATest() {}

    int getInteger() const { return intValue; }
    float getFloat() const { return floatValue; }

    void setInteger(int value) { intValue = value; }
protected:
    int intValue;
    float floatValue;
};

int main()
{
    REGISTER_PROPERTY( AATest, "integer", getInteger, setInteger );
    REGISTER_READONLY_PROPERTY( AATest, "float", getFloat );

    auto obj = typeof( AATest ).create();
    auto aaType = typeof( AATest );
    auto field = aaType.getField( "integer" );
    auto integerValue = field.getValue( obj );

    auto readOnlyField = aaType.getField( "float" );
    auto floatValue = readOnlyField.getValue( obj );
}
```
