#pragma once

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <iostream>
using namespace std;

class RO5
{
public:
    RO5(int num, std::string title)
        :str{ title }
    {
        data = new int;
        *data = num;
    }

    ~RO5()
    {
        if (data != nullptr)
        {
            delete data;
        }
    }

    RO5(const RO5& rhs)
    {
        *this = rhs;
    }

    RO5(RO5&& temporary) noexcept
    {
        *this = std::move(temporary);
    }

    RO5& operator=(const RO5& rhs)
    {
        str = rhs.str;
        *data = *rhs.data;

        return *this;
    }

    RO5& operator=(RO5&& temporary) noexcept
    {
        delete data;

        str = std::move(temporary.str);

        data = temporary.data;
        temporary.data = nullptr;

        return *this;
    }

    void print()
    {
        if (data != nullptr)
            std::cout << str << *data << "\n";
        else
            std::cout << "trash class : after using move \n";
    }

private:
    int* data;
    std::string str;
};

class RO0
{
public:
    RO0(int num, std::string str) : ro5(num, str) {}

    void print()
    {
        ro5.print();
    }

private:
    RO5 ro5;
};

int main(void)
{
    {
        RO0 rule{ 10, "first : " };
        RO0 of{ 15, "second : " };
        RO0 _5{ 20, "third : " };

        rule.print();
        of.print();
        _5.print();

        std::cout << "\n\n";

        rule = of;

        rule.print();
        of.print();
        _5.print();

        std::cout << "\n\n";

        rule = std::move(_5);

        rule.print();
        of.print();
        _5.print();

        std::cout << "\n\n";
    }

    _CrtDumpMemoryLeaks();

    return 0;
}
