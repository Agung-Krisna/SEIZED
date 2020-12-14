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
#include <SDL2/SDL_keyboard.h>

#define MAX_BULLET 1000

typedef struct 
{
    float x, y, dy;
    short life;//we use short because we don't want to allocate many memory for this.
    int imageNow, chWalk, facingLeft, shooting, visible;
    int alive;
    SDL_Texture *sheetTexture;
} Man;

typedef struct
{
    float x, y, dx;
} Bullet;

SDL_Texture *bulletTexture;
SDL_Texture *bgTexture;
Bullet *bullets[MAX_BULLET] = { NULL };

Man enemy;

int globalTime = 0;

void addBullet(float x, float y, float dx)
{
    int found = -1;
    int start = 0;

    for (start = 0; start < MAX_BULLET; start++)
    {
        if (bullets[start] == NULL)
        {
            found = start;
            break;
        }
    }
    if (found >= 0)
    {
        int start = found;
        bullets[start] = malloc (sizeof(Bullet));
        bullets[start] -> x = x;
        bullets[start] -> y = y;
        bullets[start] -> dx = dx;
    }
}

void removeBullet(int index)
{
    if(bullets[index])
    {
        free(bullets[index]);
        bullets[index] = NULL;
    }
}

int processEvent(SDL_Window *window, Man *man)
{
    SDL_Event event;
    int exit = 0;
    
    while (SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_KEYDOWN:
            {
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        exit = 1;
                    break;
                }
            }
            break;
            case SDL_QUIT:
                exit = 1;
            break;
        }
    }

    const Uint8 *state = SDL_GetKeyboardState(NULL);

    if (!man -> shooting)
    {
        if(state[SDL_SCANCODE_LEFT])
        {
            man -> x -= 3; //changing the x position by decrementing 3 
            man -> chWalk = 1; //boolean to check if the character is walking or not
            man -> facingLeft = 1; //self - explanatory

            if (globalTime % 6 == 0)
            {
            //this is going to be an error, because we have no sprites.
                man -> imageNow++;
                man -> imageNow %= 4;
            }
        }
        else if (state[SDL_SCANCODE_RIGHT])
        {
            man -> x += 3; //moving the x position
            man -> chWalk = 1; // like a boolean to check if the character is moving or not
            man -> facingLeft = 0; //like a boolean as well. 

            if (globalTime % 6 == 0)
            {
            //remember to fix this when the sprites is ready
                man -> imageNow ++;
                man -> imageNow %= 4;
            }
        }
        //ADDITIONAL, MAKE THE CHARACTER JUMPS;
        //Remember to add that later.
    }
    if (!man -> chWalk)
    {
        if(state[SDL_SCANCODE_Z] || state[SDL_SCANCODE_X])
        {
            if(globalTime % 6 == 0)
            {
                if(man -> imageNow == 4)
                {
                    man -> imageNow = 5;
                }
                else
                {
                    man -> imageNow = 4;
                }
                if(!man -> facingLeft)
                {
                    addBullet(man -> x + 35, man -> y + 20, 3);
                }
                else
                {
                    addBullet(man -> x + 5, man -> y + 20, -3);
                }
            }

            man -> shooting = 1;
        }
        else
        {
            man -> imageNow = 4;//the dude is standing in image 4
            man -> shooting = 0;
        }
    }
}
void makeRenderer(SDL_Renderer *renderer, Man *man)
{
    SDL_SetRenderDrawColor  (renderer, //specify the name of the renderer
                            0, //RED value that are used to draw on the rendering target. 
                            0, //GREEN value that are used to draw on the rendering target.
                            255, //BLUE value that are used to draw on the rendering target.
                            255); //keep this value to 255. ALPHA value that are used to draw on the rendering target.

    SDL_RenderClear (renderer);

    SDL_SetRenderDrawColor (renderer, 255, 255, 255, 255);

    SDL_RenderCopy(renderer, bgTexture, NULL, NULL);

    if(man -> visible)
    {
        SDL_Rect srcRect = { 40 * man -> imageNow, 0, 40, 50 };
        SDL_Rect rect = { man -> x, man -> y, 40, 50 };
        SDL_RenderCopyEx (renderer, man -> sheetTexture, &srcRect, &rect, 0, NULL, man -> facingLeft);
    }

    if (enemy.visible)//it's different because man is a pointer and enemy is a constructor(?)
    {
        SDL_Rect eSrcRect = { 40 * enemy.imageNow, 0, 40, 50 }; //enemy Source rectangle
        SDL_Rect eRect = { enemy.x, enemy.y, 40, 50 };
        SDL_RenderCopyEx (renderer, enemy.sheetTexture, &eSrcRect, &eRect, 0, NULL, enemy.facingLeft);
    }

    int start = 0;
    for (start = 0; start < MAX_BULLET; start ++)
    {
        if (bullets[start])
        {
            SDL_Rect rect = { bullets[start] -> x, bullets[start] -> y, 8, 8 }; // the sprites for the bullets are 8 by 8
            SDL_RenderCopy (renderer, bulletTexture, NULL, &rect);
        }
    }

    SDL_RenderPresent(renderer);
}

