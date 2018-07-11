//
// Created by Gyebro on 2018. 02. 09..
//

#include <c++/fstream>
#include <iomanip>
#include <ctime>
#include "highlighter.h"
#include "common.h"

highlighter::highlighter(string tooltip_file, string userconfig_file) :
        tcfg(tooltip_file),
        ucfg(userconfig_file) {
    indentationLevel = 0;
    lineNumber = 0;
}

void highlighter::highlightHTML(string input_file) {
    // Update scalar parameters
    pcfg.update(input_file);
    ifstream input(input_file);
    string out_file = input_file.substr(0,input_file.length()-10)+"index.html";
    cout << "Highlighting APDL script '"+input_file+"', output: '"+out_file+"'\n";
    ofstream output(out_file);
    output << "<!DOCTYPE html>\n";
    output <<  "<html lang=\"en\">\n";
    output <<  "<link rel=\"stylesheet\" type=\"text/css\" href=\"../style.css\">\n";
    output <<  "<head>\n";
    output << "<title>FEA Ansys APDL tutorial</title>\n";
    output << "<meta charset=\"UTF-8\">\n";
    output << "</head>\n<body>\n";
    string line;

    // Reset line number and indent level
    indentationLevel = 0;
    lineNumber = 0;

    while(!input.eof()){
        vector<string> lines;
        do{
            getline(input, line, '\n');
            ++lineNumber;
            lines.push_back(line);
        }while(!line.empty());
        lines.pop_back();

        if(lines[0][0] == '!' && lines[0][1] != ' '){ // this type of line marks the beginning of a 'none-code' environment
            if(lines[0]=="!skip")
                output << processLinesHTMLSkip(lines);
            else if(lines[0]=="!fig") {
                output << processLinesHTMLFigure(lines);
            }
            else{
                output << "<div class='" + lines[0].substr(1) + "'>\n<p>\n";
                for(size_t i = 1; i < lines.size(); i++)
                    output << lines[i] << "\n";
                output << "</p>\n</div>\n";
            }
        } else{
            for(string &ln : lines)
                output << processLinesHTMLCode(ln) << "\n";
        }
    }

    output << get_footer(input_file);
    output << "</body>\n";
    output.close();
}

void highlighter::highlightTeX(string input_file) {
    // Update scalar parameters
    pcfg.update(input_file);
    ifstream input(input_file);
    string out_file = input_file.substr(0,input_file.length()-10)+"index.tex";
    cout << "Highlighting APDL script '"+input_file+"', output: '"+out_file+"'\n";
    ofstream output(out_file);
    output << "\\input{../../TeXheader.tex}\n";
    string line;

    while(!input.eof()){
        vector<string> lines;
        do{
            getline(input, line, '\n');
            ++lineNumber;
            lines.push_back(line);
        }while(!line.empty());
        lines.pop_back();

        if(lines[0][0] == '!' && lines[0][1] != ' '){ // this type of line marks the beginning of a 'none-code' environment
            if(lines[0]=="!skip")
                output << processLinesTeXSkip(lines);
            else if(lines[0]=="!fig") {
                output << processLinesTeXFigure(lines);
            }
            else{
                output << "\\begin{" + lines[0].substr(1) + "}\n";
                for(size_t i = 1; i < lines.size(); i++)
                    output <<  "\\noindent\n" + lines[i] + "\n";
                output << "\\end{" + lines[0].substr(1) + "}\n\n";
            }
        } else{
            for(string &ln : lines)
                output << "\\noindent\n" + ReplaceAll(processLinesTeXCode(ln), "_", "\\_") + "\n\n";
        }
    }

    output << "\\end{document}";
    output.close();
}

string processLinesHTMLSkip(vector<string>& lines){
    string outputString = "";
    outputString += "<!--\n";
    lines.erase(lines.begin());
    for(string &ln : lines)
        outputString += ln + "\n";
    outputString += "-->\n";
    return outputString;
}

string processLinesHTMLFigure(vector<string>& lines){
    string outputString = "";
    outputString += "<figure>\n<img src='./images/" + lines[1].substr(2) + "' alt='" + lines[1].substr(2) + "'>\n" +
                    "<figcaption>" + lines[2].substr(2) + "</figcaption>\n</figure>";
    return outputString;
}

string highlighter::get_footer(string input_file) {
    string filename = split(input_file, '/').back();
    string footer = "";
    footer += "<p id='footer'>";
    footer += "! This webpage was generated with " + get_product_name()
              + " (version: " + get_version_string() + ")"
              + " on: " + get_timestamp_string() + ", "
              + " from APDL script: <a target='_blank' href='" + filename + "'>" + filename + "</a>";
    footer += "</p>";
    return footer;
}

string highlighter::get_product_name() {
    return PRODUCT_NAME;
}

string highlighter::get_version_string() {
    return VERSION_STRING;
}

string highlighter::get_timestamp_string() {
    time_t t = time(nullptr);
    tm time = *localtime(&t);
    stringstream ss;
    ss << put_time(&time, "%d.%m.%Y %H:%M:%S");
    return ss.str();
}

