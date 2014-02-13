#include"NodeFunction.h"


namespace NodeFunc
{
	const char* ToCString(const String::Utf8Value& value)
	{	
		return *value ? *value: "string conversion failed";
	}
	TSTRING Replace( const TSTRING& orignStr, const TSTRING& oldStr, const vector<TSTRING>&vc )
	{
		size_t pos = 0; 
		TSTRING tempStr = orignStr; 
		TSTRING::size_type newStrLen; 
		TSTRING::size_type oldStrLen = oldStr.length();
		int i=0;

		while(true) 
		{ 
			pos = tempStr.find(oldStr, pos); 
			if (pos == TSTRING::npos) break; 
			TSTRING s=vc.at(i);
			s = _T("\'")+s +_T("\'");
			newStrLen= s.length(); 

			tempStr.replace(pos, oldStrLen, s);         
			pos += newStrLen;
			i++;

		} 

		return tempStr; 

	}
	TSTRING MergeSql(const Arguments&args)
	{
		String::Utf8Value str(args[0]);
		const char * pstr = ToCString(str);
		TSTRING s1;
#ifdef OS_WIN32
		s1 = encodeConv::CodingConv::Utf82Unicode(pstr);
#elif defined OS_LINUX
		s1 = pstr;

#endif

		vector<TSTRING> vc;
		int length = args.Length();
		if (args[length-1]->IsFunction())
		{
			length -= 1;
		}

		for(int i = 1; i<length;i++)
		{
			String::Utf8Value str1(args[i]);
			const char * pstr_tmp = ToCString(str1);
			//cout<<"pstr_tmp:"<<pstr_tmp<<endl;
			TSTRING tmp;
#ifdef OS_WIN32
			tmp = encodeConv::CodingConv::Utf82Unicode(pstr_tmp);
#elif defined OS_LINUX
			tmp = pstr_tmp;
#endif
			vc.push_back(tmp);

		}
		TSTRING sql=Replace(s1,_T("?"),vc);

		return sql;


	}
};
