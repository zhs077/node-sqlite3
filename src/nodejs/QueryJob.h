#ifndef _QUERY_JOB_H_
#define _QUERY_JOB_H_

#include <node.h>
#include <v8.h>
using namespace v8;
#include "Sqlite3Connection.h"
#include "Statement.h"



class QueryJob
{
public:
	TSTRING sql; //sql½Å±¾
	Persistent<Object>js_obj;
	uv_async_t main_async;
	uv_work_t work_pool;
	uv_thread_t worker_thread;
	TSTRING error;
	Sqlite3Connection* pClient;
	Statement* pStatement;
	CSqliteRecordsetPtr ptr;



public:
	//static void query(string sql,OCIConnection* pConn,vector<map<string,string> >&result);
	//static void queryTempdata(QueryJob* query_job);
	//static void queryHistoryTricks(QueryJob* query_job);


	QueryJob(){pClient = NULL;pStatement = NULL;}
	~QueryJob(){};


};
#endif

