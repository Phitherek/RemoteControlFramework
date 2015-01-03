#include <iostream>
#include <cstdlib>
using namespace std;

int main(int argc, char** argv) {
    cout << "Stdout test:";
    for(int i = 1; i < argc; i++) {
        cout << " " << argv[i];
    }
    cout << endl;
    cerr << "Stderr test" << endl;
    return EXIT_FAILURE;
}
