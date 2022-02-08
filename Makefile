CC=gcc
CXX=g++
RM=rm -f

TigerClipboardServer.o: src/TigerClipboardServer.h src/TigerClipboardServer.cpp
	$(CXX) -c src/TigerClipboardServer.cpp