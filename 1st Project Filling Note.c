#include <windows.h>
#include <stdio.h>
#include <SDL2/SDL.h>

// Ekran özellikleri
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Birim kare özellikleri
const int UNIT_SIZE = 20; // Birim karenin boyutu
const int TRANSPARENCY = 128; // Saydamlık derecesi (0-255 arası)

// Fonksiyon toplamı
void drawGrid(SDL_Renderer *renderer);
void drawUnitSquare(SDL_Renderer *renderer, int x, int y, int transparency);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("Saydam Birim Kareler", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    int quit = 0;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        drawGrid(renderer);

        // Belirli birim kareleri saydam bir şekilde boyayın
        drawUnitSquare(renderer, 3, 2, TRANSPARENCY);
        drawUnitSquare(renderer, 6, 4, TRANSPARENCY);
        // Diğer birim kareleri burada ekleyin

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

// Birim kareyi çiz
void drawUnitSquare(SDL_Renderer *renderer, int x, int y, int transparency) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, transparency); // Kırmızı renk, belirli saydamlık
    SDL_Rect rect = {x * UNIT_SIZE, y * UNIT_SIZE, UNIT_SIZE, UNIT_SIZE};
    SDL_RenderFillRect(renderer, &rect);
}

// Izgarayı çiz
void drawGrid(SDL_Renderer *renderer) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);

    for (int i = 0; i <= SCREEN_WIDTH; i += UNIT_SIZE) {
        SDL_RenderDrawLine(renderer, i, 0, i, SCREEN_HEIGHT);
    }

    for (int i = 0; i <= SCREEN_HEIGHT; i += UNIT_SIZE) {
        SDL_RenderDrawLine(renderer, 0, i, SCREEN_WIDTH, i);
    }
}
