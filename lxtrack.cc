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
	int result;
	cout << "LxTrack" << " " << VERSION << endl;
	initApi();
	cfg = new CConfig;
	log = new CLog(cfg->s_Log);
	log->intro();
	for (unsigned int i=0;i<cfg->S_Scandir.size();i++)
	{
		result=cfg->S_Scandir[i].A_Area.Open();
		if (result==-1)
		{
			string logstr="could not open area ";
			logstr+=cfg->S_Scandir[i].A_Area.s_Path;
			logstr+="!";
			log->add(3, logstr);
			continue;
		}
		else 
		cfg->S_Scandir[i].A_Area.Scan(cfg->O_Op, cfg->A_Action, cfg->S_Scandir[i].firstMask, cfg->S_Scandir[i].lastMask);
	}
	closeApi();
	delete cfg;
	log->add(9, "finishing");
	log->outro();
	delete log;
	return 0;
}
