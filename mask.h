#ifndef _MASK_H_
#define _MASK_H_
#include <vector>
#include <string>
#include "ftnaddr.h"
#include "bastypes.h"

class CMask
{
	public:
		/* methods */
		CMask(string sndr, string recpt, CFtnAddr frm, CFtnAddr to, string subj,
				dword flgs);
		CMask();
		~CMask();
		CMask(const CMask& msk);
		/* operators */
		CMask& operator=(const CMask& msk);
		CMask& operator=(const string& str);
		bool operator==(const CMask& msk);
		/* variables */
		string			s_Sender;
		string			s_Recipient;
		CFtnAddr		F_From;
		CFtnAddr		F_To;
		string			s_Subject;
		dword			d_Flags;
		vector<int>		i_match;
};
#endif
