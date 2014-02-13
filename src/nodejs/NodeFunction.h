#ifndef _NODE_FUNCTION_H_
#define _NODE_FUNCTION_H_
#include<node.h>
#include<v8.h>
using namespace v8;
using namespace node;
#include"../../sqlite3_encrypt/platform_config.h"
#include "../../third_party/CodingConv/encodeutil.h"
namespace NodeFunc
{
	const char* ToCString(const String::Utf8Value& value);
	TSTRING Replace( const TSTRING& orignStr, const TSTRING& oldStr, const vector<TSTRING>&vc );
	TSTRING MergeSql(const Arguments&args);
};
#endif
