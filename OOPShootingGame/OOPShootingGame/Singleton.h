#pragma once

template < typename T >
class Singleton
{
protected:
    Singleton() { }
    Singleton(Singleton&) {}
    virtual ~Singleton() {}
    Singleton operator=(Singleton&) {}

public:
    static T* GetInstance()
    {
        if (instance == nullptr)
            instance = new T;

        return instance;
    };

    static void DestroyInstance()
    {
        if (instance)
        {
            delete instance;
            instance = nullptr;
        }
    };

private:
    static inline T* instance;
};
