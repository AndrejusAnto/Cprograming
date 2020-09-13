//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <dirent.h>
#include <string.h>
#include <SDL2/SDL_image.h>
#include <time.h>

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
const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;
const int X_POS = 0;
const int Y_POS = 0;


//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Current displayed texture
SDL_Texture* gTexture = NULL;

// The images that correspond to a keypress
SDL_Texture* gKeyPressSurfaces[ KEY_PRESS_SURFACE_TOTAL ];


struct ImagesList {
	char imgdef[20];
	char imgup[20];
	char imgdown[20];
	char imgleft[20];
	char imgright[20];

};

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
		gWindow = SDL_CreateWindow( "SDL Tutorial", X_POS, Y_POS, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
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

struct ImagesList imagelist()
{
	const char ch = '.';
	char *ret;
	struct dirent *de;  // Pointer for directory entry 
	// opendir() returns a pointer of DIR type.
	struct ImagesList Images;
	DIR *dr = opendir("."); 
	if (dr == NULL)  // opendir returns NULL if couldn't open directory 
	{
		printf("Could not open current directory" );
	}
	// Refer http://pubs.opengroup.org/onlinepubs/7990989775/xsh/readdir.html 
	// for readdir() 
	while ((de = readdir(dr)) != NULL)
	{
		ret = strrchr(de->d_name, ch);
		if (ret != NULL)
		{
			if (!strcmp(ret, ".bmp"))
			{
				if (!strcmp(de->d_name, "press.bmp"))
				{
					strcpy(Images.imgdef, de->d_name);

				}
				else if (!strcmp(de->d_name, "up.bmp"))
				{
					strcpy(Images.imgup, de->d_name);;

				}
				else if (!strcmp(de->d_name, "down.bmp"))
				{
					strcpy(Images.imgdown, de->d_name);

				}
				else if (!strcmp(de->d_name, "left.bmp"))
				{
					strcpy(Images.imgleft, de->d_name);

				}
				else if (!strcmp(de->d_name, "right.bmp"))
				{
					strcpy(Images.imgright, de->d_name);

				}
			}
		}
	}
	closedir(dr);
	return Images;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	struct ImagesList Images = imagelist();
	// struct ImagesList Images = {"press.bmp", "up.bmp", "down.bmp", "left.bmp", "right.bmp"};
	gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT] = loadTexture(Images.imgdef, gRenderer);
	if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT] == NULL )
	{
		printf( "Unable to load image! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}

	gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP] = loadTexture(Images.imgup, gRenderer);
	if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP] == NULL )
	{
		printf( "Unable to load image! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}

	gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN] = loadTexture(Images.imgdown, gRenderer);
	if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN] == NULL )
	{
		printf( "Unable to load image! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}

	gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT] = loadTexture(Images.imgleft, gRenderer);
	if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT] == NULL )
	{
		printf( "Unable to load image! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}

	gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT] = loadTexture(Images.imgright, gRenderer);
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
	int sqw = 40;
	int sqh = 40;
	int w;
	int h;
	srand(time(NULL));
	int tilemap[20][30];
	int wsize = SCREEN_WIDTH / sqw;
	int hsize = SCREEN_HEIGHT / sqh;

	for (h = 0; h < hsize ; h++)
	{
		for (w = 0; w < wsize; w++)
		{
			int rn = rand()%2;
			tilemap[h][w] = rn;
		}
	}

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

				// primitives
				// SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				// SDL_RenderClear(gRenderer);
				// uzpildytas staciakampis
				int nw = 0;
				for (h = 0; h < hsize ; h++)
				{
					int nh = 0;
					for (w = 0; w < wsize; w++)
					{
						
						SDL_Rect fillRect = {nh, nw, sqw, sqh};
						if (tilemap[h][w] == 0)
						{
							SDL_SetRenderDrawColor(gRenderer , 0, 90, 0, 0);
						}
						else
						{
							SDL_SetRenderDrawColor(gRenderer , 43, 29, 14, 0);
						}
						SDL_RenderFillRect(gRenderer, &fillRect);
						nh = nh + 40;
					}
					nw = nw + 40;
				}

				// // tuscias staciakampis
				// SDL_Rect outRect = {SCREEN_WIDTH/ 6, SCREEN_HEIGHT/ 6, SCREEN_WIDTH / 2, SCREEN_WIDTH / 2};
				// SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0xFF);
				// SDL_RenderDrawRect(gRenderer, &outRect);

				// // linija
				// SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0xFF, 0xFF );
				// SDL_RenderDrawLine( gRenderer, 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2 );

				// // punktirine
				// SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0x00, 0xFF );
				// for( int i = 0; i < SCREEN_HEIGHT; i += 4 )
				// {
				// 	SDL_RenderDrawPoint( gRenderer, SCREEN_WIDTH / 2, i );
				// }

				// update 
				SDL_RenderPresent(gRenderer);
			}
			
		}
	}

	//Free resources and close SDL
	closeapp();
	// imagelist();

	return 0;
}
