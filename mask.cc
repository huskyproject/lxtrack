#include <string>
#include "mask.h"

CMask::CMask()
{
}

CMask::~CMask()
{

}

CMask::CMask(const CMask& msk)
{
        s_Sender = msk.s_Sender;
        s_Recipient = msk.s_Recipient;
        F_From = msk.F_From;
        F_To = msk.F_To;
        s_Subject = msk.s_Subject;
        d_Flags = msk.d_Flags;
}

CMask::CMask(string sndr, string recpt, CFtnAddr frm, CFtnAddr to, string subj, dword flgs)
{
	s_Sender = sndr;
	s_Recipient = recpt;
	F_From = frm;
	F_To = to;
	s_Subject = subj;
	d_Flags = flgs;
}

CMask& CMask::operator=(const CMask& msk)
{
	if ((this)==(&msk)) return (*this);
	s_Sender = msk.s_Sender;
	s_Recipient = msk.s_Recipient;
	s_Subject=msk.s_Subject;
	F_From=msk.F_From;
	F_To=msk.F_To;
	d_Flags=msk.d_Flags;
	return (*this);
}

CMask& CMask::operator=(const string& str)
{
	char *buf;
	buf = new char[128];
	buf=const_cast<char*>(str.c_str());
	s_Sender=strsep(&buf, ",");
	F_From=strsep(&buf, ",");
	s_Recipient=strsep(&buf, ",");
	F_To=strsep(&buf, ",");
	s_Subject=strsep(&buf, ",");
	strsep(&buf,",");
	delete [] buf;
	return (*this);
}

bool CMask::operator==(const CMask& msk)
{
	bool sndMatch=false;
	bool rcpMatch=false;
	bool frmMatch=false;
	bool toMatch=false;
	bool subjMatch=false;
	if (msk.s_Sender == s_Sender) sndMatch=true;
	if (s_Sender=="*") sndMatch=true;
	if (msk.s_Recipient == s_Recipient) rcpMatch=true;
	if (s_Recipient=="*") rcpMatch=true;
	if (F_From == msk.F_From) frmMatch=true;
	if (F_To == msk.F_To) toMatch=true;
	if (msk.s_Subject == s_Subject) subjMatch=true;
	if (s_Subject=="*") subjMatch=true;
	if (sndMatch && rcpMatch && frmMatch && toMatch && subjMatch) return true; 
	else return false;

}

