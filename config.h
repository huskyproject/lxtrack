#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <string>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include "area.h"
#include "mask.h"
#include "action.h"
#include "scndr.h"
#include "op.h"

typedef struct
{
	string s_Token;
	string s_RestOfLine;
} param;

class CConfig
{
	public:
		CConfig();
		CFtnAddr F_Home;
		string s_Home;
		string s_Outbound;
		string s_Inbound;
		string s_Log;
		vector<CScandir> S_Scandir;
		vector<COperation> O_Op;
		vector<CAction> A_Action;
	protected:
		bool openConfig(string path);
		string getLine();
		param getParam(string line);
		bool closeConfig();
		FILE * cfgFile;
};
#endif
