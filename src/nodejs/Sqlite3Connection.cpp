#include "Sqlite3Connection.h"
#include "QueryJob.h"
#include "ExcuteJob.h"
#include <uv.h>
#include "NodeFunction.h"
using namespace NodeFunc;



Persistent<FunctionTemplate> Sqlite3Connection::constructorTemplate;

void Sqlite3Connection::Init(Handle<Object> target) 
{
	HandleScope scope;
	Local<FunctionTemplate> t = FunctionTemplate::New(New);
	constructorTemplate = Persistent<FunctionTemplate>::New(t);
	constructorTemplate->InstanceTemplate()->SetInternalFieldCount(1);
	constructorTemplate->SetClassName(String::NewSymbol("Connection"));

	NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "execute", Execute);
	NODE_SET_PROTOTYPE_METHOD(constructorTemplate,"executesync",ExecuteSync);
	NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "close", Close);
	NODE_SET_PROTOTYPE_METHOD(constructorTemplate,"getLastError",GetLastError);
	NODE_SET_PROTOTYPE_METHOD(constructorTemplate,"begin",BeginTrans);
	NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "commit", CommitTrans);
	NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "rollback", RollbackTrans);
	NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "close", Close);
	NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "query", Query);
	NODE_SET_PROTOTYPE_METHOD(constructorTemplate,"isopen",IsOpen);
	NODE_SET_PROTOTYPE_METHOD(constructorTemplate,"enterkey",EnterKey);
	NODE_SET_PROTOTYPE_METHOD(constructorTemplate,"changekey",ChangeKey);
	NODE_SET_PROTOTYPE_METHOD(constructorTemplate,"delkey",DelKey);
	target->Set(String::NewSymbol("Connection"), constructorTemplate->GetFunction());
}

Handle<Value> Sqlite3Connection::New(const Arguments& args) 
{
	HandleScope scope;

	Sqlite3Connection *pConn = new Sqlite3Connection();
	pConn->Wrap(args.This());
	return args.This();
}

Sqlite3Connection::Sqlite3Connection()
{
}

Sqlite3Connection::~Sqlite3Connection()
{
	closeConnection();
}



Handle<Value>Sqlite3Connection::ExecuteSync(const Arguments& args)
{
	TSTRING sql = MergeSql(args);
	Sqlite3Connection *pConn = ObjectWrap::Unwrap<Sqlite3Connection>(args.This());
	HandleScope scope;
	BOOL bRet = pConn->m_pConn->Excute(sql.c_str());
	return scope.Close(Boolean::New(bRet));

}

