#ifndef IS_OLDGCC
#include <iostream>
#else
#include <iostream.h>
#endif
#include <cstdio>
#include <string>
#include <time.h>
#include "ftnaddr.h"
#include "mask.h"
#include "area.h"
#include "action.h"
#include "msg.h"
#include "version.h"
#include "pkt.h"
#include "global.h"

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
	string logstr="Writing Message to File " + s_Filename;
	log->add(2,logstr);
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
        CMsg SrcMessage;
        CMsg TgtMessage;
        /* open messages */
        SrcMessage.Open(msgnum, Area);
        TgtMessage.New(Area);
cout << "created message" << endl;
        /* Write Headers */
        TgtMessage.F_From=SrcMessage.F_To;
        TgtMessage.s_From=SrcMessage.s_To;
        TgtMessage.F_To=SrcMessage.F_From;
        TgtMessage.s_To=SrcMessage.s_From;

        /* Write Subject */
        TgtMessage.s_Subject="[bounce]";
        TgtMessage.s_Subject+=SrcMessage.s_Subject;

        /* write attributes */
        TgtMessage.d_Attr=MSGPRIVATE;

        string s_Text;
        string s_Kludges;
cout << "writing kludges" << endl;
        /* write fmpt, topt, intl info */
        char buf[25];
        if (TgtMessage.F_From.point!=0) 
	{
	   sprintf(buf, "\001FMPT %i", TgtMessage.F_From.point);
	   s_Kludges+=buf;
	}
	if (TgtMessage.F_To.point!=0) 
	{
	   sprintf(buf, "\001TOPT %i", TgtMessage.F_To.point);
	   s_Kludges+=buf;
	}
	sprintf(buf, "\001INTL %i:%i/%i %i:%i/%i", 
				TgtMessage.F_To.zone, TgtMessage.F_To.net, TgtMessage.F_To.node,
				TgtMessage.F_From.zone, TgtMessage.F_From.net, TgtMessage.F_From.node);
	s_Kludges+=buf;
cout << "wrote kludges!" << endl;	
	string s_Temp;
	int i_spaces=0;
cout << "writing messageText" << endl;
	TgtMessage.s_Ctrl+=s_Kludges;

	TgtMessage.Write();
	TgtMessage.Close();
	SrcMessage.Close();
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
        DestMsg.d_Attr=SrcMsg.d_Attr;
	DestMsg.sent=true;
	DestMsg.Write();
	string logstr="Copied Message to Area " + destarea;
	log->add(2,logstr);
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
	DestMsg.d_Attr=SrcMsg.d_Attr;
        DestMsg.sent=true;
        DestMsg.Write();
	string logstr="Moved message to Area " + destarea;
	log->add(2, logstr); 
	SrcMsg.Delete(Area);
	SrcMsg.deleted=true;
        DestMsg.Close();
        AArea.Close();
        return 0;
}
		
int CPackmailAction::run()
{
	CPkt pkt;
	string fromnode, tonode, passwd;
        string temp=param;
	while (temp[0]==' ') temp.erase(0,1);
        do
        {
/*	   if (temp[0]==(*temp.end()))
	   {
		string logstr="Invalid packmail statement while processing from-address!";
		log->add(5, logstr);
		cerr << logstr << endl;
		exit(0);
	   }*/
           fromnode+=temp[0];
           temp.erase(0,1);
        } while (temp[0]!=' ');
        while (temp[0]==' ') temp.erase(0,1);
        do
        {
/*           if (temp[0]==(*temp.end()))
           {
                string logstr="Invalid packmail statement while processing uplink!";
                log->add(5, logstr);
                cerr << logstr << endl;
                exit(0);
           }*/
           tonode+=temp[0];
           temp.erase(0,1);
        } while (temp[0]!=' ');
	while(temp[0]==' ') temp.erase(0,1);
	passwd=temp;

	pkt.fromNode=const_cast<char*>(fromnode.c_str());
	pkt.toNode=const_cast<char*>(tonode.c_str());
	pkt.password=passwd;
	pkt.Message.Open(msgnum, Area);
        char taddr[20];
        char faddr[20];
	char viaaddr[20];
        sprintf(faddr, "%i:%i/%i.%i", 
		pkt.Message.F_From.zone, pkt.Message.F_From.net, pkt.Message.F_From.node, pkt.Message.F_From.point);
        sprintf(taddr, "%i:%i/%i.%i",
	                pkt.Message.F_To.zone, pkt.Message.F_To.net, pkt.Message.F_To.node, pkt.Message.F_To.point);
        sprintf(viaaddr, "%i:%i/%i.%i",
                        pkt.toNode.zone, pkt.toNode.net, pkt.toNode.node, pkt.toNode.point);
	string logstr="Packed Message from "; 
	logstr+=faddr;
	logstr+=" to ";
	logstr+= taddr;
	logstr+=" via "; 
	logstr+=viaaddr;
	log->add(2, logstr);
	pkt.create();
	pkt.Message.d_Attr |= MSGSENT;
	pkt.Message.Write();
	pkt.Message.Close();
	return 0;
}

