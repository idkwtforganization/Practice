#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <cstdio>
#include <cctype>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

enum type_of_lex {
    LEX_NULL,                                                                                   /* 0*/
    LEX_AND, LEX_REAL, LEX_DO, LEX_INT,                                                         /* 9*/
    LEX_NOT, LEX_OR, LEX_PROGRAM, LEX_READ, LEX_WHILE, LEX_WRITE, LEX_GOTO, LEX_BREAK, LEX_STRING,                    /*18*/
    LEX_IF, LEX_ELSE, LEX_FALSE, LEX_TRUE,
    LEX_FIN,                                                                                    /*19*/
    LEX_SEMICOLON, LEX_COMMA, LEX_COLON, LEX_ASSIGN, LEX_LPAREN, LEX_RPAREN, LEX_EQ, LEX_LSS,   /*27*/
    LEX_GTR, LEX_PLUS, LEX_MINUS, LEX_TIMES, LEX_SLASH, LEX_LEQ, LEX_NEQ, LEX_GEQ,              /*35*/
    LEX_START, LEX_FINISH,                                                                      /*37*/
    LEX_NUM,                                                                                    /*38*/
    LEX_ID,
    LEX_STR,
    LEX_REALBODY,                                                                                   /*39*/
    POLIZ_LABEL,                                                                                /*40*/
    POLIZ_ADDRESS,                                                                              /*41*/
    POLIZ_GO,                                                                                   /*42*/
    POLIZ_FGO                                                                                   /*43*/
};

/////////////////////////  Класс Lex  //////////////////////////
class Lex {
    type_of_lex t_lex;
    int v_lex;
    string str_v_lex;
    double real_v_lex;
public:
    Lex(type_of_lex t = LEX_NULL, int v = 0, string s = "", double r = 0)
        : t_lex(t), v_lex(v), str_v_lex(s), real_v_lex(r) {}
    type_of_lex get_type() const {
        return t_lex;
    }
    int get_value() const {
        return v_lex;
    }
    string get_str_value() const {
        return str_v_lex;
    }
    double get_real_value() const {
        return real_v_lex;
    }
    friend ostream& operator<<(ostream& s, Lex l);
};

/////////////////////  Класс Ident  ////////////////////////////

class Ident {
    string name;
    bool declare;
    type_of_lex type;
    bool assign;
    int value;
public:
    Ident() {
        declare = false;
        assign = false;
    }
    bool operator==(const string& s) const {
        return name == s;
    }
    Ident(const string n) {
        name = n;
        declare = false;
        assign = false;
    }
    string get_name() const {
        return name;
    }
    bool get_declare() const {
        return declare;
    }
    void put_declare() {
        declare = true;
    }
    type_of_lex get_type() const {
        return type;
    }
    void put_type(type_of_lex t) {
        type = t;
    }
    bool get_assign() const {
        return assign;
    }
    void put_assign() {
        assign = true;
    }
    int get_value() const {
        return value;
    }
    void put_value(int v) {
        value = v;
    }
};

//////////////////////  TID  ///////////////////////

vector<Ident> TID;

int put(const string& buf) {
    vector<Ident>::iterator k;

    if ((k = find(TID.begin(), TID.end(), buf)) != TID.end())
        return k - TID.begin();
    TID.push_back(Ident(buf));
    return TID.size() - 1;
}

double string_to_double(const string& s) {
    istringstream i(s);
    double x;
    if (!(i >> x))
        return 0;
    return x;
}
/////////////////////////////////////////////////////////////////

class Scanner {
    FILE* fp;
    char c;
    int look(const string buf, const char** list) {
        int i = 0;
        while (list[i]) {
            if (buf == list[i])
                return i;
            ++i;
        }
        return 0;
    }
    void gc() {
        c = fgetc(fp);
    }
public:
    static const char* TW[], * TD[];
    Scanner(const char* program) {
        if (!(fp = fopen(program, "r")))
            throw "can’t open file";
    }
    Lex get_lex();
};

const char*
Scanner::TW[] = { "", "and", "real", "do",
                 "int", "not", "or", "program",
                 "read", "while", "write", "goto", "break",
                 "string", "if", "else", "false", "true", NULL };

