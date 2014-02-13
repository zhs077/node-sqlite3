#ifndef _EXCUTE_JOB_H_
#define _EXCUTE_JOB_H_

#include <node.h>
#include <v8.h>
using namespace v8;
#include "Sqlite3Connection.h"




class ExcuteJob
{
public:
	TSTRING sql; //sql脚本
	Persistent<Object>js_obj;
	uv_async_t main_async;
	uv_work_t req;
	TSTRING error;
	Sqlite3Connection* pClient;
	TSTRING result; //结果
	string key; //秘钥多字节编码
	typedef enum{EXCUTE,BEGIN,COMMIT,ROLLBACK,ENTER_KEY,CHANGE_KEY,DEL_KEY,CLOSE} excute_type;
	excute_type type;
	

public:
	

	ExcuteJob(){pClient = NULL;}
	~ExcuteJob(){};


};
#endif

