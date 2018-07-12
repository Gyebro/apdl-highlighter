//
// Created by Gyebro on 2018. 02. 09..
//

#include <c++/fstream>
#include <iomanip>
#include <ctime>
#include "highlighter.h"
#include "common.h"

highlighter::highlighter(string APDLElemsFile, string userconfig_file) :
        tcfgAPDL(APDLElemsFile, tooltipType::APDLELEM),
        tcfgPARAM(),
        ucfg(userconfig_file) {
    indentationLevel = 0;
    lineNumber = 0;
}

void highlighter::updateParams(string scriptFile){
    tcfgPARAM = tooltip_config(scriptFile, tooltipType::PARAM);
}

void highlighter::highlightHTML(string input_file) {
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
    unsigned parNum = 0;
    while(!input.eof()){
        vector<string> lines;
        do{
            getline(input, line, '\n');
            ++lineNumber;
            lines.push_back(line);
        }while(!input.eof() && !line.empty());
        lines.pop_back();

        if(lines[0][0] == '!' && lines[0][1] != ' '){ // this type of line marks the beginning of a 'none-code' environment
            if(lines[0]=="!skip")
                output << processLinesHTMLSkip(lines);
            else if(lines[0]=="!fig") {
                output << processLinesHTMLFigure(lines);
            }
            else if(lines[0]=="!par"){
                parNum++;
                if(parNum > 1)
                    output << "</div>\n";
                output << "<button class=\"paragraphtitle\">" + lines[1] + "</button>\n" + "<div class=\"paragraphcontent\">\n";
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
    output << "</div>\n"; // close the last paragraph
    output << get_footer(input_file);
    output << "\n<script src='../../myScripts.js'></script>\n</body>\n</html>";
    output.close();
}

void highlighter::highlightTeX(string input_file) {
    // Update scalar parameters
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
        }while(!input.eof() && !line.empty());
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
                    "<figcaption>" + lines[2].substr(2) + "</figcaption>\n</figure><br>\n";
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
        // Check for = sign first, Assume A=B, second part has to be processed as well
        size_t loc = 0;
        if (has_string(command,"=",loc)) {
            converted += "<span class='param'>" + trim_spaces(command.substr(0,loc)) + "</span>";
            command = command.substr(loc);
        }
        vector<string> splittedString;
        vector<char> splitterDelims;
        split_at_multiple(command, "=+-*/(),", splittedString, splitterDelims, true);
        if(splittedString[0] == "" && string("*/").find(splitterDelims[0]) != string::npos){ // chopped of the first character of an APDL function
            splittedString[1] = string(1, splitterDelims[0]) + splittedString[1];
            splittedString.erase(splittedString.begin());
            splitterDelims.erase(splitterDelims.begin());
        }
//              indent the line
        if (splittedString.size()>0 && splittedString[0] == "*ENDDO")
            indentationLevel--;
        for (size_t iii = 1; iii <= indentationLevel; iii++) {
            for (size_t jjj=0; jjj<ucfg.get_indent_size(); jjj++) {
                converted += "&nbsp";
            }
        }
        if (splittedString.size()>0 && splittedString[0] == "*DO")
            indentationLevel++;
        for(size_t i = 0; i < splittedString.size(); i++) {
            if(splittedString[i].empty())
                ;
            else if(is_digits(splittedString[i])){ // it contains only numbers -> cannot be annotated
                converted += splittedString[i];
            }
            else{
                converted += get_tooltip(splittedString[i]);
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
                    converted += "\\quad ";
                }
            }
            if (trim_spaces(parts[0]) == "*DO")
                indentationLevel++;
            string url;
            string tooltip = get_tooltip(trim_spaces(parts[0]));
            if (url.length() > 0) {
                converted += "\\href{{" + url + "}}{" + trim_spaces(parts[0]) + "}";
            } else {
                converted +=  trim_spaces(parts[0]);
            }
            for (size_t i = 1; i < parts.size(); i++) {
                converted += "," + trim_spaces(parts[i]);
            }
            if (!comment.empty()) {
                converted += "\\quad !" + comment + "\n";
            }
        }
    }
    return converted;
}

string highlighter::get_tooltip(string keyword) {
    string tt = "";
    string tooltip_newline = "\n";
    for (tooltip t : tcfgAPDL.get_tooltips()) {
        if (t.keyword == keyword) {
            tt += "<span class='keyword'>";
            tt += "<a href='" + ucfg.get_help_root() + t.url + "' target='_blank'>" + keyword + "</a>";
            tt += "<span class='tooltiptext'>";
//            Removing the 'unnecessary' text from the tooltip
//            tt += "<a href='" + t.url + "' target='_blank'>";
//            tt += t.keyword + "</a>" + tooltip_newline;
            tt += "<b>" + t.usage + "</b>" + tooltip_newline;
            tt += t.description;
            tt += "</span></span>";
            return tt;
        }
    }
    for (tooltip t : tcfgPARAM.get_tooltips()) {
        if (t.keyword == keyword) {
            tt += "<span class='param'>" + keyword
                  + "<span class='tooltiptext'>" + keyword + " = " + t.description + "</span>"
                  + "</span>";
            return tt;
        }
    }
    cout << "Line " << lineNumber << " Warning: tooltip for keyword: '" + keyword + "' not found!\n";
    return keyword;
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