const char*
Scanner::TD[] = { "@", ";", ",", ":", "=", "(", ")", "==",
                 "<", ">", "+", "-", "*", "/", "<=", "!=",
                 ">=", "{", "}", NULL };

Lex Scanner::get_lex() {
    enum state { H, IDENT, NUMB, COM, ALE, NEQ, STR, REAL };
    int d, j, t = 10;
    double dd;
    string buf;
    state CS = H;
    do {
        gc();
        switch (CS) {
        case H:
            if (c == ' ' || c == '\n' || c == '\r' || c == '\t');
            else if (isalpha(c)) {
                buf.push_back(c);
                CS = IDENT;
            }
            else if (c == '"') {
                buf.push_back(c);
                CS = STR;
            }
            else if (isdigit(c)) {
                d = c - '0';
                CS = NUMB;
            }
            else if (c == '<' || c == '>' || c == '=') {
                buf.push_back(c);
                CS = ALE;
            }
            else if (int(c) == EOF)
                return Lex(LEX_FIN);
            else if (c == '!') {
                buf.push_back(c);
                CS = NEQ;
            }
            else if (c == '/') {
                gc();
                if (c == '*')
                    CS = COM;
                else {
                    ungetc(c, fp);
                    return Lex(LEX_SLASH, 13);
                }
            }
            else {
                buf.push_back(c);
                if ((j = look(buf, TD))) {
                    return Lex((type_of_lex)(j + (int)LEX_FIN), j);
                }
                else
                    throw c;
            }
            break;
        case IDENT:
            if (isalpha(c) || isdigit(c)) {
                buf.push_back(c);
            }
            else {
                ungetc(c, fp);
                if ((j = look(buf, TW))) {
                    return Lex((type_of_lex)j, j);
                }
                else {
                    j = put(buf);
                    return Lex(LEX_ID, j);
                }
            }
            break;
        case NUMB:
            if (isdigit(c)) {
                d = d * 10 + (c - '0');
            }
            else if (c == '.') {
                dd = d;
                CS = REAL;
            }
            else {
                ungetc(c, fp);
                return Lex(LEX_NUM, d);
            }
            break;
        case REAL:
            if (isdigit(c)) {
                dd = dd + (1.0 * (c - '0')) / t;
                t *= 10;
            }
            else if (int(c) == EOF) {
                throw c;
            }
            else
                return Lex(LEX_REALBODY, 0, "", dd);
        case COM:
            if (c == '*') {
                gc();
                if (c == '/')
                    CS = H;
                else if (int(c) == EOF)
                    throw c;
            }
            else if (int(c) == EOF)
                throw c;
            break;
        case STR:buf.push_back(c);
            if (c == '"')
                return Lex(LEX_STR, 0, buf);
            else if (int(c) == EOF)
                throw c;
            break;
        case ALE:
            if (c == '=') {
                buf.push_back(c);
                j = look(buf, TD);
                return Lex((type_of_lex)(j + (int)LEX_FIN), j);
            }
            else {
                ungetc(c, fp);
                j = look(buf, TD);
                return Lex((type_of_lex)(j + (int)LEX_FIN), j);
            }
            break;
        case NEQ:
            if (c == '=') {
                buf.push_back(c);
                j = look(buf, TD);
                return Lex(LEX_NEQ, j);
            }
            else
                throw '!';
            break;
        } //end switch
    } while (true);
}

ostream& operator<<(ostream& s, Lex l) {
    string t;
    if (l.t_lex < LEX_FIN)
        t = Scanner::TW[l.t_lex];
    else if (l.t_lex >= LEX_FIN && l.t_lex < LEX_NUM)
        t = Scanner::TD[l.t_lex - LEX_FIN];
    else if (l.t_lex == LEX_NUM)
        t = "NUMB";
    else if (l.t_lex == LEX_ID)
        t = TID[l.v_lex].get_name();
    else if (l.t_lex == LEX_STR) {
        t = "STR";
        s << '(' << t << ',' << l.str_v_lex << ");" << endl;
        return s;
    }
    else if (l.t_lex == LEX_REALBODY) {
        t = "REAL NUMB";
        s << '(' << t << ',' << l.real_v_lex << ");" << endl;
        return s;
    }
    else
        throw l;
    s << '(' << t << ',' << l.v_lex << ");" << endl;
    return s;
}

