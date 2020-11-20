#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <time.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
int main(int argc, char *argv[]){
    char errors[100];
    FILE *ferrors;
    ferrors = fopen("errors/errors.txt", "a+");//file I/O
    time_t now;
    time(&now);//giving the time so that we know precisely at what time our program failed.
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        printf("Error initializing the game. The cause is %s\n", SDL_GetError());
        sprintf(errors, "[%s] = Error initializing the game. The cause is %s\n", ctime(&now), SDL_GetError());//moving it to a text file so it would be easier to debug.
        fputs(errors, ferrors);//sending it to the file.
        fclose(ferrors);
        return EXIT_FAILURE; //return error. 
    }
    SDL_Window *window; //making the window pointer
    
    SDL_Init(SDL_INIT_VIDEO);//initiating the SDL

    window = SDL_CreateWindow   ("SEIZED",               //title
                                SDL_WINDOWPOS_CENTERED,  //initial position for x axis
                                SDL_WINDOWPOS_CENTERED,  //initial position for y axis
                                1080,                     //width (pixels)
                                720,                     //height (pixels)
                                0);                      //flags
    
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, render_flags);//making the renderer
    if(!renderer)
    {
        printf("Error in processing renderer\n");
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_Surface *image; //making surface struct called image

    char* imagePath = "Resources/coming_soon.jpg"; //giving the image path
    image = IMG_Load(imagePath);//sending the image path to the IMG_Load
    if(!image)
    {
        printf("Image error loaded\n");
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;
    }
    while (1);//while true (looping forever)
    SDL_Quit();
    return 0;
}
