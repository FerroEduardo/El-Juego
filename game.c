#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "customlib.h"


//VERSÃO
#define version "0.0.09" 
//gcc -o game game.c -lSDL2 -lSDL2_image -lSDL2_mixer -lm -Wall customlib.h -Wno-switch 
//-Wno-switch remove todos os warns relacionados ao Wswitch
//compila e abre se nao tiver erro
//gcc -o game game.c -lSDL2 -lSDL2_image -lSDL2_mixer -lm -Wall customlib.h -Wno-switch && ./game
//SDL_Image, SDL_Mixer, SDL_Ttf, SDL_Gfx
//http://www.lazyfoo.net/tutorials/SDL/01_hello_SDL/index2.php
//https://www.youtube.com/watch?v=yFLa3ln16w0

//tamanho da janela
//largura
const int SCREEN_WIDTH = 640;
//altura
const int SCREEN_HEIGHT = 480;

//speed in pixels/second
#define SCROLL_SPEED 300
#define SPEED 300

#define speedPlayer 10

//variaveis globais
SDL_Window *window = NULL;
SDL_Renderer *render = NULL;
SDL_Surface *surfPlayer = NULL;
SDL_Texture *texturePlayer = NULL;



int inicializar();
int finalizar();
void nobreu();
int startAudio();
int startRenderer();
/*
SDL surface is merely a struct that represents image data in memory.
A texture corresponds to image data loaded into the graphic's hardware's memory.
So a surface is in regular memory, and a texture is in this separate VRAM.
*/

