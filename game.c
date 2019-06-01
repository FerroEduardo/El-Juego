#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
//gcc -o game game.c -lSDL2 -lSDL2_image -lm -Wall `sdl2-config --libs`

//SDL_Image, SDL_Mixer, SDL_Ttf, SDL_Gfx
//http://www.lazyfoo.net/tutorials/SDL/01_hello_SDL/index2.php
//https://www.youtube.com/watch?v=yFLa3ln16w0

//tamanho da janela
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//speed in pixels/second
#define SCROLL_SPEED 300

//variaveis globais
SDL_Window *window = NULL;
SDL_Surface *screenSurface = NULL;



int inicializar();
int finalizar();
/*
SDL surface is merely a struct that represents image data in memory.
A texture corresponds to image data loaded into the graphic's hardware's memory.
So a surface is in regular memory, and a texture is in this separate VRAM.
*/

int main(int argc, char* args[]){ 

    inicializar();  
    //cria flags para o renderer
    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    SDL_Renderer *render = SDL_CreateRenderer(window,-1,render_flags);
    if(render==NULL){
        printf("Deu merda no RENDERER! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    //carrega imagem na memoria do pc,provavelmente na RAM
    SDL_Surface *screenSurface = IMG_Load("recursos/mario.png");
    if(screenSurface==NULL){
        printf("Deu merda ao criar SURFACE! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(render);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    //carrega imagem na memoria de video,VRAM
    SDL_Texture *texture = SDL_CreateTextureFromSurface(render, screenSurface);
    SDL_FreeSurface(screenSurface);
    if(texture==NULL){
        printf("Deu merda ao criar TEXTURE! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(render);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    SDL_RenderClear(render);
    //sdl_rect, struct do tamanho do sprite e posicao
    SDL_Rect mario;
    /*
    mario.x = 0;
    mario.y = 0;
    mario.w = 32;
    mario.h = 32;
    */

    //pega as dim da textura
    SDL_QueryTexture(texture, NULL, NULL, &mario.w, &mario.h);

    //posicao do sprite no meio inferior da janela
    mario.x = (SCREEN_WIDTH - mario.w) / 2;

    //require float resolution for y position
    float y_pos = SCREEN_HEIGHT;

    //animation loop
    while(mario.y>=-mario.h){
        //limpar janela a cada frame
        SDL_RenderClear(render);

        //set the y pos in the struct
        mario.y = (int) y_pos;

        //draw the image to the window
        SDL_RenderCopy(render,texture, NULL, &mario);
        SDL_RenderPresent(render);

        //update sprite position
        y_pos -= (float) SCROLL_SPEED / 60;

        //wait 1/60th of a second
        SDL_Delay(1000/60);
    }
    

    //limpa janela, remove o "lixo" e tranforma o fundo em preto
    //SDL_RenderClear(render);

    //traz imagem para a janela, "copia a textura e renderiza"
    //ler sobre SDL_Rect
    //double buffering
    //SDL_RenderCopy(render, texture, NULL, NULL);
    //SDL_RenderPresent(render);

    //em 5000ms=5s
    //SDL_Delay(5000);

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(render);
    
    
	finalizar();
    return 0;
}
//-------------------------------------------------------------------------------------
int inicializar(){
    SDL_Init(SDL_INIT_EVERYTHING);

        if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
            printf("Deu merda para iniciar o SDL! SDL_Error: %s\n", SDL_GetError());
            SDL_Quit();
            return 1;
        }
        printf("Iniciou o SDL\n");
        window = SDL_CreateWindow("v0.0.0", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if(window == NULL){
            printf("Deu merda na janela! SDL_Error: %s\n", SDL_GetError());
            SDL_Quit();
            return 1;
        }
        printf("Iniciou a JANELA\n");
        //Get window surface
        //screenSurface = SDL_GetWindowSurface(window);

        //Fill the surface white
        //SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0x53, 0x5F, 0x8F ) );
    
        //Update the surface
        //SDL_UpdateWindowSurface(window);
        
        //Wait two seconds
        //SDL_Delay(2000);
            
        
}

int finalizar(){
    //Destroy window
	SDL_DestroyWindow(window);
	window = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}