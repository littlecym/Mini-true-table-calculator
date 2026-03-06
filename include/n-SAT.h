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
    TK_NEG,
    TK_IMP,
    TK_IFF
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
    {"[a-zA-Z]+", TK_VAR},
    {"<->", TK_IFF},
    {"->", TK_IMP}
};

#define NR_REGEX ARRLEN(rules)

#define SETW(str, fmt, len) ANSI_FMT(truncate(str, len - 1), fmt) << fillBlank(truncate(str, len - 1), len)

static regex_t re[NR_REGEX] = {};
static Token tokens[512] = {};
static int nr_token = 0;

void init_regex();

bool make_token(char *e, map<string, bool> *var, int *nr_token);

void gen_rpn(vector<Token> *rpn);

int get_priority(int type);

bool eval(vector<Token> rpn, map<string, bool> var, bool *ans);

string truncate(string str, size_t len);

string fillBlank(string str, size_t len);

#define MAX_NR_VAR 16

#define ALIGN 8

#endif