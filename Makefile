# include Husky-Makefile-Config
include ../huskymak.cfg

ifeq ($(DEBUG), 1)
  CFLAGS=	$(WARNFLAGS) $(DEBCFLAGS) -I$(INCDIR)
  LFLAGS= $(DEBLFLAGS)
else
  CFLAGS=	$(WARNFLAGS) $(OPTCFLAGS) -s -I$(INCDIR)
  LFLAGS= $(OPTLFLAGS)
endif

CDEFS=-D$(OSTYPE) -DCONFIGDIR=\"lxtrack.cfg\" $(ADDCDEFS)

ifeq ($(SHORTNAME), 1)
  LOPT= -L$(LIBDIR) -lsmapi
else
  LOPT= -L$(LIBDIR) -lsmapi
endif

OBJS=	action$(OBJ)  \
	area$(OBJ)    \
	config$(OBJ)  \
	ftnaddr$(OBJ) \
	global$(OBJ)  \
	log$(OBJ)     \
	lxtrack$(OBJ) \
	mask$(OBJ)    \
	msg$(OBJ)     \
	pkt$(OBJ)     \
        strsep$(OBJ)

HDRS=action.h area.h bastypes.h config.h ftnaddr.h global.h log.h mask.h \
  msg.h op.h pkt.h scndr.h strsep.h version.h

all: lxtrack$(EXE) lxtrack.1.gz

.cc$(OBJ): $(HDRS)
	$(GPP) $(CFLAGS) $(CDEFS) $(COPT) -c $< -o $@

lxtrack$(EXE): $(OBJS)
	$(GPP) $(OBJS) $(LFLAGS) $(LOPT) -o lxtrack

lxtrack.1.gz: lxtrack.1
	gzip -c lxtrack.1 > lxtrack.1.gz

clean:
	-$(RM) $(RMOPT) *$(OBJ)
	-$(RM) $(RMOPT) core

distclean: clean
	-$(RM) $(RMOPT) lxtrack$(EXE)
	-$(RM) $(RMOPT) lxtrack.1.gz

install: lxtrack$(EXE)
	$(INSTALL) $(IBOPT) lxtrack$(EXE) $(BINDIR)
	$(INSTALL) $(IMOPT) lxtrack.1.gz $(MANDIR)/man1

uninstall:
	-$(RM) $(RMOPT) $(BINDIR)$(DIRSEP)lxtrack$(EXE) 
	-$(RM) $(RMOPT) $(MANDIR)$(DIRSEP)man1$(DIRSEP)lxtrack.1.gz 

