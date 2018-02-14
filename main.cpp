#include <c++/iostream>
#include "highlighter.h"

using namespace std;

string filenames[] = {"R1", "R2", "H1", "H3", "SOL1"};

int main() {
    cout << "Running APDL highlighter\n";
    highlighter h("config/rules.txt", "config/hints.txt");
    for (string filename : filenames) {
        h.highlight("input/"+filename+".txt");
    }
    return 0;
}