Handle<Value>Sqlite3Connection::Execute(const Arguments& args)
{
	TSTRING sql = MergeSql(args);
	Sqlite3Connection* connection = ObjectWrap::Unwrap<Sqlite3Connection>(args.This());
	ExcuteJob* excute_job = new ExcuteJob();
	excute_job->sql = sql;
	excute_job->pClient = connection;
	excute_job->js_obj = Persistent<Object>::New(args[args.Length()-1]->ToObject());
	excute_job->type = excute_job->EXCUTE;
	excute_job->req.data = excute_job;
	//uv_async_init(uv_default_loop(),&excute_job->main_async,excute_asyn_callback);
	connection->Ref();
	uv_queue_work(uv_default_loop(),&excute_job->req,uv_execute,uv_execute_after);
	//uv_async_send(&excute_job->main_async);
	return Undefined();

}
void Sqlite3Connection::uv_execute(uv_work_t* req)
{
	ExcuteJob* excute_job = static_cast<ExcuteJob*>(req->data);
	BOOL bret = excute_job->pClient->m_pConn->Excute(excute_job->sql.c_str());
	if (FALSE == bret)
	{
		excute_job->error = excute_job->pClient->m_pConn->GetLastError();

	}

}
void Sqlite3Connection::uv_execute_after(uv_work_t* req,int status)
{
	ExcuteJob* excute_job = static_cast<ExcuteJob*>(req->data);
	excute_job->pClient->Unref();
	HandleScope scope;
	Handle<Value> argv[1];
	if (!excute_job->error.empty())
	{
#ifdef OS_WIN32
			argv[0] = String::New(encodeConv::CodingConv::Unicode2Utf8(excute_job->error.c_str()).c_str());
#elif defined OS_LINUX
			argv[0] = String::New(excute_job->error.c_str());
#endif

	}
	else
	{
		argv[0] = Undefined();
	}
	v8::TryCatch tryCatch;
	excute_job->js_obj->CallAsFunction(Object::New(),1,argv);
	if (tryCatch.HasCaught()) 
	{
		node::FatalException(tryCatch);
	}
	excute_job->js_obj.Dispose();
	delete excute_job;
	excute_job = NULL;
	scope.Close(Undefined());
}
void Sqlite3Connection::excute_asyn_callback(uv_async_t* handle, int status)
{
	/*
	
	Handle<Value> argv[1];
	HandleScope scope;
	
	BOOL bRet = false;
	ExcuteJob* excute_job = static_cast<ExcuteJob*>(handle->data);
	
	
	switch(excute_job->type)
	{
	case  excute_job->BEGIN:
		bRet = excute_job->pClient->m_pConn->BeginTrans();
		break;
	case  excute_job->COMMIT:
		bRet = excute_job->pClient->m_pConn->CommitTrans();
		break;
	case  excute_job->ROLLBACK:
		bRet = excute_job->pClient->m_pConn->RollbackTrans();
		break;
;
	case  excute_job->CLOSE:
		bRet = excute_job->pClient->m_pConn->CloseSqlite();
		break;
	default:
		excute_job->error = _T("unknow type");
	}
	if (false == bRet)
	{
#ifdef OS_WIN32
		argv[0] = String::New(encodeConv::CodingConv::Unicode2Utf8(excute_job->pClient->m_pConn->GetLastError().c_str()).c_str());	
#elif defined OS_LINUX
		argv[0] = String::New(query_job->pConn->GetLastError().c_str());	
#endif
		
	}
	else
	{
		argv[0] = Undefined();
	}
	v8::TryCatch tryCatch;
	excute_job->js_obj->CallAsFunction(Object::New(),1,argv);
	if (tryCatch.HasCaught()) 
	{
		node::FatalException(tryCatch);
	}
	excute_job->js_obj.Dispose();
	
	uv_close((uv_handle_t*)&excute_job->main_async,NULL);
	cout<<"close"<<endl;
	delete excute_job;
	excute_job = NULL;
	scope.Close(Undefined());*/

}
 Handle<Value>Sqlite3Connection::BeginTrans(const Arguments& args)
{
	Sqlite3Connection *pConn = ObjectWrap::Unwrap<Sqlite3Connection>(args.This());
	HandleScope scope;
	BOOL bRet = pConn->m_pConn->BeginTrans();
	return scope.Close(Boolean::New(bRet));
	//ExcuteJob* excute_job = new ExcuteJob();
	//excute_job->js_obj = Persistent<Object>::New(args[args.Length()-1]->ToObject());
	//excute_job->pClient = pConn;
	//excute_job->main_async.data = excute_job;
	//excute_job->type = excute_job->BEGIN;
	//uv_async_init(uv_default_loop(),&excute_job->main_async,excute_asyn_callback);
	//uv_async_send(&excute_job->main_async);
	//return Undefined();

}

 Handle<Value>Sqlite3Connection::RollbackTrans(const Arguments& args)
 {
	 Sqlite3Connection *pConn = ObjectWrap::Unwrap<Sqlite3Connection>(args.This());
	 HandleScope scope;
	 BOOL bRet = pConn->m_pConn->RollbackTrans();
	 return scope.Close(Boolean::New(bRet));
	 /* ExcuteJob* excute_job = new ExcuteJob();
	 excute_job->js_obj = Persistent<Object>::New(args[args.Length()-1]->ToObject());
	 excute_job->pClient = pConn;
	 excute_job->main_async.data = excute_job;
	 excute_job->type = excute_job->ROLLBACK;
	 uv_async_init(uv_default_loop(),&excute_job->main_async,excute_asyn_callback);
	 uv_async_send(&excute_job->main_async);
	 return Undefined();*/

 }
 Handle<Value>Sqlite3Connection::CommitTrans(const Arguments& args)
 {
	 Sqlite3Connection *pConn = ObjectWrap::Unwrap<Sqlite3Connection>(args.This());
	 HandleScope scope;
	 BOOL bRet = pConn->m_pConn->CommitTrans();
	 return scope.Close(Boolean::New(bRet));
	 /* ExcuteJob* excute_job = new ExcuteJob();
	 excute_job->js_obj = Persistent<Object>::New(args[args.Length()-1]->ToObject());
	 excute_job->pClient = pConn;;
	 excute_job->main_async.data = excute_job;
	 excute_job->type = excute_job->COMMIT;
	 uv_async_init(uv_default_loop(),&excute_job->main_async,excute_asyn_callback);
	 uv_async_send(&excute_job->main_async);
	 return Undefined();*/

 }
 //Handle<Value>Sqlite3Connection::Close(const Arguments& args)
 //{
	// Sqlite3Connection *pConn = ObjectWrap::Unwrap<Sqlite3Connection>(args.This());
	// ExcuteJob* excute_job = new ExcuteJob();
	// excute_job->js_obj = Persistent<Object>::New(args[args.Length()-1]->ToObject());
	// excute_job->pConn = pConn->m_pConn;
	// excute_job->main_async.data = excute_job;
	// excute_job->type = excute_job->CLOSE;
	// uv_async_init(uv_default_loop(),&excute_job->main_async,excute_asyn_callback);
	// uv_async_send(&excute_job->main_async);
	// return Undefined();

 //}
