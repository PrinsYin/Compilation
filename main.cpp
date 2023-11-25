
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
            std::cout << "��ȡ" << file_name << "�ļ�ʧ�ܣ����������롣\n";
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
    std::string grammar_file_name = getFileName("�������﷨�ļ��� (ֱ�ӻس�ʹ��Ĭ��ֵ 'grammar.txt'): ", "grammar.txt");
    std::string code_file_name = getFileName("����������ļ��� (ֱ�ӻس�ʹ��Ĭ��ֵ 'code.txt'): ", "code.txt");
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
        cout << "�ʷ� / �﷨�����ɹ��� �ʷ���������������lexerout.txt���﷨������������parser.txt���﷨���������  tree.dot  �ļ���\n��ͨ������Ŀ¼�µ�python�ű�ʹ��graphviz�����ͼ���ļ�\n";
    }
    else
    {
        cout << "�﷨����ʧ�ܣ�������Ϣ���£�\n" << message << "\n";
    }
    return 0;
}

