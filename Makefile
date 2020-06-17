SOURCE = inputdevs.c
OBJECT = inputdevs.o
TARGET = inputdevs.so

DEPENDENCIES = liblua5.3-dev
# !!! you should install it yourself.
# !!! "sudo apt update; sudo apt install liblua5.3-dev".

PREFIX1 = /usr/local/lib/lua/5.3
PREFIX2 = /usr/lib/x86_64-linux-gnu/lua/5.3
PREFIX3 = /usr/lib/lua/5.3
PREFIX4 = /usr/local/lib/lua/5.3

.PHONY: all clean install uninstall

all: $(TARGET)
$(TARGET): $(OBJECT)
	ld -llua5.3 -shared -fPIC $(OBJECT) -o $(TARGET)
$(OBJECT): $(SOURCE)
	gcc -c -fPIC $(SOURCE) -o $(OBJECT)

clean:
	rm $(OBJECT) $(TARGET)

install:
	mkdir -p $(PREFIX1) $(PREFIX2) $(PREFIX3) $(PREFIX4)
	cp $(TARGET) $(PREFIX1)/
	cp $(TARGET) $(PREFIX2)/
	cp $(TARGET) $(PREFIX3)/
	cp $(TARGET) $(PREFIX4)/

uninstall:
	rm -f $(PREFIX1)/$(TARGET) $(PREFIX2)/$(TARGET) $(PREFIX3)/$(TARGET) $(PREFIX4)/$(TARGET)