Handle<Value>Sqlite3Connection::Query(const Arguments& args)
{   
	TSTRING sql = MergeSql(args);
	
	HandleScope scope;
	Sqlite3Connection* connection = ObjectWrap::Unwrap<Sqlite3Connection>(args.This());
	QueryJob* queryjob = new QueryJob();
	queryjob->sql = sql;
	queryjob->js_obj =Persistent<Object>::New(args[args.Length()-1]->ToObject());
	queryjob->pClient = connection;
	connection->Ref();
	queryjob->work_pool.data = queryjob;
	uv_queue_work(uv_default_loop(),&queryjob->work_pool,uv_query,uv_query_after);
	return Undefined();


}

void Sqlite3Connection::uv_query(uv_work_t* req)
{
	QueryJob* query_job = static_cast<QueryJob*>(req->data);
	BOOL bRet = FALSE;
	try
	{

	CSqliteRecordsetPtr ptr = query_job->pClient->m_pConn->Query(query_job->sql.c_str(),bRet);
	if (bRet == FALSE)
	{
		query_job->error = query_job->pClient->m_pConn->GetLastError();
		return;
	}
		query_job->ptr = ptr;
	}
	catch(std::exception e)
	{
		cout<<"exception"<<e.what()<<endl;
	}

	

}
void Sqlite3Connection::uv_query_after(uv_work_t* req,int status)
{
		QueryJob* query_job = static_cast<QueryJob*>(req->data);
		Handle<Value>argv[2];
		HandleScope scope;
		query_job->pClient->Unref();
		
		if (!query_job->error.empty()) // error
		{
#ifdef OS_WIN32
			//connJob->error = encodeConv::CodingConv::Unicode2Utf8(connJob->error.c_str());
			argv[0] = String::New(encodeConv::CodingConv::Unicode2Utf8(query_job->error.c_str()).c_str());	
#elif defined OS_LINUX

			argv[0] = String::New(query_job->error.c_str());	
#endif

			argv[1] = Undefined();	

		}
		else
		{
			argv[0] = Undefined();
			Handle<Object> statement = Statement::s_ct->GetFunction()->NewInstance();
			(node::ObjectWrap::Unwrap<Statement>(statement))->SetRecordsetPtr(query_job->ptr);
			argv[1] = statement;

		}

		v8::TryCatch tryCatch;
		query_job->js_obj->CallAsFunction(Object::New(),2,argv);
		if (tryCatch.HasCaught()) 
		{
			node::FatalException(tryCatch);
		}
		query_job->js_obj.Dispose();
		delete query_job;
		query_job = NULL;
		


}
void Sqlite3Connection::query_asyn_thread_work_histroy(void* arg)
{
	QueryJob* query_job = static_cast<QueryJob*>(arg);
	//uv_sem_wait(&sem);
	//query_job->queryHistoryTricks(query_job);
	//uv_sem_post(&sem);
	query_job->main_async.data = query_job;
	uv_async_send(&query_job->main_async);

}



