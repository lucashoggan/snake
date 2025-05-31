#include <SDL3/SDL.h>
#include <iostream>
#include <algorithm>
#include <random>

const bool INFINITE_PLAY = true;
const int APPLE_COUNT = 4;


const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;



const int BOARD_ROWS = 20;
const int BOARD_COLS = 20;
const int BOARD_HEIGHT = 500;
const int BOARD_WIDTH = 500;
const int BOARD_X_START = (WINDOW_WIDTH-BOARD_WIDTH)/2;
const int BOARD_Y_START = (WINDOW_HEIGHT-BOARD_HEIGHT)/2;

const int PLACE_HEIGHT = BOARD_HEIGHT/BOARD_ROWS;
const int PLACE_WIDTH =BOARD_WIDTH/BOARD_COLS;

const int SNAKE_COLOUR[4] = {0, 255, 0, 255};
const int BLANK_COLOUR[4] = {255, 255, 255, 255};
const int APPLE_COLOUR[4] = {255, 0, 0, 255};

void renderBoard(SDL_Renderer *renderer, int board[BOARD_COLS*BOARD_ROWS]) {
    for (int j=0;j<BOARD_ROWS;j++) {
        for (int i=0;i<BOARD_COLS;i++) {
            SDL_FRect tmp_rect {BOARD_X_START+i*PLACE_WIDTH, BOARD_Y_START+j*PLACE_HEIGHT, PLACE_WIDTH, PLACE_HEIGHT};
            int place_val = board[BOARD_ROWS*j+i];
            if (place_val == -1) {
                SDL_SetRenderDrawColor(renderer, APPLE_COLOUR[0], APPLE_COLOUR[1], APPLE_COLOUR[2], APPLE_COLOUR[3]);
            } else if (place_val == 0) {
                SDL_SetRenderDrawColor(renderer, BLANK_COLOUR[0], BLANK_COLOUR[1], BLANK_COLOUR[2], BLANK_COLOUR[3]);
            } else {
                SDL_SetRenderDrawColor(renderer, SNAKE_COLOUR[0], SNAKE_COLOUR[1], SNAKE_COLOUR[2], SNAKE_COLOUR[3]);
            }
            SDL_RenderFillRect(renderer, &tmp_rect);
        }
    }
}

int random_int(int min, int max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(min, max);
    return distrib(gen);
}

void getSnakeHeadPos(int board[BOARD_ROWS*BOARD_COLS], int snakeLength, int &r, int &c) {
    for (int i=0;i<BOARD_ROWS;i++) {
        for (int j=0;j<BOARD_COLS;j++) {
            if (board[BOARD_ROWS*i+j] == snakeLength) {
                r = i;
                c = j;
            } 
        }
    }
    
    
}

bool moveInBound(int board[BOARD_ROWS*BOARD_COLS], int snakeLength, int dir) {
    int snakeHeadPos[2] = {-1, -1}; // row n, col n
    getSnakeHeadPos(board, snakeLength, snakeHeadPos[0], snakeHeadPos[1]);
    if (snakeHeadPos[0] == 0 && dir == 0) {
        return false;
    } else if (snakeHeadPos[0] == BOARD_ROWS-1 && dir == 2) {
        return false;
    } else if (snakeHeadPos[1] == 0 && dir == 3) {
        return false;
    } else if (snakeHeadPos[1] == BOARD_COLS-1 && dir == 1) {
        return false;
    }
    return true;
}

bool moveHitSnake(int board[BOARD_ROWS*BOARD_COLS], int snakeLength, int dir) {
    int snakeHeadPos[2] = {-1, -1}; // row n, col n
    getSnakeHeadPos(board, snakeLength, snakeHeadPos[0], snakeHeadPos[1]);
    if (dir == 0) /* upwards */ {
        if (board[BOARD_ROWS*(snakeHeadPos[0]-1)+snakeHeadPos[1]] > 0) return true;
    } else if (dir == 1) /* right */ {
        if (board[BOARD_ROWS*snakeHeadPos[0] + snakeHeadPos[1]+1] > 0) return true;
    } else if (dir == 2) /* down */ {
        if (board[BOARD_ROWS*(snakeHeadPos[0]+1)+snakeHeadPos[1]] > 0) return true;
    } else if (dir == 3) /* left */ {
        if (board[BOARD_ROWS*snakeHeadPos[1] + snakeHeadPos[1]-1] > 0) return true;
    }
    return false;
}

bool moveHitApple(int board[BOARD_ROWS*BOARD_COLS], int snakeLength, int dir) {
    int snakeHeadPos[2] = {-1, -1}; // row n, col n
    getSnakeHeadPos(board, snakeLength, snakeHeadPos[0], snakeHeadPos[1]);
    if (dir == 0) /* upwards */ {
        if (board[BOARD_ROWS*(snakeHeadPos[0]-1)+snakeHeadPos[1]] == -1) return true;
    } else if (dir == 1) /* right */ {
        if (board[BOARD_ROWS*snakeHeadPos[0] + snakeHeadPos[1]+1] == -1) return true;
    } else if (dir == 2) /* down */ {
        if (board[BOARD_ROWS*(snakeHeadPos[0]+1)+snakeHeadPos[1]] == -1) return true;
    } else if (dir == 3) /* left */ {
        if (board[BOARD_ROWS*snakeHeadPos[1] + snakeHeadPos[1]-1] == -1) return true;
    }
    return false;
}

void generateApple(int board[BOARD_ROWS*BOARD_COLS]) {
    int idx;
    do {
        idx = random_int(0, BOARD_ROWS*BOARD_COLS);
    } while (board[idx] != 0);
    board[idx] = -1;
}

