
CC=g++
CFLAGS=-c -std=c++11 -Ilodepng

algebra.o: algebra.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) algebra.cpp 

misc.o: misc.cpp
	$(CC) $(CFLAGS) misc.cpp 

polyroots.o: polyroots.cpp
	$(CC) $(CFLAGS) polyroots.cpp 

scene.o: scene.cpp
	$(CC) $(CFLAGS) scene.cpp 

camera.o: camera.cpp
	$(CC) $(CFLAGS) camera.cpp 

photon.o: photon.cpp
	$(CC) $(CFLAGS) photon.cpp 

material.o: material.cpp
	$(CC) $(CFLAGS) material.cpp 

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp 

lodepng.o: lodepng/lodepng.cpp
	$(CC) $(CFLAGS) lodepng/lodepng.cpp -o lodepng.o

all: algebra.o misc.o polyroots.o scene.o camera.o photon.o material.o lodepng.o main.o
	$(CC) algebra.o misc.o polyroots.o scene.o camera.o photon.o material.o lodepng.o main.o -o project


