CC= g++
CFLAGS=	-Wall -s
CDEFS=	-DUNIX -DCONFIGDIR=\"lxtrack.cfg\"
COPT=   -O3 -m486 -fomit-frame-pointer -fstrength-reduce -fexpensive-optimizations 

TARGET= lxtrack

SMAPI=smapi

OBJS=	action.o  \
	area.o    \
	config.o  \
	ftnaddr.o \
	global.o  \
	log.o     \
	lxtrack.o \
	mask.o    \
	msg.o     \
	pkt.o

.cc.o:
	$(CC) $(CFLAGS) $(CDEFS) $(COPT) -c $< -o $@

debug: $(OBJS)
	$(CC) $(OBJS) -l$(SMAPI) -ggdb -o lxtdbg

release: $(OBJS)
	$(CC) $(OBJS) -l$(SMAPI) -s -o lxtrack

clean:
	rm lxtrack lxtdbg *.o
