#pragma once
#pragma once
#include<map>
#include<vector>
#include<string>
#include<sstream>
#include<set>
#include<unordered_set>
#include<unordered_map>

using namespace std;

#define LEX_NOTNUM 0
#define LEX_DEX 1
#define LEX_OCT 2
#define LEX_HEX 3
#define LEX_FLOAT 4
#define LEX_EXP 5
#define EPSILON "none"


struct Grammar {
	string left;
	vector<string> right;
	bool operator==(Grammar& g) const {
		return this->left == g.left && this->right == g.right;
	}
};

struct Project
{
	int index;
	int point;// 0表示·在最前面，以此类推
	set<string> follow;

	bool operator<(const Project& gp) const {
		if (this->index < gp.index)
			return true;
		else if (this->index > gp.index)
			return false;
		else if (this->point < gp.point)
			return true;
		else if (this->point > gp.point)
			return false;
		else if (this->follow.size() < gp.follow.size())
			return true;
		else if (this->follow.size() > gp.follow.size())
			return false;
		else {
			for (auto it1 = this->follow.cbegin(), it2 = gp.follow.cbegin();
				it1 != this->follow.cend();
				++it1, ++it2) {
				if (*it1 < *it2)
					return true;
				else if (*it1 > *it2)
					return false;
			}
			return false;
		}
	}

	bool operator==(const Project& gp) const {
		//重载运算符，方便项目间比较
		return this->index == gp.index && this->point == gp.point && this->follow == gp.follow;
	}
	bool operator!=(const Project& gp) const {
		//重载运算符，方便项目间比较
		return !(*this == gp);
	}
};

struct Quaternion {
	string op;
	string arg1;
	string arg2;
	string result;
};

enum State {
	ERRORR,      //错误
	START,      //起始
	NUMBER,     //数字
	OPSIGN,     //操作符
	STRING,     //字符串
	COMLINE,    //单行注释
	COMBLOCK    //多行注释
};
enum Type {
	INTT,    // 整型
	VOIDD    // 空型
};

// 数据类型
enum Kind {
	VARR,    // 变量
	FUNC,   // 函数
	ARRAY   // 数组
};
struct SymbolTable;

// 符号项
struct Symbol {
	int id; // 符号表入口
	Type t; // 语义变量
	Kind k; // 数据类型
	int offset;     // 相对地址
	vector<int> dm; // 维度(数组)
	SymbolTable* pTable = NULL;  // 子符号表指针
};
//符号表的包装
struct SymbolTable {
	int width = 0;              // 地址宽度
	vector<Symbol> table;       // 符号表
	SymbolTable* parent = NULL; // 父符号表

	// 清空符号表
	void clear() {
		width = 0;
		table.clear();
	}
	// 向符号表插入符号
	void insert(int id, Type t, Kind k, int offset) {
		Symbol temp;
		temp.id = id;
		temp.t = t;
		temp.k = k;
		temp.offset = offset;

		for (int i = 0; i < table.size(); i++) {
			if (table[i].id == id)
				throw string("ERROR: ID-") + to_string(id) + string(" already exists.\n");
		}

		table.push_back(temp);
	}
	// 插入维度信息
	void insertDM(int id, vector<int>dm) {
		for (int i = 0; i < table.size(); i++) {
			if (table[i].id == id && ((table[i].k == ARRAY) || (table[i].k == FUNC))) {
				table[i].dm = dm;
				break;
			}
		}
	}
	// 插入函数的子符号表
	void insertFunc(int id, SymbolTable* funcTable) {
		for (int i = 0; i < table.size(); i++) {
			if (table[i].id == id && table[i].k == FUNC)
				table[i].pTable = funcTable;
		}
	}
};

struct TreeNode {
	pair<string, int> data;
	TreeNode* parent = NULL;
	vector<TreeNode*> children;

	int pos;
	int TList;
	int FList;

	int val = 0;      // 值的大小
	Type t = Type::INTT;   // 语义变量
	Kind k = Kind::VARR;   // 数据类型
	string place;   // 地址入口
	int width = 0;  // 地址宽度
	vector<int> dm; // 维度信息
	vector<string> params;  // 参数表

	TreeNode() {
		parent = NULL;
		children.clear();
		data = pair<string, int>("", -1);
	}
};

struct Block {
	int begin;
	int end;
	vector<string> waitVar;
	vector<string> activeVar;
	vector<string> uselessVar;
};

// DAG节点
struct DAGItem {
	bool useful = false;
	bool isLeaf;
	string value;
	string op;
	vector<string> label;
	Quaternion code;

	int parent = -1;
	int lChild = -1;
	int rChild = -1;
	int tChild = -1;
	bool isRemain = false;

	bool operator== (DAGItem b) {
		bool f1 = this->isLeaf == b.isLeaf;
		bool f2 = this->value == b.value;
		bool f3 = this->op == b.op;
		bool f4 = this->label.size() == b.label.size();
		bool f5 = this->parent == b.parent;
		bool f6 = this->lChild == b.lChild;
		bool f7 = this->rChild == b.rChild;
		bool f8 = true;

		for (auto i = 0; i < this->label.size() && i < b.label.size(); i++) {
			if (this->label[i] != b.label[i]) {
				f8 = false;
				break;
			}
		}

		return f1 & f2 & f3 & f4 & f5 & f6 & f7 & f8;
	}
};

////////////////////////////////////////////////
/// 代码生成结构
// 待用活跃信息
struct MsgTableItem {
	int no;

	Quaternion TAS;
	pair<int, bool> arg1;
	pair<int, bool> arg2;
	pair<int, bool> result;
};

// 寄存器分配
struct AysTableItem {
	Quaternion TAS;
	vector<string> objCode;
	map<string, vector<pair<string, int>>> RVALUE;
	map<string, vector<string>> AVALUE;
};

