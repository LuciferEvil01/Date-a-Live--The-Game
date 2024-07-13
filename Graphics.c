# include <SDL2/SDL.h>
void handle_events(SDL_Event* event,int* running)
{
    while (SDL_PollEvent(event)){if (event->type == SDL_QUIT) running=0;}
}
void loop(SDL_Renderer* renderer)
{
    int runing = 1;
    while (runing == 1)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event)){if (event.type == SDL_QUIT) runing=0;}
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }
}
void clear (SDL_Window* window,SDL_Renderer* renderer)
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}


void Render()
{
    SDL_Window* window= SDL_CreateWindow("Space Invader",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,640,480,SDL_WINDOW_SHOWN );
    if (!window)
    {
        printf("Error al crear la ventana:%s\n",SDL_GetError());
    }
    SDL_Renderer* Renderer =SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    if (!Renderer)
    {
        printf("Error al crear el renderer%s\n",SDL_GetError());
    }
}