#pragma once

#include <map>

#include "statements.h"
#include "tokenizer.h"

class parser
{
    while_statement* try_parse_while_statement();
    if_statement* try_parse_if_statement();
    eq_statement* try_parse_eq_statement();
    expression* try_parse_expression();
    statement* try_parse_statement();
    statement_list* try_parse_statement_until_end();

public:
    tokenizer tokenizer;

    parser(string input) : tokenizer(input) { }

    statement_list* parse();

    //checker
    map<string, vector<bool>> Used;
    map<string, vector<string>> Var_to_str;

    void check(statement* Program);
};
