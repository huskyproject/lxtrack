#include <string>
#include <cstdio>

#include "global.h"
#include "pkt.h"
extern "C" 
{
#include "../smapi/progprot.h"
#include "../smapi/msgapi.h"
};


int CPkt::openPkt()
{
	string completename;
	char name[9];
	sprintf(name, "%04x%04x.out", toNode.net, toNode.node);
	completename=cfg->s_Outbound + '/' + name;
	f_pkt=fopen(completename.c_str(), "rb");
	if (f_pkt==NULL) 
	{
		f_pkt=fopen(completename.c_str(), "wb");
		newPkt=true;
	}
	else
	{
		fclose(f_pkt);
		newPkt=false;
		f_pkt=fopen(completename.c_str(), "ab");
		fseek(f_pkt, -2, SEEK_END);
	}
	if (f_pkt==NULL) return -1;
	return 0;
}

int CPkt::writePkt()
{
	time_t tm;
	struct tm *dt;
	time(&tm);
	dt=localtime(&tm);
	if (newPkt==true)
	{
		fwrite(&fromNode.node, 2, 1, f_pkt);
		fwrite(&toNode.node, 2,1, f_pkt);
		fwrite(&dt->tm_year+1900, 2,1, f_pkt);
		fwrite(&dt->tm_mon+1, 2,1, f_pkt);
		fwrite(&dt->tm_mday, 2,1, f_pkt);
		fwrite(&dt->tm_hour, 2,1, f_pkt);
		fwrite(&dt->tm_min, 2,1, f_pkt);
		fwrite(&dt->tm_sec, 2,1, f_pkt);
		int pktVer=2, Baud=0;
		fwrite(&Baud, 2, 1, f_pkt);
		fwrite(&pktVer, 2, 1, f_pkt);
		fwrite(&fromNode.net, 2,1, f_pkt);
		fwrite(&toNode.net, 2,1, f_pkt);
		short prodCode=0xfe;
		short prodRev=0;
		fwrite(&prodCode, 1, 1, f_pkt);
		fwrite(&prodRev, 1, 1, f_pkt);
		fputs(password.c_str(), f_pkt);
		if (password.length()<8) for (int i=password.length();i<8;i++) fputc(0, f_pkt);
		fwrite(&fromNode.zone, 2, 1, f_pkt);
		fwrite(&toNode.zone, 2, 1, f_pkt);
		byte filler=0;
		fwrite(&filler, 2, 1, f_pkt);
		int capword=1;
		fwrite(&capword, 2, 1, f_pkt);
		prodCode=0;
		fwrite(&prodCode, 1, 1, f_pkt);
		int prodRevLow=1;
		fwrite(&prodRevLow, 1, 1, f_pkt);
                fwrite(&capword, 2, 1, f_pkt);
                fwrite(&fromNode.zone, 2, 1, f_pkt);
                fwrite(&toNode.zone, 2, 1, f_pkt);
                fwrite(&fromNode.point, 2, 1, f_pkt);
                fwrite(&toNode.point, 2, 1, f_pkt);
		int specificData=1;
 		fwrite(&specificData, 4, 1, f_pkt);
	}
	fputc(2, f_pkt);
	fputc(0, f_pkt);

	fwrite(&Message.F_From.node, 2, 1, f_pkt);
        fwrite(&Message.F_To.node, 2, 1, f_pkt);
        fwrite(&Message.F_From.net, 2, 1, f_pkt);
        fwrite(&Message.F_To.net, 2, 1, f_pkt);
	Message.d_Attr=MSGSENT|MSGPRIVATE;
	fwrite(&Message.d_Attr, 2, 1, f_pkt);
	int cost=0;
	fwrite(&cost, 2, 1, f_pkt);
	fputs("                   " , f_pkt);

	fputs(Message.s_From.data(), f_pkt);
	fputc(0, f_pkt);
        fputs(Message.s_To.data(), f_pkt);
        fputc(0, f_pkt);
        fputs(Message.s_Subject.data(), f_pkt);
        fputc(0, f_pkt);
        fputs(Message.s_Ctrl.data(), f_pkt);
        fputs(Message.s_MsgText.data(), f_pkt);
        fputc(0, f_pkt);

	return 0;
}


int CPkt::closePkt()
{
	fputc(0, f_pkt); fputc(0, f_pkt);
	if (f_pkt!=NULL) fclose(f_pkt);
	return 0;
}


int CPkt::create()
{
	openPkt();
	writePkt();
	closePkt();
	return 0;
}
