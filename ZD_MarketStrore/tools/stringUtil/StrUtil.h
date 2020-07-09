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

//trim指示是否保留空串，默认为保留
std::vector<std::string> tokenize(const std::string& src, std::string tok, bool trim=false, std::string null_subst="")throw(std::string);

//使用一个完整的串delimit（而不是其中的某个字符）来分割src串,没有trim选项，即严格分割
std::vector<std::string> split(const std::string& src, std::string delimit, std::string null_subst="")throw(std::string);

std::string trim(char const *str);

}
#endif // STRUTIL_H
