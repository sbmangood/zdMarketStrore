
#include "StrUtil.h"

using namespace std;
namespace STR_util
{ 
static const S_T npos = -1;

vector<string> tokenize(const string& src, string tok, bool trim, string null_subst)throw(std::string)
{
	if( src.empty() || tok.empty() ) throw "tokenize: empty string\0";

	vector<string> v;
	S_T pre_index = 0, index = 0, len = 0;
	while( (index = src.find_first_of(tok, pre_index)) != npos )
	{
		if( (len = index-pre_index)!=0 )
		   v.push_back(src.substr(pre_index, len));
		else if(trim==false)
		   v.push_back(null_subst);
		pre_index = index+1;
	}
	string endstr = src.substr(pre_index);
	if( trim==false ) v.push_back( endstr.empty()? 

	null_subst:endstr );
	else if( !endstr.empty() ) v.push_back(endstr);
	return v;
}


vector<string> split(const string& src, string delimit, string null_subst)throw(std::string)
{
	if( src.empty() || delimit.empty() ) throw "split: empty string\0";

	vector<string> v;
	S_T deli_len = delimit.size();
	long index = npos, last_search_position = 0;
	while( (index=src.find(delimit, last_search_position))!=npos )
	{
		if(index==last_search_position)
		   v.push_back(null_subst);
		else
		   v.push_back( src.substr(last_search_position, index- last_search_position) );
		last_search_position = index + deli_len;
	}
	string last_one = src.substr(last_search_position);
	v.push_back( last_one.empty()? null_subst:last_one );
	return v;
}

/*
string trim(char const *str)
{
  // Trim leading non-letters
  while(!isalnum(*str)) str++;

  // Trim trailing non-letters
  char* end = (char*)str + strlen(str) - 1;
  while(end > str && !isalnum(*end)) end--;

  return string(str, end+1);
}
*/

string trim(char const *str)
{
  // Trim leading non-letters
	char *str1 = (char *)str;
  while(*str1 ==' ') str1++;

  // Trim trailing non-letters
  char* end = (char*)str1 + strlen(str1) - 1;
  while(end > str1 && *end == ' ') end--;

  return string(str1, end+1);
}

}