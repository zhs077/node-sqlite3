#include "DbSqlite.h"



inline TCHAR  mytoupper(TCHAR &elem)
{
	if (islower(elem))
	{
		elem -=32;

	}
	return elem;

}


CSqliteConn::CSqliteConn()
{
		//m_bEncrypt = FALSE;
		m_bConnect = FALSE;
		m_db = NULL;
		m_szText = _T("");
};
CSqliteConn::~CSqliteConn()
{
	if (m_db)
	{
		sqlite3_close(m_db);
	}
};

BOOL CSqliteConn::IsOpen()const
{
	return m_bConnect;
}


BOOL CSqliteConn::OpenSqlite(LPCTSTR lpszConnectString)
{
	m_bConnect = true;
	m_szText = lpszConnectString;

	int rc = _sqlite3_open(lpszConnectString, &m_db);

	if ( rc != SQLITE_OK )
	{
		m_bConnect = FALSE;
	}

	return m_bConnect;
}
BOOL CSqliteConn::CloseSqlite()
{
	 int rc = _sqlite3_close(m_db);
	 m_db = NULL;
	 return rc == SQLITE_OK ? TRUE : FALSE;
}

CSqliteRecordsetPtr CSqliteConn::Query(LPCTSTR lpszSql,BOOL &bRetVal)
{
	CSqliteRecordsetPtr pSqliteRecordSet(new CSqliteRecordset());
	bRetVal = TRUE;
	if (m_db == NULL)
	{
		m_szText = _T("数据库未连接");
		return pSqliteRecordSet;
	}
	int nRet = 0;
	nRet = _sqlite3_prepare(m_db,lpszSql,-1,&pSqliteRecordSet->m_stmt,0);
	if (nRet != SQLITE_OK)
		bRetVal = FALSE;
	return pSqliteRecordSet;

}

BOOL CSqliteConn::Excute(LPCTSTR lpszSql)
{
	sqlite3_stmt *stmt;
	int ret;
	ret=_sqlite3_prepare(m_db,lpszSql,-1,&stmt,0);
	ret= _sqlite3_step(stmt);
	if(ret != SQLITE_DONE)
	{
		return FALSE;
	}
	_sqlite3_finalize(stmt);//避免数据库被锁
	return TRUE;
}
//CSqliteRecordset* CSqliteConn::Statement(TSTRING statement)
//{
//	CSqliteRecordset* stmt = NULL;
//	stmt = new CSqliteRecordset(statement, m_db);
//	return stmt;
//}

//BOOL CSqliteConn::DirectStatement(TSTRING sql)
//{
//	CSqliteRecordset* pStatement = this->Statement(sql);
//	bool fResult = pStatement->Execute();
//	delete pStatement;
//	return fResult;
//
//}
BOOL CSqliteConn::BeginTrans()
{
	//m_szText = _T("begin");
	//return DirectStatement(_T("begin"));
	int rc = _sqlite3_exec(m_db,"begin transaction",0,0,NULL);
	return rc == SQLITE_OK ? TRUE : FALSE;
}


BOOL CSqliteConn::CommitTrans()
{
	//m_szText = _T("commit");
	int rc = _sqlite3_exec(m_db,"commit transaction",0,0,NULL);
	return rc == SQLITE_OK ? TRUE : FALSE;
}

BOOL CSqliteConn::RollbackTrans()
{
	//m_szText = _T("rollback");
	int rc = _sqlite3_exec(m_db,"rollback transaction",0,0,NULL);
	return rc == SQLITE_OK ? TRUE : FALSE;
}
TSTRING CSqliteConn::GetLastError()
{
	m_szText = (const TCHAR*)_sqlite3_errmsg(m_db);
	return m_szText;
}


//针对事务
//BOOL CSqliteRecordset::Execute()
//{
//	//TSTRING szMessage;
//	//TSTRING szCaption = _T("CSqlStatement::Execute");
//
//	int rc = _sqlite3_step(m_stmt);
//
//	if ( rc == SQLITE_BUSY )
//	{
//		//szMessage = _T("SQLITE_BUSY");
//
//		return FALSE;
//	}
//
//	if ( rc == SQLITE_ERROR )
//	{
//		//szMessage = _T("SQLITE_ERROR");
//
//		return FALSE;
//	}
//
//	if ( rc == SQLITE_MISUSE )
//	{
//		//szMessage = _T("SQLITE_MISUSE");
//		return FALSE;
//	}
//
//	if ( rc != SQLITE_DONE )
//	{
//		return FALSE;
//	}
//
//	_sqlite3_reset(m_stmt);
//	return TRUE;
//}
//

