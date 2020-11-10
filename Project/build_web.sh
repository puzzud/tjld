emcc -c src/input.c -o bin/web/input.o -s USE_SDL=2
emcc -c src/color.c -o bin/web/color.o -s USE_SDL=2
emcc -c src/video.c -o bin/web/video.o -s USE_SDL=2
emcc -c src/core.c -o bin/web/core.o -s USE_SDL=2
emcc -c src/tjld.c -o bin/web/tjld.o -s USE_SDL=2

emcc bin/web/tjld.o bin/web/core.o bin/web/video.o bin/web/color.o bin/web/input.o -o bin/web/tjld.html -s USE_SDL=2

#emcc src/tjld.c src/core.c src/video.c src/color.c src/input.c -o bin/web/tjld.html -s USE_SDL=2
#emcc bin/web/tjld.o -o bin/web/tjld.html -s USE_SDL=2
