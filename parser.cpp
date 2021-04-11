#include "parser.h"

statement_list* parser::parse() {
    statement_list* p = new statement_list();
    while (true) {
        statement* s = try_parse_statement();
        if (!s)
            break;
        p->statements.emplace_back(s);
    }
    return p;
}

statement_list* parser::try_parse_statement_until_end() {
    unique_ptr<statement_list> p(new statement_list());
    while (true) {
        statement* s = try_parse_statement();
        if (!s)
            break;
        p->statements.emplace_back(s);
    }

    // end
    token t = tokenizer.get_next();
    if (t.type != tt_end)
        throw exception("expected end after statement list");
    tokenizer.move_ahead();
    return p.release();
}

statement* parser::try_parse_statement() {
    statement* result = nullptr;
    // while
    result = try_parse_while_statement();
    if (result)
        return result;

    // if
    result = try_parse_if_statement();
    if (result)
        return result;

    // =
    result = try_parse_eq_statement();
    return result;
}


while_statement* parser::try_parse_while_statement() {
    // while
    token t = tokenizer.get_next();
    if (t.type != tt_while)
        return nullptr;
    tokenizer.move_ahead();

    // expression
    unique_ptr<expression> exp(try_parse_expression());
    if (!exp)
        throw exception("expression expected after while(");

    // statement list
    statement_list* st(try_parse_statement_until_end());
    if (!st)
        throw exception("statement list expected after while()");

    // result
    while_statement* ws = new while_statement();
    ws->statements = move(st);
    ws->exp = *exp;
    return ws;
}

if_statement* parser::try_parse_if_statement() {
    // if
    token t = tokenizer.get_next();
    if (t.type != tt_if)
        return nullptr;
    tokenizer.move_ahead();

    // expression
    unique_ptr<expression> exp(try_parse_expression());
    if (!exp)
        throw exception("expression expected after if(");

    // statement list
    statement_list* st(try_parse_statement_until_end());
    if (!st)
        throw exception("statement list expected after if()");

    // result
    if_statement* ifs = new if_statement();
    ifs->statements = move(st);
    ifs->exp = *exp;
    return ifs;
}

eq_statement* parser::try_parse_eq_statement() {
    eq_statement* eqs = new eq_statement();

    // var
    token t = tokenizer.get_next();
    if (t.type != tt_var)
        return nullptr;
    tokenizer.move_ahead();
    eqs->eq_string = t.string_value;
    eqs->var = variable(t.string_value);

    // =
    t = tokenizer.get_next();
    if (t.type != tt_eq)
        throw exception("= expected");
    tokenizer.move_ahead();
    eqs->eq_string += " = ";

    // expression
    expression* exp = try_parse_expression();
    if (!exp)
        throw exception("expression expected after =");
    eqs->eq_string += exp->str;

    // result
    eqs->exp = *exp;
    return eqs;
}

expression* parser::try_parse_expression() {
    expression* result = new expression();
    string str = "";

    bool excepted = true; // tru -- const/vsr/(, false -- op/)
    while (true) {
        token t = tokenizer.get_next();
        if (t.type == tt_open && excepted == true)
            excepted = true;
        else if (t.type == tt_close && excepted == false)
            excepted = false;
        else if (t.type == tt_op && excepted == false)
            excepted = true;
        else if (t.type == tt_const && excepted == true)
            excepted = false;
        else if (t.type == tt_var && excepted == true) {
            result->values.push_back(variable(t.string_value));
            excepted = false;
        }
        else
            break;
        str += t.string_value + " ";
        tokenizer.move_ahead();
    }
    if (str.size() == 0)
        return nullptr;
    result->str = str;
    return result;
}

void parser::check(statement* Program) {
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
