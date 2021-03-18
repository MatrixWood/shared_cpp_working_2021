#include <iostream>
#include "nullable.h"

using namespace std;

int main(int argc, char* argv[])
{
  (void)argc;
  (void)argv;

    Nullable<int> ni1;
    Nullable<int> ni2 = nullptr;
    Nullable<int> ni3 = 3;
    Nullable<int> ni4 = 4;
    ni4 = nullptr;
    Nullable<int> ni5 = 5;
    Nullable<int> ni6;
    ni6 = ni3;
    Nullable<int> ni7(ni3);
    //Nullable<int> ni8 = NULL; // This is an error in gcc/clang but it's ok in VS12

    cout << (ni1 == nullptr ? "True" : "False") << endl; // True
    cout << (ni2 == nullptr ? "True" : "False") << endl; // True
    cout << (ni2 == 3 ? "True" : "False") << endl; // False
    cout << (ni2 == ni3 ? "True" : "False") << endl; // False
    cout << (ni3 == 3 ? "True" : "False") << endl; // True
    cout << (ni2 == ni4 ? "True" : "False") << endl; // True
    cout << (ni3 == ni5 ? "True" : "False") << endl; // False
    cout << (ni3 == ni6 ? "True" : "False") << endl; // True
    cout << (ni3 == ni7 ? "True" : "False") << endl; // True

    //cout << ni1 << endl; // Doesn't compile
    //cout << ni3 << endl; // Doesn't compile
    cout << ni3.Value << endl; // 3
    //cout << ni1.Value << endl; // Throw exception
    //cout << ni2.Value << endl; // Throw exception
    //ni3.Value = 2; // Doesn't compile
    cout << sizeof(ni1) << endl; // 8 on VS15

    return 0;
}