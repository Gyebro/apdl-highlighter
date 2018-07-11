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
    tooltip_config tcfg;
    parameter_config pcfg;
    user_config ucfg;

    unsigned indentationLevel;
    unsigned lineNumber;

    string processLinesHTMLCode(string line);
    string get_tooltip(string keyword, string& url);
    string get_footer(string input_file);
    string get_product_name();
    string get_version_string();
    string get_timestamp_string();
public:
    highlighter(string tooltip_file, string userconfig_file);
    void highlightHTML(string input_file);
    void highlightTeX(string input_file);

    string processLinesTeXCode(string line);
};

string processLinesTeXSkip(vector<string> &lines);
string processLinesHTMLSkip(vector<string>& lines);
string processLinesTeXFigure(vector<string> &lines);
string processLinesHTMLFigure(vector<string>& lines);
#endif //APDL_HIGHLIGHT_HIGHLIGHTER_H
