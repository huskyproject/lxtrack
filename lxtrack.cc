#include <iostream>
#include <cstdio>
#include <string>

#include "area.h"
#include "msg.h"
#include "action.h"
#include "config.h"
#include "log.h"
#include "global.h"
#include "version.h"


int main()
{
	cout << "Linux Track" << " " << VERSION << endl;
	initApi();
	cfg = new CConfig;
	log = new CLog(cfg->s_Log);
	log->intro();
	for (unsigned int i=0;i<cfg->S_Scandir.size();i++)
	{
		cfg->S_Scandir[i].A_Area.Open();
		cfg->S_Scandir[i].A_Area.Scan(cfg->O_Op, cfg->A_Action, cfg->S_Scandir[i].firstMask, cfg->S_Scandir[i].lastMask);
	}
	closeApi();
	delete cfg;
	log->add(9, "finishing");
	log->outro();
	delete log;
	return 0;
}
