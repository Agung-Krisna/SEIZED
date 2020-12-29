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
} Entity;

typedef struct
{
  	float x, y, dx;
} Bullet;

SDL_Texture *bulletTexture;
SDL_Texture *backgroundTexture;
Bullet *bullets[MAX_BULLET] = { NULL };
Entity enemy;

int globalTime = 0; //global time here refers to the changes in textures during walking and shooting.

void addBullet(float x, float y, float dx)
{
  	int found = -1;
  	int start = 0;
  	for(start = 0; start < MAX_BULLET; start++)
	{
		if(bullets[start] == NULL)
		{
		found = start;
		break;
		}
	}
    
	if(found >= 0)
	{
		int start = found;
		bullets[start] = malloc (sizeof(Bullet));
		bullets[start]->x = x;
		bullets[start]->y = y;
		bullets[start]->dx = dx;    
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

int Event(SDL_Window *window, Entity *man)//processing the events during the game
{
	SDL_Event event;
	int done = 0;

	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
		case SDL_WINDOWEVENT_CLOSE:
		{
			if(window)
			{
			SDL_DestroyWindow(window);
			window = NULL;
			done = 1;
			}
		}
		break;
		case SDL_KEYDOWN:
		{
			switch(event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				done = 1;
			break;
			}
		}
		break;
		case SDL_QUIT:
			done = 1;
		break;
		}
	}
	
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if(!man->shooting)
	{
		if(state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_A])
		{
		man->x -= 3;
		man->chWalk = 1;
		man->facingLeft = 1;
		
		if(globalTime % 9 == 0)
		{
			man->imageNow++;
			man->imageNow %= 8;  
		}  
		}
		else if(state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_D])
		{
		man->x += 3;
		man->chWalk = 1;
		man->facingLeft = 0;
		
		if(globalTime % 9 == 0)
		{
			man->imageNow++;
			man->imageNow %= 4;  
		}  
		}
		else
		{
		man->chWalk = 0;
		man->imageNow = 8;
		}
	}

	if(!man->chWalk)
	{
		if(state[SDL_SCANCODE_X] || state[SDL_SCANCODE_Z])//shoot using x and z
		{
		if(globalTime % 9 == 0)
		{
			if(man->imageNow == 8)         
			man->imageNow = 0;
			else
			man->imageNow = 0;
			
			if(!man->facingLeft)
			{
			addBullet(man->x+35, man->y+20, 3); 
			}
			else
			{
			addBullet(man->x+5, man->y+20, -3);         
			}
		}  
	
		man->shooting = 1;
		}
		else 
		{
		man->imageNow = 8;  
		man->shooting = 0;
		}
	}
	
	if(state[SDL_SCANCODE_UP] && !man->dy)
	{
		man->dy = -8;
	}
	
	return done;
}

void makeRenderer(SDL_Renderer *renderer, Entity *man)
{
  
  SDL_SetRenderDrawColor  (renderer, //specify the name of the renderer
                            0, //RED value that are used to draw on the rendering target. 
                            0, //GREEN value that are used to draw on the rendering target.
                            255, //BLUE value that are used to draw on the rendering target.
                            255); //keep this value to 255. ALPHA value that are used to draw on the rendering target.
  
  //Clear the screen (to blue)
  SDL_RenderClear(renderer);
  
  //set the drawing color to white
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  
  SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

  //protagonist / character
  if(man->visible)
  {
    SDL_Rect srcRect = { 40*man->imageNow, 0, 40, 50 };  
    SDL_Rect rect = { man->x, man->y, 40, 50 };  
    SDL_RenderCopyEx(renderer, man->sheetTexture, &srcRect, &rect, 0, NULL, man->facingLeft);
  }

  //enemy
  if(enemy.visible)
  {
    SDL_Rect eSrcRect = { 40*enemy.imageNow, 0, 40, 50 };  
    SDL_Rect eRect = { enemy.x, enemy.y, 40, 50 };  
    SDL_RenderCopyEx(renderer, enemy.sheetTexture, &eSrcRect, &eRect, 0, NULL, enemy.facingLeft);
  }

  for(int i = 0; i < MAX_BULLET; i++) if(bullets[i])
  {
    SDL_Rect rect = { bullets[i]->x, bullets[i]->y, 8, 8 };  
    SDL_RenderCopy(renderer, bulletTexture, NULL, &rect);
  }
  
  //We are done drawing, "present" or show to the screen what we've drawn
  SDL_RenderPresent(renderer);
}

