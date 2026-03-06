#ifndef __N_SAT_H__
#define __N_SAT_H__

#include "common.h"
#include <regex.h>

#define ARRLEN(arr) (sizeof(arr) / sizeof(arr[0]))

enum TK {
    TK_NOTYPE = 256,
    TK_VAR,
    TK_AND,
    TK_OR,
    TK_NEG
};

struct Token {
    int type;
    char str[32];
};

static struct Rule {
    const char *regex;
    int token_type;
} rules[] = {
    {" +", TK_NOTYPE},
    {"\\(", '('},
    {"\\)", ')'},
    {"&", TK_AND},
    {"\\|", TK_OR},
    {"!", TK_NEG},
    {"[a-zA-Z]+", TK_VAR}
};

#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};
static Token tokens[512] = {};
static int nr_token = 0;

void init_regex();

bool make_token(char *e, map<string, bool> *var, int *nr_token);

void gen_rpn(vector<Token> *rpn);

int get_priority(int type);

bool eval(vector<Token> rpn, map<string, bool> var, bool *ans);

#define MAX_NR_VAR 16

#endif