void Logic(Man *man)
{
    int start = 0;
    man -> y += man -> dy;
    man -> dy += 0.5;
    if (man -> y > 60)
    {
        man -> y = 60;
        man -> dy = 0;
    }

    for(start = 0; start < MAX_BULLET; start ++)
    {
        if (bullets[start])
        {
            bullets[start] -> x += bullets[start] -> dx;

            if  (bullets[start] -> x > enemy.x && 
                bullets[start] -> x < enemy.x + 40 &&
                bullets[start] -> y < enemy.y &&
                bullets[start] -> y < enemy.y + 50)
                {
                    enemy.alive = 0;
                }
            if (bullets[start] -> x < -1000 || bullets[start] -> x > 1000)
            {
                removeBullet(start);
            }
        }
    }

    if (enemy.alive == 0 && globalTime % 6 == 0)
    {
        if (enemy.imageNow < 6)
        {
            enemy.imageNow = 6;
        }
        else if (enemy.imageNow >= 6)
        {
            enemy.imageNow++;
            if (enemy.imageNow > 7)
            {
                enemy.visible = 0;
                enemy.imageNow = 7;
            }
        }
    }
    
    globalTime++;
}
/*
void cleaner()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(man -> sheetTexture);
    SDL_DestroyTexture(bgTexture);
    SDL_DestroyTexture(bulletTexture);
    SDL_DestroyTexture(enemy.sheetTexture);

    int start = 0;
    for (start = 0; start < MAX_BULLET; start ++)
    {
        removeBullet(start);
    }

    SDL_Quit();
}
*/
int main(int argc, char *argv[])
{
    char errors[100];
    time_t now;
    int exit = 0;
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

    Man man;
    man.x = 50;
    man.y = 0;
    man.imageNow = 1;
    man.alive = 1;
    man.visible = 1;
    man.facingLeft = 0;

    enemy.x = 250;
    enemy.y = 60;
    enemy.imageNow = 1;
    enemy.facingLeft = 1;
    enemy.alive = 1;
    enemy.visible = 1;
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
        return EXIT_FAILURE;
    }

    SDL_Surface *sheet = IMG_Load ("Resources/goodguy.png");

    if(!sheet)
    {
        printf("Cannot find goodguy\n");
        return 1;
    }
    
    man.sheetTexture = SDL_CreateTextureFromSurface (renderer, sheet);
    SDL_FreeSurface(sheet);
  
    while(exit == 0 || exit == 1)
    {
        exit = processEvent(window, &man);

        Logic(&man);

        makeRenderer(renderer, &man);

        SDL_Delay(1000/60);
    }
    
    /*SELF EXPLANATORY*/
    SDL_DestroyTexture(man.sheetTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    int start = 0;
    for (start = 0; start < MAX_BULLET; start ++)
    {
        removeBullet(start);
    }
    SDL_Quit();
    return 0;
}
