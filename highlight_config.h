//
// Created by Gyebro on 2018. 02. 09..
//

#ifndef APDL_HIGHLIGHT_HIGHLIGHT_CONFIG_H
#define APDL_HIGHLIGHT_HIGHLIGHT_CONFIG_H

#include <string>
#include <vector>

using namespace std;

enum tooltipType{
    APDLELEM, PARAM
};

struct tooltip {
    tooltipType ttT;
    string keyword;
    string usage;
    string description;
    string url;
};

class tooltip_config {
private:
    vector<tooltip> tooltips;
public:
    tooltip_config(const string& fileName, tooltipType ttT);
    tooltip_config(){tooltips = vector<tooltip>();};
    const vector<tooltip> &get_tooltips() const;
};

class user_config {
private:
    string help_root;
    size_t indent_size;
public:
    user_config(string filename);
    const string &get_help_root() const;
    size_t get_indent_size() const;
};

#endif //APDL_HIGHLIGHT_HIGHLIGHT_CONFIG_H
