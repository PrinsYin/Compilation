#include<iostream>
#include<vector>
#include<set>
#include<string>
#include<fstream>
#include<map>
#include <stdlib.h> 
#include<algorithm>
#include"parser.h"
using namespace std;


set<string> Parser::getStrFIRST(const vector<string>& Ba)
{
	set<string> res;
	if (Ba.size() == 0)
	{
		return res;
	}
	else
		return FIRST[Ba[0]];
}

Parser::Parser(const string& filename)
{
	errorLine = 0;
	errorToken = "";///？？？？？？？？？？？？？？？？？？

	loadGrammar(filename);
	getVN();
	getVT();
	getFIRST();
	getLR1States();
	getACTION();
	getGOTO();

}

bool Parser::loadGrammar(const string& filename)
{
	// 打开文件
	string line, str;
	Grammar g;
	ifstream file(filename, ios::binary | ios::in);
	if (!file.is_open())
		return false; // 文件打开失败，返回失败标志

	// 临时变量和结构体的声明
	

	// 逐行读取文件内容
	while (getline(file, line))
	{
		
		if (line.empty())
			continue;
		g.right.clear();
		stringstream lstream(line);
		lstream >> g.left;
		lstream >> str;
		while (lstream >> str)
		{
			g.right.push_back(str);
		}
		rules.push_back(g);
	}

	// ?????????????????
	g.right.clear();
	g.left = rules[0].left + EXT_CHAR;
	g.right.push_back(rules[0].left);
	rules.insert(rules.begin(), g);

	return true; 
}


void Parser::getVN()
{
	for (const auto& rule : rules)
		VN.insert(rule.left);
}

void Parser::getVT()
{
	for (auto& rule : rules)
	{
		for (auto& ri : rule.right)
			if (!VN.count(ri))
				VT.insert(ri);
	}
	VT.insert("#");//空
}

