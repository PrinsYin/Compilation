
#include "lexer_tools.h"
#include<iostream>

/**
 *
 * @brief 初始化关键字节点信息
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
 * @param src 读取的输入字符
 * @return 返回src是否是数字
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
 * @param src 读取的输入字符
 * @return 返回src是否是字母
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
 * @param src 读取的输入字符串
 * @return 返回src是否是关键字
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
 * @function 返回词法分析后代码的种别码和string
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
 * @param fin 读取的文件
 * @function 词法分析 
 */
void Lex::scan(ifstream& fin, ofstream& fout)
{
    char src;
    bool note = false;//注释标志//
    bool note1 = false;//注释标志/**/
    int row = 0;
    int code = -1;//种别码
    string token = "";//存储读取的字符串
    init_node();
    while ((src = fin.get()) != EOF)
    {
        if (src == ' ' || src == '\t')
        {
            continue;
        }
        else if (src == '\n')
        {
            // //注释遇到换行失效
            // /*注释遇到换行不失效

            if (note)
            {
                note = false;
            }
            row++;
            continue;
        }
        else if (isNum(src) && (!note) && (!note1))//数值
        {
            //TODO 优化数据类型
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
        else if (isLetter(src) && (!note) && (!note1))//关键字、标识符
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
            //如果//注释无效
            if (!note) {
                // /*注释无效
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
                        if (ch == '*')//注释直到遇见*/
                        {
                            note1 = true;
                            token += ch;
                            code = code::NOTE;
                            result.push_back(make_pair(token,code));
                            fout << token << " " << code << endl;
                            
                        }
                        else if (ch == '/')//注释直到回车
                        {
                            note = true;
                            token += ch;
                            code = code::NOTE;

                        }
                        else//算符/
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
                            code = -1;//TODO,报错
                            fin.seekg(-1, SEEK_CUR);
                        }
                    }
                    else
                    {
                        //错误字符
                        token += src;
                        code = code::ERROR;
                    }
                }
                else//前面出现/*注释
                {
                    //TODO考虑全面/**/出现的情况，这里只考虑了*/
                    if (src == '*')
                    {
                        token += src;
                        char ch = fin.get();
                        if (ch == '/')//视为注释结尾*/
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