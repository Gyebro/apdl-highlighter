//
// Created by Gyebro on 2018. 02. 09..
//

#ifndef APDL_HIGHLIGHT_HIGHLIGHTER_H
#define APDL_HIGHLIGHT_HIGHLIGHTER_H


#include "highlight_config.h"

#define PRODUCT_NAME "APDL highlighter"
#define VERSION_STRING "0.1-beta"

class highlighter {
private:
    highlight_config hcfg;
    tooltip_config tcfg;
    parameter_config pcfg;
    user_config ucfg;
    unsigned indentationLevel;
    unsigned lineNumber;
    string convert_line(string line, bool& in_paragraph);
    string get_tooltip(string keyword, string& url);
    string get_footer(string input_file);
    string get_product_name();
    string get_version_string();
    string get_timestamp_string();
public:
    highlighter(string config_file, string tooltip_file, string userconfig_file);
    void highlight(string input_file);
};


#endif //APDL_HIGHLIGHT_HIGHLIGHTER_H