void Parser::getFIRST()
{
	for (const auto& vt : VT) {
		FIRST[vt] = { vt };
	}

	vector<int> list;	//记录产生式右部第一个符号为非终结符的文法
	for (auto it = rules.begin(); it != rules.end(); ++it)
	{
			string first = it->right.front();
			if (VT.count(first))
				FIRST[it->left].insert(first);
			else
				list.push_back(distance(rules.begin(), it));
	}

	bool flag,ep;
	while (1) {
		flag = false;
		for (const auto& i : list) {
			ep = false;
			for (const auto& elem1 : rules[i].right)
			{
				ep = false;
				if (VN.count(elem1))
				{
					//考虑A->A..的特殊情况
					if (rules[i].left == elem1) {
							break;
					}
					//若出现A->B...,则将B的first集全部加到A中
					for (const auto& elem2 : FIRST[elem1]) 
						if (FIRST[rules[i].left].insert(elem2).second) 
							flag = true;
					if (!ep)
						break;	//若不含空,则后续不用继续加入
				}//?????????????????????????ep????????
				else
					break;
			}

		}
		if (!flag)	//如果first集不再增加,则返回
			break;
	}
}
///122 ppt
set<Project> Parser::getCLOSURE(const set<Project>& I)
{

	set< Project > closure(I);			//project_set自身的所有项目都在闭包中
	set<Project> old_project(I);	//辅助集合
	set<Project> new_project;
	int before = 0,after=0;
	bool flag;
	while (true) {
		before = old_project.size();
		flag = false;
		for (const auto& i : old_project) {	//扫描上一次产生的所有项目
			Grammar rule = rules[i.index];
			if (rule.right.size() > i.point && VN.count((rule.right[i.point]))) //为非终结符且不是最后一个符号
			{
				//A->α.Bβ型
				string B = rule.right[i.point];
				//求出first(βa)
				set<string> firstAfterDot;
				vector<string> symbolsAfterDot;

				
				for (auto symbol = rules[i.index].right.begin() + i.point + 1; symbol != rules[i.index].right.end(); symbol++)
				{
					symbolsAfterDot.push_back(*symbol);
				}
				if(symbolsAfterDot.size() == 0)
				{
					for (const auto& follow : i.follow)
						firstAfterDot.insert(follow);
				}
				else
					firstAfterDot=FIRST[symbolsAfterDot[0]];//???????????

				for (int j = 0; j < rules.size(); j++) 
				{
					//扫描所有B->γ型的产生式
					if (rules[j].left == B) {
						//若CLOSURE中不存在{B->γ,firstba},则加入
						bool flag1 = false;
						for (auto it = closure.begin(); it != closure.end(); ++it) {
							if (it->index == j && it->point == 0) {
								//项目在集合
								flag1 = true;
								set<string>temp;
								set_intersection(it->follow.begin(), it->follow.end(), firstAfterDot.begin(), firstAfterDot.end(), inserter(temp, temp.begin()));
								if (temp!=firstAfterDot) {
									//若follows不完整,则插入新的follows
									flag = true;
									//由于集合元素的值无法修改,故只能覆盖之
									auto ngp = *it;
									for (auto firstba_elem : firstAfterDot)
										ngp.follow.insert(firstba_elem);
									closure.erase(it);
									closure.insert(ngp);
									new_project.insert(ngp);
								}
								break;
							}
						}
						if (!flag1) {
							//否则插入新项目
							flag = true;
							closure.insert({ j,0,firstAfterDot });
							new_project.insert({ j,0,firstAfterDot });
						}
					}
				}
			}
		}
		after = new_project.size();
		if (!flag)	//不再增加,则返回
			break;
		old_project = new_project;	//对新添加项目进行下一轮扫描
		new_project.clear();
	}

	return closure;
	
}
//124 GO(I X) CLOSURE(J)
////项目集规范组
map <string, set<Project>> Parser::getLR1States()
{
	set<string> tmp = { "#" };
	map <string, set<Project>> ProjectSet;
	set<set<Project>> C_sup;
	vector <set<Project> >C;

	Project init = { 0,0,tmp };
	set<Project> _GO, J = { init };
	_GO = getCLOSURE(J);
	C.emplace_back(_GO);
	C_sup.insert(J);
	ProjectSet["I0"] = C[0];
	int num = 1;

	for (auto i = 0; i < C.size(); i++)
	{
		for (auto& vn : VN)//非终结符
		{
			J.clear();
			for (auto& elem : C[i])//遍历每一个项目
			{
				for (auto j = 0; j < rules[elem.index].right.size(); j++)//找到项目中类似A->·vn的
				{
					if (rules[elem.index].right[j] == vn && elem.point == j)//找到
					{
						J.insert({ elem.index,elem.point + 1,elem.follow });//点后移
						break;
					}

				}
			}
			_GO = getCLOSURE(J);    //获得GO（C[i],vn）
			if (_GO.size() > 0)    //不为空集
			{
				if (!C_sup.count(_GO))
				{
					GO[{"I" + to_string(i), vn}] = "I" + to_string(num);
					C.emplace_back(_GO);
					C_sup.insert(_GO);
					ProjectSet["I" + to_string(num++)] = _GO;
				}
				else
				{
					vector<set<Project>>::iterator it = find(C.begin(), C.end(), _GO);
					vector<set<Project>>::iterator begin = find(C.begin(), C.end(), C.front());
					int index = it - begin;
					GO[{"I" + to_string(i), vn}] = "I" + to_string(index);
				}
			}
			else
				GO[{"I" + to_string(i), vn}] = "error";
		}

		for (auto& vt : VT)
		{
			J.clear();
			for (auto& elem : C[i])
			{
				for (int j = 0; j < rules[elem.index].right.size(); j++)
				{
					if (rules[elem.index].right[j] == vt && elem.point == j)
					{
						J.insert({ elem.index,elem.point + 1,elem.follow });
						break;
					}

				}
			}
			_GO = getCLOSURE(J);
			if (_GO.size() > 0)    //不为空集
			{
				if (!C_sup.count(_GO))
				{
					GO[{"I" + to_string(i), vt}] = "I" + to_string(num);
					C.emplace_back(_GO);
					C_sup.insert(_GO);
					ProjectSet["I" + to_string(num++)] = _GO;
				}
				else
				{
					vector<set<Project>>::iterator it = find(C.begin(), C.end(), _GO);
					vector<set<Project>>::iterator begin = find(C.begin(), C.end(), C.front());
					int index = it - begin;
					GO[{"I" + to_string(i), vt}] = "I" + to_string(index);
				}
			}
			else
				GO[{"I" + to_string(i), vt}] = "error";
		}
	}
	lr1states = C;
	return ProjectSet;
}


void Parser::getACTION()
{
	for (int i = 0; i < lr1states.size(); i++)
	{
		for (auto item : lr1states[i])
		{
			//接受
			if (rules[item.index].left == string(START) + EXT_CHAR && rules[item.index].right.front() == START && rules[item.index].right.size() == item.point)
			{
				ACTION[{"I" + to_string(i), "#"}] = "acc";
			}
			//移进
			else if (rules[item.index].right.size() > item.point)
			{
				string var = rules[item.index].right[item.point];
				if (VT.count(var))//是终结符
				{
					string tmp = GO[{"I" + to_string(i), var}];
					if (tmp != "error")
						tmp = GO[{"I" + to_string(i), var}].substr(1, tmp.length());
					ACTION[{"I" + to_string(i), var}] = string("s") + tmp;
				}
			}
			//规约
			else
			{
				for (auto fol : item.follow)
				{
					ACTION[{"I" + to_string(i),fol}] = string("r") + to_string(item.index);
				}
			}
		}
	}
	//不能填表的全赋为error
	for (int i = 0; i < lr1states.size(); i++)
	{
		for (auto vt : VT)
			if (ACTION[{"I" + to_string(i), vt}] == "")
				ACTION[{"I" + to_string(i), vt}] = "error";
	}
}

