#include <cstdio>
#include <iostream>
#include <string>
#include <time.h>
#include "ftnaddr.h"
#include "mask.h"
#include "area.h"
#include "action.h"
#include "msg.h"

CAction::CAction()
{
}

CAction::CAction(const CAction & act)
{
	param=act.param;
}

CAction::~CAction()
{
}

int CFileAction::run()
{
	CMsg Message;
	Message.Open(msgnum, Area);
	f_txtFile=fopen(s_Filename.c_str(), "w");
	fputs("From: ",f_txtFile);
	fputs(Message.s_From.c_str(),f_txtFile);
	fprintf(f_txtFile, ", %i:%i/%i.%i", Message.F_From.zone, Message.F_From.net, Message.F_From.node, Message.F_From.point);
	fputs("\n",f_txtFile);
        fputs("To  : ",f_txtFile);
        fputs(Message.s_To.c_str(),f_txtFile);
        fprintf(f_txtFile, ", %i:%i/%i.%i", Message.F_To.zone, Message.F_To.net, Message.F_To.node, Message.F_To.point);
        fputs("\n",f_txtFile);
        fputs("Subject: ",f_txtFile);
        fputs(Message.s_Subject.c_str(),f_txtFile);
        fputs("\n",f_txtFile);
        fputs("---------------------\n",f_txtFile);
	for (unsigned int i=0;i<Message.s_MsgText.length();i++)
	{
	   if (Message.s_MsgText[i]=='\r') Message.s_MsgText[i]='\n';
	}
        fputs(Message.s_MsgText.c_str(),f_txtFile);
        fputs("\n",f_txtFile);
	Message.Close();
	fclose(f_txtFile);
	return 0;
}

int CHdrFileAction::run()
{
        CMsg Message;
        Message.Open(msgnum, Area);
        f_txtFile=fopen(s_Filename.c_str(), "w");
        fputs("From: ",f_txtFile);
        fputs(Message.s_From.c_str(),f_txtFile);
        fprintf(f_txtFile, ", %i:%i/%i.%i", Message.F_From.zone, Message.F_From.net, Message.F_From.node, Message.F_From.point);
        fputs("\n",f_txtFile);
        fputs("To  : ",f_txtFile);
        fputs(Message.s_To.c_str(),f_txtFile);
        fprintf(f_txtFile, ", %i:%i/%i.%i", Message.F_To.zone, Message.F_To.net, Message.F_To.node, Message.F_To.point);
        fputs("\n",f_txtFile);
        fputs("Subject: ",f_txtFile);
        fputs(Message.s_Subject.c_str(),f_txtFile);
	fputs("----------------------\n", f_txtFile);
        Message.Close();
        fclose(f_txtFile);
        return 0;
}

