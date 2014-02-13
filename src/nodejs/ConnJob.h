#ifndef _CONN_JOB_
#define  _CONN_JOB_
#include "DbSqlite.h"
#include "Sqlite3Client.h"
#include <node.h>
#include <v8.h>
using namespace v8;
class ConnJob
{
public:
	ConnJob(void)
	{
		pClient = NULL;
		//error = NULL;
		error.clear();
		pConnect = NULL;
	}
	~ConnJob(void){}
public:
	//string strDblink;
	//string strUserName;
	//string strPwd;
	TSTRING path;
	Persistent<Object>js_obj;
	Sqlite3Client* pClient;
	//const char* error;
	TSTRING error;
	CSqliteConn* pConnect;

	uv_work_t req;
	uv_async_t main_async;

};

#endif
