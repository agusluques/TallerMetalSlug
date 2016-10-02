#include <iostream>
#include <cstring>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

using namespace std;

class LTexture
{
	public:
		//Initializes variables
		LTexture();

		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile( string path );

		//Deallocates texture
		void free();

		//Renders texture at given point
		void render( int x, int y, SDL_Rect* clip = NULL , int flag = 0);
		void render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip );

		//Gets image dimensions
		int getWidth();
		int getHeight();

		void setWindow(SDL_Renderer* ventana);

	private:
		//The actual hardware texture
		SDL_Texture* mTexture;

		SDL_Renderer* window;
		//Image dimensions
		int mWidth;
		int mHeight;
};

LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
	window = NULL;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

void LTexture::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

bool LTexture::loadFromFile( std::string path )
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
	
		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( window, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}


void LTexture::render( int x, int y, SDL_Rect* clip, int flagFondo )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	if (flagFondo != 0){
		renderQuad.w = 800;//COMENTARIO: ESTO NO PUEDE ESTAR HARCODEADO
		renderQuad.h = 600;
	}

	//Render to screen
	SDL_RenderCopy( window, mTexture, clip, &renderQuad );
}

void LTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx( window, mTexture, clip, &renderQuad, angle, center, flip );
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

void LTexture::setWindow(SDL_Renderer* ventana){
	window = ventana;
}