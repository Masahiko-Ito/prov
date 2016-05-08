#PROVDIR=/usr/local/prov
PROVDIR=$(HOME)/prov
AWKDIR=$(PROVDIR)/awk
BINDIR=$(PROVDIR)/bin
ETCDIR=$(PROVDIR)/etc

all:	brckt k2c ovps t2p

brckt:	brckt.c
	gcc -O -o $@ $<
k2c:	k2c.c
	gcc -O -o $@ $<
ovps:	ovps.c
	gcc -O -o $@ $<
t2p:	t2p.c
	gcc -O -o $@ $<

install:
	install -d $(AWKDIR) $(BINDIR) $(ETCDIR)
	install csv2ps.awk $(AWKDIR)/
	install -s brckt k2c ovps t2p $(BINDIR)/
	install prov $(BINDIR)/
	install a4v.ps $(ETCDIR)

clean:	
	rm brckt k2c ovps t2p
