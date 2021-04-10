#pragma once

#include <string>

using namespace std;

enum token_type
{
    // keywords
    tt_while,
    tt_if,
    tt_end,

    tt_var,
    tt_const,

    // operators
    tt_op,
    tt_eq,
    tt_open,
    tt_close,

    // end of file
    tt_eof,

    // parse error
    tt_error
};

struct token
{
    token_type type;
    string string_value;
    int num_value;
};
