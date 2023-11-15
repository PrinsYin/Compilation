#pragma once
#include<iostream>
#include<vector>
#include<set>
#include<fstream>
#include<algorithm>
#include<sstream>
#include<stack>
#include<map>

#include"lexer_tools.h"
using namespace std;
#define START "PROGRAM"
#define EXT_CHAR "'"  //�ع��ķ�����ı��



class Parser {

public:
	vector<vector<vector<string>>> stackTable;  //������
	set<string> VT;//�ս��
	set<string> VN;//���ս��
	map<string, set<string>> FIRST;//first��
	vector<Grammar> rules;//���в���ʽ
	//vector<set<Project>>  lr1projects;  //SR(1)�ķ���������Ŀ
	vector<set<Project>>  lr1states; //��Ŀ���淶��
	map<pair<string, string>, string> GO;  //GO���ϣ�����GO(I0,1)=I2
	map<pair<string, string>, string> GOTO;
	map<pair<string, string>, string> ACTION;

	set<string> getStrFIRST(const vector<string>& Ba);

	Parser(const string& filename);
	bool loadGrammar(const string& filename);
	void getVT();
	void getVN();
	void getFIRST();
	set<Project> getCLOSURE(const set<Project>& I);
	map <string, set<Project>> getLR1States();
	void getGOTO();
	void getACTION();
	void drawTree(TreeNode* root);
	void GetTree(fstream &fp,TreeNode* p);
	string tryParse(const string& LexResStr, ifstream& fin, ofstream& fout);


	TreeNode* root;
	int errorLine;
	string errorToken;
	
	vector<string>ProcessNameTable;


	vector<string> strStack;    //���봮
	vector<string> strState;    //״̬ջ
	vector<string> strSymbol;   //����ջ
	/*
	enum codefor {
		NUM = 0,//��ֵ
		ID,//��ʶ��
		INTT,//int
		VOIDD,//void
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
		NONE,//
	};
	
	unordered_map<string, codefor> getEnum = {
		{"NUM", NUM},
		{"ID", ID},
		{"INT", INTT},
		{"VOID", VOIDD},
		{"IF", IF},
		{"ELSE", ELSE},
		{"WHILE", WHILE},
		{"RETURN", RETURN},
		{"PLUS", PLUS},
		{"MIUS", MIUS},
		{"ASSIGN", ASSIGN},
		{"NEQ", NEQ},
		{"EQ", EQ},
		{"LEQ", LEQ},
		{"GEQ", GEQ},
		{"GT", GT},
		{"LT", LT},
		{"MUL", MUL},
		{"DIV", DIV},
		{"LR", LR},
		{"RR", RR},
		{"LB", LB},
		{"RB", RB},
		{"SEMI", SEMI},
		{"COMMA", COMMA},
		{"END", END},
		{"NOTE", NOTE},
		{"none", NONE},
	};
	std::unordered_map<codefor,string> getEnumName = {
	{NUM, "NUM"},
	{ID, "ID"},
	{INTT, "INT"},
	{VOIDD, "VOID"},
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
	{NONE, "none"},
	};*/

	//Parser();
	//~Parser();

};
