//
// Created by Gyebro on 2018. 02. 09..
//

#include "highlight_config.h"
#include <c++/fstream>
#include "common.h"

tooltip_config::tooltip_config(const string& fileName, tooltipType ttT) {
    ifstream in(fileName);
    string line;
    tooltip t;
    vector<string> parts;
    vector<char> delims;
    tooltips.clear();
    if(ttT == tooltipType::APDLELEM){
        while(getline(in,line,'\n')) {
            parts = split(line,'\t');
            if (parts.size() >= 4) {
                t.ttT = tooltipType ::APDLELEM;
                t.keyword =     parts[0];
                t.usage =       parts[1];
                t.description = parts[2];
                t.url =         parts[3];
                tooltips.push_back(t);
            }
        }
    }
    else{
        while(getline(in,line,'\n')) {
            parts.clear();
            delims.clear();
            split_at_multiple(line,"=!",parts,delims,true);
            if (delims[0] == '=') {
                t.ttT = tooltipType ::PARAM;
                t.keyword= parts[0];
                t.usage = "";
                t.description = parts[1];
                t.url = "";
                tooltips.push_back(t);
            }
        }
    }
    in.close();
}

const vector<tooltip> &tooltip_config::get_tooltips() const {
    return tooltips;
}


const string &user_config::get_help_root() const {
    return help_root;
}

size_t user_config::get_indent_size() const {
    return indent_size;
}

user_config::user_config(string filename) {
    // Set default configuration
    indent_size = 2;
    help_root = "https://www.sharcnet.ca/Software/Ansys/17.0/en-us/help";
    ifstream in(filename);
    string line, key, value;
    vector<string> parts;
    while(getline(in,line,'\n')) {
        // ! is the comment character
        if (line[0] != '!') {
            parts = split(line, '\t');
            if (parts.size() == 2) {
                key = parts[0]; value = parts[1];
                // We won't have many config keys, so process them in an if-else here
                if (key == "HELP_ROOT") {
                    help_root = value;
                } else if (key == "INDENT_SIZE") {
                    indent_size = stoul(value);
                } else {
                    cout << "Warning: unsupported settings (" << key << ") found in config file!\n";
                }
            }
        }
    }
    in.close();
}
