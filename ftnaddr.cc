#include <string>
#include <malloc.h>
#include <stdlib.h>

#include "ftnaddr.h"

CFtnAddr::CFtnAddr()
{
	zone=0;
	net=0;
	node=0;
	point=0;
}

CFtnAddr::CFtnAddr(int a, int b, int c, int d)
{
        zone=a;
        net=b;
        node=c;
        point=d;
}

CFtnAddr::CFtnAddr(char *str)
{
	char *buf;
	char *strbuf;
	if (str==NULL) 
	{
		zone=65535;
		net=65535;
		node=65535;
		point=65535;
		return;
	}
	strbuf=new char[strlen(str)+1];
	strcpy(strbuf,str);
        buf=new char[strlen(str)+1];	  
	buf=strsep(&strbuf,":");
        if (buf!=NULL)
        {
                if (strstr(buf, "*") == NULL)
                        zone=atoi(buf);
                else
                        zone=65535;
        }
        else
                return;
        buf=strsep(&strbuf, "/");
        if (buf!=NULL)
        {
                if (strstr(buf, "*") == NULL)
                        net=atoi(buf);
                else
                        net=65535;
        }
        else
                return;
		  buf=strsep(&strbuf, ".");
        if (buf!=NULL)
        {
                if (strstr(buf, "*") == NULL)
                        node=atoi(buf);
                else
                        node=65535;
        }
        else
                return;
		  strcpy(buf,strbuf);
        if (buf!=NULL)
        {
                if (strstr(buf, "*") == NULL)
                        point=atoi(buf);
                else
                        point=65535;
        }
        else
                return;

}

CFtnAddr& CFtnAddr::operator=(const CFtnAddr & ftn)
{
        zone=ftn.zone;
        node=ftn.node;
        net=ftn.net;
        point=ftn.point;
        return (*this);
}

CFtnAddr& CFtnAddr::operator=(const NETADDR addr)
{
        zone=addr.zone;
        node=addr.node;
        net=addr.net;
        point=addr.point;
        return (*this);
}

CFtnAddr CFtnAddr::parseFromStr(char * str)
{
	char *buf;
	char *strbuf;
	strbuf=new char[strlen(str)];
	strcpy(strbuf,str);
        buf=new char[strlen(str)+1];
	while (strbuf[0]==' ') strsep(&strbuf, " ");
	if (strbuf[0]=='*' && strbuf[1]!=':')
	{
		zone=65535;
		net=65535;
		node=65535;
		point=65535;
		return (*this);
	}
	buf=strsep(&strbuf,":");
        if (buf!=NULL)
        {
                if (strstr(buf, "*") == NULL)
                        zone=atoi(buf);
                else
                        zone=65535;
        }
        else
        return NULL;
        buf=strsep(&strbuf, "/");
        if (buf!=NULL)
        {
                if (strstr(buf, "*") == NULL)
                        net=atoi(buf);
                else
                        net=65535;
        }
        else
                return NULL;
		  buf=strsep(&strbuf, ".");
        if (buf!=NULL)
        {
                if (strstr(buf, "*") == NULL)
                        node=atoi(buf);
                else
                        node=65535;
        }
        else
                return NULL;
		  strcpy(buf,strbuf);
        if (buf!=NULL)
        {
                if (strstr(buf, "*") == NULL)
                        point=atoi(buf);
                else
                        point=65535;
        }
        else
                return NULL;
	return (*this);
}

bool CFtnAddr::operator==(const CFtnAddr& ftn)
{
	bool zMatch=false, nMatch=false, fMatch=false, pMatch=false;	
	if (ftn.zone==zone) zMatch=true;
	if (zone==65535) zMatch=true;	
        if (ftn.net==net) nMatch=true;
        if (net==65535) nMatch=true;
        if (ftn.node==node) fMatch=true;
        if (node==65535) fMatch=true;
        if (ftn.point==point) pMatch=true;
        if (point==65535) pMatch=true;
	if (zMatch && nMatch && fMatch && pMatch)		
 		return true;
	else 
		return false;
}

bool CFtnAddr::operator==(char *ftnstr)
{
	CFtnAddr addr;
	addr=parseFromStr(ftnstr);
	if (addr==(*this)) return true;
	else return false;
}

CFtnAddr& CFtnAddr::operator=(char *ftnstr)
{
	(*this)=parseFromStr(ftnstr);
	return (*this);
}
