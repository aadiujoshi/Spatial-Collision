#include <SDL2/SDL.h>
#include <math.h>
#include <chrono>
#include <iostream>
#include "header_paths.h"
#include window_h
#include object_h

#define randf() (rand()/(RAND_MAX*1.0f))
#define timeNs() std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch().count()

namespace gfx {

    window::window(int width, int height) : 
            objs(std::vector<phys::object>()), capture_x(-1), capture_y(-1), partition_type(event::update_event::SPATIAL_HASHING){

        if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL initialization failed: %s", SDL_GetError());
        }

        SDL_Window* h_window;
        SDL_Renderer* h_renderer;
        if (SDL_CreateWindowAndRenderer(width, height,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE,
            &h_window, &h_renderer) < 0) {

            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create SDL window and renderer: %s", SDL_GetError());
            SDL_Quit();
        }
        SDL_DisplayMode dm;
        SDL_GetCurrentDisplayMode(0, &dm);
        std::cout << "dimensions: " << dm.w << "x" << dm.h << std::endl;

        handle = h_window;
        renderer = new gfx::renderer(h_renderer);
        mouse = { 0 };
    }

    window::~window() {

    }

    void window::start_window() {
        std::thread update_thread(&gfx::window::update_loop, this);
        update_thread.detach();

        main_loop();
    }

    void window::update_loop() {
        int prev_delay = 100;
        while (!quit) {

            long long tns = timeNs();

            event::update_event ue(100, 10, 10, objs, partition_type, );

            for (size_t i = 0; i < objs.size(); i++) {
                objs[i].update(ue);
            }

            prev_delay = (timeNs() - tns) + 10;
        }
    }

    void window::main_loop() {
        SDL_Event event;
        SDL_Renderer* rend_handle = renderer->handle;

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
                            SDL_RenderPresent(rend_handle);
                        }
                        break;
                    }
                    case SDL_KEYDOWN: {
                        SDL_Keycode key = event.key.keysym.sym;
                        break;
                    }
                    case SDL_MOUSEBUTTONDOWN: {
                        SDL_MouseButtonEvent bt = event.button;
                        if (bt.button == SDL_BUTTON_LEFT) {
                            // copy
                            mouse = bt;
                        }

                        capture_x = mouse.x;
                        capture_y = mouse.y;

                        break;
                    }
                    case SDL_MOUSEBUTTONUP: {
                        SDL_MouseButtonEvent bt = event.button;
                        if (bt.button == SDL_BUTTON_LEFT) {
                            mouse = { 0 };
                        }



                        break;
                    }
                    case SDL_MOUSEMOTION: {
                        //left button is held down
                        if (mouse.timestamp != 0) {
                            mouse = event.button;
                        }
                    }
                }
            }

            SDL_SetRenderDrawColor(rend_handle, 0, 0, 0, 0);
            SDL_RenderClear(rend_handle);

            event::paint_event pe(this);

            // Render your graphics here
            render(pe);

            // Update the screen
            SDL_RenderPresent(rend_handle);
            std::string f = std::to_string(objs.size()) + " | " + std::to_string(0x3B9ACA00 / (timeNs() - nstart));
            SDL_SetWindowTitle(handle, (const char*)(f.c_str()));
        }

        // Clean up resources
        SDL_DestroyRenderer(rend_handle);
        SDL_DestroyWindow(handle);
        SDL_Quit();
    }

    void window::render(event::paint_event& e) {
        for (size_t i = 0; i < objs.size(); i++) {
            objs[i].paint(e);
        }
    }

    int window::getWidth() {
        int w, h;
        SDL_GetWindowSize(handle, &w, &h);
        return w;
    }

    int window::getHeight() {
        int w, h;
        SDL_GetWindowSize(handle, &w, &h);
        return h;
    }
}