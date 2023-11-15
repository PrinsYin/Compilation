#pragma once
#include <string>
#include <fstream>
#include<vector>
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
    NUM = 0,//��ֵ
    ID,//��ʶ��
    INT,//int
    VOID,//void
    IF,//if
    ELSE,//else
    WHILE,//while
    RETURN,//return 
    PLUS,//���+
    MIUS,//-
    ASSIGN,//=
    NEQ,//!=
    EQ,//==
    LEQ,//<=
    GEQ,//>=
    GT,//>
    LT,//<
    MUL,//*
    DIV,//����
    LR,//(
    RR,//)
    LB,//{
    RB,//}
    SEMI,//;
    COMMA,//,
    END,//#
    NOTE,//ע��
    ERROR,//����
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
        pair<string, int>  GetNextToken();
        Lex(string fname, string tar) { file_name = fname; target_name = tar; };
        bool isNum(const char src);
        bool isLetter(const char src);
        bool isKeyWord(string src, int& code);
        void scan(ifstream& fin, ofstream& fout);
        void init_node();
        void BackIndex() {if (index) { --index; }};
};

