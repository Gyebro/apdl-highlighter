//
// Created by Gyebro on 2018. 02. 09..
//

#ifndef APDL_HIGHLIGHT_HIGHLIGHTER_H
#define APDL_HIGHLIGHT_HIGHLIGHTER_H


#include "highlight_config.h"

class highlighter {
private:
    highlight_config hcfg;
    tooltip_config tcfg;
    parameter_config pcfg;
    string convert_line(string line, bool& in_paragraph);
    string get_tooltip(string keyword);
public:
    highlighter(string config_file, string tooltip_file);
    void highlight(string input_file);
};


#endif //APDL_HIGHLIGHT_HIGHLIGHTER_H
