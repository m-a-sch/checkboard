#include <SDL2/SDL.h>
#include <stdbool.h>
// inspired by https://doi.org/10.1177/20416695231184388
// and https://twitter.com/Neuro_Skeptic/status/1679159046631194624?s=20
// compile with gcc checkboard.c -lSDL2 -o checkboard
// run with ./checkboard
int main(int argc, char *argv[]) {
    int width = 840; // window width
    int height = 100; // window height
    struct boardSquare {
        int h;
        int w;
    };

    struct boardSquare myboardSquare = { 10 , 50 };
    int line_height = 6; // moving line height
    int delay=20;

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
          switch (argv[i][1]) {
            case 'w':
                  width = atoi(argv[i + 1]);
                  break;
            case 'h':
                  height = atoi(argv[i + 1]);
                  break;
            case 'H':
                  myboardSquare.h = atoi(argv[i + 1]);
                  break;
            case 'W':
                  myboardSquare.w = atoi(argv[i + 1]);
                  break;
            case 'l':
                  line_height = atoi(argv[i + 1]);
                  break;
            case 'd':
                  delay = atoi(argv[i + 1]);
                  break;
            default:
              printf("Unknown parameter: %s\n usage: checkboard -w (window width)\n-h (height)\n-W (checkboard width)\n-H (height)\n-l (line height)\n-d (delay)", argv[i]);
              return 1;
          }
        }
      }

    
    int middleHeight = height / 2;

    
    // Initialize SDL.
    SDL_Init(SDL_INIT_VIDEO);

    // Create a window.
    SDL_Window *window = SDL_CreateWindow("The rocking line illusion", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create window: %s", SDL_GetError());
        return 1;
    }
    // Create a renderer.
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create renderer: %s", SDL_GetError());
        return 1;
    }
    
    int s=0;
    // Wait for the user to press the escape key or close the window
    bool quit = false;
    while (!quit) {
        // Set the background color to grey
        SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
        SDL_RenderClear(renderer);
        // Draw the checkerboard pattern.
        int x = 0;
        bool whiteOnTop = true;
        while (x < width) {
            SDL_Rect rectA = {x, middleHeight - (myboardSquare.h/2), myboardSquare.w, myboardSquare.h / 2};
            SDL_Rect rectB = {x, middleHeight, myboardSquare.w, myboardSquare.h / 2};
            if (whiteOnTop) {
                SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
                SDL_RenderFillRect(renderer, &rectA);
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(renderer, &rectB);
            } else {
                SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
                SDL_RenderFillRect(renderer, &rectB);
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(renderer, &rectA);
            }
            x += myboardSquare.w;
            whiteOnTop = !whiteOnTop;
        }
        // done with creating the check board
        // now we are going to animate a line horizontaly accross the window
    
        SDL_Rect line = {s, middleHeight - (line_height/2), myboardSquare.w, line_height};
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &line);
        SDL_RenderPresent(renderer);
        SDL_Event event;
        SDL_WaitEventTimeout(&event,delay);
        if (event.type == SDL_QUIT) {
            quit = true;
        } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
            quit = true;
        }
        if ((s + line.w) > width) {
            s=0;
        } else {
            s += 1;
        }
        
           
    }

  // Cleanup.
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}

