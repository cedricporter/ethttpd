all: ethttpd

ethttpd:
	(cd src; $(MAKE)) && mv src/ethttpd .

clean:
	rm ethttpd
	(cd src; $(MAKE) clean)
