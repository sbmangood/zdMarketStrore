#ifndef STRUTIL_H
#define STRUTIL_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <vector>
namespace STR_util
{

typedef std::basic_string<char>::size_type S_T;

//trimָʾ�Ƿ����մ���Ĭ��Ϊ����
std::vector<std::string> tokenize(const std::string& src, std::string tok, bool trim=false, std::string null_subst="")throw(std::string);

//ʹ��һ�������Ĵ�delimit�����������е�ĳ���ַ������ָ�src��,û��trimѡ����ϸ�ָ�
std::vector<std::string> split(const std::string& src, std::string delimit, std::string null_subst="")throw(std::string);

std::string trim(char const *str);

}
#endif // STRUTIL_H
