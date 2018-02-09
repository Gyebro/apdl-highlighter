#include <c++/iostream>
#include "highlighter.h"

using namespace std;

int main() {
    cout << "Running APDL highlighter\n";
    highlighter h("config/rules.txt", "config/hints.txt");
    h.highlight("input/R1.txt");

    return 0;
}