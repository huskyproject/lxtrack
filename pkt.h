#ifndef _PKT_H_
#define _PKT_H_
#include <string>
#include <cstdio>
#include "mask.h"
#include "msg.h"
#include "ftnaddr.h"

class CPkt
{
	public:
	   CMsg Message;
	   CFtnAddr toNode;
	   CFtnAddr fromNode;
	   int year;
	   int month;
	   int day;
	   int hour;
	   int minute;
	   int second;
	   int baud;
	   int pktver;
	   string password;
	   
	   int create();
	protected:
	   FILE *f_pkt;
	   bool newPkt;
	   int openPkt();
	   int writePkt();
	   int closePkt();
};
#endif