void Parser::getGOTO()
{
	for (auto vn : VN)
	{
		for (int i = 0; i < lr1states.size(); i++)
		{
			GOTO[{"I" + to_string(i),vn}] = GO[{"I" + to_string(i), vn}];
		}
	}
}

/*enum c111 {
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
//};
*/
//他这个没有分成op，语法必须得扩充，也不能用op
/*
string Parser::tryParse(const string& LexResStr)
{
	L.lexerOpen(LexResStr);

	 // 变量初始化
		int line = 1;
	vector<int> state;
	vector<pair<string, int>> symbol;
	pair<string, int> nextToken;
	pair<string, int> token = L.lexerAnalyse();
	pair<string, int> bubble = pair<string, int>(EPSILON, -1);
	TreeNode* tp = nullptr;
	stack<TreeNode*> treeNodeStack;
	fstream fp("parser.txt", ios::out | ios::binary);
	/// 初始压栈
	state.push_back(0);
	symbol.push_back(pair<string, int>(string("#"), -1));

	/// 循环语法分析
	while (token.first != "ERROR") {
		//换行重读
		if (token.first == "NL") {
			line++;
			token = L.lexerAnalyse();
			continue;
		}
		//出错终止
		if (find(VT.begin(), VT.end(), getEnumName[codefor(token.second)]) == VT.end()) {
			string msg = string("ERROR: ") + getEnumName[codefor(token.second)] + string(" is not a terminator.\n");
			fp << msg << endl;
			fp.close();
			return msg;
		}

		//动作状态机
		string action = ACTION[{"I" + to_string(state.back()), int(token.second)}];
		if (action == "acc") {
			fp << "Accept!" << endl;
			fp.close();
			root = tp;
			return "Accept!!!";
		}
		if (action == "error")
		{
			if (token != bubble) {
				nextToken = token;
				token = bubble;
			}
			else {
				string msg;
				msg = string("ERROR: Parser detected error on line ") + to_string(line) + string(" (") + getEnumName[codefor(token.second)] + string(").\n");
				fp << msg << endl;
				fp.close();
				return msg;
			}

		}
		if (action.substr(0, 1) == "s")  //移进
		{
			tp = new(nothrow)TreeNode;
			if (!tp) {
				string msg;
				msg = string("ERROR: Memory crashed on line ") + to_string(line) + string(" (") + getEnumName[codefor(token.second)] + string(").\n");
				fp << msg << endl;
				fp.close();
				return msg;
			}
			tp->data = {token.first,1};
			treeNodeStack.push(tp);

			//压栈
			state.push_back(atoi(action.substr(1, action.length()).c_str()));
			symbol.push_back(token);

			if (token == bubble)
				token = nextToken;
			else
				token = L.lexerAnalyse();

		}
		if (action.substr(0, 1) == "r")  //归约
		{
			int index = atoi(action.substr(1, action.length()).c_str());	//找到原文法中对应的索引
			int length = rules[index].right.size();	//得到原文法右边有几个符号，后续需要将状态栈和符号栈出栈length个
			for (int i = length-1; i >= 0; i--) {
				if (symbol.back().first == rules[index].right[i]) {
					symbol.pop_back();
					state.pop_back();
				}
				else {
					string msg;
					msg = string("ERROR: Parser detected error on line ") + to_string(line) + string(" (") + getEnumName[codefor(token.second)] + string(").\n");
					msg += string("-Note：") + symbol.back().first + string(" is different from ") + rules[index].right[i] + string(".\n");
					fp << msg << endl;
					fp.close();
					return msg;
				}
			}




			
			symbol.push_back(pair<string, int>(rules[index].left, -1));
			state.push_back(atoi(GOTO[{"I" + to_string(state.back()), symbol.back().first}].substr(1, GOTO[{"I" + to_string(state.back()), symbol.back().first}].length()).c_str()));

		}
		//保存一下 没有啥用
		strState.clear();
		strSymbol.clear();
		strStack.clear();
		for (int i = 0; i < state.size(); i++)
			strState.push_back(to_string(state[i]));
		for (int i = 0; i < symbol.size(); i++)
			strSymbol.push_back(symbol[i].first);
		strStack.push_back(getEnumName[codefor(token.second)]);

		stackTable.push_back({ strState, strSymbol, strStack });
	};
	return "error";
}*/

