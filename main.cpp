#include <c++/iostream>
#include "highlighter.h"

using namespace std;

string filenames[] = {"R1", "R2", "R3",
                      "D1", "D2",
                      "D4", "D5",
                      "D7",
                      "H1", "H2", "H3",
                      "NL1",
                      "P1",
                      "S3",
                      "SOL1", "SOL2",
                      "ST1", "ST3", "ST4",
                      "T1"
                     };

int main() {
    cout << "Running APDL highlighter\n";
    highlighter h("config/rules.txt", "config/hints_eng.txt", "config/config.txt");
    for (string filename : filenames) {
        h.highlight("input/"+filename+".txt");
    }
    return 0;
}