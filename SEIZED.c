/*
HELLO DEVS,
In order to understand and contribute to the development of the game, 
here are the pre-requisite:
=====================================================
You need to understand C programming language and some of its core concept like if-else, structs, etc.
You need to understand how SDL works, because this would be created mainly with SDL.
You need to know how to do comments on your code, because not everyone is you.
You need to be able how to troubleshoot your own code, don't push code that is not done yet!
You need to state what kind of changes that you made in the pull requests.
You need to understand that game programming is not easy!
****************************************************
Keep Coding for the Win!
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
int main(int argc, char *argv[]){
    char errors[100];
    time_t now;
    int running = 1;
    FILE *ferrors;
    ferrors = fopen("errors/errors.txt", "a+");//file I/O
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
    
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;//type render_flags to set the renderer so that it would use the computer VRAM.
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
    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, image); //making the texture so that the image is loaded to the memory
    SDL_FreeSurface(image);//freeing the surface because it already converted into a texture.
    if(!tex)
    {
        printf("Error creating texture\n");
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_RenderClear(renderer); //clearing the renderer so that it would produce black screen instead of crashing.

    /*Drawing the image into the window with Double Buffer Technique*/
    SDL_RenderCopy(renderer, tex, NULL, NULL); //doing the processing in the background
    SDL_RenderPresent(renderer);//presenting the image to the user.

    SDL_Event exit; //creating an event called exit.

    while(running == 1)//I WISH WE HAVE BOOLEAN HERE, BUT WE MUST ADAPT!
    {
        while(SDL_PollEvent(&exit) != 0)//poll event means that we will record the event that is done by the user.
        {
            if(exit.type == SDL_QUIT)//SDL_QUIT is an Enum, while SDL_Quit is a function.
                running = 0;//changing the value of running.
        }
        SDL_UpdateWindowSurface(window);//updating the window in each turn
        
        // determine velocity
        x_vel = y_vel = 0;
        if (up && !down) y_vel = -SPEED;
        if (down && !up) y_vel = SPEED;
        if (left && !right) x_vel = -SPEED;
        if (right && !left) x_vel = SPEED;

        // update positions
        x_pos += x_vel / 60;
        y_pos += y_vel / 60;

        // collision detection with bounds
        if (x_pos <= 0) x_pos = 0;
        if (y_pos <= 0) y_pos = 0;
        if (x_pos >= 1080 - dest.w) x_pos = 1080 - dest.w;
        if (y_pos >= 720 - dest.h) y_pos = 720 - dest.h;

        // set the positions in the struct
        dest.y = (int) y_pos;
        dest.x = (int) x_pos;
        
        // clear the window
        SDL_RenderClear(renderer);

        /* draw the image to the window */
        SDL_RenderCopy(renderer, tex, NULL, &dest);	//doing the processing in the background
        SDL_RenderPresent(renderer);	//presenting the image to the user

        // wait 1/60th of a second
        SDL_Delay(1000/60);
    }
   }
    
    /*SELF EXPLANATORY*/
    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
