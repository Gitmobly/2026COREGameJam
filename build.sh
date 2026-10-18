set -e

if [ -d "./build" ]; then 
 rm -r ./build
fi
mkdir ./build

CFLAGS_STATIC="-I src/vendor/raylib -L lib -Wl,-Bstatic -lraylib -Wl,-Bdynamic -lm -lpthread -ldl -lrt -lGL -lX11"
CFLAGS="-Wall -Wextra -Wpedantic -Werror -g src/*/*.c src/*.c -o build/game"

if [[ $OSTYPE == darwin* ]]; then
    # todo: static?
    clang `pkg-config --libs --cflags raylib` $CFLAGS
else
    gcc $CFLAGS_STATIC $CFLAGS
fi

if [ "$1" == "--run" ]; then
    ./build/game
fi