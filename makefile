RUN 				:=
ifeq ($(OS),Windows_NT)
	RUN += g++ src/*.cpp src/native-toolkit/*.cpp -Iinclude -Llib -DOS2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -llibfftw3-3 -lcomdlg32 -lole32 -o bin64/musico.exe
else 	
	RUN += g++ src/*.cpp src/native-toolkit/*.cpp -Iinclude -lSDL2 -lSDL2_image -lSDL2_ttf -lfftw3 -o musico
endif

all: 
	$(RUN)