#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "Provide an image.\n");
		return 1;
	}

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

	SDL_Window *window = SDL_CreateWindow("immy", 1, 1, SDL_WINDOW_RESIZABLE | SDL_WINDOW_TRANSPARENT);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);

	SDL_Surface* surface = IMG_Load(argv[1]);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (surface == NULL) {
		fprintf(stderr, "Failed to open image '%s'\n", argv[1]);
		return 1;
	}

	SDL_SetRenderVSync(renderer, 1);

	float w, h;
	SDL_GetTextureSize(texture, &w, &h);
	SDL_SetWindowSize(window, (int)w, (int)h);
	SDL_SetWindowShape(window, surface);
	SDL_DestroySurface(surface);

	SDL_FRect dst_rect = { 0.0f, 0.0f, w, h };

	bool running = true;
	while (running) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_EVENT_QUIT:
				running = false;
			default:
				break;
			}
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);
		int x, y;
		SDL_GetWindowSizeInPixels(window, &x, &y);
		dst_rect.w = (float)x;
		dst_rect.h = (float)y;
		SDL_RenderTexture(renderer, texture, NULL, &dst_rect);
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

