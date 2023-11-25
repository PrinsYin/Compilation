
#include"Parser.h"
#include<iostream>
#include<sstream>
using namespace std;

std::string getFileName(const std::string& prompt, const std::string& default_file_name)
{
    std::string file_name;
    std::ifstream fin;

    while (true)
    {
        std::cout << prompt;
        std::getline(std::cin, file_name);

        if (file_name.empty())
        {
            file_name = default_file_name;
        }

        fin.open(file_name, std::ios::in);
        if (!fin.is_open())
        {
            std::cout << "读取" << file_name << "文件失败，请重新输入。\n";
            continue;
        }
        else
        {
            fin.close();
            break;
        }
    }

    return file_name;
}

int main()
{
    std::string grammar_file_name = getFileName("请输入语法文件名 (直接回车使用默认值 'grammar.txt'): ", "grammar.txt");
    std::string code_file_name = getFileName("请输入代码文件名 (直接回车使用默认值 'code.txt'): ", "code.txt");
    cout << "*******************\n";
    Parser parser(grammar_file_name);
    ifstream fp;
    fp.open(code_file_name, ios::in);
    stringstream buffer;
    buffer << fp.rdbuf();
    string str(buffer.str());

    ifstream fin;
    ofstream fout;
    string target_name = "lexerout.txt";

    fin.open(code_file_name, ios::in);
    fout.open(target_name, ios::out);

    

    string message = parser.tryParse(str, fin, fout,code_file_name);
    if (message == "Accept!!!")
    {
        parser.drawTree(parser.root);
        cout << "词法 / 语法分析成功， 词法分析结果已输出到lexerout.txt，语法分析结果已输出parser.txt，语法树已输出到  tree.dot  文件，\n请通过运行目录下的python脚本使用graphviz来获得图像文件\n";
    }
    else
    {
        cout << "语法分析失败，错误信息如下：\n" << message << "\n";
    }
    return 0;
}

