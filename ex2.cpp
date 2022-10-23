#pragma once

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <iostream>
using namespace std;

class RO3
{
public:
    RO3(int num, std::string title)
        :str{title}
    {
        data = new int;
        *data = num;
    }

    ~RO3()
    {
        if (data != nullptr)
        {
            delete data;
        }
    }

    RO3(const RO3& rhs)
    {
        *this = rhs;
    }

    RO3& operator=(const RO3& rhs)
    {
        str = rhs.str;
        *data = *rhs.data;

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

int main(void)
{
    {
        RO3 rule{ 10, "first : " };
        RO3 of{ 15, "second : " };

        rule.print();
        of.print();

        std::cout << "\n\n";

        rule = of;

        rule.print();
        of.print();

        std::cout << "\n\n";
    }
    
    _CrtDumpMemoryLeaks();

    return 0;
}
