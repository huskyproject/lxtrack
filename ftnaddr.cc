/******************************************************************************/
/*   _______ _______                                                          */
/*   |     | |     |            This file is part of the TTrack distribution  */
/*   --| |-- --| |--                                                          */
/*     | |     | |               Copyright (C) 1999 Michael Mleczko           */
/*     |_|     |_| RACK                                                       */
/*                                                                            */
/*     Fido     : 2:2444/1101.47                                              */
/*     Internet : 667@atomicfront.de                                          */
/*     Snail    : Hustadtring 63, 44801 Bochum                                */
/*                                                                            */
/******************************************************************************/
#include <string>
#include <fstream>
#include <iostream>
extern "C"
{
#include "../smapi/msgapi.h"
}
#include "ftnaddr.h"

CFtnAddr::CFtnAddr()
{
	// initialize with default values
	zone=65535;
	net=65535;
	node=65535;
	point=65535;
	domain="";
}

CFtnAddr::CFtnAddr(const CFtnAddr & in)
{
	// initialize with values from in
	zone=in.zone;
	net=in.net;
	node=in.node;
	point=in.point;
	domain=in.domain;
}

CFtnAddr::CFtnAddr(string str)
{
	// get data from string
	this->getFromStr(str);
}

CFtnAddr::~CFtnAddr()
{
}
	
ostream&	operator<<(ostream& os, CFtnAddr out)
{
	os << out.Zone() << ":" << out.Net() << "/" << out.Node() << "." << out.Point() << "@" << out.Domain();
	return os;
}

string& operator>>(string& str, CFtnAddr& in)
{
	in.getFromStr(str);
	return str;
}

istream&	operator>>(istream& is, CFtnAddr& in)
{
   string str;
	is >> str;
	in.getFromStr(str);
	return is;
}

CFtnAddr& CFtnAddr::operator=(CFtnAddr in)
{
	zone=in.Zone();
	net=in.Net();
	node=in.Node();
	point=in.Point();
	domain=in.Domain();
	return (*this);
}

CFtnAddr& CFtnAddr::operator=(string str)
{
	getFromStr(str);
	return (*this);
}

CFtnAddr& CFtnAddr::operator=(NETADDR addr)
{
	zone=addr.zone;
	net=addr.net;
	node=addr.node;
	point=addr.point;
	domain="";
	return (*this);
}

CFtnAddr& CFtnAddr::operator=(char * str)
{
	string str2;
	str2=str;
   getFromStr(str2);
   return (*this);
}
		
		

bool operator==(CFtnAddr arg1, CFtnAddr arg2)
{
	bool equal=true;
	if (arg1.Zone()!=arg2.Zone()) equal=false;
   if (arg1.Net()!=arg2.Net()) equal=false;
   if (arg1.Node()!=arg2.Node()) equal=false;
   if (arg1.Point()!=arg2.Point()) equal=false;
   if (arg1.Domain()!=arg2.Domain()) equal=false;
	return equal;
}

bool operator==(CFtnAddr arg1, char * arg2)
{
	bool equal=false;
	CFtnAddr addr;
	addr=arg2;
	if (arg1==addr) equal=true;
	return equal;
}

bool CFtnAddr::isPoint()
{
	if (point==0) return false;
	else return true;
}
	
void CFtnAddr::getFromStr(string& str)
{
   string Z, Ne, No, P, Dom;
	bool ZDone=false;
	bool NeDone=false;
	bool NoDone=false;
	bool PDone=false;
	bool DomDone=false;
	
	for (unsigned int i=0;i<str.length();i++)
	{
		if (str[i]==':') { ZDone=true; i++; }
		if (str[i]=='/') { NeDone=true; i++; }
		if (str[i]=='.' && PDone==false) { NoDone=true; i++; }
		if (str[i]=='@') { PDone=true; i++; }
		
		if (!ZDone)
			Z+=str[i];
		else
		if (!NeDone)
			Ne+=str[i];
		else
		if (!NoDone)
			No+=str[i];
		else
		if (!PDone)
			P+=str[i];
		else
		if (!DomDone)
			Dom+=str[i];

	}
	zone=atoi(Z.c_str());
	net=atoi(Ne.c_str());
	node=atoi(No.c_str());
	point=atoi(P.c_str());
	domain=Dom;
}

int CFtnAddr::Zone()
{
	return zone;
};

int CFtnAddr::Net()
{
	return net;
}

int CFtnAddr::Node()
{
	return node;
}

int CFtnAddr::Point()
{
	return point;
}

string CFtnAddr::Domain()
{
	return domain;
};
										
