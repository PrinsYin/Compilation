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
////    cout << "请输入词法分析的来源文件:";
////    cin >> file_name;
////    cout << "请输入输出词法分析的目标文件:";
////    cin >> target_name;
////    fin.open(file_name, ios::in);
////    if (!fin.is_open())
////    {
////        cout << "读取" << file_name << "文件失败" << endl;
////        return -1;
////    }
////    fout.open(target_name, ios::out);
////    if (!fout.is_open())
////    {
////        cout << "目标" << file_name << "文件打开失败" << endl;
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