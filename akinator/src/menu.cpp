#include <stdio.h>
#include "logger.h"
#include "akinator.h"

void akinator_t::render_button(SDL_Renderer* renderer, const button_t* button, TTF_Font* font, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, 128, 0, 128, 255);
    SDL_RenderFillRect(renderer, &button->rect);

    SDL_Surface* text_surface = TTF_RenderText_Solid(font, button->label, color);
    SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    SDL_Rect text_rect = {button->rect.x + 10, button->rect.y + 10, text_surface->w, text_surface->h};

    SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);

    SDL_FreeSurface(text_surface);
    SDL_DestroyTexture(text_texture);
}

void akinator_t::render_footer(SDL_Renderer* renderer, TTF_Font* font) {
    SDL_Surface* text_surface = TTF_RenderText_Solid(font, "Made by @aliffka", {0, 0, 0, 255});
    if (text_surface == nullptr) {
        printf("Error rendering text: %s\n", TTF_GetError());
    }

    SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    SDL_Rect text_rect = {5, 450, text_surface->w, text_surface->h};

    SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);

    SDL_DestroyTexture(text_texture);
    SDL_FreeSurface(text_surface);
}

int akinator_t::play_akinator_game() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        LOG(ERROR, "SDL_Init Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    if (TTF_Init() == -1) {
        LOG(ERROR,"TTF_Init Error: %s\n", TTF_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_Window* window = SDL_CreateWindow("Akinator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        LOG(ERROR,"SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        LOG(ERROR, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    TTF_Font* font = TTF_OpenFont("/System/Library/Fonts/Supplemental/Arial.ttf", 18);
    if (font == nullptr) {
        LOG(ERROR, "TTF_OpenFont Error: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    TTF_Font* footer_font = TTF_OpenFont("/System/Library/Fonts/Supplemental/Arial.ttf", 16);
    if (footer_font == nullptr) {
        LOG(ERROR, "TTF_OpenFont Error: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_Color text_color = {255, 255, 255, 255};

    int quit = 0;
    SDL_Event e = {};

    while (quit == false && quit_game_ == false) {
        SDL_SetRenderDrawColor(renderer, 0, 128, 128, 255);
        SDL_RenderClear(renderer);

        for (size_t i = 0; i < buttons_amount; i++) {
            render_button(renderer, &BUTTONS_[i], font, text_color);
        }

        render_footer(renderer, footer_font);

        SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }

            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x = e.button.x;
                int y = e.button.y;

                for (size_t i = 0; i < buttons_amount; i++) {
                    if (x >= BUTTONS_[i].rect.x && x <= BUTTONS_[i].rect.x + BUTTONS_[i].rect.w &&
                        y >= BUTTONS_[i].rect.y && y <= BUTTONS_[i].rect.y + BUTTONS_[i].rect.h) {
                        BUTTONS_[i].action(this);
                    }
                }
            }
        }
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return EXIT_SUCCESS;
}
