.PHONY: all clean

all: wp-vis

##############################################################################
wp-vis: wp-vis.o wp-vis.gresources.o
	gcc -o $@ $@.o $@.gresources.o `pkg-config --libs gtk4 gdk-3.0` -lm

wp-vis.o: wp-vis.c
	gcc -c -o $@  `pkg-config --cflags gtk4 gdk-3.0` $<

wp-vis.gresources.o: wp-vis.gresources.c
	gcc -c -o $@ `pkg-config --cflags gtk4 gdk-3.0` $<

wp-vis.gresources.c: wp-vis.gresource.xml wp-vis.ui
	glib-compile-resources $< --target=$@ --generate-source

##############################################################################
clean:
	-rm *.o *.gresources.c

dist-clean:
	-rm wp-vis
