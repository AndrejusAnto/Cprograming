//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <dirent.h>
#include <string.h>
#include <SDL2/SDL_image.h>

enum KeyPressSurfaces
{
	KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_TOTAL,
};

//Screen dimension constants
const int SCREEN_WIDTH = 400;
const int SCREEN_HEIGHT = 400;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Current displayed texture
SDL_Texture* gTexture = NULL;

// The images that correspond to a keypress
SDL_Texture* gKeyPressSurfaces[ KEY_PRESS_SURFACE_TOTAL ];

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) != 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", 20, 20, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );

			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				//Initialize PNG loading
				int imgsflags = IMG_INIT_PNG;
				// The reason the code is like that is because we only care about the PNG loading bit.
				 // If we get that, that means we can continue. In other cases this code would be different, 
				// but we're not dealing with that here.
				if ( !(IMG_Init(imgsflags) & imgsflags) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

SDL_Texture* loadTexture( char *impath, SDL_Renderer *ren)
{
	//The final optimized image
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( impath );
	if( loadedSurface == NULL )
	{
		printf( "asdasdasd Unable to load image! SDL Error: %s\n", SDL_GetError() );

	}
	else
	{
		//Convert surface to screen format
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if( newTexture == NULL )
		{
			printf( "Unable to optimize image! SDL Error: %s\n", SDL_GetError() );
		}

		//Get rid of old loaded surface
		// It's important to note that SDL_ConvertSurface returns a copy of the original in a new format. 
		// The original loaded image is still in memory after this call. 
		// This means we have to free the original loaded surface or we'll have two copies of the same image in memory.

		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;
	int i;

	// struct ImagesList Images = {"press.bmp", "up.bmp", "down.bmp", "left.bmp", "right.bmp"};
	gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT] = loadTexture("press.bmp", gRenderer);
	if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT] == NULL )
	{
		printf( "Unable to load image! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}

	gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP] = loadTexture("up.bmp", gRenderer);
	if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP] == NULL )
	{
		printf( "Unable to load image! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}

	gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN] = loadTexture("down.bmp", gRenderer);
	if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN] == NULL )
	{
		printf( "Unable to load image! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}

	gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT] = loadTexture("left.bmp", gRenderer);
	if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT] == NULL )
	{
		printf( "Unable to load image! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}

	gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT] = loadTexture("right.bmp", gRenderer);
	if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT] == NULL )
	{
		printf( "Unable to load image! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}

	return success;
}

void closeapp()
{
	SDL_DestroyTexture(gTexture);
	gTexture = NULL;
	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gRenderer = NULL;
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

int main()
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
		SDL_Quit();
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
			SDL_Quit();
		}
		else
		{
			bool quit = false;
			SDL_Event e;
			gTexture = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ];

			while (!quit)
			{
				while (SDL_PollEvent(&e) != 0)
				{
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
					 else if( e.type == SDL_KEYDOWN )
					{
						//Select surfaces based on key press
						switch( e.key.keysym.sym )
						{
							case SDLK_UP:
							case SDLK_w:
							gTexture = gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ];
							break;

							case SDLK_DOWN:
							case SDLK_s:
							gTexture = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ];
							break;

							case SDLK_LEFT:
							case SDLK_a:
							gTexture = gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ];
							break;

							case SDLK_RIGHT:
							case SDLK_d:
							gTexture = gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ];
							break;

							default:
							gTexture = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ];
							break;
						}
					}
				}
				//Clear screen
				SDL_RenderClear(gRenderer);
				SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
				SDL_RenderPresent(gRenderer);
			}
			
		}
	}

	//Free resources and close SDL
	closeapp();
	// imagelist();

	return 0;
}