string highlighter::processLinesHTMLCode(string line){
    string converted = "";
    // Handle line end comments first
    vector<string> outer = split(line,'!');
    string command = "";
    string comment = "";
    if (!outer.empty()) {
        command = outer[0];
        for (size_t j=1; j<outer.size(); j++) {
            comment += outer[j];
        }
    }
    if (!command.empty()) {
        // Check for = sign first, Assume A=B, second part has to processed as well
        size_t loc = 0;
        if (has_string(command,"=",loc)) {
            converted += "<span class='param'>" + trim_spaces(command.substr(0,loc)) + "</span>";
            command = command.substr(loc);
        }
        vector<string> splittedString;
        vector<char> splitterDelims;
        split_at_multiple(command, "=+-*/(),", splittedString, splitterDelims);
//              indent the line
        if (splittedString.size()>1 && splittedString[1] == "ENDDO")
            indentationLevel--;
        for (size_t iii = 1; iii <= indentationLevel; iii++) {
            for (size_t jjj=0; jjj<ucfg.get_indent_size(); jjj++) {
                converted += "&nbsp;";
            }
        }
        if (splittedString.size()>1 && splittedString[1] == "DO")
            indentationLevel++;
        for(size_t i = 0; i < splittedString.size(); i++) {
            if(!splittedString[i].empty())
            {
                // assume that it is a function -> try to get the function tooltip for it
                string url;
                string tooltip = get_tooltip(splittedString[i], url);
                if (url.length() > 0) { // it is a function indeed
                    converted += "<span class='keyword'>";
                    converted += "<a href='" + url + "' target='_blank'>" + splittedString[i] + "</a>";
                    converted += tooltip + "</span>";
                } else { // it is a numeric or scalar
                    converted +=pcfg.annotate(splittedString[i]);
                }
            }
            if(i < splitterDelims.size())
                converted += splitterDelims[i];
        }
        if (!comment.empty()) {
            converted += "<span class='comm inline'>!" + comment + "</span>";
        }
        converted += "<br>\n";
    }
    return converted;
}

string highlighter::processLinesTeXCode(string line) {
    string converted = "";
    // Handle line end comments first
    vector<string> outer = split(line,'!');
    string command = "";
    string comment = "";
    if (!outer.empty()) {
        command = outer[0];
        for (size_t j=1; j<outer.size(); j++) {
            comment += outer[j];
        }
    }
    if (!command.empty()) {
        // Check for = sign first, Assume A=B
        size_t loc = 0;
        if (has_string(command,"=",loc)) {
            converted += command;
        } else {
            vector<string> parts = split(command, ',');
//              indent the line
            if (trim_spaces(parts[0]) == "*ENDDO")
                indentationLevel--;
            for (size_t iii = 1; iii <= indentationLevel; iii++) {
                for (size_t jjj=0; jjj<ucfg.get_indent_size(); jjj++) {
                    converted += "\\quad;";
                }
            }
            if (trim_spaces(parts[0]) == "*DO")
                indentationLevel++;
            string url;
            string tooltip = get_tooltip(trim_spaces(parts[0]), url);
            if (url.length() > 0) {
                converted += "\\href{{" + url + "}}{" + trim_spaces(parts[0]) + "}";
            } else {
                converted +=  trim_spaces(parts[0]);
            }
            for (size_t i = 1; i < parts.size(); i++) {
                converted += "," + trim_spaces(parts[i]);
            }
            if (!comment.empty()) {
                converted += "!" + comment + "\n";
            }
        }
    }
    return converted;
}

string highlighter::get_tooltip(string keyword, string& url) {
    string tt = "";
    string tooltip_newline = "\n";
    for (tooltip t : tcfg.get_tooltips()) {
        if (t.keyword == keyword) {
            tt += "<span class='tooltiptext'>";
//            Removing the 'unnecessary' text from the tooltip
//            tt += "<a href='" + t.url + "' target='_blank'>";
//            tt += t.keyword + "</a>" + tooltip_newline;
            tt += "<b>" + t.usage + "</b>" + tooltip_newline;
            tt += t.description;
            tt += "</span>";
            url = ucfg.get_help_root() + t.url;
            return tt;
        }
    }
    cout << "Line " << lineNumber << " Warning: tooltip for keyword: '" + keyword + "' not found!\n";
    return tt;
}

string processLinesTeXSkip( vector<string> &lines) {
    string outputString ="";
    for(string& ln: lines)
        outputString += "% " + ln + "\n";
    return  outputString;
}

string processLinesTeXFigure(vector<string> &lines) {
    string outputString ="";
    outputString += "\\begin{figure}\n\\centering\n";
    outputString += "\\includegraphics[scale = 1]{{" + lines[1].substr(2) + "}}\n";
    outputString += "\\caption{" + lines[2].substr(2) + "}\n";
    outputString += "\\end{figure}\n\n";
    return  outputString;
}
