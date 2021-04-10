#include "tokenizer.h"

void tokenizer::set_next_token()
{
    while (curridx < endidx && text[curridx] == ' ')
        curridx++;

    if (curridx == endidx) {
        next_token.type = tt_eof;
        return;
    }

    char c = text[curridx];

    if (c == '(' || c == ')') {
        next_token.type = (c == '(') ? tt_open : tt_close;
        next_token.string_value = c;
        curridx++;
        return;
    }

    // keywords
    if (isalpha(c)) {
        string result;
        while (curridx < endidx && isalpha(text[curridx])) {
            result += text[curridx];
            curridx++;
        }

        if (result == "while")
            next_token.type = tt_while;
        else if (result == "if")
            next_token.type = tt_if;
        else if (result == "end")
            next_token.type = tt_end;
        else {
            next_token.type = tt_var;
            next_token.string_value = result;
        }
        return;
    }

    // const
    if (isdigit(c)) {
        string result;
        while (curridx < endidx && isdigit(text[curridx])) {
            result += text[curridx];
            curridx++;
        }
        int converted = stoi(result);
        next_token.type = tt_const;
        next_token.num_value = converted;
        next_token.string_value = to_string(converted);
        return;
    }

    // operator
    if (c == '=') {
        next_token.type = tt_eq;
        next_token.string_value = c;
        curridx++;
        return;
    }
    if (c == '<' || c == '>' || c == '+' || c == '-' || c == '*' || c == '/') {
        next_token.type = tt_op;
        next_token.string_value = c;
        curridx++;
        return;
    }

    next_token.type = tt_error;
}
