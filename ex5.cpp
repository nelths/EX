#pragma once

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <iostream>
using namespace std;

static int counting = 0;

class rvo
{
public:
    rvo(int nums) : num(new int) 
    {
        *num = nums;

        counting++;

        std::cout << "default constructor \n";
    }

    ~rvo()
    {
        delete num;

        std::cout << "custom destructor \n\n";
    }

    rvo(const rvo& rvos)
    {
        *this = rvos;

        counting++;

        std::cout << "copy constructor \n";
    }

    rvo(rvo&& rvos) noexcept
    {
        *this = std::move(rvos);

        counting++;

        std::cout << "move constructor \n";
    }

    rvo& operator=(const rvo& rvos)
    {
        *num = *rvos.num;
        std::cout << "copy assignment operator \n";

        return *this;
    }

    rvo& operator=(rvo&& rvos) noexcept
    {
        delete num;
        num = rvos.num;
        rvos.num = nullptr;

        std::cout << "move assignment operator \n";

        return *this;
    }

    int ReturnValue()
    {
        return *(num);
    }

private:
    int* num;
};

rvo return_value_optimization(int i)
{
    return rvo(i);
}

rvo named_return_value_optimization(int i)
{
    rvo rvos(i);

    return rvos;
}

int main(void)
{
    {
        rvo rvo1(10);
        rvo rvo2(11);

        rvo rvo3 = return_value_optimization(20);
        rvo rvo4 = named_return_value_optimization(21);
    }
    
    _CrtDumpMemoryLeaks();

    std::cout << counting << "\n";
}
