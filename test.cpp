#include "print.hpp"
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <string>
using namespace std;

struct MyObj {int i;};

ostream& operator<<(ostream& out, const MyObj& in)
{
    out << "<MyObj:" << in.i << ">";
    return out;
}

template <typename T>
double bench(T&& func)
{
    auto rval = clock();
    for (int i=0; i<10000; ++i) func(i);
    rval = clock()-rval;
    return double(rval)/(CLOCKS_PER_SEC);
}

double benchPrint()
{
    return bench([](int i)
    {
        cout << "Hi: " << "print (" << i << ")\n";
        cout.flush();
    });
}

double benchPrintf()
{
    return bench([](int i)
    {
        printf("Hi: %s%d%s", "printf(", i, ")\n");
        fflush(stdout);
    });
}

int main()
{
    //cout.sync_with_stdio(false);
    
    string name = "Ralir";
    MyObj mine {7};
    
    print ("Hello, I'm %$ and my object is %$.\n", name, mine);
    print ("\n");
    print ("Floats: %.e6$ .\n", 7.5);
    print ("Width 10: %10$.\n", 7);
    print ("Width 10: %-x*$. %$.\n", 10, 73, 73);
    
    print (std::cerr, "Error: %$.\n", 7);
    
    auto ptime = benchPrint();
    auto pftime = benchPrintf();
    
    print ("Time   cout: %$\n", ptime);
    print ("Time printf: %$\n", pftime);
}
