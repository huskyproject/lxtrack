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
#ifndef _NODE_H
#define _NODE_H

class CFtnAddr
{
	public:				/*------- data ------*/
		int		zone;
		int		net;
		int		node;
		int		point;
		string	domain;

	public:					/*-- constructors ---*/
		CFtnAddr();
		CFtnAddr(const CFtnAddr & in);
		CFtnAddr(string str);

	public:					/*--- destructors ---*/
		~CFtnAddr();
	
	public:					/*---- operators ----*/
		/* output */
		friend ostream&	operator<<(ostream& os, CFtnAddr out);

		/* input */
		friend string&		operator>>(string& str, CFtnAddr& in);
		friend istream&	operator>>(istream& is, CFtnAddr& in);
		
		/* assignment */
		CFtnAddr&			operator=(CFtnAddr in);
		CFtnAddr&			operator=(string str);
		CFtnAddr&			operator=(char * str);
		CFtnAddr&			operator=(NETADDR addr);

		/* comparison */
		friend bool			operator==(CFtnAddr arg1, CFtnAddr arg2);
		friend bool			operator==(CFtnAddr arg1, char * arg2);

	public:					/*------- misc -------*/
		bool		isPoint();
		int		Zone();
		int		Net();
		int 		Node();
		int		Point();
		string	Domain();
	
	public:
		void		getFromStr(string& str);

};
#endif
		
