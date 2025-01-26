all:
	gcc main.c -o zinc -lm -ldl

install: all
	sudo mv zinc /usr/local/bin/

uninstall:
	sudo rm -fr zinc /usr/local/zinc

remove: uninstall
