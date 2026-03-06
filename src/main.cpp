#include "n-SAT.h"

int main(int argc, char *argv[]) {
    ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);
    char *str = NULL;
    str = (char *)malloc(sizeof(char) * ARRLEN(tokens));
    cout << ANSI_FMT("[input] ", ANSI_FG_CYAN) << flush;
    cin >> str;
    if (strlen(str) >= ARRLEN(tokens)) {
        cout << ANSI_FMT("String too long.", ANSI_FG_RED) << endl;
        return 1;
    }
    cout << ANSI_FMT(str, ANSI_FG_CYAN) << endl;
    init_regex();
    map<string, bool> var;
    int nr_var = 0;
    if (!make_token(str, &var, &nr_var)) {
        return 1;
    }
    vector<Token> rpn;
    gen_rpn(&rpn);
    int param = 1 << nr_var;
    bool ans = 0;
    for (pair<string, bool> pi : var) {
        cout << ANSI_FMT(pi.first, ANSI_FG_GREEN) << ' ';
    }
    cout << endl;
    for (int i = 0 ; i < param ; ++i) {
        int bit = 0;
        for (pair<const string, bool> &pi : var) {
            pi.second = (i >> bit) & 1;
            ++bit;
        }
        if (!eval(rpn, var, &ans)) {
            cout << ANSI_FMT("Invalid string.", ANSI_FG_RED) << endl;
            return 1;
        }
        if (ans) {
            for (pair<string, bool> pi : var) {
                cout << ANSI_FMT(to_string(pi.second), ANSI_FG_YELLOW) << ' ';
            }
            cout << endl;
        }
    }
    free(str);
    return 0;
}