string Parser::tryParse(const string& LexResStr, ifstream& fin, ofstream& fout)
{
	
	Lex L("example1.txt", "lexerout.txt");
	L.scan(fin,fout);  // 打开词法分析器，处理输入字符串
	// 变量初始化
	int line = 1;
	vector<int> state;
	vector<pair<string, string>> symbol;
	pair<string, string> nextToken;
	pair<string, string> token = L.GetNextToken();  // 获取第一个令牌

	pair<string, string> bubble = pair<string, string>("none", "none");
	TreeNode* tp = nullptr;
	stack<TreeNode*> treeNodeStack;
	fstream fp("parser.txt", ios::out | ios::binary);

	// 初始压栈
	state.push_back(0);
	symbol.push_back(pair<string, string>(string("#"), string("#")));
	// 循环语法分析
	while (token.first != "ERROR") 
	{
		if (token.first == "NL") {
			line++;
			token = L.GetNextToken();
			continue;
		}

		if (find(VT.begin(), VT.end(), token.second) == VT.end())//如果不是止符，报错
		{
			string msg = string("ERROR: ") + (token.second) + string(" is not a terminator.\n");
			fp << msg << endl;
			fp.close();
			return msg;
		}

		// 动作状态机
		string action = ACTION[{"I" + to_string(state.back()), token.second}];
		if (action == "acc") {  // 如果动作是“接受”，则接受输入并返回
			fp << "Accept!" << endl;
			fp.close();
			root = tp;
			return "Accept!!!";
		}
		if (action == "error")
		{
			if (token != bubble) 
			{
				nextToken = token;
				token = bubble;
			}
			else 
			{
				string msg;
				msg = string("ERROR: Parser detected error on line ") + to_string(line) + string(" (") + token.second + string(").\n");
				fp << msg << endl;
				fp.close();
				return msg;
			}
		}
		if (action.substr(0, 1) == "s")  // 如果动作是“移进”，则将新状态和令牌压入堆栈，并获取下一个令牌
		{
			tp = new(nothrow)TreeNode;
			if (!tp) {
				string msg;
				msg = string("ERROR: Memory crashed on line ") + to_string(line) + string(" (") + token.second + string(").\n");
				fp << msg << endl;
				fp.close();
				return msg;
			}
			tp->data = { token.first,1 };
			treeNodeStack.push(tp);

			state.push_back(atoi(action.substr(1, action.length()).c_str()));
			symbol.push_back(token);

			if (token == bubble)
				token = nextToken;
			else
				token = L.GetNextToken();
		}
		if (action.substr(0, 1) == "r")  // 如果动作是“归约”，则根据规则从堆栈中弹出符号和状态，并将结果压入堆栈
		{
			int index = atoi(action.substr(1, action.length()).c_str());
			int length = rules[index].right.size();
			for (int i = length - 1; i >= 0; i--) {
				if (symbol.back().second== rules[index].right[i]) {
					symbol.pop_back();
					state.pop_back();
				}
				else {
					string msg;
					msg = string("ERROR: Parser detected error on line ") + to_string(line) + string(" (") + token.second + string(").\n");
					msg += string("-Note：") + symbol.back().second + string(" is different from ") + rules[index].right[i] + string(".\n");
					fp << msg << endl;
					fp.close();
					return msg;
				}
			}

			symbol.push_back(pair<string, string>(rules[index].left, rules[index].left));
			state.push_back(atoi(GOTO[{"I" + to_string(state.back()), symbol.back().second}].substr(1, GOTO[{"I" + to_string(state.back()), symbol.back().second}].length()).c_str()));
		}
		/*
		// 保存状态，无实际功能，但可能用于调试或记录
		strState.clear();
		strSymbol.clear();
		strStack.clear();
		for (int i = 0; i < state.size(); i++)
			strState.push_back(to_string(state[i]));
		for (int i = 0; i < symbol.size(); i++)
			strSymbol.push_back(symbol[i].first);
		strStack.push_back(token.second);

		stackTable.push_back({ strState, strSymbol, strStack });*/
	};
	return "error";
}

void Parser::GetTree(fstream &fp,TreeNode* p)
{
	static int i = 0;
	if (p) {
		string tmp = p->data.first;
		if (tmp == "{" || tmp == "}" || tmp == ">" || tmp == "<=")
			tmp = "\\" + tmp;
		fp << "node" + to_string(i) + "[label = \"" << tmp << "\"];\n";
		p->data.second = i++;
		if (p->parent)
			fp << "node" + to_string(p->parent->data.second) + "->node" + to_string(p->data.second) << ";" << endl;

		for (auto child : p->children)
		{
			GetTree(fp, child);
		}
	}
}

void Parser::drawTree(TreeNode* root)
{
	fstream fp("tree.txt", ios::binary | ios::out);
	fp << "digraph g{" << endl;
	fp << "splines = \"line\";\nnode[shape = record, height = .1]; " << endl;
	TreeNode* p = root;
	GetTree(fp, root);
	fp << "}" << endl;
	fp.close();
}