//////////////////////////  Класс Parser  /////////////////////////////////
template<class T, class T_EL>
void from_st(T& st, T_EL& i) {
    i = st.top();
    st.pop();
}

class Parser {
    Lex curr_lex;
    type_of_lex c_type;
    int c_val;
    bool flag = true;
    Scanner scan;
    stack<int> st_int;
    stack<type_of_lex> st_lex;
    void P();
    void D1();
    void D();
    void O();
    void O1();
    void Oor();
    void Oeq();
    void Oand();
    void Oplusminus();
    void Oper();
    void Operand();
    void B();
    void V();
    void C();
    void S();
    void E();
    void E1();
    void T();
    void F();
    void dec(type_of_lex type);
    void check_id();
    void check_op();
    void check_not();
    void eq_type();
    void eq_bool();
    void check_id_in_read();
    void gl() {
        curr_lex = scan.get_lex();
        c_type = curr_lex.get_type();
        c_val = curr_lex.get_value();
    }
public:
    vector<Lex> poliz;
    Parser(const char* program) : scan(program) {}
    void analyze();
};

void Parser::analyze() {
    gl();
    P();
    if (c_type != LEX_FIN)
        throw curr_lex;
    cout << endl << "There are no problems" << endl;
}

void Parser::P() {
    if (c_type == LEX_PROGRAM) {
        gl();
    }
    else throw curr_lex;
    if (c_type == LEX_START)
        gl();
    else throw curr_lex;
    D();
    O();
    if (c_type == LEX_FINISH) {
        gl();
    }
    else throw curr_lex;
}

void Parser::D()
{
    D1();
    while (c_type == LEX_SEMICOLON) {
        gl();
        D1();
    }
}

void Parser::D1()
{
    if (c_type == LEX_INT or c_type == LEX_STRING or c_type == LEX_REAL) {
        gl();
        V();
        while (c_type == LEX_COMMA) {
            gl();
            V();
        }
    }
}

void Parser::V() {
    if (c_type == LEX_ID) {
        gl();
        if (c_type == LEX_ASSIGN) {
            gl();
            C();
        }
    }
    else throw curr_lex;
}

void Parser::C() {
    if (c_type == LEX_PLUS or c_type == LEX_MINUS) {
        gl();
        if (c_type == LEX_NUM) {
            gl();
        }
        else if (c_type == LEX_REALBODY) {
            gl();
        }
        else throw curr_lex;
    }
    else if (c_type == LEX_NUM) {
        gl();
    }
    else if (c_type == LEX_REALBODY) {
        gl();
    }
    else if (c_type == LEX_STR) {
        gl();
    }
    else throw curr_lex;
}

void Parser::O() { //operators
    O1();
    while (flag) {
        O1();
    }
}

