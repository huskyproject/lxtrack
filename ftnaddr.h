#ifndef _FTNADDR_H_
#define _FTNADDR_H_
extern "C"
{
#include "../smapi/msgapi.h"
}

class CFtnAddr
{
	public:
		CFtnAddr();
		CFtnAddr(int a, int b, int c, int d);
		CFtnAddr(char * str);
		CFtnAddr& operator=(const CFtnAddr & ftn);
		CFtnAddr& operator=(char *ftnstr);
		CFtnAddr& operator=(const NETADDR addr);
		bool operator==(const CFtnAddr & ftn);
		bool operator==(char *ftnstr);
		int zone;
		int net;
		int node;
		int point;
	protected:
		CFtnAddr parseFromStr(char * str);
};
#endif
