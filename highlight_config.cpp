//
// Created by Gyebro on 2018. 02. 09..
//

#include "highlight_config.h"
#include <c++/fstream>
#include "common.h"

html_rule highlight_config::generate_html_rule(string line) {
    vector<string> parts = split(line,'\t');
    html_rule r;
    r.token = parts[0];
    switch (parts[1][0]) {
        case '.':
            r.repl_type = html_entity::CLASS;
            r.repl_value = parts[1].substr(1);
            break;
        case '#':
            r.repl_type = html_entity::ID;
            r.repl_value = parts[1].substr(1);
            break;
        default:
            r.repl_type = html_entity::TAG;
            r.repl_value = parts[1];
            break;
    }
    return r;
}

highlight_config::highlight_config(string filename) {
    ifstream in(filename);
    string line;
    html_rule r;
    while(getline(in,line,'\n')) {
        html_rules.push_back(generate_html_rule(line));
    }
}

const vector<html_rule> &highlight_config::get_rules() const {
    return html_rules;
}

tooltip_config::tooltip_config(string filename) {
    ifstream in(filename);
    string line;
    tooltip t;
    while(getline(in,line,'\n')) {
        vector<string> parts = split(line,'\t');
        if (parts.size() >= 4) {
            t.keyword =     parts[0];
            t.usage =       parts[1];
            t.description = parts[2];
            t.url =         parts[3];
            tooltips.push_back(t);
        }
    }
}

const vector<tooltip> &tooltip_config::get_tooltips() const {
    return tooltips;
}
