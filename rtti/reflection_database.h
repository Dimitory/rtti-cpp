#pragma once

namespace Dimitory
{
    class ReflectionDatabase
    {
    public:
        ReflectionDatabase& operator=( const ReflectionDatabase& ) = delete;
        ReflectionDatabase& operator=( ReflectionDatabase&& ) = delete;
        ReflectionDatabase( ReflectionDatabase&& ) = delete;

        static ReflectionDatabase& getInstance()
        {
            static ReflectionDatabase instance;
            return instance;
        }

    private:
        template <class T>
        void registerReflection()
        {
            T::RTTI_Init();
        }

        ReflectionDatabase();
        ~ReflectionDatabase() = default;
    };
}