void Logic(Entity *man)
{
	man->y += man->dy;
	man->dy += 0.5;
	if(man->y > 285)
	{
		man->y = 285;
		man->dy = 0;
	}
	
	for(int i = 0; i < MAX_BULLET; i++) if(bullets[i])
	{
		bullets[i]->x += bullets[i]->dx;
		
		//collision detector, simple without any complex data structures
		if(bullets[i]->x > enemy.x && bullets[i]->x < enemy.x+40 && 
		bullets[i]->y > enemy.y && bullets[i]->y < enemy.y+50)
		{
		enemy.alive = 0;
		} 
		
		if(bullets[i]->x < -1000 || bullets[i]->x > 1000)
		removeBullet(i);
	}
	
	if(enemy.alive == 0 && globalTime % 6 == 0)
	{
		if(enemy.imageNow < 3)
		enemy.imageNow = 3;
		else if(enemy.imageNow >= 3)
		{
			enemy.imageNow++;
			if(enemy.imageNow > 5)
			{
				enemy.visible = 0;
				enemy.imageNow = 5;
				enemy.imageNow++;
				if(enemy.imageNow > 9){
					enemy.visible = 0;
					enemy.imageNow = 6;
				}
			}
		}
	}
	
	globalTime++;
}

int main(int argc, char *argv[])
{
	SDL_Window *window;                    // Declare a window
	SDL_Renderer *renderer;                // Declare a renderer
	
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        printf("Error initializing the game. The cause is %s\n", SDL_GetError());
        return EXIT_FAILURE; //return error. 
    }
	
	SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2
	
	Entity man;
	man.x = 60;
	man.y = 180;
	man.imageNow = 5;  
	man.alive = 1;
	man.visible = 1;
	man.facingLeft = 0;
	
	enemy.x = 400;
	enemy.y = 285;
	enemy.imageNow = 1;
	enemy.facingLeft = 1;  
	enemy.alive = 1;
	enemy.visible = 1;
	
	//Create an application window with the following settings:
	window = SDL_CreateWindow   ("SEIZED",               //title
                                SDL_WINDOWPOS_CENTERED,  //initial position for x axis
                                SDL_WINDOWPOS_CENTERED,  //initial position for y axis
                                540,                     //width (pixels)
                                360,                     //height (pixels)
                                0);                      //flags


    Uint32 render_flags = SDL_RENDERER_ACCELERATED;//type render_flags to set the renderer so that it would use the computer VRAM.
    
	renderer = SDL_CreateRenderer(window, -1, render_flags);//making the renderer

	SDL_RenderSetLogicalSize(renderer, 540, 360);

	
	SDL_Surface *sheet = IMG_Load("Resources/goodguy.png");
	if(!sheet)
	{
		printf("Cannot find sheet\n");
		return 1;
	}
	
	man.sheetTexture = SDL_CreateTextureFromSurface(renderer, sheet);  
	SDL_FreeSurface(sheet);
	
	//load enemy
	sheet = IMG_Load("Resources/enemy.png");
	if(!sheet)
	{
		printf("Cannot find sheet\n");
		return 1;
	}
	
	enemy.sheetTexture = SDL_CreateTextureFromSurface(renderer, sheet);  
	SDL_FreeSurface(sheet);  

	//load the bg  
	SDL_Surface *bg = IMG_Load("Resources/background.png");
	
	if(!sheet)
	{
		printf("Cannot find background\n");
		return 1;
	}

	backgroundTexture = SDL_CreateTextureFromSurface(renderer, bg);
	SDL_FreeSurface(bg);

	//load the bullet  
	SDL_Surface *bullet = IMG_Load("Resources/bullet.png");
	
	if(!bullet)
	{
		printf("Cannot find bullet\n");
		return 1;
	}

	bulletTexture = SDL_CreateTextureFromSurface(renderer, bullet);
	SDL_FreeSurface(bullet);
	
	// The window is open: enter program loop (see SDL_PollEvent)
	int done = 0;
	
	//Event loop
	while(!done)
	{
		//Checking for events during the game
		done = Event(window, &man);
		
		//Updating the logic
		Logic(&man);
		
		//Displaying the renderer
		makeRenderer(renderer, &man);
		
		//delay 1/6 of a second
		SDL_Delay(1000/60);
	}
	
	
	// Close and destroy the window
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyTexture(man.sheetTexture);
	SDL_DestroyTexture(backgroundTexture);
	SDL_DestroyTexture(bulletTexture);
	SDL_DestroyTexture(enemy.sheetTexture);
	
	for(int i = 0; i < MAX_BULLET; i++)
		removeBullet(i);
	
	// Clean up
	SDL_Quit();
	return 0;
}

