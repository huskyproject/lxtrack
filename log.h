#ifndef _LOG_H_
#define _LOG_H_

#include <cstdio>
#include <string>
#include <time.h>

class CLog
{
	public:
	    CLog();
	    ~CLog();
	    CLog(string s_File);
	    int add(int type, string str);
	    int intro();
	    int outro();
	    bool noclose;
	protected:
	    FILE *f_log;
	    time_t tm;
	    struct tm *dt;	
};
#endif
