
#ifndef _PLATFORM_CINFIG_H_
#define  _PLATFORM_CINFIG_H_

#if defined (__unix) || defined (__linux__) || defined(__QNX__) || defined (_AIX)
#ifndef OS_LINUX
#define OS_LINUX
#endif
#elif defined (_WIN32) || defined (__WIN32) || defined (WIN32) || defined (__WIN32__)
#ifndef OS_WIN32
#define OS_WIN32
#endif
#endif


#if defined  OS_LINUX
#define strncpy_s(dest,num,src,count) strncpy(dest,src,count)
#define sprintf_s  snprintf





#elif defined OS_WIN32

//#define UNICODE   //windows下的sqlite3使用unicode编码


#endif


#include "sqlite3_def.h"
#include <stdio.h>
#include <vector>
#include <map>
#include<cassert>
#include<string>
#include<cstring>
#include<cstdio>
#include<cstdlib>
#include <iostream>
#include <algorithm>
#include <memory>
#include <cctype>
using namespace std;


#ifdef UNICODE
typedef wstring TSTRING;
typedef const wchar_t* LPCTSTR;
typedef wchar_t TCHAR;

#define   _T(x)L ## x

#else
typedef string TSTRING;
typedef const char* LPCTSTR;
typedef char TCHAR;
#define   _T(x) x

#endif



#define  BOOL bool
#define  TRUE true
#define  FALSE false






namespace Sqlite3_XX
{
	enum VARENUM
	{
		VT_NULL =1,
		VT_I4 = 3,
		VT_BSTR = 8,
		VT_R8 = 5
	};

	typedef unsigned short VARTYPE;
	struct tagVARIANT
	{
		VARTYPE vt;

		long lVal;
		TSTRING bstrVal;
		int intVal;
		double dblVal;



	};
	typedef tagVARIANT VARIANT;



};








#endif
