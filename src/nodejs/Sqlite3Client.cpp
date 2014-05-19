
#include "Sqlite3Client.h"
#include "Sqlite3Connection.h"
#include "ConnJob.h"
#include "Statement.h"
#include "encodeutil.h"
#include "NodeFunction.h"
using namespace NodeFunc;

Persistent<FunctionTemplate> Sqlite3Client::s_ct;




Sqlite3Client::Sqlite3Client(){}
Sqlite3Client::~Sqlite3Client(){}
void Sqlite3Client::Init(Handle<Object> target) 
{
	HandleScope scope;

	Local<FunctionTemplate> t = FunctionTemplate::New(New);
	s_ct = Persistent<FunctionTemplate>::New(t);
	s_ct->InstanceTemplate()->SetInternalFieldCount(1);
	s_ct->SetClassName(String::NewSymbol("Sqlite3Client"));  //名字就是以后new 的类型
	//NODE_SET_PROTOTYPE_METHOD(s_ct, "open", Open);
	//NODE_SET_PROTOTYPE_METHOD(s_ct, "open", Open);
	//只有一个函数的话在windows平台下会报错采用如下写法.
	t->PrototypeTemplate()->Set(String::NewSymbol("open"),
      FunctionTemplate::New(Open)->GetFunction());
	t->PrototypeTemplate()->Set(String::NewSymbol("openSync"),
		FunctionTemplate::New(OpenSync)->GetFunction());
	target->Set(String::NewSymbol("Sqlite3Client"), s_ct->GetFunction());
	
}

Handle<Value> Sqlite3Client::New(const Arguments& args) {
	HandleScope scope;
	Sqlite3Client *client = new Sqlite3Client();
	client->Wrap(args.This());
	return args.This();
}


Handle<Value> Sqlite3Client::Open(const Arguments& args) {
	Sqlite3Client* client = ObjectWrap::Unwrap<Sqlite3Client>(args.This());
	HandleScope scope;
	String::Utf8Value strPath(args[0]);
	ConnJob* connJob = new ConnJob();
#ifdef OS_WIN32
	connJob->path = encodeConv::CodingConv::s2ws(ToCString(strPath));
#elif defined OS_LINUX
	connJob->path = ToCString(strPath);
#endif
	
	connJob->js_obj = Persistent<Object>::New(args[args.Length()-1]->ToObject());
	connJob->pClient = client;
	connJob->main_async.data = connJob;
	uv_async_init(uv_default_loop(),&connJob->main_async,open_asyn_callback);
	//client->Ref();
	//uv_queue_work(uv_default_loop(), &connJob->req, EIO_Open, (uv_after_work_cb)EIO_AfterOpen);
	uv_async_send(&connJob->main_async);

	return Undefined();
}

Handle<Value>Sqlite3Client::OpenSync(const Arguments& args)
{
	Sqlite3Client* client = ObjectWrap::Unwrap<Sqlite3Client>(args.This());
	HandleScope scope;
	Handle<Value> argv[1];
	String::Utf8Value strPath(args[0]);
	TSTRING path;
#ifdef OS_WIN32
	path = encodeConv::CodingConv::s2ws(ToCString(strPath));
#elif defined OS_LINUX
	path = ToCString(strPath);
#endif
	CSqliteConn* pConn = new CSqliteConn();
	bool bRet = pConn->OpenSqlite(path.c_str());
	if (bRet)
	{
		Handle<Object> connection = Sqlite3Connection::constructorTemplate->GetFunction()->NewInstance();
		(node::ObjectWrap::Unwrap<Sqlite3Connection>(connection))->setConnection(pConn);
		argv[0] = connection;

	}
	else
	{
		delete pConn;
		argv[0]=Undefined();
		

	}
	return scope.Close(argv[0]);
	
}

