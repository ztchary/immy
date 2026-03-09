#include <stdio.h>
#include <stdlib.h>
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

	int frames = 1;
	int *delays = NULL;
	SDL_Texture **textures = NULL;

	IMG_Animation *anim = IMG_LoadAnimation(argv[1]);
	if (anim != NULL) {
		frames = anim->count;
		delays = anim->delays;
		textures = malloc(anim->count * sizeof(SDL_Texture *));
		for (int i = 0; i < frames; i++) {
			textures[i] = SDL_CreateTextureFromSurface(renderer, anim->frames[i]);
		}
	} else {
		textures = malloc(sizeof(SDL_Texture *));
		SDL_Surface *surface = IMG_Load(argv[1]);
		if (surface == NULL) {
			fprintf(stderr, "Failed to open image '%s': %s\n", argv[1], SDL_GetError());
			return 1;
		}
		*textures = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_DestroySurface(surface);
	}


	SDL_SetRenderVSync(renderer, 1);

	float w, h;
	SDL_GetTextureSize(textures[0], &w, &h);
	SDL_SetWindowSize(window, (int)w, (int)h);

	SDL_FRect dst_rect = { 0.0f, 0.0f, w, h };

	bool running = true;
	Uint64 last_time = SDL_GetTicks();

	for (int frame = 0; running;) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_EVENT_QUIT:
				running = false;
			default:
				break;
			}
		}

		if (frames > 1) {
			Uint64 time = SDL_GetTicks();
			int delay = delays[frame];
			if (delay <= 0) delay = 100;

			if (time - last_time >= (Uint64)delay) {
				frame = (frame + 1) % frames;
				last_time = time;
			}
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);
		int x, y;
		SDL_GetWindowSizeInPixels(window, &x, &y);
		dst_rect.w = (float)x;
		dst_rect.h = (float)y;
		SDL_RenderTexture(renderer, textures[frame], NULL, &dst_rect);
		SDL_RenderPresent(renderer);
	}

	if (anim != NULL) IMG_FreeAnimation(anim);
	for (int i = 0; i < frames; i++) {
		SDL_DestroyTexture(textures[i]);
	}
	free(textures);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

