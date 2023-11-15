#include"lexer_tools.h"
#include"Parser.h"
#include<iostream>
#include<sstream>

/*预处理：所有相邻空白符、跳格符等编辑性字符合并为一个空格【不删换行】
且先删除注释，这样就避免了行注释的影响

*/





int main()
{



		Parser parser("grammar.txt");
		//Optimizer optimizer;     // 代码优化器

		string message;
		fstream fp("example1.txt", ios::in);
		stringstream buffer;
		buffer << fp.rdbuf();
		string str(buffer.str());

		ifstream fin;
		   ofstream fout;
		   string file_name="example1.txt";
		   string target_name="lexerout.txt";
		   /*cout << "请输入词法分析的来源文件:";
		   cin >> file_name;
		   cout << "请输入输出词法分析的目标文件:";
		   cin >> target_name;*/
		   fin.open(file_name, ios::in);
		   if (!fin.is_open())
		   {
		       cout << "读取" << file_name << "文件失败" << endl;
		       return -1;
		   }
		   fout.open(target_name, ios::out);
		   if (!fout.is_open())
		   {
		       cout << "目标" << file_name << "文件打开失败" << endl;
		       return -1;
		   }


		message = parser.tryParse(str, fin, fout);
		if (message == "Accept!!!")
			parser.drawTree(parser.root);
		cout << message << endl;


	return 0;
}

