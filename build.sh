set -e

if [ -d "./build" ]; then 
 rm -r ./build
fi
mkdir ./build

CFLAGS="-Wall -Wextra -Wpedantic -Werror -g src/*.c -o build/game"

if [[ $OSTYPE == darwin* ]]; then
    # todo: static?
    clang `pkg-config --libs --cflags raylib` $CFLAGS
else
    gcc \
        -Wall -Wextra -Wpedantic -Werror \
        -I./src/vendor/raylib \
        -L./lib \
        ./src/*.c \
        -Wl,-Bstatic  -lraylib \
        -Wl,-Bdynamic -lm -lpthread -ldl -lrt -lGL -lX11 \
        -o ./build/game
fi

if [ "$1" == "--run" ]; then
    ./build/game
fi
