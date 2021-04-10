#include <fstream>
#include <map>

#include "parser.h"
#include "statements.h"

map<string, vector<bool>> Used;
map<string, vector<string>> Var_to_str;

void check(statement* Program) {
    vector<statement*> Statements = Program->get_vector_statements();
    for (int i = 0; i < Statements.size(); ++i) {
        string st_type = Statements[i]->get_type();
        if (st_type == "eq") {
            Used[Statements[i]->get_var()].push_back(false);
            Var_to_str[Statements[i]->get_var()].push_back(Statements[i]->get_string());

            //check
            auto Values = Statements[i]->get_expr();
            for (auto var_right : Values)
                for (auto var_left : Used) {
                    if (var_right.name == var_left.first)
                        Used[var_right.name][var_left.second.size() - 1] = true;
                }
        }

        if (st_type == "while" || st_type == "if") {
            auto Values = Statements[i]->get_expr();
            for (auto var_right : Values)
                for (auto var_left : Used) {
                    if (var_right.name == var_left.first)
                        Used[var_right.name][var_left.second.size() - 1] = true;
                }

            check(Statements[i]->get_list());
        }
    }

}

void main() {
    ifstream ifs("test.txt");
    ofstream ofs("out.txt");

    string input = "";
    string s;

    while (ifs >> s)
        input += " " + s;
    parser Parser(input);
    statement_list* Program = Parser.parse();
    
    check(Program);
    for (auto var : Used)
        for (int i = 0; i < var.second.size(); ++i)
            if (var.second[i] == false)
                ofs << Var_to_str[var.first][i] << "\n";
}
