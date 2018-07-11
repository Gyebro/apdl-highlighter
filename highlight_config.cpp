//
// Created by Gyebro on 2018. 02. 09..
//

#include "highlight_config.h"
#include <c++/fstream>
#include "common.h"

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

parameter_config::parameter_config() {
    params.resize(0);
}

void parameter_config::update(string filename) {
    params.resize(0);
    ifstream in(filename);
    string line;
    scalar_param sp;
    vector<string> parts;
    while(getline(in,line,'\n')) {
        if (trim_spaces(line)[0] != '!') {
            parts = split(line,'=');
            if (parts.size() > 1) {
                sp.param = parts[0];
                sp.value = parts[1];
                params.push_back(sp);
            }
        }
    }
    in.close();
}

const vector<scalar_param> &parameter_config::getParams() const {
    return params;
}

/**
 * Annotates a string using the scalar parameters
 * @param argument The argument to be annotated
 * @return The same argument with scalar parameters annotated
 */
string parameter_config::annotate(string argument) {
    string annotated = argument;
    size_t i;
    for (scalar_param sp : params) {
        if (has_string(annotated, sp.param, i)) {
            // TODO: The same token may be used more than once
            if (i > 1) continue;
            string pre_param = "";
            if (i == 1) {
                char pre_param_c = annotated[0];
                pre_param += pre_param_c;
                if (!ispunct(pre_param_c)) continue;
            }
            string param = annotated.substr(i, sp.param.length());
            string post_param = annotated.substr(i+sp.param.length());
            if (post_param.length() > 0 && !ispunct(post_param[0])) continue;
            annotated = pre_param + "<span class='param'>" + param
                        + "<span class='tooltiptext'>" + sp.param + " = " + sp.value + "</span>"
                        + "</span>" + post_param;
        }
    }
    return annotated;
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
