#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#define M_PI 3.14159265358979323846

// made by Téo JAUFFRET

void draw_circle(SDL_Renderer *renderer, int cx, int cy, int radius) {
    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius;

    while (y >= x) {
        SDL_RenderDrawPoint(renderer, cx + x, cy + y);
        SDL_RenderDrawPoint(renderer, cx - x, cy + y);
        SDL_RenderDrawPoint(renderer, cx + x, cy - y);
        SDL_RenderDrawPoint(renderer, cx - x, cy - y);
        SDL_RenderDrawPoint(renderer, cx + y, cy + x);
        SDL_RenderDrawPoint(renderer, cx - y, cy + x);
        SDL_RenderDrawPoint(renderer, cx + y, cy - x);
        SDL_RenderDrawPoint(renderer, cx - y, cy - x);

        x++;
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }
    }
}

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_TIMER|SDL_INIT_VIDEO) < 0) {
        printf("Error while loading SDL.");
        return 1;
    }

    SDL_Window *window;
    SDL_Renderer *renderer;

    SDL_CreateWindowAndRenderer(300, 300, SDL_WINDOW_SHOWN, &window, &renderer);
    SDL_SetWindowTitle(window, "Digital Clock");

    bool running = true;

    time_t rawtime;
    struct tm *timeinfo;

    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        draw_circle(renderer, 150, 150, 120);
    
        time(&rawtime);
        timeinfo = localtime(&rawtime);

        int sec = (timeinfo->tm_sec)-15;
        float rad = sec * M_PI / 180.0;
        float posY = (150 + 100 * sin(rad*6));
        float posX = (150 + 100 * cos(rad*6)); 

        int minute = (timeinfo->tm_min)-15;
        float rad_min = minute * M_PI / 180.0;
        float min_posY = (150 + 80 * sin(rad_min*6));
        float min_posX = (150 + 80 * cos(rad_min*6));

        int hours = (timeinfo->tm_hour)-3;
        float rad_hours = hours * M_PI / 180.0;
        float hours_posY = (150 + 60 * sin(rad_hours*30));
        float hours_posX = (150 + 60 * cos(rad_hours*30));

        SDL_RenderDrawLine(renderer, 150, 150, hours_posX, hours_posY);
        SDL_RenderDrawLine(renderer, 150, 150, min_posX, min_posY);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderDrawLine(renderer, 150, 150, posX, posY);

        SDL_RenderPresent(renderer);
        SDL_Delay(1000);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}