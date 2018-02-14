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
