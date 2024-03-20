all:
	gcc src/*.c -o zinc -lm

install: all
	mv zinc /usr/local/bin/

clean:
	rm -fr zinc /usr/local/zinc *.jpg
