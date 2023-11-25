#include<map>
#include<vector>
#include<string>
#include<sstream>
#include<set>
#include<unordered_set>
#include<unordered_map>

using namespace std;



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




struct TreeNode {
	pair<string, int> data;
	TreeNode* parent = NULL;
	vector<TreeNode*> children;

	int pos;
	int TList;
	int FList;

	int val = 0;      // ֵ�Ĵ�С
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
