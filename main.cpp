#include <fstream>

#include "parser.h"
#include "statements.h"

void main() {
    ifstream ifs("test.txt");
    ofstream ofs("out.txt");

    string input = "";
    string s;

    while (ifs >> s)
        input += " " + s;
    parser Parser(input);
    statement_list* Program;
    try {
        Program = Parser.parse();
    } 
    catch (exception Exp) {
        ofs << Exp.what();
        return;
    }
    
    Parser.check(Program);
    
    for (auto var : Parser.Used)
        for (int i = 0; i < var.second.size(); ++i)
            if (var.second[i] == false)
                ofs << Parser.Var_to_str[var.first][i] << "\n";
}