int CMovemailAction::run()
{
        CPkt pkt;
        string fromnode, tonode, passwd, dir;
        string temp=param;
        while (temp[0]==' ') temp.erase(0,1);
        do
        {
           if (temp[0]==(*temp.end()))
           {
                string logstr="Invalid movemail statement! ToNode not specified";
                log->add(5, logstr);
                cerr << logstr << endl;
                exit(0);
           }
           
	   fromnode+=temp[0];
           temp.erase(0,1);
        } while (temp[0]!=' ');
        while (temp[0]==' ') temp.erase(0,1);
        do
        {
           if (temp[0]==(*temp.end()))
           {
                string logstr="Invalid movemail statement! FromNode not specified";
                log->add(5, logstr);
                cerr << logstr << endl;
                exit(0);
           }

           tonode+=temp[0];
           temp.erase(0,1);
        } while (temp[0]!=' ');
        while(temp[0]==' ') temp.erase(0,1);
        do
        {
           if (temp[0]==(*temp.end()))
           {
                string logstr="Invalid movemail statement! wrong directory";
                log->add(5, logstr);
                cerr << logstr << endl;
                exit(0);
           }

           dir+=temp[0];
           temp.erase(0,1);
        } while (temp[0]!=' ');

        passwd=temp;

        pkt.fromNode=const_cast<char*>(fromnode.c_str());
        pkt.toNode=const_cast<char*>(tonode.c_str());
        pkt.password=passwd;
	pkt.dir=dir;
        pkt.Message.Open(msgnum, Area);
        char taddr[20];
        char faddr[20];
        char viaaddr[20];
        sprintf(faddr, "%i:%i/%i.%i",
                pkt.Message.F_From.zone, pkt.Message.F_From.net, pkt.Message.F_From.node, pkt.Message.F_From.point);        sprintf(taddr, "%i:%i/%i.%i",
                        pkt.Message.F_To.zone, pkt.Message.F_To.net, pkt.Message.F_To.node, pkt.Message.F_To.point);        sprintf(viaaddr, "%i:%i/%i.%i",
                        pkt.toNode.zone, pkt.toNode.net, pkt.toNode.node, pkt.toNode.point);
        string logstr="Packed Message from ";
        logstr+=faddr;
        logstr+=" to ";
        logstr+= taddr;
        logstr+=" via ";
        logstr+=viaaddr;
        log->add(2, logstr);
        pkt.create();
        pkt.Message.d_Attr |= MSGSENT;
        pkt.Message.Write();
        pkt.Message.Close();
        return 0;
}

int CRewriteAction::run()
{
	CMask newMask;
	newMask=param;
	CMsg Message;
	Message.Open(msgnum, Area);
	/* change sender */
	if (newMask.s_Sender[0]!='*')
		Message.s_From=newMask.s_Sender;
	/* change recipient */
	if (newMask.s_Recipient[0]!='*') 
		Message.s_To=newMask.s_Recipient;
	/* change fromA */
	char *eaddr=new char[24];
	strcpy(eaddr,"65535:65535/65535.65535");
	if (newMask.F_From==eaddr) {}
	else 
		Message.F_From=newMask.F_From;
        /* change toA */
        if (newMask.F_To==eaddr) {}
	else
                Message.F_To=newMask.F_To;
 
       /* change subject */
        if (newMask.s_Subject[0]!='*')
                Message.s_Subject=newMask.s_Subject;
	Message.Write();
	Message.Close();
	delete eaddr;
	return 0;
}
 
int CDisplayAction::run()
{
	log->add(2, param);
	return 0;
}

int CSemaphoreAction::run()
{
	f_semFile=fopen(param.c_str(), "w");
	fclose(f_semFile);
	return 0;
}


