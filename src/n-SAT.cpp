#include "n-SAT.h"

void init_regex() {
    for (int i = 0 ; i < NR_REGEX ; ++i) {
        regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    }
}

bool make_token(char *e, map<string, bool> *var, int *nr_var) {
    int position = 0;
    int i;
    regmatch_t pmatch;
    nr_token = 0;
    while (e[position] != '\0') {
        for (i = 0; i < NR_REGEX; i ++) {
            if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
                char *substr_start = e + position;
                int substr_len = pmatch.rm_eo;
                position += substr_len;
                switch (rules[i].token_type) {
                    case TK_NOTYPE:
                        break;
                    case TK_VAR:
                        strncpy(tokens[nr_token].str, substr_start, substr_len);
                        tokens[nr_token].str[substr_len] = '\0';
                        tokens[nr_token].type = rules[i].token_type;
                        if (var->find(string(tokens[nr_token].str)) == var->end()) {
                            ++*nr_var;
                            var->insert({string(tokens[nr_token].str), 0});
                        }
                        ++nr_token;
                        break;
                    case TK_AND:
                    case TK_OR:
                    case TK_NEG:
                    case '(':
                    case ')':
                        tokens[nr_token].type = rules[i].token_type;
                        ++nr_token;
                        break;
                }
                break;
            }
        }
        if (i == NR_REGEX) {
            cout << ANSI_FMT("Invalid string.", ANSI_FG_RED) << endl;
            return 0;
        }
    }
    if (*nr_var == 0) {
        cout << ANSI_FMT("Too few var(s).", ANSI_FG_RED) << endl;
        return 0;
    }
    if (*nr_var > MAX_NR_VAR) {
        cout << ANSI_FMT("Too many var(s).", ANSI_FG_RED) << endl;
        return 0;
    }
    cout << ANSI_FMT(to_string(*nr_var) + " var(s).", ANSI_FG_CYAN) << endl;
    return 1;
}

void gen_rpn(vector<Token> *rpn) {
    stack<Token> stk;
    Token top_token = {};
    for (int i = 0 ; i < nr_token ; ++i) {
        switch (tokens[i].type) {
            case TK_VAR:
                rpn->push_back(tokens[i]);
                break;
            case '(':
                stk.push(tokens[i]);
                break;
            case ')':
                while (!stk.empty()) {
                    top_token = stk.top();
                    stk.pop();
                    if (top_token.type == '(') break;
                    rpn->push_back(top_token);
                }
                break;
            default:
                while (!stk.empty() && get_priority(stk.top().type) > get_priority(tokens[i].type)) {
                    top_token = stk.top();
                    stk.pop();
                    rpn->push_back(top_token);
                }
                if (stk.empty() || get_priority(stk.top().type) < get_priority(tokens[i].type)) {
                    stk.push(tokens[i]);
                } else {
                    if (tokens[i].type == TK_NEG) {
                        stk.push(tokens[i]);
                    } else {
                        rpn->push_back(tokens[i]);
                    }
                }
        }
    }
    while (!stk.empty()) {
        rpn->push_back(stk.top());
        stk.pop();
    }
}

int get_priority(int type) {
    switch (type) {
        case '(':
            return 0;
        case TK_OR:
            return 1;
        case TK_AND:
            return 2;
        case TK_NEG:
            return 3;
        default:
            return -1;
    }
}

bool eval(vector<Token> rpn, map<string, bool> var, bool *ans) {
    stack<bool> stk;
    bool opt1 = 0, opt2 = 0;
    for (Token tk : rpn) {
        switch (tk.type) {
            case '(':
            case ')':
                return 0;
            case TK_VAR:
                stk.push(var[string(tk.str)]);
                break;
            case TK_AND:
                if (stk.size() < 2) return 0;
                opt1 = stk.top();
                stk.pop();
                opt2 = stk.top();
                stk.pop();
                stk.push(opt1 & opt2);
                break;
            case TK_OR:
                if (stk.size() < 2) return 0;
                opt1 = stk.top();
                stk.pop();
                opt2 = stk.top();
                stk.pop();
                stk.push(opt1 | opt2);
                break;
            case TK_NEG:
                if (stk.size() < 1) return 0;
                opt1 = stk.top();
                stk.pop();
                stk.push(!opt1);
                break;
            default:
                return 0;
        }
    }
    if (stk.empty()) return 0;
    *ans = stk.top();
    return 1;
}