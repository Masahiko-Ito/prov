#PROV2DIR=/usr/local/prov2
PROV2DIR=$(HOME)/prov2
AWKDIR=$(PROV2DIR)/awk
BINDIR=$(PROV2DIR)/bin
ETCDIR=$(PROV2DIR)/etc

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
	install prov2 $(BINDIR)/
	install a4v.ps $(ETCDIR)

clean:	
	rm brckt k2c ovps t2p
