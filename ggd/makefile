all:
	gcc -o search -Wall -lstdc++ -I/opt/local/lib/glib-2.0/include/ -I/opt/local/include/glib-2.0/ -framework Cocoa -L/opt/local/lib -lglib-2.0 hash.cpp slideIO.c board.c search.c dfs.cpp searchMain.cpp
ubuntu:
	gcc -o search -Wall -lstdc++ `pkg-config --cflags --libs glib-2.0` hash.cpp slideIO.c board.c search.c dfs.cpp searchMain.cpp wfs.cpp
ubuntudebug:
	gcc -o debugSearch -g -Wall -lstdc++ `pkg-config --cflags --libs glib-2.0` hash.cpp slideIO.c board.c search.c dfs.cpp searchMain.cpp
ubuntudebugprint:
	gcc -DDEBUG -o debugSearch -g -Wall -lstdc++ `pkg-config --cflags --libs glib-2.0` hash.cpp slideIO.c board.c search.c dfs.cpp searchMain.cpp
searchDebug:
	gcc -DDEBUG -g -o debugSearchm -Wall -lstdc++ -I/opt/local/lib/glib-2.0/include/ -I/opt/local/include/glib-2.0/ -framework Cocoa -L/opt/local/lib -lglib-2.0 hash.cpp slideIO.c board.c search.c dfs.cpp searchMain.cpp
dfsTest: 
	gcc -Wall -lstdc++ -I/opt/local/lib/glib-2.0/include/ -I/opt/local/include/glib-2.0/ -framework Cocoa -L/opt/local/lib -lglib-2.0 hash.cpp slideIO.c board.c search.c dfs.cpp dfsTest.cpp
	#gcc -Wall -lstdc++ slideIO.c board.c search.c wfs.cpp wfsTest.cpp hash.cpp
debug:
	gcc -DCHECK -Wall -O0 -g -lstdc++ -I/opt/local/lib/glib-2.0/include/ -I/opt/local/include/glib-2.0/ -framework Cocoa -L/opt/local/lib -lglib-2.0 hash.cpp slideIO.c board.c search.c dfs.cpp dfsTest.cpp
	#gcc -Wall -O0 -g -lstdc++ slideIO.c board.c search.c wfs.cpp wfsTest.cpp hash.cpp
ansCheck:
	gcc -g -o ansCheck -Wall -O0 -lstdc++ slideIO.c board.c search.c ansCheck.cpp
wfs: wfs.cpp wfsMain.cpp wfs.h	
	gcc -Wall -o wfs -O3 -lstdc++ slideIO.c board.c search.c wfs.cpp wfsMain.cpp
wfsDebug: wfs.cpp	wfsMain.cpp wfs.h
	gcc -DDEBUG -g -Wall -o wfsDebug -lstdc++ slideIO.c board.c search.c wfs.cpp wfsMain.cpp
