#pragma once

#include "token.h"

class tokenizer {
    const string text;
    int curridx;       // текущая позиция в тексте
    int endidx;        // длина текста

    token next_token;

    void set_next_token();

public:
    tokenizer(string text) : text(text), curridx(0), endidx(text.length()) {
        next_token.num_value = 0;
        next_token.string_value = "";
        set_next_token();
    }

    token get_next() { return next_token; }
    void move_ahead() { set_next_token(); }
};
