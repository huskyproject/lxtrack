#include <string>
#include <cstring>
#include <cstdio>
#include "config.h"
#include "mask.h"
	


bool CConfig::openConfig(string path)
{
	cfgFile=fopen(path.c_str(), "r");
	if (cfgFile!=NULL) return false;
	else return true;
}

bool CConfig::closeConfig()
{
	if (cfgFile!=NULL) fclose(cfgFile);
	return true;
}

string CConfig::getLine()
{
	char *line;
	string order;
	unsigned char counter=0;
	line = new char[256];
	if (fgets(line, 256, cfgFile)==NULL) return "\n";
	for (;counter<strlen(line);counter++) if (line[counter]==92) line[counter]='\\';
	if (line[0]==' ') strsep(&line, " ");
	line = strsep(&line, "\n");
	order = line;
	delete [] line;
	return order;
}	

param CConfig::getParam(string line)
{
	string token;
	param parm;
	while (line[0]==' ') line.erase(0,1);
	if (line[line.size()]=='\n') line.erase(line.size(), 1);
	do 
	{
		token+=line[0];
		line.erase(0,1);
	} while (line[0]!=' ');

	while (line[0]==' ') line.erase(0,1);	
	parm.s_Token=token;
	parm.s_RestOfLine=line;
	for (unsigned int i=0;i<parm.s_Token.length();i++) parm.s_Token[i]=tolower(parm.s_Token[i]);
	return parm;
}

CScandir::CScandir(string s_Path)
{
	if (s_Path[0]=='$') 
	{
		A_Area.i_type=MSGTYPE_SQUISH;
		s_Path.erase(0,1);
	}
	else
		A_Area.i_type=MSGTYPE_SDM;
	A_Area.s_Path=s_Path;
}	

CScandir::~CScandir()
{
}

COperation::COperation(string s_MaskStr)
{
	CMask TempMask;
	TempMask=s_MaskStr;
	M_Mask=TempMask;
}


COperation::~COperation()
{
}

CConfig::CConfig()
{
	param parm;
	string s_Line;
	string s_Token;
	
	int scn=-1, msk=-1, actn=-1;
	
	openConfig(CONFIGDIR);
	while ((s_Line=getLine())!="\n") 
	{
		if (s_Line=="") continue;
		parm=getParam(s_Line);
		if (parm.s_Token=="home")
		{
			s_Home=parm.s_RestOfLine;
		}
		if (parm.s_Token=="log")
		{
			s_Log=parm.s_RestOfLine;
		}
		if (parm.s_Token=="outbound")
		{
			s_Outbound=parm.s_RestOfLine;
		}
		if (parm.s_Token=="inbound")
		{
			s_Inbound=parm.s_RestOfLine;
		}
		if (parm.s_Token=="scandir") 
		{
			scn++;
			CScandir scndr(parm.s_RestOfLine);
			S_Scandir.push_back(scndr);
			if (msk!=-1) S_Scandir[scn].firstMask=msk+1;
			else S_Scandir[scn].firstMask=0;
			if (scn>0) S_Scandir[scn-1].lastMask=msk;
		}
		if (parm.s_Token=="mask")
		{
			msk++;
			COperation op(parm.s_RestOfLine);
			O_Op.push_back(op);
			if (actn!=-1) O_Op[msk].firstAction=actn+1;
			else O_Op[msk].firstAction=0;
			if (msk>0) O_Op[msk-1].lastAction=actn;
		}
		if (parm.s_Token=="action")
		{
			actn++;
			CAction act;
			act.param=parm.s_RestOfLine;
			A_Action.push_back(act);
		}
	}
	if (S_Scandir.size()==1) S_Scandir[0].lastMask=O_Op.size()-1; 
	if (O_Op.size()==1) O_Op[0].lastAction=A_Action.size()-1;
	O_Op[O_Op.size()-1].lastAction=A_Action.size()-1;
	closeConfig();	 
}
