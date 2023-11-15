#include"lexer_tools.h"
#include"Parser.h"
#include<iostream>
#include<sstream>

/*Ԥ�����������ڿհ׷���������ȱ༭���ַ��ϲ�Ϊһ���ո񡾲�ɾ���С�
����ɾ��ע�ͣ������ͱ�������ע�͵�Ӱ��

*/





int main()
{



		Parser parser("grammar.txt");
		//Optimizer optimizer;     // �����Ż���

		string message;
		fstream fp("example1.txt", ios::in);
		stringstream buffer;
		buffer << fp.rdbuf();
		string str(buffer.str());

		ifstream fin;
		   ofstream fout;
		   string file_name="example1.txt";
		   string target_name="lexerout.txt";
		   /*cout << "������ʷ���������Դ�ļ�:";
		   cin >> file_name;
		   cout << "����������ʷ�������Ŀ���ļ�:";
		   cin >> target_name;*/
		   fin.open(file_name, ios::in);
		   if (!fin.is_open())
		   {
		       cout << "��ȡ" << file_name << "�ļ�ʧ��" << endl;
		       return -1;
		   }
		   fout.open(target_name, ios::out);
		   if (!fout.is_open())
		   {
		       cout << "Ŀ��" << file_name << "�ļ���ʧ��" << endl;
		       return -1;
		   }


		message = parser.tryParse(str, fin, fout);
		if (message == "Accept!!!")
			parser.drawTree(parser.root);
		cout << message << endl;


	return 0;
}

