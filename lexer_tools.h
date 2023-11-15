#pragma once
#include <string>
#include <fstream>
#include<vector>
#include"dataDef.h"
using namespace std;
# define KEY_NUM 6

struct Node {
    int code;//种别码
    string token;//对应的字符串
};
/*
@brief 种别码
*/
enum code {
    NUM = 0,   // 0 数值
    ID,        // 1 标识符
    INT,       // 2 int
    VOID,      // 3 void
    IF,        // 4 if
    ELSE,      // 5 else
    WHILE,     // 6 while
    RETURN,    // 7 return 
    PLUS,      // 8 算符+
    MIUS,      // 9 -
    ASSIGN,    // 10 =
    NEQ,       // 11 !=
    EQ,        // 12 ==
    LEQ,       // 13 <=
    GEQ,       // 14 >=
    GT,        // 15 >
    LT,        // 16 <
    MUL,       // 17 *
    DIV,       // 18 除号
    LR,        // 19 (
    RR,        // 20 )
    LB,        // 21 {
    RB,        // 22 }
    SEMI,      // 23 ;
    COMMA,     // 24 ,
    END,       // 25 #
    NOTE,      // 26 注释
    ERROR,     // 27 错误
};

//词法分析类
class Lex {
    private:
        string file_name;
        string target_name;
        Node keys[KEY_NUM];//存储关键字信息的数组
        const string keyword[KEY_NUM] = { "int","void","if","else","while","return" };
        vector <pair<string,int>>result;//存储词法分析结果
        int index = 0;
    public:
        /*unordered_map<code, string> getCodeName =
        {
            {NUM, "NUM"},
            {ID, "ID"},
            {INT, "INT"},
            {VOID, "VOID"},
            {IF, "IF"},
            {ELSE, "ELSE"},
            {WHILE, "WHILE"},
            {RETURN, "RETURN"},
            {PLUS, "PLUS"},
            {MIUS, "MIUS"},
            {ASSIGN, "ASSIGN"},
            {NEQ, "NEQ"},
            {EQ, "EQ"},
            {LEQ, "LEQ"},
            {GEQ, "GEQ"},
            {GT, "GT"},
            {LT, "LT"},
            {MUL, "MUL"},
            {DIV, "DIV"},
            {LR, "LR"},
            {RR, "RR"},
            {LB, "LB"},
            {RB, "RB"},
            {SEMI, "SEMI"},
            {COMMA, "COMMA"},
            {END, "END"},
            {NOTE, "NOTE"},
        };*/
        unordered_map<code, string> getCodeName =
        {
            {NUM, "NUM"},
            {ID, "ID"},
            {INT, "INT"},
            {VOID, "VOID"},
            {IF, "IF"},
            {ELSE, "ELSE"},
            {WHILE, "WHILE"},
            {RETURN, "RETURN"},
            {PLUS, "OP1"},
            {MIUS, "OP1"},
            {ASSIGN, "ASSIGN"},
            {NEQ, "CMP"},
            {EQ, "CMP"},
            {LEQ, "CMP"},
            {GEQ, "CMP"},
            {GT, "CMP"},
            {LT, "CMP"},
            {MUL, "OP2"},
            {DIV, "OP2"},
            {LR, "("},
            {RR, ")"},
            {LB, "{"},
            {RB, "}"},
            {SEMI, ";"},
            {COMMA, ","},
            {END, "END"},
            {NOTE, "NOTE"},
        };
        pair<string, string>  GetNextToken();
        Lex(string fname, string tar) { file_name = fname; target_name = tar; };
        bool isNum(const char src);
        bool isLetter(const char src);
        bool isKeyWord(string src, int& code);
        void scan(ifstream& fin, ofstream& fout);
        void init_node();
        void BackIndex() {if (index) { --index; }};
};

