#ifndef _STATEMENT_H_
#define _STATEMENT_H_
#include <node.h>
#include <v8.h>
using namespace v8;
using namespace node;
#include "DbSqlite.h"

class Statement : public ObjectWrap
{
public:
	Statement(){}
	~Statement(){};

	static void Init(Handle<Object> target);
	static Handle<Value> New(const Arguments& args);
	static Handle<Value> MoveFirst(const Arguments& args);
	static Handle<Value> MoveNext(const Arguments& args);
	static Handle<Value> MoveLast(const Arguments& args);
	static Handle<Value> GetFieldCount(const Arguments& args);
	static Handle<Value> GetRecordCount(const Arguments& args);
	static Handle<Value> GetFieldValueByIndex(const Arguments& args);
	static Handle<Value> GetFieldValueByName(const Arguments& args);
	static Handle<Value> GetFieldName(const Arguments& args);

	
	void SetRecordsetPtr(CSqliteRecordsetPtr ptr)
	{
		pRecord = ptr;
	}
public:
	static Persistent<FunctionTemplate> s_ct;
private:
	CSqliteRecordsetPtr pRecord;





	

};
#endif
