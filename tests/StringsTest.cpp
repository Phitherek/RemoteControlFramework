#include <iostream>
#include <cstdlib>
#include <string>
using namespace std;

int main() {
    string str = "KEYWORD data";
    cout << str.find("KEYWORD") << endl << (str.find("KEYWORD") == string::npos) << endl << string::npos << endl << str.substr(str.find("KEYWORD")+8) << endl;
    
    return EXIT_SUCCESS;
}