void Sqlite3Client::open_asyn_callback(uv_async_t* handle, int status)
{

	Handle<Value> argv[2];
	HandleScope scope;
	ConnJob* connJob = static_cast<ConnJob*>(handle->data);
	BOOL bRet = false;
	try
	{
		CSqliteConn* pConn = new CSqliteConn();
		bRet = pConn->OpenSqlite(connJob->path.c_str());
		if (false == bRet)
		{
			
			connJob->error = pConn->GetLastError();
			if (pConn)
			{
				delete pConn;
				pConn = NULL;
			}
			
#ifdef OS_WIN32
			//connJob->error = encodeConv::CodingConv::Unicode2Utf8(connJob->error.c_str());
			argv[0] = String::New(encodeConv::CodingConv::Unicode2Utf8(connJob->error.c_str()).c_str());	
#elif defined OS_LINUX

			argv[0] = String::New(connJob->error.c_str());	
#endif
			argv[1] = Undefined();
			
		

		}
		else
		{
			connJob->pConnect = pConn;
			argv[0] = Undefined();
			Handle<Object> connection = Sqlite3Connection::constructorTemplate->GetFunction()->NewInstance();
			(node::ObjectWrap::Unwrap<Sqlite3Connection>(connection))->setConnection(connJob->pConnect);
			argv[1] = connection;

		}


	}
	catch(std::exception e)
	{
	
		cout<<__FILE__<<__LINE__<<e.what()<<endl;
#ifdef OS_WIN32
		connJob->error = encodeConv::CodingConv::s2ws(e.what());
#elif defined OS_LINUX
		connJob->error = e.what();
#endif


	}
	v8::TryCatch tryCatch;
	connJob->js_obj->CallAsFunction(Object::New(),2,argv);
	if (tryCatch.HasCaught()) 
	{
		node::FatalException(tryCatch);
	}
	connJob->js_obj.Dispose();
	uv_close((uv_handle_t*)&connJob->main_async,NULL);
	delete connJob;
	connJob = NULL;
	
	scope.Close(Undefined());

}

void Sqlite3Client::EIO_Open(uv_work_t* req) {
	ConnJob* connJob = static_cast<ConnJob*>(req->data);
	BOOL bRet = false;
		try
		{
			CSqliteConn* pConn = new CSqliteConn();
			if (pConn == NULL)
			{
			
				throw ("new sqlite3 fail");
			}
			bRet = pConn->OpenSqlite(connJob->path.c_str());
			if (false == bRet)
			{
				
				connJob->error = pConn->GetLastError();

			}
			else
			{
				connJob->pConnect = pConn;
			}
		
			
		}
		catch(std::exception e)
		{
			cout<<__FILE__<<__LINE__<<e.what()<<endl;
#ifdef OS_WIN32
			connJob->error = encodeConv::CodingConv::s2ws(e.what());
#elif defined OS_LINUX
			connJob->error = e.what();
#endif

			
		}


		
			
	
}

void Sqlite3Client::EIO_AfterOpen(uv_work_t* req, int status) {
	HandleScope scope;
	ConnJob* connJob = static_cast<ConnJob*>(req->data);
	connJob->pClient->Unref();
	Handle<Value> argv[2];
	if(!connJob->error.empty())
	{
		if (connJob->pConnect)
		{
			delete connJob->pConnect;
			connJob->pConnect = NULL;
		}
		
#ifdef OS_WIN32
		//connJob->error = encodeConv::CodingConv::Unicode2Utf8(connJob->error.c_str());
		argv[0] = String::New(encodeConv::CodingConv::Unicode2Utf8(connJob->error.c_str()).c_str());	
#elif defined OS_LINUX

		argv[0] = String::New(connJob->error.c_str());	
#endif
		argv[1] = Undefined();
	} 
	else 
	{
		argv[0] = Undefined();
		Handle<Object> connection = Sqlite3Connection::constructorTemplate->GetFunction()->NewInstance();
		(node::ObjectWrap::Unwrap<Sqlite3Connection>(connection))->setConnection(connJob->pConnect);
		argv[1] = connection;

		
	}

	v8::TryCatch tryCatch;
	connJob->js_obj->CallAsFunction(Object::New(),2,argv);
	if (tryCatch.HasCaught()) 
	{
		node::FatalException(tryCatch);
	}
	connJob->js_obj.Dispose();
	delete connJob;
	connJob = NULL;
	scope.Close(Undefined());
}

extern "C"
{
	static void init(Handle<Object> target) 
	{
		Sqlite3Client::Init(target);
		Sqlite3Connection::Init(target);
		Statement::Init(target);
		
	}

	NODE_MODULE(node_sqlite3, init);
}
