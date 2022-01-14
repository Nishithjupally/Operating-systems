#include <iostream>
#include <string>

using namespace std;

 void foo(string a, string b, string c)
  {

  }

 int argument(string s)
{
    cout<<"arg"<<s<<"\n";
    return s;
}

int main(void)
{
    foo(argument("Mr. A"), argument("Mr. B"), argument("Mr. C"));
    return 0;
}