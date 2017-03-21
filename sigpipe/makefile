all: tserver tclient

tserver:
	make -C server
	cp server/tserver .
tclient:
	make -C client
	cp client/tclient .

clean:
	make -C server clean
	make -C client clean
	rm -f tserver
	rm -f tclient
	rm -f .bt*
