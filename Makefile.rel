CC= g++
CFLAGS=	-Wall -s
CDEFS=	-DUNIX 
COPT=   -O3 -m486 -fomit-frame-pointer -fstrength-reduce -fexpensive-optimizations -g

TARGET= lxtrack

SMAPI=smapi

OBJS=	action.o  \
	area.o    \
	config.o  \
	ftnaddr.o \
	log.o     \
	lxtrack.o \
	mask.o    \
	msg.o     

.cc.o:
	$(CC) $(CFLAGS) $(CDEFS) $(COPT) -c $< -o $@

lxtrack: $(OBJS)
	$(CC) $(OBJS) -l$(SMAPI) -o $(TARGET)

clean:
	rm lxtrack *.o
