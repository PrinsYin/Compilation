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

/**
 * getStrFIRST ��������ʽ�Ҳ��ķ��ż��ϣ��������ǵ�FIRST����
 * @param Ba һ���ַ����������������ʽ�Ҳ��ķ��ż��ϡ�
 * @return ���ؼ���õ���FIRST��������һ���ַ������ϡ�
 */

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

/**
 * Parser ��ʼ���������������ظ������ķ��ļ���
 * @param filename �ķ��ļ���·����
 */
Parser::Parser(const string& filename)
{
	errorLine = 0;
	errorToken = "";///������������������������������������

	loadGrammar(filename);
	getVN();
	getVT();
	getFIRST();
	getLR1States();
	getACTION();
	getGOTO();

}

/**
 * loadGrammar �Ӹ������ļ��м����ķ���
 * @param filename �ķ��ļ���·����
 * @return ����ļ����سɹ����򷵻�true�����򷵻�false��
 */
bool Parser::loadGrammar(const string& filename)
{
	// ���ļ�
	string line, str;
	Grammar g;
	ifstream file(filename, ios::binary | ios::in);
	if (!file.is_open())
		return false; // �ļ���ʧ�ܣ�����ʧ�ܱ�־

	// ��ʱ�����ͽṹ�������
	

	// ���ж�ȡ�ļ�����
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



/**
 * getVN �Ӹ������ķ��л�ȡ���еķ��ս�������洢��VN�����С�
 */
void Parser::getVN()
{
	for (const auto& rule : rules)
		VN.insert(rule.left);
}

/**
 * getVT �Ӹ������ķ��л�ȡ���е��ս�������洢��VT�����С�
 */
void Parser::getVT()
{
	for (auto& rule : rules)
	{
		for (auto& ri : rule.right)
			if (!VN.count(ri))
				VT.insert(ri);
	}
	VT.insert("#");//��
}

/**
 * getFIRST ����ʹ洢���з��ս����FIRST����
 */
void Parser::getFIRST()
{
	for (const auto& vt : VT) {
		FIRST[vt] = { vt };
	}

	vector<int> list;	//��¼����ʽ�Ҳ���һ������Ϊ���ս�����ķ�
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
					//����A->A..���������
					if (rules[i].left == elem1) {
							break;
					}
					//������A->B...,��B��first��ȫ���ӵ�A��
					for (const auto& elem2 : FIRST[elem1]) 
						if (FIRST[rules[i].left].insert(elem2).second) 
							flag = true;
					if (!ep)
						break;	//��������,��������ü�������
				}//?????????????????????????ep????????
				else
					break;
			}

		}
		if (!flag)	//���first����������,�򷵻�
			break;
	}
}

/**
 *getCLOSURE ���ݸ�������Ŀ��I���������ıհ���
 * @param I ��Ŀ��������һ��Project���͵ļ��ϣ�Project������һ����������ʽ���������λ�ú�follow set�Ľṹ����ࡣ
 * @return ���ؼ���õ��ıհ�������һ��Project���͵ļ��ϡ�
 */

