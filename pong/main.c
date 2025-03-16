#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 *
 * Pong game in C & SDL2
 * Made by Téo JAUFFRET
 * 
 */

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not init! error : %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() == -1) {
        printf("SDL_ttf could not init!, error %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 600, 400, SDL_WINDOW_SHOWN);

    if (!window) {
        printf("Error while creating window, error %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer) {
        printf("Error while creating renderer, error %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    int score_player = 0;
    char score_player_buffer[20];
    int score_ai = 0;
    char score_ai_buffer[20];

    sprintf(score_player_buffer, "%d", score_player);
    sprintf(score_ai_buffer, "%d", score_ai);

    TTF_Font *font = TTF_OpenFont("./Arial.ttf", 24);
    if (!font) {
        printf("Error while init font! error %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Color white = {255, 255, 255};

    int running = 1;
    SDL_Event event;
    SDL_Rect rect = {15, 150, 10, 95};
    SDL_Rect airect = {600-25, 100, 10, 95};
    SDL_Rect ball = {300, 200, 10, 10};

    float dx = 5;
    float dy = 2;
    int pause = 0;

    srand(time(NULL));
    float r = 0.6 + (float)rand() / (RAND_MAX / (0.8 - 0.6));
    printf("AI level : %f\n", r);

    const Uint8 *state = SDL_GetKeyboardState(NULL);

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        if (pause) {
            srand(time(NULL));
            float r = 0.6 + (float)rand() / (RAND_MAX / (0.8 - 0.6));
            printf("AI level : %f\n", r);
            SDL_Delay(1500);
            pause = 0;
        }

        if (state[SDL_SCANCODE_UP]) {
            rect.y = rect.y - 3;
        }

        if (state[SDL_SCANCODE_DOWN]) {
            rect.y = rect.y + 3;
        }
        ball.x += 1 * dx;
        ball.y += 1 * dy;
        airect.y = ball.y * r;

        if (SDL_HasIntersection(&ball, &airect)) {
            dx = -dx;
        }

        if (SDL_HasIntersection(&ball, &rect)) {
            dx = -dx;
        }

        if (ball.y > 380 || ball.y < 10) {
            dy = -dy;
        }

        if (ball.x > 580) {
            ball.x = 300;
            ball.y = 200;
            dx = -dx;
            dy = -dy;

            rect.x = 15;
            rect.y = 150;
            airect.x = 600-15;
            airect.y = 150;

            score_player += 1;
            sprintf(score_player_buffer, "%d", score_player);

            pause = 1;
        }

        if (ball.x < 20) {
            ball.x = 300;
            ball.y = 200;
            dx = -dx;
            dy = -dy;

            rect.x = 15;
            rect.y = 150;
            airect.x = 600-15;
            airect.y = 150;

            score_ai += 1;
            sprintf(score_ai_buffer, "%d", score_ai);

            pause = 1;
        }

        SDL_Surface *score_player_text = TTF_RenderText_Solid(font, score_player_buffer, white);
        if (score_player_text == NULL) {
            printf("Unable to render ScorePlayerText, error : %s\n", TTF_GetError());
            TTF_CloseFont(font);
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            TTF_Quit();
            SDL_Quit();
            return 1;
        }

        SDL_Surface *score_ai_text = TTF_RenderText_Solid(font, score_ai_buffer, white);
        if (score_ai_text == NULL) {
            printf("Unable to render ScoreAIText, error : %s\n", TTF_GetError());
            TTF_CloseFont(font);
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            TTF_Quit();
            SDL_Quit();
            return 1;
        }

        SDL_Texture *scorePlayerTexture = SDL_CreateTextureFromSurface(renderer, score_player_text);
        SDL_FreeSurface(score_player_text);
        SDL_Rect renderScorePlayer = {40, 30, 30, 60};

        SDL_Texture *scoreAITexture = SDL_CreateTextureFromSurface(renderer, score_ai_text);
        SDL_FreeSurface(score_ai_text);
        SDL_Rect renderScoreAI = {600-60, 30, 30, 60};

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &ball);
        SDL_RenderFillRect(renderer, &rect);
        SDL_RenderFillRect(renderer, &airect);

        SDL_RenderCopy(renderer, scorePlayerTexture, NULL, &renderScorePlayer);
        SDL_RenderCopy(renderer, scoreAITexture, NULL, &renderScoreAI);

        SDL_DestroyTexture(scoreAITexture);
        SDL_DestroyTexture(scorePlayerTexture);
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}