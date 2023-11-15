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
	int point;// 0��ʾ������ǰ�棬�Դ�����
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
		//�����������������Ŀ��Ƚ�
		return this->index == gp.index && this->point == gp.point && this->follow == gp.follow;
	}
	bool operator!=(const Project& gp) const {
		//�����������������Ŀ��Ƚ�
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
	ERRORR,      //����
	START,      //��ʼ
	NUMBER,     //����
	OPSIGN,     //������
	STRING,     //�ַ���
	COMLINE,    //����ע��
	COMBLOCK    //����ע��
};
enum Type {
	INTT,    // ����
	VOIDD    // ����
};

// ��������
enum Kind {
	VARR,    // ����
	FUNC,   // ����
	ARRAY   // ����
};
struct SymbolTable;

// ������
struct Symbol {
	int id; // ���ű����
	Type t; // �������
	Kind k; // ��������
	int offset;     // ��Ե�ַ
	vector<int> dm; // ά��(����)
	SymbolTable* pTable = NULL;  // �ӷ��ű�ָ��
};
//���ű�İ�װ
struct SymbolTable {
	int width = 0;              // ��ַ���
	vector<Symbol> table;       // ���ű�
	SymbolTable* parent = NULL; // �����ű�

	// ��շ��ű�
	void clear() {
		width = 0;
		table.clear();
	}
	// ����ű�������
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
	// ����ά����Ϣ
	void insertDM(int id, vector<int>dm) {
		for (int i = 0; i < table.size(); i++) {
			if (table[i].id == id && ((table[i].k == ARRAY) || (table[i].k == FUNC))) {
				table[i].dm = dm;
				break;
			}
		}
	}
	// ���뺯�����ӷ��ű�
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

	int val = 0;      // ֵ�Ĵ�С
	Type t = Type::INTT;   // �������
	Kind k = Kind::VARR;   // ��������
	string place;   // ��ַ���
	int width = 0;  // ��ַ���
	vector<int> dm; // ά����Ϣ
	vector<string> params;  // ������

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

// DAG�ڵ�
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
/// �������ɽṹ
// ���û�Ծ��Ϣ
struct MsgTableItem {
	int no;

	Quaternion TAS;
	pair<int, bool> arg1;
	pair<int, bool> arg2;
	pair<int, bool> result;
};

// �Ĵ�������
struct AysTableItem {
	Quaternion TAS;
	vector<string> objCode;
	map<string, vector<pair<string, int>>> RVALUE;
	map<string, vector<string>> AVALUE;
};

