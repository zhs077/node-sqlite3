#ifndef  _DBSQLITE_H_H
#define  _DBSQLITE_H_H

#include "platform_config.h"
class CSqliteRecordset;
typedef auto_ptr<CSqliteRecordset> CSqliteRecordsetPtr;
class CSqliteConn
{
public:
	CSqliteConn(void);
	~CSqliteConn(void);
	friend class CSqliteRecordset;
public:
	//输入密钥
	BOOL EnterKey(const char* lpstrKey);
	//给没有加密的数据库添加密钥
	BOOL AddKey(const char* lpstrKey);
	//清空密钥
	BOOL DelKey();
	//变更密钥
	BOOL ChangeKey(const char* lpstrKey);

public:
	BOOL OpenSqlite(LPCTSTR lpszConnectString);	//连接数据库
	BOOL CloseSqlite();							//关闭连接
	BOOL Excute(LPCTSTR lpszSql);				//执行删除，插入，更新
	CSqliteRecordsetPtr Query(LPCTSTR lpszSql,BOOL& bRet);	//查询
	BOOL BeginTrans();							//事务开始
	BOOL RollbackTrans();						//事务回滚
	BOOL CommitTrans();							//事务提交
	TSTRING GetLastError();						//获取错误信息
	BOOL IsOpen() const;						//判断数据库是否已连接
	//BOOL IsEncrypt()const;
private:
	sqlite3* m_db;								//连接句柄
	TSTRING m_szText;							//错误信息
	BOOL m_bConnect;
	//BOOL m_bEncrypt;

private:
	//BOOL DirectStatement(TSTRING statement);		//用于事务
	//CSqliteRecordset*Statement(TSTRING statement); //用于事务

};

class CSqliteRecordset
{
public:
	friend class CSqliteConn;
public:
	CSqliteRecordset();
	~CSqliteRecordset();
	CSqliteRecordset(TSTRING statement, sqlite3* db);
public:
	//BOOL Open(CSqliteConn *pSqliteConn,LPCTSTR lpszSql);//查询
	BOOL MoveNext();									//移下一行
	BOOL MoveFirst();									//移到首行
	BOOL MoveLast();									//移到末行
public:
	long GetFieldCount();								//字段数
	long GetRecordCount();								//记录数

	TSTRING GetFieldName(int nCol);						//字段名
	Sqlite3_XX::VARIANT GetFieldValue(LPCTSTR lpszFieldName);		//字段值
	Sqlite3_XX::VARIANT GetFieldValue(int nCol);					//字段值
private:
	sqlite3_stmt* m_stmt;								//记录集句柄
	TSTRING m_szText;
	//BOOL Execute();					//用于事务
};


#endif
