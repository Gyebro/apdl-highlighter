#include <c++/iostream>
#include "highlighter.h"

using namespace std;

string filenames[] = {"R1", "R2", "H1", "H3", "R3", "S3", "SOL1", "ST1", "ST4", "P1", "NL1", "ST3", "H2"};

int main() {
    cout << "Running APDL highlighter\n";
    highlighter h("config/rules.txt", "config/hints_eng.txt", "config/config.txt");
    for (string filename : filenames) {
        h.highlight("input/"+filename+".txt");
    }
    return 0;
}