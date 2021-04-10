#pragma once

#include <vector>
#include <memory>
#include <string>

using namespace std;

struct constant {
    int value;
};

struct variable {
    string name;

    variable(string name = "") : name(name) { }
};

struct statement {
    virtual string get_string() {
        return "";
    };
    virtual string get_type() = 0;
    virtual string get_var() {
        return "";
    }
    virtual vector<variable> get_expr() {
        return vector<variable>();
    }
    virtual statement* get_list() {
        return nullptr;
    }
    virtual vector<statement*> get_vector_statements() {
        return vector<statement*>();
    }
};

struct expression {
    vector<variable> values;
    string str;
};

struct statement_list : public statement {
    vector<statement*> statements;

    string get_type() override {
        return "list";
    }

    vector<statement*> get_vector_statements() override {
        return statements;
    }

    statement_list() { }

    statement_list(statement_list&& other) noexcept : statements(move(other.statements)) { }
};

struct while_statement : public statement {
    statement_list* statements;
    expression exp;

    string get_type() override {
        return "while";
    }

    vector<variable> get_expr() override {
        return exp.values;
    }

    statement* get_list() override {
        return statements;
    }
};

struct eq_statement : public statement {
    variable var;
    expression exp;
    string eq_string;

    string get_string() override {
        return eq_string;
    }

    string get_var() override {
        return var.name;
    }

    string get_type() override {
        return "eq";
    }

    vector<variable> get_expr() override {
        return exp.values;
    }
};

struct if_statement : public statement {
    statement_list* statements;
    expression exp;

    string get_type() override {
        return "if";
    }

    vector<variable> get_expr() override {
        return exp.values;
    }

    statement* get_list() override {
        return statements;
    }
};


