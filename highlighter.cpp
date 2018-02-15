//
// Created by Gyebro on 2018. 02. 09..
//

#include <c++/fstream>
#include "highlighter.h"
#include "common.h"

highlighter::highlighter(string config_file, string tooltip_file): hcfg(config_file), tcfg(tooltip_file) {

}

void highlighter::highlight(string input_file) {
    // Update scalar parameters
    pcfg.update(input_file);
    ifstream input(input_file);
    string out_file = input_file.substr(0,input_file.length()-3)+"html";
    cout << "Highlighting APDL script '"+input_file+"', output: '"+out_file+"'\n";
    ofstream output(out_file);
    output << "<!DOCTYPE html>\n";
    output << "<html lang=\"en\">\n"
            " <link rel=\"stylesheet\" type=\"text/css\" href=\"style.css\">\n"
            "<head>\n";
    output << " <title>FEA Ansys APDL tutorial</title>\n";
    output << "</head>\n<body>\n";
    string line;
    bool in_paragraph = false;
    while(getline(input, line, '\n')) {
        output << convert_line(line, in_paragraph);
    }
    output << "</body>\n";
    output.close();
}

string highlighter::convert_line(string line, bool& in_paragraph) {
    string converted = "";
    bool token_match = false;
    for (html_rule r : hcfg.get_rules()) {
        if (line.length() >= r.token.length()) {
            if (line.substr(0,r.token.length()) == r.token) { // Token matches
                // Manage paragraph start/end
                switch (r.repl_type) {
                    case html_entity::CLASS:
                    case html_entity::ID:
                        if (!in_paragraph) {
                            converted += "<p>";
                            in_paragraph = true;
                        }
                        break;
                    case html_entity::TAG:
                        if (in_paragraph) {
                            converted += "</p>";
                            in_paragraph = false;
                        }
                        break;
                }
                // Convert line and add html tags
                switch (r.repl_type) {
                    case html_entity::CLASS:
                        converted+="<span class='"+r.repl_value+"'>";
                        converted+=line;
                        converted+="</span>";
                        converted+="<br>\n";
                        break;
                    case html_entity::ID:
                        converted+="<span id='"+r.repl_value+"'>";
                        converted+=line;
                        converted+="</span>";
                        converted+="<br>\n";
                        break;
                    case html_entity::TAG:
                        converted+="<"+r.repl_value+">";
                        // If the token is replaced with a tag, print the token in "hidden" form
                        converted+="<span class='hidden'>";
                        converted+=line.substr(0,r.token.length())+"</span>";
                        converted+=trim_spaces(line.substr(r.token.length()));
                        converted+="</"+r.repl_value+">";
                        converted+="\n";
                        break;
                }
                token_match = true;
                break; // Continue after first token match!
            }
        }
    }
    if (!token_match) {
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
                converted += "<span class='param'>" + command.substr(0,loc) + "</span>";
                converted += command.substr(loc);
                converted += "<br>\n";
            } else {
                vector<string> parts = split(command, ',');

//                indent the line
                if(parts[0] == "*ENDDO")
                    indentationLevel--;
                for(size_t iii = 1; iii <= indentationLevel; iii++)
                    converted += "&emsp;";
                if(parts[0] == "*DO")
                    indentationLevel++;

                string url;
                string tooltip = get_tooltip(trim_spaces(parts[0]), url);
                if (url.length() > 0) {
                    converted += "<span class='keyword'><a href='" + url + "' target='_blank'>" + trim_spaces(parts[0]) + "</a>";
                } else {
                    converted += "<span class='keyword'>" + trim_spaces(parts[0]);
                }
                // Find tooltip for this keyword if any
                converted += tooltip;
                converted += "</span>";
                for (size_t i = 1; i < parts.size(); i++) {
                    converted += "," + pcfg.annotate(trim_spaces(parts[i]));
                }
                if (!comment.empty()) {
                    converted += "<span class='comment inline'>!" + comment + "</span>";
                }
                converted += "<br>\n";
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
            url = t.url;
            return tt;
        }
    }
    cout << "Warning: tooltip for keyword: '" + keyword + "' not found!\n";
    return tt;
}
