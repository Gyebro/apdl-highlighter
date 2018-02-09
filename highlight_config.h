//
// Created by Gyebro on 2018. 02. 09..
//

#ifndef APDL_HIGHLIGHT_HIGHLIGHT_CONFIG_H
#define APDL_HIGHLIGHT_HIGHLIGHT_CONFIG_H

#include <string>
#include <vector>

using namespace std;

enum html_entity {
    TAG,
    CLASS,
    ID
};

struct html_rule {
    string token;
    html_entity repl_type;
    string repl_value;
};

class highlight_config {
private:
    vector<html_rule> html_rules;
    html_rule generate_html_rule(string line);
public:
    highlight_config(string filename);
    const vector<html_rule> &get_rules() const;
};

struct tooltip {
    string keyword;
    string usage;
    string description;
    string url;
};

class tooltip_config {
private:
    vector<tooltip> tooltips;
public:
    tooltip_config(string filename);
    const vector<tooltip> &get_tooltips() const;
};


#endif //APDL_HIGHLIGHT_HIGHLIGHT_CONFIG_H
