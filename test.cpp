#include "print.hpp"
#include <string>
using namespace std;

struct MyObj {int i;};

ostream& operator<<(ostream& out, const MyObj& in)
{
    out << "<MyObj:" << in.i << ">";
    return out;
}

int main()
{
    string name = "Ralir";
    MyObj mine {7};
    
    print ("Hello, I'm %$ and my object is %$.\n", name, mine);
    print ("\n");
    print ("Floats: %.e6$ .\n", 7.5);
    print ("Width 10: %10$.\n", 7);
    print ("Width 10: %-x*$. %$.\n", 10, 73, 73);
}
