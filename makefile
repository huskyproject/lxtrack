CC= g++
CFLAGS=	-Wall -ggdb
CDEFS=	-DUNIX -DCONFIGDIR=\"lxtrack.cfg\"
COPT=   -O3 -m486 -fomit-frame-pointer -fstrength-reduce -fexpensive-optimizations 

TARGET= lxtrack

SMAPI=smapilnx

OBJS=	action.o  \
	area.o    \
	config.o  \
	ftnaddr.o \
	global.o  \
	log.o     \
	lxtrack.o \
	mask.o    \
	msg.o     \
	pkt.o     \
        strsep.o

.cc.o:
	$(CC) $(CFLAGS) $(CDEFS) $(COPT) -c $< -o $@

debug: $(OBJS)
	$(CC) $(OBJS) -l$(SMAPI) -g -o lxtdbg -L../smapi

release: $(OBJS)
	$(CC) $(OBJS) -l$(SMAPI) -s -o lxtrack

clean:
	rm *.o 

distclean:
	rm *.o core lxtdbg lxtrack
