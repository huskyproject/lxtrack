#include <stdio.h>
#include <iostream.h>
#include <cstring>
#include "msg.h"
#include "area.h"
#include "config.h"
#include "log.h"
#include "global.h"

int initApi()
{
	struct _minf m;
	m.req_version = 0;
        m.def_zone = cfg->F_Home.zone;
        if (MsgOpenApi(&m) != 0) 
	{
        	exit(1);
   	}
	return 0;
}

int closeApi()
{
	MsgCloseApi();
	return 0;
}

CArea::CArea()
{
}

CArea::CArea(const CArea & ar)
{
	s_Path=ar.s_Path;
	i_type=ar.i_type;
}

CArea::~CArea()
{
}

int CArea::Open(string Path)
{
        if (Path[0]=='$')
        {
                i_type=MSGTYPE_SQUISH;
                Path.erase(0,1);
        }
        else
        i_type=MSGTYPE_SDM;

	a_Area=MsgOpenArea((unsigned char *)Path.c_str(), MSGAREA_CRIFNEC, i_type);
	if (a_Area!=NULL)
	{
		i_msgNum=MsgGetHighMsg(a_Area);
		string logstr="opening area " + s_Path;
                log->add(1, logstr);
		return 0;
	}
	else
	{
		a_Area=NULL;
		cerr << "Could not open area: \"" << Path << "\"!\n";
		return -1;
	}
}

int CArea::Open()
{
	a_Area=MsgOpenArea((unsigned char *)s_Path.c_str(), MSGAREA_NORMAL, i_type);
	if (a_Area!=NULL)
	{
		i_msgNum=MsgGetHighMsg(a_Area);
		string logstr="opening area " + s_Path;
		log->add(1, logstr);
		return 0;
	}
	else
	{
		a_Area=NULL;
		cerr << "Could not open area:" << s_Path << "!\n";
		return -1;
	}
}

int CArea::Close()
{
	if (a_Area!=NULL)
	{
		MsgCloseArea(a_Area);
		return 0;
	}
	else
	{
		cerr << "Area not open!\n";
		return -1;
	}
}

HAREA CArea::GetArea()
{
	return a_Area;
}

int CArea::Scan(vector<COperation> M_ScanFor, vector<CAction> A_Execute, unsigned int start, unsigned int stop)
{
   CMsg Message;
   CMask actualMask;
   int num=1;
   int result;
   for (unsigned int i=1;i<i_msgNum+1;i++)
   {
       result=Message.Open(i, a_Area);
       if (result==-1)
       {
	  continue;
       }
       for (unsigned int j=start;j<stop+1;j++)
       {
	  actualMask=Message.GetMask();
	  /*------------ scan for matching mask ---------------*/
	  if (M_ScanFor[j].M_Mask==actualMask) 
    	  {
   	     M_ScanFor[j].M_Mask.i_match.push_back(i);
	     /*-------- execute associated actions ------------*/
	     for (int k=M_ScanFor[j].firstAction; k<M_ScanFor[j].lastAction+1;k++)
             {
                /*------ get action type -------*/
	        string type;
		string RestParam;
		string temp;

		/*------ write action data to vars -------*/
		temp=A_Execute[k].param;
        	while (temp[0]==' ') temp.erase(0,1);
	        if (temp[temp.length()-1]=='\n') temp.erase(temp.length()-1, 1);
        	do
        	{
                   type+=temp[0];
                   temp.erase(0,1);
        	} while (temp[0]!=' ');
        	while (temp[0]==' ') temp.erase(0,1);
        	RestParam=temp;
		char number[6];
		sprintf(number, "%i", i);

		/*----- action file -----*/
		if (type=="file")
		{
                   CFileAction TempAction;
                   TempAction.s_Filename=RestParam;
		   if (M_ScanFor[j].M_Mask.i_match.size()>1) TempAction.s_Filename+=number;
                   TempAction.msgnum=i;
                   TempAction.Area=a_Area;
                   TempAction.run();
                }

		/*----- action headerfile -----*/
		if (type=="hdrfile")
		{
		   CHdrFileAction TempAction;
		   TempAction.s_Filename=RestParam;
		   if (M_ScanFor[j].M_Mask.i_match.size()>1) TempAction.s_Filename+=number;
                   TempAction.msgnum=i;
                   TempAction.Area=a_Area;
                   TempAction.run();
		}
		/*----- action bounce --------*/
		if (type=="bounce")
		{
		   CBounceAction TempAction;
		   TempAction.param=RestParam;
		   TempAction.msgnum=i;
		   TempAction.Area=a_Area;
		   TempAction.run();
		}
		/*------- action copy --------*/
		if (type=="copy")
		{
		   CCopyAction TempAction;
		   TempAction.param=RestParam;
		   TempAction.Area=a_Area;
		   TempAction.msgnum=i;
		   TempAction.run();
		}
                /*------- action move --------*/
                if (type=="move")
                {
                   CMoveAction TempAction;
                   TempAction.param=RestParam;
		   TempAction.Area=a_Area;
                   TempAction.msgnum=i;
                   TempAction.run();
                }
		/*-------- packmail action -------*/
		if (type=="packmail")
		{
		    if (((Message.d_Attr & MSGSENT) != MSGSENT) && ((Message.d_Attr & MSGLOCAL) == MSGLOCAL))
		    {
		       CPackmailAction TempAction;
		       TempAction.param=RestParam;
		       TempAction.Area=a_Area;
		       TempAction.msgnum=i;
		       TempAction.run();
		    }
		}
		/*-------- movemail action -------*/
	        if (type=="movemail")
                {
                    if (((Message.d_Attr & MSGSENT) != MSGSENT) && ((Message.d_Attr & MSGLOCAL) == MSGLOCAL))
                    {
                       CMovemailAction TempAction;
                       TempAction.param=RestParam;
                       TempAction.Area=a_Area;
                       TempAction.msgnum=i;
                       TempAction.run();
                    }
                }
		/*-------- rewrite action -----------*/
		if (type=="rewrite")
		{
		    CRewriteAction TempAction;
		    TempAction.param=RestParam;
		    TempAction.Area=a_Area;
		    TempAction.msgnum=i;
		    TempAction.run();
		}
		/*--------- display action ----------*/
		if (type=="display")
		{
		    CDisplayAction TempAction;
		    TempAction.param=RestParam;
		    TempAction.run();
		}
		
		/*------- semaphore action ----------*/
		if (type=="semaphore")
		{
		    CSemaphoreAction TempAction;
		    TempAction.param=RestParam;
	            TempAction.run();
		}
		num++;
             }
	     Message.Close();
	  }
      }
   }
	return 0;
}


