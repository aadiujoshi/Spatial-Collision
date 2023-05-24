#include <SDL2/SDL.h>
#include <math.h>
#include <chrono>

#define WIN32_LEAN_AND_MEAN 0xBEEF

#define randf() (rand()/(RAND_MAX*1.0f))
#define timeNs() std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch().count()


int main(int argc, char* argv[]) {
    const int width = 600;
    const int height = 600;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL initialization failed: %s", SDL_GetError());
        return -1;
    }

    SDL_Window* window;
    SDL_Renderer* renderer;
    if (SDL_CreateWindowAndRenderer(width, height, 
                                    SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE,
                                    &window, &renderer) 
                                    < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create SDL window and renderer: %s", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    bool quit = false;
    SDL_Event event;
    while (!quit) {

        long long nstart = timeNs();

        while (SDL_PollEvent(&event)) {

            switch (event.type) {
                case SDL_QUIT: {
                    quit = true;
                    break;
                }
                case SDL_WINDOWEVENT: {
                    if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                        SDL_RenderPresent(renderer);
                    }
                    break;
                }
            }
        }

        // Clear the renderer
        //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        //SDL_RenderClear(renderer);

        // Render your graphics here
        SDL_SetRenderDrawColor(renderer,    (Uint8)(randf() * 255),
                                            (Uint8)(randf() * 255),
                                            (Uint8)(randf() * 255),
                                            255);
        int cw, ch;
        SDL_GetWindowSize(window, &cw, &ch);

        SDL_Rect r = {  (cw / 2)- (cw / 4) / 2,
                        (ch / 2) - (ch / 4) / 2,
                        (cw / 4), 
                        (ch / 4)};
        SDL_RenderFillRect(renderer, &r);

        // Update the screen
        SDL_RenderPresent(renderer);

        std::string f = std::to_string(0x3B9ACA00 / (timeNs() - nstart));

        SDL_SetWindowTitle(window, (const char*)(f.c_str()));
    }

    // Clean up resources
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
