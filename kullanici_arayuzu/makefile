UYGULAMA=kullanici_arayuzu

$(UYGULAMA): $(UYGULAMA).o mosq.o
	export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig/
	gcc -o $(UYGULAMA) $(UYGULAMA).o mosq.o `pkg-config --libs gtk+-3.0` -L/usr/local/lib -lmosquitto


$(UYGULAMA).o: $(UYGULAMA).c
	gcc  -c $(UYGULAMA).c `pkg-config --cflags gtk+-3.0`

mosq.o: mosq.c
	gcc -Wall -c mosq.c -I /usr/include



clean:
	rm -rf *.o $(UYGULAMA)
