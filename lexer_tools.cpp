
#include "lexer_tools.h"
#include<iostream>

/**
 *
 * @brief ��ʼ���ؼ��ֽڵ���Ϣ
 */
void Lex::init_node()
{
    for (int i = 0; i < KEY_NUM; i++)
    {
        this->keys[i].code = i + 2;
        this->keys[i].token = keyword[i];
    }
}

/**
 *
 * @param src ��ȡ�������ַ�
 * @return ����src�Ƿ�������
 */
bool Lex::isNum(const char src)
{
    if (src >= '0' && src <= '9')
    {
        return true;
    }
    return false;
}


/**
 *
 * @param src ��ȡ�������ַ�
 * @return ����src�Ƿ�����ĸ
 */
bool Lex::isLetter(const char src)
{
    if (src >= 'a' && src <= 'z')
    {
        return true;
    }
    else if (src >= 'A' && src <= 'Z')
    {
        return true;
    }
    return false;
}

/**
 *
 * @param src ��ȡ�������ַ���
 * @return ����src�Ƿ��ǹؼ���
 */
bool Lex::isKeyWord(string src, int& code)
{
    for (int i = 0; i < KEY_NUM; i++)
    {
        if (strcmp(src.c_str(), this->keys[i].token.c_str()) == 0)
        {
            code = this->keys[i].code;
            return true;
        }
    }
    code = -1;
    return false;
}

/**
 *
 * @function ���شʷ������������ֱ����string
 */
pair<string, string> Lex::GetNextToken()
{
    string a;
    /*
    if (index <=result.size())
    {
        pair<string, string> res = make_pair(result[index].first, string(getCodeName[code(result[index++].second)]));
        if (res.second == "")
        {
            int b=code(result[index-1].second);
            a = getCodeName[code(result[index-1].second)];
            std::cout << a << b;
        }
          */
    if (index < result.size())
    {
        pair<string, string> res = make_pair(result[index].first, string(getCodeName[code(result[index].second)]));
        index++;
        return res;
     }
    else if (index ==result.size())
        return make_pair("#", "#"); 
    else
        return make_pair("", "");
}

/**
 *
 * @param fin ��ȡ���ļ�
 * @function �ʷ����� 
 */
void Lex::scan(ifstream& fin, ofstream& fout)
{
    char src;
    bool note = false;//ע�ͱ�־//
    bool note1 = false;//ע�ͱ�־/**/
    int row = 0;
    int code = -1;//�ֱ���
    string token = "";//�洢��ȡ���ַ���
    init_node();
    while ((src = fin.get()) != EOF)
    {
        if (src == ' ' || src == '\t')
        {
            continue;
        }
        else if (src == '\n')
        {
            // //ע����������ʧЧ
            // /*ע���������в�ʧЧ

            if (note)
            {
                note = false;
            }
            row++;
            continue;
        }
        else if (isNum(src) && (!note) && (!note1))//��ֵ
        {
            //TODO �Ż���������
            while (isNum(src))
            {
                token += src;
                src = fin.get();
            }
            fin.seekg(-1, SEEK_CUR);
            code = code::NUM;
            result.push_back(make_pair(token, code));
            fout << token << " " << code::NUM << endl;
            token = " ";
        }
        else if (isLetter(src) && (!note) && (!note1))//�ؼ��֡���ʶ��
        {
            while (isLetter(src) || isNum(src))
            {
                token += src;
                src = fin.get();
            }
            fin.seekg(-1, SEEK_CUR);
            if (isKeyWord(token, code))
            {
                result.push_back(make_pair(token, code));
                fout << token << " " << code << endl;
            }
            else
            {
                code = code::ID;
                result.push_back(make_pair(token, code));
                fout << token << " " << code << endl;

            }
            token = "";
        }
        else {
            //���//ע����Ч
            if (!note) {
                // /*ע����Ч
                if (!note1)
                {
                    if (src == '+')
                    {
                        token += src;
                        code = code::PLUS;
                    }
                    else if (src == '-')
                    {
                        token += src;
                        code = code::MIUS;
                    }
                    else if (src == ')')
                    {
                        token += src;
                        code = code::RR;
                    }
                    else if (src == '(')
                    {
                        token += src;
                        code = code::LR;
                    }
                    else if (src == '{')
                    {
                        token += src;
                        code = code::LB;
                    }
                    else if (src == '}')
                    {
                        token += src;
                        code = code::RB;
                    }
                    else if (src == '{')
                    {
                        token += src;
                        code = code::LB;
                    }
                    else if (src == '#')
                    {
                        token += src;
                        code = code::LB;
                    }
                    else if (src == ',')
                    {
                        token += src;
                        code = code::COMMA;
                    }
                    else if (src == ';')
                    {
                        token += src;
                        code = code::SEMI;
                    }
                    else if (src == '*')
                    {
                        token += src;
                        code = code::MUL;
                    }
                    else if (src == '/')
                    {
                        token += src;
                        char ch = fin.get();
                        if (ch == '*')//ע��ֱ������*/
                        {
                            note1 = true;
                            token += ch;
                            code = code::NOTE;
                            result.push_back(make_pair(token,code));
                            fout << token << " " << code << endl;
                            
                        }
                        else if (ch == '/')//ע��ֱ���س�
                        {
                            note = true;
                            token += ch;
                            code = code::NOTE;

                        }
                        else//���/
                        {
                            code = code::DIV;
                            fin.seekg(-1, SEEK_CUR);
                        }
                    }
                    else if (src == '=')
                    {
                        token += src;
                        char ch = fin.get();
                        if (ch == '=')//==
                        {
                            token += ch;
                            code = code::EQ;
                        }
                        else
                        {
                            code = code::ASSIGN;
                            fin.seekg(-1, SEEK_CUR);
                        }
                    }
                    else if (src == '>')
                    {
                        token += src;
                        char ch = fin.get();
                        if (ch == '=')//>=
                        {
                            token += ch;
                            code = code::GEQ;
                        }
                        else
                        {
                            code = code::GT;
                            fin.seekg(-1, SEEK_CUR);
                        }
                    }
                    else if (src == '<')
                    {
                        token += src;
                        char ch = fin.get();
                        if (ch == '=')//<=
                        {
                            token += ch;
                            code = code::LEQ;
                        }
                        else
                        {
                            code = code::LT;
                            fin.seekg(-1, SEEK_CUR);
                        }
                    }
                    else if (src == '!')
                    {
                        token += src;
                        char ch = fin.get();
                        if (ch == '=')//!=
                        {
                            token += ch;
                            code = code::GEQ;
                        }
                        else
                        {
                            code = -1;//TODO,����
                            fin.seekg(-1, SEEK_CUR);
                        }
                    }
                    else
                    {
                        //�����ַ�
                        token += src;
                        code = code::ERROR;
                    }
                }
                else//ǰ�����/*ע��
                {
                    //TODO����ȫ��/**/���ֵ����������ֻ������*/
                    if (src == '*')
                    {
                        token += src;
                        char ch = fin.get();
                        if (ch == '/')//��Ϊע�ͽ�β*/
                        {
                            token += ch;
                            note1 = false;
                            code = code::NOTE;
                        }
                        else {
                            fin.seekg(-1, SEEK_CUR);
                        }

                    }
                } 
                if (!note1)
                {
                    result.push_back(make_pair(token, code));
                    fout << token << " " << code << endl;
                }
                token = "";
            }

        }
    }
}