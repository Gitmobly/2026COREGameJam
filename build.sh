set -e

rm -r ./build
mkdir ./build

gcc \
    -Wall -Wextra -Wpedantic -Werror \
    -I./src/vendor/raylib \
    -L./lib \
    ./src/*.c \
    -Wl,-Bstatic  -lraylib \
    -Wl,-Bdynamic -lm -lpthread -ldl -lrt -lGL -lX11 \
    -o ./build/game \

if [ "$1" == "--run" ]; then
    ./build/game
fi
