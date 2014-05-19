#ifndef  _ORACLE_BINDINGS_
#define _ORACLE_BINDINGS_

#include <v8.h>
#include <node.h>
using namespace node;
using namespace v8;



class Sqlite3Client : public ObjectWrap
{
public:
	static void Init(Handle<Object> target);
	static Handle<Value>New(const Arguments& args);
	static Handle<Value>Open(const Arguments& args);
	static Handle<Value>IsOpen(const Arguments& args);
	static void EIO_Open(uv_work_t* req);
	static void EIO_AfterOpen(uv_work_t* req, int status);
	static void open_asyn_callback(uv_async_t* handle, int status);
	static Handle<Value>OpenSync(const Arguments& args);
	Sqlite3Client();
	~Sqlite3Client();
	


private:
	static Persistent<FunctionTemplate> s_ct;

	
};
#endif
