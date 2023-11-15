#pragma once
#include <string>
#include <fstream>
#include<vector>
#include"dataDef.h"
using namespace std;
# define KEY_NUM 6

struct Node {
    int code;//�ֱ���
    string token;//��Ӧ���ַ���
};
/*
@brief �ֱ���
*/
enum code {
    NUM = 0,   // 0 ��ֵ
    ID,        // 1 ��ʶ��
    INT,       // 2 int
    VOID,      // 3 void
    IF,        // 4 if
    ELSE,      // 5 else
    WHILE,     // 6 while
    RETURN,    // 7 return 
    PLUS,      // 8 ���+
    MIUS,      // 9 -
    ASSIGN,    // 10 =
    NEQ,       // 11 !=
    EQ,        // 12 ==
    LEQ,       // 13 <=
    GEQ,       // 14 >=
    GT,        // 15 >
    LT,        // 16 <
    MUL,       // 17 *
    DIV,       // 18 ����
    LR,        // 19 (
    RR,        // 20 )
    LB,        // 21 {
    RB,        // 22 }
    LK,		// 23 [
    RK,		// 24 ]
    SEMI, 	// 25 ;
    COMMA,    // 26 ,
    END,       // 27 #
    NOTE,      // 28 ע��
    ERROR,     // 29 ����
};

//�ʷ�������
class Lex {
    private:
        string file_name;
        string target_name;
        Node keys[KEY_NUM];//�洢�ؼ�����Ϣ������
        const string keyword[KEY_NUM] = { "int","void","if","else","while","return" };
        vector <pair<string,int>>result;//�洢�ʷ��������
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
            {RETURN, "RET"},
            {PLUS, "+"},
            {MIUS, "-"},
            {ASSIGN, "ASSIGN"},
            {NEQ, "!="},
            {EQ, "=="},
            {LEQ, "<="},
            {GEQ, ">="},
            {GT, ">"},
            {LT, "<"},
            {MUL, "*"},
            {DIV, "/"},
            {LR, "("},
            {RR, ")"},
            {LB, "{"},
            {RB, "}"},
            {LK, "["},
            {RK, "]"},
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

