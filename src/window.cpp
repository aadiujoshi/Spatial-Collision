#include <SDL2/SDL.h>
#include <SDL2/SDL_assert.h>
#include <math.h>
#include <chrono>
#include <iostream>
#include <mutex>
#include <condition_variable>
#include "header_paths.h"
#include window_h
#include object_h
#include spatial_partition_h

#define randf() (rand()/(RAND_MAX*1.0f))
#define timeNs() std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch().count()

namespace gfx {
    window::window(int width, int height) : 
            objs(std::vector<phys::object>()), 
            capture_x(-1), capture_y(-1), 
            partition_type(event::update_event::QUADTREE) {

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
        screen_width = dm.w;
        screen_height = dm.h;

        handle = h_window;
        renderer = new gfx::renderer(h_renderer, *this);
        mouse = { 0 };
        tick_delay_ns = 10;

        //handle preloaded textures
        gfx::texture::green_circle = new gfx::texture("green_circle.png", (*renderer));

        if (partition_type == event::update_event::QUADTREE) {
            partition_method = std::make_unique<sp::quadtree>(objs);
        }
        if (partition_type == event::update_event::SPATIAL_HASHING) {
            partition_method = std::make_unique<sp::spatial_hashing>(objs);
        }
    }

    window::~window() {

    }

    void window::start_window() {
        //std::thread update_thread(&gfx::window::update_loop, this);
        //update_thread.detach();

        main_loop();
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
                        
                        if (key == SDLK_c) {
                            partition_method.get()->clear();
                            objs.clear();
                        }

                        if (key == SDLK_s) {
                            render_partition = !render_partition;
                        }

                        if (key == SDLK_LSHIFT && mouse.timestamp != 0) {
                            phys::object::object_param op = { 0 };
                            float nx, ny;
                            gfx::renderer::to_units(mouse.x, mouse.y, &nx, &ny);
                            //op.radius = randf() * 20 + 1;
                            op.radius = 1;
                            op.x = nx;
                            op.y = sp::UNITS_HEIGHT - ny;
                            op.vx = (capture_x - mouse.x) * (1.0f / gfx::ppu_x);
                            op.vy = -(capture_y - mouse.y) * (1.0f / gfx::ppu_y);
                            op.mass = 10;

                            //std::cout << nx << "  " << ny << "  " << op.vx << "  " << op.vy << "  " << gfx::ppu_x << "  " << gfx::ppu_y << std::endl;

                            objs.emplace_back(op);
                            //update with new obj
                            partition_method.get()->insert(objs.back());
                        }

                        break;
                    }
                    case SDL_MOUSEBUTTONDOWN: {
                        SDL_MouseButtonEvent bt = event.button;
                        if (bt.button == SDL_BUTTON_LEFT) {
                            // copy
                            mouse = bt;

                            capture_x = mouse.x;
                            capture_y = mouse.y;
                        }
                        break;
                    }
                    case SDL_MOUSEBUTTONUP: {
                        SDL_MouseButtonEvent bt = event.button;
                        if (bt.button == SDL_BUTTON_LEFT) {
                            mouse = { 0 };
                            capture_x = -1;
                            capture_y = -1;
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

            // Render graphics here
            render_update(pe);

            // Update the screen
            SDL_RenderPresent(rend_handle);
            std::string f = std::to_string(objs.size()) + " objs | fps: " + std::to_string(0x3B9ACA00 / (timeNs() - nstart));
            SDL_SetWindowTitle(handle, (const char*)(f.c_str()));
        }

        // Clean up resources
        SDL_DestroyRenderer(rend_handle);
        SDL_DestroyWindow(handle);
        delete renderer;
        SDL_Quit();
    }

    void window::render_update(event::paint_event& e) {

        bool use_dynamic = true;

        gfx::ppu_x = (this->getWidth() / sp::UNITS_WIDTH);
        gfx::ppu_y = (this->getHeight() / sp::UNITS_HEIGHT);

        SDL_Renderer* rh = e.renderer->handle;

        long long tns = timeNs();

        //-------------------------------------------------------------------
        //update

        //std::cout << partition_method.get() << std::endl;

        event::update_event ue( use_dynamic ? tick_delay_ns : tick_speed, objs, partition_type, *partition_method.get());

        for (size_t i = 0; i < objs.size(); i++) {
            objs[i].update(ue);
        }

        partition_method.get()->refresh();

        //-------------------------------------------------------------------

        if (0) {
            SDL_SetRenderDrawColor(rh, 255, 255, 0, 255);

            for (int i = 0; i <= sp::UNITS_WIDTH; i++) {
                SDL_RenderDrawLine(rh, i * (getWidth() / sp::UNITS_WIDTH), 0, i * (getWidth() / sp::UNITS_WIDTH), getHeight());
            }

            for (int i = 0; i <= sp::UNITS_HEIGHT; i++) {
                SDL_RenderDrawLine(rh, 0, i * (getHeight() / sp::UNITS_HEIGHT), getWidth(), i * (getHeight() / sp::UNITS_HEIGHT));
            }
        }
        //-------------------------------------------------------------------
        //render
        if(render_partition)
            partition_method.get()->paint(e);

        for (size_t i = 0; i < objs.size(); i++) {
            objs[i].paint(e);
        }
        //-------------------------------------------------------------------

        SDL_SetRenderDrawColor(rh, 255, 0, 0, 255);
        if (capture_x != -1) {
            SDL_RenderDrawLine(rh, capture_x, capture_y, mouse.x, mouse.y);
        }

        //-------------------------------------------------------------------
        tick_delay_ns = (timeNs() - tns);

        if(!use_dynamic)
            ns_delay(tick_speed - tick_delay_ns);

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


    void ns_delay(long long ns) {
        if (ns <= 0)
            return;

        auto start = std::chrono::high_resolution_clock::now();
        long long elapsed = 0;

        while (elapsed < ns) {
            auto end = std::chrono::high_resolution_clock::now();
            elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        }
    }
}