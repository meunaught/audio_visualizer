g++.exe src/*.cpp -Iinclude -Llib -DOS2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -llibfftw3-3 -o bin64/musico.exe
cd bin64
./musico.exe
cd ..
