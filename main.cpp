#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <chrono>

#ifdef _WIN32
    #include <windows.h>
    #include <psapi.h>
#endif

#include "spritegroup.h"
#include "logic.h"

#define TARGET_FPS 60

typedef std::chrono::steady_clock steady_clock;
typedef std::chrono::microseconds us;
double stabilize_fps(std::chrono::steady_clock::time_point timePoint1) {
    auto timePoint2 = steady_clock::now();
    unsigned long long frameTime = std::chrono::duration_cast<us>(timePoint2 - timePoint1).count();
    long long timeToSleep = (1000000.0 / TARGET_FPS) - frameTime;
    if (timeToSleep > 0) {
        SDL_Delay(timeToSleep / 1000.0);
        timePoint2 = steady_clock::now();
        frameTime = std::chrono::duration_cast<us>(timePoint2 - timePoint1).count();
    }
    return frameTime / 1000.0;
}

int main_program() {
    Logic::Node goal_node = Logic::Node::from_type("base/goal");
    Logic::Node and_node = Logic::Node::from_type("gate/and");
    Logic::Node c1_node = Logic::Node::from_type("constant/bool");
    Logic::Node c2_node = Logic::Node::from_type("constant/bool");
    goal_node.add_input(&and_node);
    and_node.add_input(&c1_node);
    and_node.add_input(&c2_node);

    std::initializer_list<std::pair<bool, bool>> list = {{false, false}, {false, true}, {true, false}, {true, true}};
    for (std::pair<bool, bool> pair : list) {
        c1_node.value = pair.first;
        c2_node.value = pair.second;
        Logic::ValueType v = goal_node.resolve(nullptr);
        std::cout << pair.first << " " << pair.second << " " << std::get<bool>(v) << std::endl;
    }

    SDL_Window *window;
    SDL_Renderer *renderer;

    window = SDL_CreateWindow("My SDL Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);
    if (window == nullptr) {
        std::cout << "Could not create SDL window" << std::endl;
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == nullptr) {
        std::cout << "Could not create SDL renderer" << std::endl;
        return 1;
    }

    IMG_Init(IMG_INIT_PNG);

    SpriteGroup sprites{RenderContext{window, renderer}};
    for (int i = 0; i < 17500; i++) {
        sprites.create("assets/test.png", rand() % (620 * 2) - 310, rand() % (460 * 2) - 230, rand() % 1000);
    }

    steady_clock::time_point timePoint1;
    bool running = true;
    double frameTime = 16.66666;
    int fc = 0;
    while (running) {
        timePoint1 = steady_clock::now();
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            }
        }
        
        sprites.render();

        SDL_RenderPresent(renderer);
        frameTime = stabilize_fps(timePoint1);
        if (fc == 10) {
            std::cout << "Frame rate: " << 1000.0 / frameTime << std::endl;
            fc = 0;
        }
        fc++;
    }
    sprites.dispose();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    IMG_Quit();
    SDL_Quit();

    return 0;
}

#ifndef USE_MAIN
int SDL_main(int argc, char **argv) {
#else
int main(int argc,char **argv)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "Failed to init SDL" << std::endl;
        return 1;
    }
#endif
#ifdef _WIN32
    PPROCESS_MEMORY_COUNTERS x = (PPROCESS_MEMORY_COUNTERS) ::operator new(sizeof(PROCESS_MEMORY_COUNTERS));
    GetProcessMemoryInfo(GetCurrentProcess(), x, sizeof(*x));
    std::cout << "Memory usage at start: " << x->WorkingSetSize << std::endl;
#endif

    int result = main_program();

#ifdef _WIN32
    GetProcessMemoryInfo(GetCurrentProcess(), x, sizeof(*x));
    std::cout << "Memory usage at end:   " << x->WorkingSetSize << std::endl;

    ::operator delete(x);
#endif

    return result;
}