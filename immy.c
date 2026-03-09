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

	IMG_Animation* anim = IMG_LoadAnimation(argv[1]);
	SDL_Surface* single_surface = NULL;
	int frame_count = 1;
	int* delays = NULL;
	SDL_Surface** surfaces = NULL;

	if (anim) {
		frame_count = anim->count;
		surfaces = anim->frames;
		delays = anim->delays;
	} else {
		single_surface = IMG_Load(argv[1]);
		if (single_surface == NULL) {
			fprintf(stderr, "Failed to open image '%s': %s\n", argv[1], SDL_GetError());
			return 1;
		}
		surfaces = &single_surface;
	}
 
	SDL_Texture** textures = malloc(frame_count * sizeof(SDL_Texture*));
	for (int i = 0; i < frame_count; i++) {
		textures[i] = SDL_CreateTextureFromSurface(renderer, surfaces[i]);
	}

	SDL_SetRenderVSync(renderer, 1);

	float w, h;
	SDL_GetTextureSize(textures[0], &w, &h);
	SDL_SetWindowSize(window, (int)w, (int)h);
	SDL_SetWindowShape(window, surfaces[0]);

	SDL_FRect dst_rect = { 0.0f, 0.0f, w, h };

	bool running = true;
	int current_frame = 0;
	Uint64 last_time = SDL_GetTicks(); // Track playback time

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

		// gif handling
		if (frame_count > 1) {
			Uint64 current_time = SDL_GetTicks();
			int delay = delays[current_frame];
			if (delay <= 0) delay = 100; // fallback

			if (current_time - last_time >= (Uint64)delay) {
				current_frame = (current_frame + 1) % frame_count;
				last_time = current_time; 
				
				SDL_SetWindowShape(window, surfaces[current_frame]);
			}
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);
		int x, y;
		SDL_GetWindowSizeInPixels(window, &x, &y);
		dst_rect.w = (float)x;
		dst_rect.h = (float)y;
		SDL_RenderTexture(renderer, textures[current_frame], NULL, &dst_rect);
		SDL_RenderPresent(renderer);
	}

	for (int i = 0; i < frame_count; i++) {
		SDL_DestroyTexture(textures[i]);
	}
	free(textures);
	
	if (anim) {
		IMG_FreeAnimation(anim);
	} else {
		SDL_DestroySurface(single_surface);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

