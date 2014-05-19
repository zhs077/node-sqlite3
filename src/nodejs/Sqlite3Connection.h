#ifndef _SQLITE3_CONNECTION_H_
#define  _SQLITE3_CONNECTION_H_
#include <v8.h>
#include <node.h>
using namespace node;
using namespace v8;
#include "DbSqlite.h"


class Sqlite3Connection : public ObjectWrap
{
	 
public:
	static void Init(Handle<Object> target);
	static Handle<Value> New(const Arguments& args);
	static Persistent<FunctionTemplate> constructorTemplate;
	static Handle<Value>Close(const Arguments& args);
	static Handle<Value>Query(const Arguments& args);
	static Handle<Value>Execute(const Arguments& args);
	static Handle<Value>ExecuteSync(const Arguments& args);
	static Handle<Value>EnterKey(const Arguments& args);
	static Handle<Value>DelKey(const Arguments& args);
	static Handle<Value>ChangeKey(const Arguments& args);
	static Handle<Value>BeginTrans(const Arguments& args);
	static Handle<Value>RollbackTrans(const Arguments& args);
	static Handle<Value>CommitTrans(const Arguments& args);
	static Handle<Value>IsOpen(const Arguments& args);
	static Handle<Value>GetLastError(const Arguments& args);
	void closeConnection();
	Sqlite3Connection();
	~Sqlite3Connection();
	void setConnection(CSqliteConn*connection);
private:
	CSqliteConn* m_pConn;


private:
	//static void query_asyn_thread_work_tempdata(void* arg);
	static void query_asyn_thread_work_histroy(void* arg);
	static void query_asyn_thread_callback(uv_async_t* req,int status);

	static void uv_close_func(uv_handle_t* handle);
	static void excute_asyn_callback(uv_async_t* handle, int status);
	static void uv_execute(uv_work_t* req);
	static void uv_execute_after(uv_work_t* req,int status);
	static void uv_query(uv_work_t* req);
	static void uv_query_after(uv_work_t* req,int status);

	
};

#endif
