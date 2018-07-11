//
// Created by Gyebro on 2018. 02. 09..
//

#ifndef APDL_HIGHLIGHT_HIGHLIGHT_CONFIG_H
#define APDL_HIGHLIGHT_HIGHLIGHT_CONFIG_H

#include <string>
#include <vector>

using namespace std;

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

struct scalar_param {
    string param;
    string value;
};

class parameter_config {
private:
    vector<scalar_param> params;
public:
    parameter_config();
    void update(string filename);
    string annotate(string argument);
    const vector<scalar_param> &getParams() const;
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
