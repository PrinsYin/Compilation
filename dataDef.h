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




struct TreeNode {
	pair<string, int> data;
	TreeNode* parent = NULL;
	vector<TreeNode*> children;

	int pos;
	int TList;
	int FList;

	int val = 0;      // 值的大小
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