int main(int argc, char* args[]){ 

    inicializar();  
    startRenderer();
    //PLAYER------------
    //carrega imagem na memoria do pc,provavelmente na RAM "mario.png"
    SDL_Surface *surfPlayer = IMG_Load("recursos/mario.png");
    if(surfPlayer==NULL){
        printf("Deu merda ao criar surfPlayer! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(render);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }    
    //carrega imagem na memoria de video,VRAM
    SDL_Texture *texturePlayer = SDL_CreateTextureFromSurface(render, surfPlayer);
    //SDL_FreeSurface(surfPlayer);
    if(texturePlayer==NULL){
        printf("Deu merda ao criar texturePlayer! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(render);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    SDL_Rect rectPlayer;
    /*
    rectPlayer.x = 0;
    rectPlayer.y = 0;
    rectPlayer.w = 32;
    rectPlayer.h = 32;
    */
    rectPlayer.w = 100;
    rectPlayer.h = 100;
    rectPlayer.x = (SCREEN_WIDTH/2) - (rectPlayer.w/2);
    rectPlayer.y = (SCREEN_HEIGHT /2) -(rectPlayer.h/2);

    // h/height altura
    // w/width largura

    SDL_RenderClear(render);
    //---------------------
    //BACKGROUND-----------
    SDL_Surface *surfBACKGROUND = IMG_Load("recursos/forest.png");
    if(surfBACKGROUND==NULL){
        printf("Deu merda ao criar surfBACKGROUND! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(render);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }    
    SDL_Texture *texBACKGROUND = SDL_CreateTextureFromSurface(render, surfBACKGROUND);
    //SDL_FreeSurface(surfBACKGROUND);
    if(texBACKGROUND==NULL){
        printf("Deu merda ao criar texBACKGROUND! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(render);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    SDL_RenderClear(render);
    int sobe, desce;
    int esquerda,direita;
    //---------------------
    //ENEMY1----------
    SDL_Surface *surfEnemy = IMG_Load("recursos/enemy1.jpeg");
    if(surfEnemy==NULL){
        printf("Deu merda ao criar surfEnemy! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(render);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }    
    SDL_Texture *texEnemy = SDL_CreateTextureFromSurface(render, surfEnemy);
    if(texEnemy==NULL){
        printf("Deu merda ao criar texEnemy! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(render);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    SDL_Rect rectEnemy1;
    rectEnemy1.w = 100;
    rectEnemy1.h = 100;
    rectEnemy1.x = 100;
    rectEnemy1.y = 190;
    //-----------------
    
    
    

    
    //set to 1 when window close button is pressed
    int close_requested = 0;
        
    //startAudio();
    //nobreu();

    //animation loop
    int startou = 0;
    while (close_requested == 0){
        if(startou==0){
            printf("Iniciou o JOGO\n");
            startou = 1;
        }
        
        //process events
        SDL_Event evento;
        
        while (SDL_PollEvent(&evento)){
            switch (evento.type){
                case SDL_QUIT:
                    close_requested = 1;
                break;
                case SDL_KEYDOWN:
                //pressiona wasd ou setas para andar
                    switch (evento.key.keysym.scancode){
                        case SDL_SCANCODE_W:
                        case SDL_SCANCODE_UP:
                            sobe=1;
                            break;
                        case SDL_SCANCODE_A:
                        case SDL_SCANCODE_LEFT:
                            esquerda=1;
                            break;
                        case SDL_SCANCODE_S:
                        case SDL_SCANCODE_DOWN:
                            desce=1;
                            break;
                        case SDL_SCANCODE_D:
                        case SDL_SCANCODE_RIGHT:
                            direita=1;
                            break;
                    }
                break;
                
                case SDL_KEYUP:
                    switch (evento.key.keysym.scancode){
                        case SDL_SCANCODE_W:
                        case SDL_SCANCODE_UP:
                            sobe=0;
                            break;
                        case SDL_SCANCODE_A:
                        case SDL_SCANCODE_LEFT:
                            esquerda=0;
                            break;
                        case SDL_SCANCODE_S:
                        case SDL_SCANCODE_DOWN:
                            desce=0;
                            break;
                        case SDL_SCANCODE_D:
                        case SDL_SCANCODE_RIGHT:
                            direita=0;
                            break;
                    }
                    break;
            }
            //fim teclas
            //colisao com "janela"            
            if(rectPlayer.y + rectPlayer.h >= SCREEN_HEIGHT){
                desce=0;
            }
            if(rectPlayer.y <= 0){
                sobe=0;
            }
            if(rectPlayer.x+rectPlayer.w >= SCREEN_WIDTH){
                direita=0;
            }
            if(rectPlayer.x <= 0){
                esquerda=0;
            }
            //fim colisao com janela
            //colisao player com outro rect
            if(rectPlayer.x+rectPlayer.w>=rectEnemy1.x &&
            rectPlayer.y < rectEnemy1.y +rectEnemy1.h &&
            rectPlayer.y + rectPlayer.h > rectEnemy1.y &&
            rectPlayer.x < rectEnemy1.x + rectEnemy1.w             
            ){
                direita=0;
            }
            if(rectPlayer.y <= rectEnemy1.y + rectEnemy1.h &&
            rectPlayer.x+rectPlayer.w>rectEnemy1.x &&
            rectPlayer.x < rectEnemy1.x + rectEnemy1.w 
            ){
                sobe=0;
            }
            if(rectPlayer.x <= rectEnemy1.x + rectEnemy1.w &&
            rectPlayer.y < rectEnemy1.y + rectEnemy1.h &&
            rectPlayer.y + rectPlayer.h > rectEnemy1.y
                       
            ){
                esquerda=0;
            }
            if(rectPlayer.y+rectPlayer.h>=rectEnemy1.y &&
            rectPlayer.y + rectPlayer.h >= rectEnemy1.y &&
            rectPlayer.x < rectEnemy1.x + rectEnemy1.w &&
            rectPlayer.x + rectPlayer.w > rectEnemy1.x
            ){
                desce=0;
            }
            //fim colisao de player com outro rect






            //fim colisao rect
            //locomocao player            
            if(sobe==1){
                rectPlayer.y -=speedPlayer;
            }
            if(desce==1){
                rectPlayer.y +=speedPlayer;
            }
            if(esquerda==1){
                rectPlayer.x -=speedPlayer;
            }
            if(direita==1){
                rectPlayer.x +=speedPlayer;
            }
            //fim locomocao

            








        SDL_RenderClear(render);
        SDL_RenderCopy(render, texBACKGROUND, NULL, NULL);
        SDL_RenderCopy(render, texturePlayer, NULL, &rectPlayer);        
        SDL_RenderCopy(render, texEnemy, NULL, &rectEnemy1);        
        SDL_RenderPresent(render);     
        SDL_Delay(1000/60); 
        
        }     
 
    }
    
    SDL_FreeSurface(surfPlayer);
    SDL_DestroyTexture(texturePlayer);
    SDL_FreeSurface(surfBACKGROUND);
    SDL_DestroyTexture(texBACKGROUND);
    SDL_DestroyRenderer(render);
    
    
	finalizar();
    return 0;
}
//------------------------------------------------------------------------------------//
int inicializar(){
    SDL_Init(SDL_INIT_EVERYTHING);

        if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
            printf("Deu merda para iniciar o SDL! SDL_Error: %s\n", SDL_GetError());
            SDL_Quit();
            return 1;
        }
        printf("Iniciou o SDL\n");
        Uint32 flagsCreateWindow = SDL_WINDOW_SHOWN | SDL_WINDOW_MOUSE_CAPTURE | SDL_WINDOW_RESIZABLE;
        window = SDL_CreateWindow(version, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, flagsCreateWindow );
        if(window == NULL){
            printf("Deu merda na janela! SDL_Error: %s\n", SDL_GetError());
            SDL_Quit();
            return 1;
        }
        printf("Iniciou a JANELA\n");
        return 0;
}

int finalizar(){
    printf("Finaliza SDL\n");
    //Destroy window
	SDL_DestroyWindow(window);
	window = NULL;

	//Quit SDL(subsystems)
	SDL_Quit();
    return 0;
}

void nobreu(){
    printf("Toca PEGA NO BREU\n");
    //salva musica para carregar/tocar depois
    Mix_Chunk *nobreu = Mix_LoadWAV("recursos/nobreu.mp3");
    Mix_Chunk *vucvuc = Mix_LoadWAV("recursos/vucvuc.mp3");
    if(nobreu==NULL){
        printf("erro NOBREU: %s\n", Mix_GetError());
    }
    //volume da faixa, "-1" muda o volume de todos os canais
    Mix_Volume(-1,MIX_MAX_VOLUME/20);
    //"-1" toca faixa no prox canal disponivel
    srand (time(NULL));
    int music0 = ((rand()) % 2);
    if(music0==1)
        Mix_PlayChannel(-1,nobreu,-1);    
    else if(music0==0)
        Mix_PlayChannel(-1,vucvuc,-1);   

}

int startAudio(){
    //inicia "driver" de audio
    printf("Iniciou AUDIO\n");
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024)==-1) {
        printf("Merda no Mix_OpenAudio: %s\n", Mix_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    return 0;
}

int startRenderer(){
    //cria flags para o renderer
    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    render = SDL_CreateRenderer(window,-1,render_flags);
    if(render==NULL){
        printf("Deu merda no RENDERER! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    return 0;
}
