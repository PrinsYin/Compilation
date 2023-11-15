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
#define EXT_CHAR "'"  //拓广文法加入的标记



class Parser {

public:
	vector<vector<vector<string>>> stackTable;  //分析表
	set<string> VT;//终结符
	set<string> VN;//非终结符
	map<string, set<string>> FIRST;//first集
	vector<Grammar> rules;//所有产生式
	//vector<set<Project>>  lr1projects;  //SR(1)文法的所有项目
	vector<set<Project>>  lr1states; //项目集规范族
	map<pair<string, string>, string> GO;  //GO集合，例如GO(I0,1)=I2
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


	vector<string> strStack;    //输入串
	vector<string> strState;    //状态栈
	vector<string> strSymbol;   //符号栈
	/*
	enum codefor {
		NUM = 0,//数值
		ID,//标识符
		INTT,//int
		VOIDD,//void
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
