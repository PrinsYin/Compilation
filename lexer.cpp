//#include <iostream>
//#include "lexer_tools.h"
//using namespace std;
//
////int main()
////{
////    
////    ifstream fin;
////    ofstream fout;
////    string file_name;
////    string target_name;
////    cout << "������ʷ���������Դ�ļ�:";
////    cin >> file_name;
////    cout << "����������ʷ�������Ŀ���ļ�:";
////    cin >> target_name;
////    fin.open(file_name, ios::in);
////    if (!fin.is_open())
////    {
////        cout << "��ȡ" << file_name << "�ļ�ʧ��" << endl;
////        return -1;
////    }
////    fout.open(target_name, ios::out);
////    if (!fout.is_open())
////    {
////        cout << "Ŀ��" << file_name << "�ļ���ʧ��" << endl;
////        return -1;
////    }
////
////    Lex MyLex=Lex(file_name, target_name);
////    MyLex.scan(fin,fout);
////
////    fin.close();
////    fout.close();
////    return 0;
////}