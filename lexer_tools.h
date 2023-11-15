#pragma once
#include <string>
#include <fstream>
#include<vector>
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
    NUM = 0,//数值
    ID,//标识符
    INT,//int
    VOID,//void
    IF,//if
    ELSE,//else
    WHILE,//while
    RETURN,//return 
    PLUS,//算符+
    MIUS,//-
    ASSIGN,//=
    NEQ,//!=
    EQ,//==
    LEQ,//<=
    GEQ,//>=
    GT,//>
    LT,//<
    MUL,//*
    DIV,//除号
    LR,//(
    RR,//)
    LB,//{
    RB,//}
    SEMI,//;
    COMMA,//,
    END,//#
    NOTE,//注释
    ERROR,//错误
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
        pair<string, int>  GetNextToken();
        Lex(string fname, string tar) { file_name = fname; target_name = tar; };
        bool isNum(const char src);
        bool isLetter(const char src);
        bool isKeyWord(string src, int& code);
        void scan(ifstream& fin, ofstream& fout);
        void init_node();
        void BackIndex() {if (index) { --index; }};
};

