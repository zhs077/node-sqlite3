#include "QueryJob.h"
//#include "../../third_party/CodingConv/encodeutil.h"

//
//struct _time_check
//{
//	DWORD _tstart;
//	DWORD _tend;
//};
//
//void tc_start (struct _time_check *tc)
//{
//
//	memset (tc, 0, sizeof (struct _time_check));
//	tc->_tstart = GetTickCount();
//
//
//}
//
////-----------------------------------------------------------------------------
//void tc_end (struct _time_check *tc)
//{
//
//	tc->_tend = GetTickCount();
//
//}
//
////-----------------------------------------------------------------------------
//double tc_val (struct _time_check *tc)
//{
//
//	return (tc->_tend - tc->_tstart);
//
//}
//void  QueryJob::queryTempdata(QueryJob* query_job)
//{
//
//	query_job->pConn->getTempdata(query_job->sql,query_job->result);
//
//	////_time_check tc;
//	////tc_start(&tc);
//	//////uv_mutex_lock()
//	////OCICursor cursor_(query_job->pConn);
//	////bool bRet = cursor_.query((query_job->sql).c_str());
//	////if (!bRet)
//	////{
//	////	
//	////	query_job->error = OCIException::getErrorMsg();
//	////	cout<<"connect error:"<<query_job->error<<endl;
//	////	return;
//
//
//	////}
//	//////int recordNums = cursor_.getRecordNums();
//	////int size = cursor_.getColNums();
//	////bRet = cursor_.moveFirst();
//	////int j = 0;
//	////string tmp;
//	////string result;
//	////try
//	////{
//	////	while(bRet)
//	////{
//	////	//map<string,string> record;
//	////	
//	////	for(int i=0;i< size;i++)
//	////	{
//	////		const char* pValue = cursor_.getFieldValue(i+1);
//	////		//cout<<"value"<<pValue<<endl;
//	////		//tmp = pValue;
//	////		
//	////		/*#ifdef OS_WIN32
//	////		const char* pColName = cursor_.getColName(i+1);
//	////		if (strcmp(pColName,"ADDRESS") == 0)
//	////		{
//	////		tmp = encodeConv::CodingConv::ascii2Utf8(pValue); 
//	////		}
//
//
//
//
//
//	////		#endif*/
//	////	
//	////		//cout<<pColName<<":"<<pVa == 0endl;
//
//	////			
//	////		//record.insert(make_pair(pColName,pValue));
//	////		result += pValue;
//	////		result += ",";
//	////	
//	////	}
//	////	//result =result.substr(0,result.length()-1);
//	////	result[result.length()-1] = ';';
//
//	////	//query_job->result.push_back(record);
//	////	bRet = cursor_.moveNext();
//	////		j++;
//	////}
//	////}
//	////
//	////catch (exception*e)
//	////{
//	////	cout<<"query:"<< e->what()<<endl;
//	////		
//	////}
//	////
//	////
//	////result[result.length()-1] = '\0';
//	////cout<<"count"<<	j<<endl;
//	//////cout <<query_job->sql<<endl;
//	////query_job->result = result;
//	////
//
//	////cursor_.close();
//	//////conn.disconnect();
//	////tc_end(&tc);
//	////cout<<"times"<<tc_val(&tc)<<"ms"<<endl;
//	
//
//
//}


//void QueryJob::queryHistoryTricks(QueryJob* queryJob)
//{
//	//string result;
//	queryJob->pConn->getHistoryTricks(queryJob->sql,queryJob->result);
//	//QueryJob->result = result;
//
//}
