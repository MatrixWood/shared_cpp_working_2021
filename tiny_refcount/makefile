CFLAGS = -Wall -finline-functions -std=gnu11 -g -O3
CXX = g++
SRCEXT = .cpp
TGTAPP = test
CODEDIR = .

INCDIR = -I$(CODEDIR)

LIBS = 

OBJS = \
	$(TMPDIR)/refcount.o \
	$(TMPDIR)/test.o

SRCDIR = $(CODEDIR)
TMPDIR = ./tmp
TGTDIR = ./exec

$(TGTAPP) : $(TMPDIR) $(OBJS) $(TGTDIR)
	$(CXX) $(OBJS) $(CFLAGS) $(INCDIR) $(LIBS) -o $(TGTDIR)/$(TGTAPP)

$(TMPDIR):
	mkdir -p $(TMPDIR)

$(TGTDIR):
	mkdir -p $(TGTDIR)

$(TMPDIR)/%.o : $(SRCDIR)/%$(SRCEXT)
	$(CXX) -c $(CFLAGS) $(INCDIR) $< -o $@	

	
.PHONY : clean
clean :
	-rm -rf $(TMPDIR)
	-rm -rf $(TGTDIR)/$(TGTAPP)