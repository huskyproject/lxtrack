#ifndef _MSG_H_
#define _MSG_H_
#include <string>
#include "area.h"

class CMsg
{
	public:
		/* data */
		string s_From;
		string s_To;
		string s_Subject;
		CFtnAddr F_From;
		CFtnAddr F_To;
		dword d_Attr;
		// CDate D_Written;     		/* Date functions not implemented yet! */
		// CDate D_Arrvd;			/* Date functions not implemented yet! */
		string s_Ctrl;
		string s_MsgText;
		int 	i_CtrlLen;
		int 	i_TextLen;
		long	i_Size;
		int i_number;
		
		/* methods */
		CMsg();
		CMsg(int i_num);
		int Open(int i_num, HAREA area);
		int New(HAREA area);
		int Write();
		int Close();
		int Delete(HAREA Area);
		CMask	GetMask();
		bool deleted;
		bool sent;
	protected:
		HMSG	hmsg;
		XMSG	xmsg;
		
		// none yet;
};		
#endif
