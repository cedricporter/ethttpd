all: ethttpd

ethttpd:
	(cd src; $(MAKE)) && cp src/ethttpd .

clean:
	rm ethttpd
	(cd src; $(MAKE) clean)

.PHONY: ethttpd
