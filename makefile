all: 
	g++ src/*.cpp src/fileinputgui/*.cpp -Iinclude -lSDL2 -lSDL2_image -lSDL2_ttf -lfftw3 -o musico