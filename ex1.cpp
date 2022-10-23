#include <iomanip>  // std::quoted C++14
#include <iostream> // cout
#include <string>   // std::string, std::to_string
#include <utility>  // std::move

struct Example
{

    int* pint{ nullptr };
    std::string string{};


    Example(int val, std::string const& _str) : pint(new int(val)), string(_str)
    {
        std::cout << "Example(" << val << ")\n";
    }


    Example(Example const& original)
        : pint(new int(*original.pint)), string(original.string)
    {
        std::cout << "Example( const& )\n";
    }

    // move ctor
    Example(Example&& original)
        : pint(original.pint),               // reuse pointer
        string(std::move(original.string)) // ask std::string to reuse its pointer
    {
        original.pint = nullptr;
        std::cout << "Example( && )\n";
    }

    void Print()
    {
        std::cout << "Print Example(";
        if (pint)
        {
            std::cout << *pint;
        }
        else
        {
            std::cout << "garbage";
        }
        std::cout << "), string = " << std::quoted(string) << "\n";
    }

    ~Example()
    {
        std::cout << "~Example( ), pint = " << (pint ? "ptr" : "null") << "\n";
        delete pint;
    }
};

Example named_RVO_function(int i)
{
    Example retval(0, "");
    *retval.pint = i;
    retval.string = std::to_string(i);
    return retval; // return by value, NRVO (Named RVO)
}

Example move_function(Example y)
{
    std::cout << "move_function starts\n";
    y.string += ", processed by move_function";
    return y; // return by value, compiler will use move ctor
}

Example combine_RVO(Example const& a1, Example const& a2)
{
    Example ret_val(a1);
    *ret_val.pint += *a2.pint;
    ret_val.string += a2.string;
    return ret_val; // compiler builds the sum inside callers memory space
}

Example combine_force_move(Example const& a1, Example const& a2)
{
    Example ret_val(a1);
    *ret_val.pint += *a2.pint;
    ret_val.string += a2.string;
    return std::move(
        ret_val); // stops RVO!!!, uses move ctor (which is less efficient)
}

Example&& combine_bad_return(Example const& a1, Example const& a2)
{
    Example ret_val(a1);
    *ret_val.pint += *a2.pint;
    ret_val.string += a2.string;
    return std::move(ret_val);
    //-------------- always a bad idea, not only inefficient, may also create a
    //dangling reference (ie we're returning a reference to a temporary)
}

int main(void)
{
    Example y2(2, "2");
    std::cout << "---named_RVO_function-------------\n";
    Example y5 = named_RVO_function(5); // optimized - RVO
    // Example(0)

    std::cout << "---move_function-------------\n";
    Example y6 = move_function(
        y2); // deep copy to move_function, move from move_function, y2 is unchanged
    // Example( const& )
    // move_function starts
    // Example( && )
    //~Example( ), pint = null

    std::cout << "---move_function(move)---------\n";
    Example y7 = move_function(std::move(
        y2)); // move to move_function, move from move_function, y2 is broken
    y2.Print();
    y7.Print();
    // Example( && )
    // move_function starts
    // Example( && )
    //~Example( ), pint = null
    // print Example(garbage), string = ""
    // print Example(2), string = "2, processed by move_function"

    std::cout << "---just having fun moving-----\n";
    Example y8 = move_function(std::move(move_function(std::move(y5))));
    y5.Print();
    y8.Print();

    std::cout << "---combine_RVO---------\n";
    Example y9(9, "nine plus ");
    Example y10(10, "ten");
    Example y12 = combine_RVO(y9, y10); // compiler builds the sum inside y12, RVO
    y12.Print();
    // Example(9)
    // Example(10)
    // Example( const& )
    // print Example(19), string = "nine plus ten"

    std::cout << "---combine_force_move--------\n";
    Example y13 = combine_force_move(y9, y10); // uses move instead of RVO
    y13.Print();
    // Example( const& )
    // Example( && )
    //~Example( ), pint = null
    // print Example(19), string = "nine plus ten"

    std::cout << "---combine_bad_return--------\n";
    Example y14 = combine_bad_return(y9, y10); // uses move instead of RVO
    y14.Print();
    // CRASH

    std::cout << "-------------------\n";

    return 0;
}