set<Project> Parser::getCLOSURE(const set<Project>& I)
{

	set< Project > closure(I);			//project_set�����������Ŀ���ڱհ���
	set<Project> old_project(I);	//��������
	set<Project> new_project;
	int before = 0,after=0;
	bool flag;
	while (true) {
		before = old_project.size();
		flag = false;
		for (const auto& i : old_project) {	//ɨ����һ�β�����������Ŀ
			Grammar rule = rules[i.index];
			if (rule.right.size() > i.point && VN.count((rule.right[i.point]))) //Ϊ���ս���Ҳ������һ������
			{
				//A->��.B����
				string B = rule.right[i.point];
				//���first(��a)
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
					//ɨ������B->���͵Ĳ���ʽ
					if (rules[j].left == B) {
						//��CLOSURE�в�����{B->��,firstba},�����
						bool flag1 = false;
						for (auto it = closure.begin(); it != closure.end(); ++it) {
							if (it->index == j && it->point == 0) {
								//��Ŀ�ڼ���
								flag1 = true;
								set<string>temp;
								set_intersection(it->follow.begin(), it->follow.end(), firstAfterDot.begin(), firstAfterDot.end(), inserter(temp, temp.begin()));
								if (temp!=firstAfterDot) {
									//��follows������,������µ�follows
									flag = true;
									//���ڼ���Ԫ�ص�ֵ�޷��޸�,��ֻ�ܸ���֮
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
							//�����������Ŀ
							flag = true;
							closure.insert({ j,0,firstAfterDot });
							new_project.insert({ j,0,firstAfterDot });
						}
					}
				}
			}
		}
		after = new_project.size();
		if (!flag)	//��������,�򷵻�
			break;
		old_project = new_project;	//���������Ŀ������һ��ɨ��
		new_project.clear();
	}

	return closure;
	
}


/**
 * getLR1States ����LR(1)��Ŀ���Ĺ淶�塣
 * @return ����һ��ӳ�䣬���еļ��Ǹ�����Ŀ�������ƣ���"I0"��"I1"�ȣ�ֵ�Ƕ�Ӧ����Ŀ����
 */
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
		for (auto& vn : VN)//���ս��
		{
			J.clear();
			for (auto& elem : C[i])//����ÿһ����Ŀ
			{
				for (auto j = 0; j < rules[elem.index].right.size(); j++)//�ҵ���Ŀ������A->��vn��
				{
					if (rules[elem.index].right[j] == vn && elem.point == j)//�ҵ�
					{
						J.insert({ elem.index,elem.point + 1,elem.follow });//�����
						break;
					}

				}
			}
			_GO = getCLOSURE(J);    //���GO��C[i],vn��
			if (_GO.size() > 0)    //��Ϊ�ռ�
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
			if (_GO.size() > 0)    //��Ϊ�ռ�
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


/**
 * getACTION ����LR(1)�������ACTION���֡���ִ�д˺����󣬽�������ACTION��ӦΪ����״̬��
 */
void Parser::getACTION()
{
	for (int i = 0; i < lr1states.size(); i++)
	{
		for (auto item : lr1states[i])
		{
			//����
			if (rules[item.index].left == string(START) + EXT_CHAR && rules[item.index].right.front() == START && rules[item.index].right.size() == item.point)
			{
				ACTION[{"I" + to_string(i), "#"}] = "acc";
			}
			//�ƽ�
			else if (rules[item.index].right.size() > item.point)
			{
				string var = rules[item.index].right[item.point];
				if (VT.count(var))//���ս��
				{
					string tmp = GO[{"I" + to_string(i), var}];
					if (tmp != "error")
						tmp = GO[{"I" + to_string(i), var}].substr(1, tmp.length());
					ACTION[{"I" + to_string(i), var}] = string("s") + tmp;
				}
			}
			//��Լ
			else
			{
				for (auto fol : item.follow)
				{
					ACTION[{"I" + to_string(i),fol}] = string("r") + to_string(item.index);
				}
			}
		}
	}
	//��������ȫ��Ϊerror
	for (int i = 0; i < lr1states.size(); i++)
	{
		for (auto vt : VT)
			if (ACTION[{"I" + to_string(i), vt}] == "")
				ACTION[{"I" + to_string(i), vt}] = "error";
	}
}


/**
 * getGOTO ����LR(1)�������GOTO���֡���ִ�д˺����󣬽�������GOTO��ӦΪ����״̬��
 */
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

/**
 * tryParse() ���Խ����������ַ�����
 *
 * @param LexResStr ��Ҫ���������ַ�����
 * @param fin �ļ�����
 * @param fout �ļ�����
 * @return ��������ɹ������� "Accept!!!"����������������ش�����Ϣ��
 */

string Parser::tryParse(const string& LexResStr, ifstream& fin, ofstream& fout,string code_file_name)
{
	
	Lex L(code_file_name, "lexerout.txt");
	L.scan(fin,fout);  // �򿪴ʷ������������������ַ���
	// ������ʼ��
	int line = 1;
	vector<int> state;
	vector<pair<string, string>> symbol;
	pair<string, string> nextToken;
	pair<string, string> token = L.GetNextToken();  // ��ȡ��һ������

	pair<string, string> bubble = pair<string, string>("none", "none");
	TreeNode* tp = nullptr;
	stack<TreeNode*> treeNodeStack;
	fstream fp("parser.txt", ios::out | ios::binary);

	// ��ʼѹջ
	state.push_back(0);
	symbol.push_back(pair<string, string>(string("#"), string("#")));
	// ѭ���﷨����
	while (token.first != "ERROR") 
	{
		if (token.first == "NL") {
			line++;
			token = L.GetNextToken();
			continue;
		}

		if (find(VT.begin(), VT.end(), token.second) == VT.end())//�������ֹ��������
		{
			string msg = string("ERROR: ") + (token.second) + string(" is not a terminator.\n");
			fp << msg << endl;
			fp.close();
			return msg;
		}

		// ����״̬��
		string action = ACTION[{"I" + to_string(state.back()), token.second}];
		if (action == "acc") {  // ��������ǡ����ܡ�����������벢����
			fp << "�ʷ� / �﷨�����ɹ����﷨���������tree�ļ���graphviz��ʽ" << endl;
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
		if (action.substr(0, 1) == "s")  // ��������ǡ��ƽ���������״̬������ѹ���ջ������ȡ��һ������
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
		if (action.substr(0, 1) == "r")  // ��������ǡ���Լ��������ݹ���Ӷ�ջ�е������ź�״̬���������ѹ���ջ
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
					msg += string("-Note��") + symbol.back().second + string(" is different from ") + rules[index].right[i] + string(".\n");
					fp << msg << endl;
					fp.close();
					return msg;
				}
			}
			/// ��
			string token_t = rules[index].left + " ->";
			for (int i = 0; i < length; i++) {
					token_t += " " + rules[index].right[i];
			}
			fp << "lexer:" << token.first << "  action:" << action << "  production:" << token_t << endl;
			tp = new(nothrow)TreeNode;
			if (!tp) {
				string msg;
				msg = string("ERROR: Memory crashed on line ") + to_string(line) + string(" (") + token.first + string(").\n");
				fp << msg << endl;
				fp.close();
				return msg;
			}
			tp->data = pair<string, int>(rules[index].left, -1);
			for (int i = 0; i < length; i++) {
				treeNodeStack.top()->parent = tp;
				tp->children.push_back(treeNodeStack.top());
				treeNodeStack.pop();
			}
			reverse(tp->children.begin(), tp->children.end());
			treeNodeStack.push(tp);

			symbol.push_back(pair<string, string>(rules[index].left, rules[index].left));
			state.push_back(atoi(GOTO[{"I" + to_string(state.back()), symbol.back().second}].substr(1, GOTO[{"I" + to_string(state.back()), symbol.back().second}].length()).c_str()));
		}
	};
	return "error";
}



/**
 * GetTree() �ݹ�ػ�ȡ�﷨�������нڵ�ͱߣ��洢��һ���ļ��С�
 *
 * @param fp fstream���͵����ã���ʾ�����洢���ɵ��﷨�����ļ�����
 * @param p TreeNode���͵�ָ�룬��ʾ��ǰ����Ľڵ㡣
 */

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

/**
 * drawTree() ʹ��Graphviz���߻����﷨�������浽һ���ļ��С�
 *
 * @param root TreeNode���͵�ָ�룬��ʾ�﷨���ĸ��ڵ㡣
 */
void Parser::drawTree(TreeNode* root)
{
	fstream fp("tree.dot", ios::binary | ios::out);
	fp << "digraph g{" << endl;
	fp << "splines = \"line\";\nnode[shape = record, height = .1]; " << endl;
	TreeNode* p = root;
	GetTree(fp, root);
	fp << "}" << endl;
	fp.close();
}
