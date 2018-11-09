# Alec Scheele
# Kathryn Thiese

CC=gcc
CFLAGS=-Wall -Wextra
SCAN_BUILD_DIR = scan-build-out

537make:  graph.o parseMake.o creategraph.o runMake.o
	$(CC) $(CFLAGS) *.o -o 537make -lpthread

graph.o: graph.c
	$(CC) $(CFLAGS) -c graph.c

parseMake.o: parseMake.c
	$(CC) $(CFLAGS) -c parseMake.c

creategraph.o: creategraph.c
	$(CC) $(CFLAGS) -c creategraph.c

runMake.o: runMake.c
	$(CC) $(CFLAGS) -c runMake.c

clean:
	rm -f *.o 	
	rm -f 537make

scan-build: clean
	scan-build -o $(SCAN_BUILD_DIR) make -f Makefile

scan-view: scan-build
	chrome -new-window $(SCAN_BUILD_DIR)/*/index.html

