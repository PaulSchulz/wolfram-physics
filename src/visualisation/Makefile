.PHONY: all clean

all: wp-vis wp-vis-2

##############################################################################
wp-vis: wp-vis.o wp-vis.gresources.o
	gcc -o $@ $@.o $@.gresources.o `pkg-config --libs gtk4`

wp-vis.o: wp-vis.c
	gcc -c -o $@  `pkg-config --cflags gtk4` $<

wp-vis.gresources.o: wp-vis.gresources.c
	gcc -c -o $@ `pkg-config --cflags gtk4` $<

wp-vis.gresources.c: wp-vis.gresource.xml wp-vis.ui
	glib-compile-resources $< --target=$@ --generate-source

##############################################################################
wp-vis-2: wp-vis-2.o wp-vis-2.gresources.o
	gcc -o $@ $@.o $@.gresources.o `pkg-config --libs gtk4`

wp-vis-2.o: wp-vis-2.c
	gcc -c -o $@  `pkg-config --cflags gtk4` $<

wp-vis-2.gresources.o: wp-vis-2.gresources.c
	gcc -c -o $@ `pkg-config --cflags gtk4` $<

wp-vis-2.gresources.c: wp-vis-2.gresource.xml wp-vis-2.ui
	glib-compile-resources $< --target=$@ --generate-source

##############################################################################
clean:
	-rm launcher *.o *.gresources.c a.out