void Sqlite3Connection::query_asyn_thread_callback(uv_async_t* req,int status)
{
	HandleScope scope;
	QueryJob* query_job = static_cast<QueryJob*>(req->data);
	Local<Value>argv[2];
	if(query_job->error !=_T(""))
	{
//#ifdef OS_WIN32
//		argv[0] = String::New(encodeConv::CodingConv::ascii2Utf8(query_job->error.c_str()).c_str());
//#else
//		argv[0] = String::New(query_job->error.c_str());
//#endif
		
#ifdef OS_WIN32
		//connJob->error = encodeConv::CodingConv::Unicode2Utf8(connJob->error.c_str());
		argv[0] = String::New(encodeConv::CodingConv::Unicode2Utf8(query_job->error.c_str()).c_str());	
#elif defined OS_LINUX

		argv[0] = String::New(query_job->error.c_str());	
#endif
		argv[1] = *Undefined();


	}
	else{
		//vector<map<string,string> >&result = query_job->result;
		//typedef vector<map<string,string> >::iterator const_it;
		//Local<Array>array = Array::New(result.size());
		//int j = 0;
		//for (const_it it = result.begin();it != result.end();++it)
		//{

		//	map<string,string>::iterator map_it;
		//	Local<Object>obj = Object::New();
		//	for (map_it =it->begin();map_it!= it->end();++map_it)
		//	{
		//		//cout<<map_it->first<<":"<<map_it->second<<endl;
		//		obj->Set(String::NewSymbol(map_it->first.c_str()), 
		//			String::New(encodeConv::CodingConv::ascii2Utf8(map_it->second.c_str()).c_str()));

		//	}
		//	array->Set(j,obj);
		//	j++;
		//}


		argv[0]=*Undefined();
#ifdef OS_WIN32
		//cout<<"win 32"<<query_job->result<<endl;
		argv[1] = String::New(encodeConv::CodingConv::Unicode2Utf8(query_job->error.c_str()).c_str());
#elif defined OS_LINUX
		argv[1] = String::New(query_job->error.c_str());
#endif

		
	
	
	}
	query_job->js_obj->CallAsFunction(Object::New(),2,argv);
	query_job->js_obj.Dispose();
	uv_close((uv_handle_t*)&query_job->main_async,uv_close_func);

	scope.Close(Undefined());

}

void Sqlite3Connection::uv_close_func(uv_handle_t* handle)
{
	
	QueryJob* job_ptr= (QueryJob *)handle->data;
	delete job_ptr;
	job_ptr = NULL;

};

Handle<Value> Sqlite3Connection::Close(const Arguments& args) 
{
	try 
	{
		Sqlite3Connection* connection = ObjectWrap::Unwrap<Sqlite3Connection>(args.This());
		
		connection->closeConnection();

		return Undefined();
	} 
	catch (const std::exception& ex) 
	{
		cout<<ex.what()<<endl;
		return Undefined();
	}
}
Handle<Value>Sqlite3Connection::IsOpen(const Arguments& args)
{
	HandleScope scope;
	BOOL bRet ;
	try
	{
		Sqlite3Connection* connection = ObjectWrap::Unwrap<Sqlite3Connection>(args.This());
		if (connection->m_pConn == NULL)  //close 时候会delete m_pConn
		{
			bRet = FALSE;
		}
		else
		{
				bRet = connection->m_pConn->IsOpen();
		}
	
	}
	
	catch (const std::exception& ex)
	{
		cout<<ex.what()<<endl;
		bRet = FALSE;
	}
	
	 return scope.Close(Boolean::New(bRet));


}

Handle<Value>Sqlite3Connection::EnterKey(const Arguments& args)
{
	Sqlite3Connection* connection = ObjectWrap::Unwrap<Sqlite3Connection>(args.This());
	HandleScope scope;
	String::Utf8Value strPath(args[0]);
	const char* key = ToCString(strPath);
	BOOL bRet = connection->m_pConn->EnterKey(key);
	return scope.Close(Boolean::New(bRet));

}
Handle<Value>Sqlite3Connection::DelKey(const Arguments& args)
{
	Sqlite3Connection* connection = ObjectWrap::Unwrap<Sqlite3Connection>(args.This());
	HandleScope scope;
	BOOL bRet = connection->m_pConn->DelKey();
	return scope.Close(Boolean::New(bRet));

}
Handle<Value>Sqlite3Connection::ChangeKey(const Arguments& args)
{
	Sqlite3Connection* connection = ObjectWrap::Unwrap<Sqlite3Connection>(args.This());
	HandleScope scope;
	String::Utf8Value strPath(args[0]);
	const char* key = ToCString(strPath);
	BOOL bRet = connection->m_pConn->ChangeKey(key);
	return scope.Close(Boolean::New(bRet));
}
Handle<Value>Sqlite3Connection::GetLastError(const Arguments& args)
{
	Sqlite3Connection* connection = ObjectWrap::Unwrap<Sqlite3Connection>(args.This());
	HandleScope scope;
	TSTRING error = connection->m_pConn->GetLastError();
	if (error.empty())
	{
		return Undefined();
	}
	else
	{
#ifdef OS_WIN32
		return scope.Close(String::New(encodeConv::CodingConv::Unicode2Utf8(error.c_str()).c_str()));
#elif defined OS_LINUX
		return scope.Close(String::New(error.c_str()));
#endif
	}
	

}
void Sqlite3Connection::setConnection(CSqliteConn* connection) 
{

	m_pConn = connection;
}


void Sqlite3Connection::closeConnection() {

	
	if( m_pConn)
	{
		m_pConn->CloseSqlite();
		delete m_pConn;
		m_pConn = NULL;
		
	}
	

}



