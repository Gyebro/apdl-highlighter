#include <c++/iostream>
#include "highlighter.h"

using namespace std;

string filenamesFEA[] = {"R1", "R2", "R3",
                         "D1", "D2", "D3",
                         "D4", "D5", "D6", "D7",
                         "H1", "H2", "H3",
                         "NL1", "NL2", "NL3", "NL4", "NL5",
                         "P1",
                         "S3",
                         "SOL1", "SOL2", "SOL3",
                         "ST1", "ST3", "ST4",
                         "T1"};
string filenamesVEMA[] = {"04het", "05het", "06het", "07het", "08het", "09het", "11het", "12het"};

int main() {
    cout << "Running APDL highlighter\n";
    highlighter h("config/rules.txt", "config/hints_eng.txt", "config/config.txt");
    for (string& filename : filenamesFEA) {
        h.highlight("input/FEA/"+filename+"/script.txt");
    }
    cout << "Warning: Hungarian hints should be used in case of hungarian courses\n";
    h = highlighter("config/rules.txt", "config/hints_eng.txt", "config/config.txt");
    for (string& filename : filenamesVEMA) {
        h.highlight("input/VEMA/"+filename+"/script.txt");
    }
    return 0;
}