bool minApplesMet(int board[BOARD_ROWS*BOARD_COLS]) {
    int count = 0;
    for (int i;i<BOARD_ROWS*BOARD_COLS;i++) {
        if (board[i] == -1) count+=1;
    }
    return APPLE_COUNT == count;
}

void addToSnake(int board[BOARD_ROWS*BOARD_COLS], int snakeLength, int dir) {
    int snakeHeadPos[2] = {-1, -1}; // row n, col n
    getSnakeHeadPos(board, snakeLength, snakeHeadPos[0], snakeHeadPos[1]);
    if (dir == 0) /* upwards */ {
        board[BOARD_ROWS*(snakeHeadPos[0]-1)+snakeHeadPos[1]] = snakeLength+1;
    } else if (dir == 1) /* right */ {
        board[BOARD_ROWS*snakeHeadPos[0]+snakeHeadPos[1]+1] = snakeLength+1;
    } else if (dir == 2) /* downwards */ {
        board[BOARD_ROWS*(snakeHeadPos[0]+1)+snakeHeadPos[1]] = snakeLength+1;
    } else if (dir == 3) /* left */ {
        board[BOARD_ROWS*snakeHeadPos[0]+snakeHeadPos[1]-1] = snakeLength+1;
    }
}

void moveSnake(int board[BOARD_ROWS*BOARD_COLS], int snakeLength, int dir /* direction */) {
    int snakeHeadPos[2] = {-1, -1}; // row n, col n
    getSnakeHeadPos(board, snakeLength, snakeHeadPos[0], snakeHeadPos[1]);

    for (int i=0;i<BOARD_ROWS;i++) {
        for (int j=0;j<BOARD_COLS;j++) {
            if (board[BOARD_ROWS*i+j] != 0 && board[BOARD_ROWS*i+j] != -1) /* if place is part of snake, decrement value */ {
                board[BOARD_ROWS*i+j] = board[BOARD_ROWS*i+j]-1;
            }
        }
    }
    if (dir == 0) /* upwards */ {
        board[BOARD_ROWS*(snakeHeadPos[0]-1)+snakeHeadPos[1]] = snakeLength;
    } else if (dir == 1) /* right */ {
        board[BOARD_ROWS*snakeHeadPos[0]+snakeHeadPos[1]+1] = snakeLength;
    } else if (dir == 2) /* downwards */ {
        board[BOARD_ROWS*(snakeHeadPos[0]+1)+snakeHeadPos[1]] = snakeLength;
    } else if (dir == 3) /* left */ {
        board[BOARD_ROWS*snakeHeadPos[0]+snakeHeadPos[1]-1] = snakeLength;
    }
}

int main() {
    int board[BOARD_ROWS*BOARD_COLS];
    int snakeLength = 3;
    int dir = 1; // direction
    std::fill_n(board, BOARD_ROWS*BOARD_COLS, 0);
    board[0] = 1;
    board[1] = 2;
    board[2] = 3;
    for (int i=0;i<APPLE_COUNT;i++) (generateApple(board));
    
    //devPrintBoard(board);

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL_Init failed %s", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Snake", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
    if (!window) {
        SDL_Log("CreateWindow failed: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Event event;
    bool running = true;

    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
    const bool* keys;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }

            if (event.type == SDL_EVENT_KEY_DOWN) {
                if (event.key.key == SDLK_UP && dir != 2) {
                    dir = 0;
                } else if (event.key.key == SDLK_RIGHT && dir != 3) {
                    dir = 1;
                } else if (event.key.key == SDLK_DOWN && dir != 0) {
                    dir = 2;
                } else if (event.key.key = SDLK_LEFT && dir != 1) {
                    dir = 3;
                }
                

            }
        }
        /*
        keys = SDL_GetKeyboardState(nullptr);
        if (keys[SDL_SCANCODE_UP]) {
            dir = 0;
        } else if (keys[SDL_SCANCODE_RIGHT]) {
            dir = 1;
        } else if (keys[SDL_SCANCODE_DOWN]) {
            dir = 2;
        } else if (keys[SDL_SCANCODE_LEFT]) {
            dir = 3;
        }
            */
        if (!minApplesMet(board)){
            generateApple(board);
        }
        if (!moveInBound(board, snakeLength, dir)) {
            std::fill_n(board, BOARD_ROWS*BOARD_COLS, -1);
            renderBoard(renderer, board);
            SDL_RenderPresent(renderer);
            if (!INFINITE_PLAY) running = false;
            else {
                snakeLength = 3;
                dir = 1; // direction
                std::fill_n(board, BOARD_ROWS*BOARD_COLS, 0);
                board[0] = 1;
                board[1] = 2;
                board[2] = 3;
                for (int i=0;i<APPLE_COUNT;i++) (generateApple(board));
            }
        } else if (moveHitSnake(board, snakeLength, dir)) {
            std::fill_n(board, BOARD_ROWS*BOARD_COLS, -1);
            renderBoard(renderer, board);
            SDL_RenderPresent(renderer);
            if (!INFINITE_PLAY) running = false;
            else {
                snakeLength = 3;
                dir = 1; // direction
                std::fill_n(board, BOARD_ROWS*BOARD_COLS, 0);
                board[0] = 1;
                board[1] = 2;
                board[2] = 3;
                for (int i=0;i<APPLE_COUNT;i++) (generateApple(board));
            }
        } else if (moveHitApple(board, snakeLength, dir)) {
            addToSnake(board, snakeLength, dir);
            snakeLength++;
            generateApple(board);
        } else {

            moveSnake(board, snakeLength, dir);

            renderBoard(renderer, board);
            SDL_RenderPresent(renderer);
            
        }
        SDL_Delay(120);
        
        
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}