BOOL CSqliteConn::EnterKey(const char* lpstrKey)
{
	int rc = sqlite3_key(m_db,lpstrKey,strlen(lpstrKey));
    return rc == SQLITE_OK ? TRUE : FALSE;
	/*m_bEncrypt =TRUE;
	if(rc != SQLITE_OK)
	{
		m_bEncrypt= FALSE;
	}
	return m_bEncrypt;*/
}
BOOL CSqliteConn::ChangeKey(const char* lpstrKey)
{

	int rc =  sqlite3_rekey(m_db,lpstrKey,strlen(lpstrKey));
	return rc == SQLITE_OK ? TRUE : FALSE;



}
BOOL CSqliteConn::DelKey()
{
	int rc = sqlite3_rekey(m_db,NULL,0);

    return rc == SQLITE_OK ? TRUE : FALSE;


}

CSqliteRecordset::CSqliteRecordset(TSTRING statement, sqlite3* db)
{
	m_stmt = NULL;

	int rc = _sqlite3_prepare(db, statement.c_str(), -1, &m_stmt, 0 );

	if (rc != SQLITE_OK)
	{
		m_stmt = NULL;
	}
}

CSqliteRecordset::CSqliteRecordset()
{
	m_stmt = NULL;
}
CSqliteRecordset::~CSqliteRecordset()
{
	if (m_stmt)
	{
		_sqlite3_finalize(m_stmt);
	}
}

BOOL CSqliteRecordset::MoveFirst()
{

	if(_sqlite3_reset(m_stmt) != SQLITE_OK)
	{
		return FALSE;
	}
	if(_sqlite3_step(m_stmt) != SQLITE_ROW)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CSqliteRecordset::MoveNext()
{
	TSTRING szError;
	int rc = _sqlite3_step(m_stmt);

	if ( rc == SQLITE_ROW )
	{
		return true;
	}

	if ( rc == SQLITE_DONE )
	{
		_sqlite3_reset(m_stmt);
		return false;
	}
	else if ( rc == SQLITE_MISUSE )
	{
		//szError = _T("CSqlStatement::NextRow SQLITE_MISUSE");
	}
	else if ( rc == SQLITE_BUSY )
	{
		//szError = _T("CSqlStatement::NextRow SQLITE_BUSY");
	}
	else if ( rc == SQLITE_ERROR )
	{
		//szError = _T("CSqlStatement::NextRow SQLITE_ERROR");
	}


	return FALSE;
}
long CSqliteRecordset::GetRecordCount()
{
	long records=0;
	while(MoveNext())
	{
		records++;
	}
	MoveFirst();
	return records;


}
long CSqliteRecordset::GetFieldCount()
{
	return _sqlite3_column_count(m_stmt);
}



TSTRING CSqliteRecordset::GetFieldName(int nCol)
{
	const TCHAR * temp =(const TCHAR *)(_sqlite3_column_name(m_stmt, nCol));
	m_szText = temp;
	transform(m_szText.begin(), m_szText.end(), m_szText.begin(), mytoupper);

	return m_szText;
}

Sqlite3_XX::VARIANT CSqliteRecordset::GetFieldValue(LPCTSTR lpszFieldName)
{
	int count = _sqlite3_column_count(m_stmt);
	TSTRING str = lpszFieldName;
	transform(str.begin(), str.end(), str.begin(), mytoupper);
	Sqlite3_XX::VARIANT var;
	for (int i = 0; i < count; ++i)
	{
		TSTRING fieldName = GetFieldName(i);
		//cout<<fieldName<<endl;
		if (fieldName == str)
		{
			var =  GetFieldValue(i);
			break;

		}
	}

	return var;


}
Sqlite3_XX:: VARIANT CSqliteRecordset::GetFieldValue(int nCol)
{
	int dt = _sqlite3_column_type(m_stmt, nCol);
	Sqlite3_XX::VARIANT var;
	switch(dt)
	{
	case SQLITE3_TEXT:
		var.vt = Sqlite3_XX::VT_BSTR;
		var.bstrVal = (TCHAR *)_sqlite3_column_text(m_stmt,nCol);

		break;
	case  SQLITE_INTEGER:
		var.vt = Sqlite3_XX::VT_I4;
		var.intVal = _sqlite3_column_int(m_stmt, nCol);

		break;
	case SQLITE_FLOAT:
		var.vt = Sqlite3_XX::VT_R8;
		var.dblVal = _sqlite3_column_double(m_stmt,nCol);
		break;
	case SQLITE_NULL:
		var.vt = Sqlite3_XX::VT_NULL;
		break;



	}
	return var;


}