int CBounceAction::run()
{
	CMsg Message;
	CMsg BncMessage;
	string temp;
	string addr;
	char buf;
	char buf2[128];
	time_t tm;
	struct tm *dt;


	temp=param;

	Message.Open(msgnum, Area);
	/*------- get parameters --------*/             
	while (temp[0]==' ') temp.erase(0,1);
        if (temp[temp.size()]=='\n') temp.erase(temp.size(), 1);
        do
        {
           addr+=temp[0];
           temp.erase(0,1);
        } while (temp[0]!=' ');
        while (temp[0]==' ') temp.erase(0,1);
	bounceFname=temp;

	/*------- open bouncefile --------*/
	f_bounceTxt=fopen(bounceFname.c_str(), "r");
	while(1)
	{
		buf=fgetc(f_bounceTxt);
		if (buf==EOF) break;
		if (buf=='\n') s_BounceText+='\r';
		else s_BounceText+=buf;
	}

	/*------ replace macros with real fields -----*/
		
	s_BounceText.replace(s_BounceText.find("%from"), 5, Message.s_From);

	s_BounceText.replace(s_BounceText.find("%to"), 3, Message.s_To);

	s_BounceText.replace(s_BounceText.find("%subj"), 5, Message.s_Subject);

	string s_FirstFrom;
	int n=0;
	while (Message.s_From[n]!=' ') s_FirstFrom+=Message.s_From[n++];
	s_BounceText.replace(s_BounceText.find("%ffrom"), 6, s_FirstFrom);

        string s_FirstTo;
        n=0;
        while (Message.s_To[n]!=' ') s_FirstTo+=Message.s_To[n++];
        s_BounceText.replace(s_BounceText.find("%fto"), 6, s_FirstTo);

	/*------- write message ----------*/
	A_FromAddress=const_cast<char*>(addr.c_str());
	BncMessage.New(Area);
	BncMessage.sent=false;
	BncMessage.s_Ctrl+="\001FMPT ";
	BncMessage.s_Ctrl+="23";
	BncMessage.s_Ctrl+="\001TOPT ";
	BncMessage.s_Ctrl+="0";
	BncMessage.s_Ctrl+="";
	BncMessage.s_MsgText=s_BounceText;
	BncMessage.s_MsgText+=Message.s_MsgText;
	BncMessage.F_From=A_FromAddress;
	BncMessage.F_To=Message.F_From;
	BncMessage.s_From="LxTrack";
	BncMessage.s_To=Message.s_From;
	BncMessage.s_Subject="[Message Bounced]: ";
	BncMessage.s_Subject+=Message.s_Subject;
   	time(&tm);
   	dt = gmtime(&tm);
   	sprintf(buf2, "\001Via LxTrack/LNX %u:%u/%u.%u @%04u%02u%02u.%02u%02u%02u.UTC %s",
           BncMessage.F_From.zone, BncMessage.F_From.net, BncMessage.F_From.node, BncMessage.F_From.point,
           dt->tm_year + 1900, dt->tm_mon + 1, dt->tm_mday, dt->tm_hour+1, dt->tm_min, dt->tm_sec, "0.01");
	BncMessage.s_MsgText+=buf2;
	BncMessage.Write();
	BncMessage.Close();
	Message.Close();
	return 0;
}

int CCopyAction::run()
{
	CArea AArea;
	CMsg SrcMsg;
	CMsg DestMsg;
	destarea=param;
	AArea.Open(destarea);
	SrcMsg.Open(msgnum, Area);
	DestMsg.New(AArea.GetArea());
	DestMsg.F_From=SrcMsg.F_From;
        DestMsg.F_To=SrcMsg.F_To;
	DestMsg.s_From=SrcMsg.s_From;
	DestMsg.s_To=SrcMsg.s_To;
	DestMsg.s_Subject=SrcMsg.s_Subject;
	DestMsg.s_MsgText=SrcMsg.s_MsgText;
	DestMsg.s_Ctrl=SrcMsg.s_Ctrl;
	DestMsg.sent=true;
	DestMsg.Write();
	DestMsg.Close();
	SrcMsg.Close();
	AArea.Close();
	return 0;
}

int CMoveAction::run()
{
        CArea AArea;
        CMsg SrcMsg;
        CMsg DestMsg;
        destarea=param;
        AArea.Open(destarea);
        SrcMsg.Open(msgnum, Area);
        DestMsg.New(AArea.GetArea());
        DestMsg.F_From=SrcMsg.F_From;
        DestMsg.F_To=SrcMsg.F_To;
        DestMsg.s_From=SrcMsg.s_From;
        DestMsg.s_To=SrcMsg.s_To;
        DestMsg.s_Subject=SrcMsg.s_Subject;
        DestMsg.s_MsgText=SrcMsg.s_MsgText;
        DestMsg.s_Ctrl=SrcMsg.s_Ctrl;
        DestMsg.sent=true;
        DestMsg.Write();
	SrcMsg.Delete(Area);
	SrcMsg.deleted=true;
        DestMsg.Close();
        AArea.Close();
        return 0;
}
		
