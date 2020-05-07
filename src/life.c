#include <curses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BOARD_WIDTH  80
#define BOARD_HEIGHT 20
#define BOARD_MEMORY_SIZE (BOARD_WIDTH * BOARD_HEIGHT * sizeof(bool))

bool state[BOARD_WIDTH][BOARD_HEIGHT];

static void tick(void) {
    int n;
    bool next[BOARD_WIDTH][BOARD_HEIGHT];
    bool north, east, south, west;

    for (int x = 0; x < BOARD_WIDTH; ++x) {
        for (int y = 0; y < BOARD_HEIGHT; ++y) {
            n = 0;

            west = (x > 0);
            east = (x < (BOARD_WIDTH - 1));
            north = (y > 0);
            south = (y < (BOARD_HEIGHT - 1));

            if (north && state[x][y - 1])               ++n;
            if (north && east && state[x + 1][y - 1])   ++n;
            if (east && state[x + 1][y])                ++n;
            if (south && east && state[x + 1][y + 1])   ++n;
            if (south && state[x][y + 1])               ++n;
            if (south && west && state[x - 1][y + 1])   ++n;
            if (west && state[x - 1][y])                ++n;
            if (north && west && state[x - 1][y - 1])   ++n;

            next[x][y] = (state[x][y] && (n == 2 || n == 3)) || n == 3;
        }
    }

    memcpy(state, next, BOARD_MEMORY_SIZE);
}

static void draw(void) {
    for (int x = 0; x < BOARD_WIDTH; ++x) {
        for (int y = 0; y < BOARD_HEIGHT; ++y) {
            mvaddch(y, x, state[x][y] ? 'X' : ' ');
        }
    }
    refresh();
}

static bool load_state(void) {
    int c = '\0';
    int x = 0;
    int y = 0;

    FILE *file = fopen("state.txt", "r");
    if (file == NULL)
        return false;

    memset(state, 0x00, BOARD_MEMORY_SIZE);

    while (true) {
        c = fgetc(file);

        if (x > BOARD_WIDTH || c == '\n') {
            x = 0;
            ++y;
            continue;
        }

        if (c == EOF || y >= BOARD_HEIGHT) 
            break;

        switch (c) {
            case 'X':
                state[x++][y] = true;
                break;
            case ' ':
                state[x++][y] = false;
                break;
            default:
                break;

        }
    }

    fclose(file);
    return true;
}

int main(void) {
    if (!load_state()) {
        printf("state.txt not found\n");
        return EXIT_FAILURE;
    }

    initscr();
    while (true) {
        draw();
        tick();
        usleep(200000);
    }

    endwin();
    return EXIT_SUCCESS;
}