void Parser::O1() {
    if (c_type == LEX_ID) {
        gl();
        if (c_type == LEX_ASSIGN) {
            gl();
            check_id();
            Oor();
            while (c_type == LEX_ASSIGN) {
                gl();
                Oeq();
            }
            if (c_type == LEX_SEMICOLON) {
                gl();
            }
            else throw curr_lex;
        }
        else if (c_type == LEX_COLON) { //for goto operator
            gl();
            O1();
        }
        else throw curr_lex;
    }
    else if (c_type == LEX_IF) {
        gl();
        if (c_type == LEX_LPAREN) {
            gl();
            Oeq();
            eq_bool();
        }
        else throw curr_lex;
        if (c_type == LEX_RPAREN) {
            gl();
            O1();
            if (c_type != LEX_ELSE) {
                //
            }
            else {
                //
            }
            if (flag == 0) throw curr_lex;
        }
        else throw curr_lex;
        if (c_type == LEX_ELSE) {
            gl();
            O1();
            if (flag == 0) throw curr_lex;
        }
    }
    else if (c_type == LEX_WHILE) {
        gl();
        if (c_type == LEX_LPAREN) {
            gl();
            Oeq();
            eq_bool();
        }
        else throw curr_lex;
        if (c_type == LEX_RPAREN) {
            gl();
            O1();
            if (flag == 0) throw curr_lex;
        }
    }
    else if (c_type == LEX_READ) {
        gl();
        if (c_type == LEX_LPAREN) {
            gl();
            if (c_type == LEX_ID) {
                check_id_in_read();
                gl();
            }
            else throw curr_lex;
            if (c_type == LEX_RPAREN) {
                gl();
            }
            else throw curr_lex;
            if (c_type == LEX_SEMICOLON) {
                gl();
            }
            else throw curr_lex;
        }
        else throw curr_lex;
    }
    else if (c_type == LEX_WRITE) {
        gl();
        if (c_type == LEX_LPAREN) {
            gl();
            Oeq();
            while (c_type == LEX_COMMA) {
                gl();
                Oeq();
            }
            if (c_type == LEX_RPAREN) {
                gl();
            }
            else throw curr_lex;
            if (c_type == LEX_SEMICOLON) {
                gl();
            }
            else throw curr_lex;
        }
        else throw curr_lex;
    }
    else if (c_type == LEX_GOTO) {
        gl();
        if (c_type == LEX_ID) {
            //Later
            gl();
        }
        else throw curr_lex;
        if (c_type == LEX_SEMICOLON) {
            gl();
        }
        else throw curr_lex;
    }
    else if (c_type == LEX_BREAK) {
        gl();
        if (c_type == LEX_SEMICOLON) {
            gl();
        }
        else throw curr_lex;
    }
    else if (c_type == LEX_START) {
        gl();
        O1();
        if (!flag) {
            flag == true;
        }
        while (c_type != LEX_FINISH) {
            O1();
            if (flag == false) {
                break;
            }
        }
        if (c_type == LEX_FINISH) {
            gl();
        }
        else throw curr_lex;
    }
    else {
        flag = false;
    }
}

void Parser::Oeq() {
    Oor();
    while (c_type == LEX_ASSIGN) {
        gl();
        Oor();
    }
}

void Parser::Oor() {
    Oand();
    while (c_type == LEX_OR) {
        gl();
        Oand();
        //check_op();
    }
}

void Parser::Oand() {
    E();
    while (c_type == LEX_AND) {
        gl();
        E();
        //check_op();
    }
}

void Parser::E() {
    Oplusminus();
    while (c_type == LEX_NEQ or c_type == LEX_LSS or c_type == LEX_GTR
        or c_type == LEX_GEQ or c_type == LEX_LEQ or c_type == LEX_EQ) {
        gl();
        Oplusminus();
        //check_op();
    }
}

void Parser::Oplusminus() {
    Oper();
    while (c_type == LEX_PLUS or c_type == LEX_MINUS) {
        gl();
        Oper();
        //check_op();
    }
}

void Parser::Oper() {
    Operand();
    while (c_type == LEX_SLASH or c_type == LEX_TIMES) {
        gl();
        Operand();
        //check_op();
    }
}

void Parser::Operand() {
    if (c_type == LEX_PLUS or c_type == LEX_MINUS) {
        gl();
        if (c_type == LEX_NUM) {
            gl();
        }
        else if (c_type == LEX_REALBODY) {
            gl();
        }
        else throw curr_lex;
    }
    else if (c_type == LEX_ID) {
        check_id();
        gl();
    }
    else if (c_type == LEX_STR) {
        gl();
    }
    else if (c_type == LEX_NUM) {
        gl();
    }
    else if (c_type == LEX_NOT) {
        gl();
        Operand();
    }
    else if (c_type == LEX_LPAREN) {
        gl();
        Oeq();
        if (c_type == LEX_RPAREN) {
            gl();
        }
        else throw curr_lex;
    }
    else throw curr_lex;
}


void Parser::check_id_in_read() {
    //Later
}

void Parser::eq_bool() {
    //Later
}

void Parser::check_id() {
    if (c_type != LEX_ID)
        return;
}

int main() {
    try {
        Parser pars("prog.txt");
        pars.analyze();
    }
    catch (char c) {
        cout << "unexpected symbol " << c << endl;
        return 1;
    }
    catch (Lex l) {
        cout << "unexpected lexeme" << l << endl;
        return 1;
    }
    catch (const char* source) {
        cout << source << endl;
        return 1;
    }
}
