#include "Statement.h"
#include "NodeFunction.h"
using namespace NodeFunc;
Persistent<FunctionTemplate>Statement::s_ct;



void Statement::Init(Handle<Object> target)
{
	HandleScope scope;
	Local<FunctionTemplate> t = FunctionTemplate::New(New);
	s_ct = Persistent<FunctionTemplate>::New(t);
	s_ct->InstanceTemplate()->SetInternalFieldCount(1);
	s_ct->SetClassName(String::NewSymbol("Statement"));
	NODE_SET_PROTOTYPE_METHOD(s_ct,"moveFirst",MoveFirst);
	NODE_SET_PROTOTYPE_METHOD(s_ct,"moveNext",MoveNext);
	NODE_SET_PROTOTYPE_METHOD(s_ct,"getFieldCount",GetFieldCount);
	NODE_SET_PROTOTYPE_METHOD(s_ct,"getRecordCount",GetRecordCount);
	NODE_SET_PROTOTYPE_METHOD(s_ct,"getFieldValueByIndex",GetFieldValueByIndex);
	NODE_SET_PROTOTYPE_METHOD(s_ct,"getFieldValueByName",GetFieldValueByName);
	NODE_SET_PROTOTYPE_METHOD(s_ct,"getFieldName",GetFieldName);
	target->Set(String::NewSymbol("Statement"),s_ct->GetFunction());


}

 Handle<Value>Statement::New(const Arguments& args)
{
	HandleScope scope;
	Statement* statement = new Statement();
	statement->Wrap(args.This());
	return args.This();
}

 Handle<Value> Statement::MoveFirst(const Arguments& args)
 {

	 Statement* pStatement = ObjectWrap::Unwrap<Statement>(args.This());
	 HandleScope scope;
	 BOOL bRet = pStatement->pRecord->MoveFirst();
	 return scope.Close(Boolean::New(bRet));
 }
 Handle<Value> Statement::MoveNext(const Arguments& args)
 {
	 Statement* pStatement = ObjectWrap::Unwrap<Statement>(args.This());
	 HandleScope scope;
	 BOOL bRet = pStatement->pRecord->MoveNext();
	 return scope.Close(Boolean::New(bRet));

 }
 Handle<Value> Statement::MoveLast(const Arguments& args)
 {
	 /* Statement* pStatement = ObjectWrap::Unwrap<Statement>(args.This());
	 HandleScope scope;
	 BOOL bRet = pStatement->pRecord->MoveLast();
	 return scope.Close(Boolean::New(bRet));*/
	return Undefined();
 }
 Handle<Value> Statement::GetFieldCount(const Arguments& args)
 {
	 Statement* pStatement = ObjectWrap::Unwrap<Statement>(args.This());
	 HandleScope scope;
	 int field_count = pStatement->pRecord->GetFieldCount();

	 return scope.Close(Integer::New(field_count));
 }
 Handle<Value> Statement::GetRecordCount(const Arguments& args)
 {

	 Statement* pStatement = ObjectWrap::Unwrap<Statement>(args.This());
	 HandleScope scope;
	 int record_count = pStatement->pRecord->GetRecordCount();

	 return scope.Close(Integer::New(record_count));
 }
 Handle<Value> Statement::GetFieldValueByIndex(const Arguments& args)
 {
	 Statement* pStatement = ObjectWrap::Unwrap<Statement>(args.This());
	 HandleScope scope;
	 int index = args[0]->Int32Value();
	 Sqlite3_XX::VARIANT var = pStatement->pRecord->GetFieldValue(index);
	 if (var.vt == Sqlite3_XX::VT_BSTR)
	 {
		 TSTRING value = var.bstrVal;
#ifdef OS_WIN32
		 return scope.Close(String::New(encodeConv::CodingConv::Unicode2Utf8(value.c_str()).c_str()));

#elif defined OS_LINUX
		  return scope.Close(String::New(value.c_str()));
#endif

	 }
	 else if (var.vt == Sqlite3_XX::VT_I4)
	 {
		 return scope.Close(Integer::New(var.intVal));
	 }
	 else if (var.vt == Sqlite3_XX::VT_R8)
	 {
		  return scope.Close(Number::New(var.dblVal));
	 }
	return Undefined();
	
 }
 Handle<Value> Statement::GetFieldValueByName(const Arguments& args)
 {
	  Statement* pStatement = ObjectWrap::Unwrap<Statement>(args.This());
	 HandleScope scope;
	 String::Utf8Value field_name(args[0]);
	// const char* pfield_name = ToCString(field_name);
	 TSTRING name;
#ifdef OS_WIN32

	 name = encodeConv::CodingConv::s2ws(ToCString(field_name));
#elif defined OS_LINUX
	 name = ToCString(field_name);
#endif
	 Sqlite3_XX::VARIANT var = pStatement->pRecord->GetFieldValue(name.c_str());
	 Local<Object> obj = Object::New();
	 if (var.vt == Sqlite3_XX::VT_BSTR)
	 {
		 TSTRING value = var.bstrVal;
#ifdef OS_WIN32
		 return scope.Close(String::New(encodeConv::CodingConv::Unicode2Utf8(value.c_str()).c_str()));

#elif defined OS_LINUX
		 return scope.Close(String::New(value.c_str()));
#endif

	 }
	 else if (var.vt == Sqlite3_XX::VT_I4)
	 {
		 return scope.Close(Integer::New(var.intVal));
	 }
	 else if (var.vt == Sqlite3_XX::VT_R8)
	 {
		 return scope.Close(Number::New(var.dblVal));
	 }
	 return Undefined();
	
 }


Handle<Value> Statement::GetFieldName(const Arguments& args)
 {
	 int index = args[0]->Int32Value();
	 HandleScope scope;
	 Statement* pStatement = ObjectWrap::Unwrap<Statement>(args.This());
	 TSTRING name = pStatement->pRecord->GetFieldName(index);

#ifdef OS_WIN32
	 return scope.Close(String::New(encodeConv::CodingConv::Unicode2Utf8(name.c_str()).c_str()));

	 
#elif defined OS_LINUX
	  return scope.Close(String::New(name.c_str()));

#endif
	  return Undefined();

 }


