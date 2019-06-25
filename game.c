#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "customlib.h"


//VERSÃO
#define version "0.0.14" 
//gcc -o game game.c -lSDL2 -lSDL2_image -lSDL2_mixer -lm -Wall customlib.h -Wno-switch 
//-Wno-switch remove todos os warns relacionados ao Wswitch
//compila e abre se nao tiver erro
//gcc -o game game.c -lSDL2 -lSDL2_image -lSDL2_mixer -lm -Wall customlib.h -Wno-switch && ./game
//SDL_Image, SDL_Mixer, SDL_Ttf, SDL_Gfx
//http://www.lazyfoo.net/tutorials/SDL/01_hello_SDL/index2.php
//https://www.youtube.com/watch?v=yFLa3ln16w0

//tamanho da janela
//largura 640
int SCREEN_WIDTH = 1280;
int width = 1280;
//altura 480
int SCREEN_HEIGHT = 720;
int height = 720;
//escala 16:9
double ESCALA = 16.f/9.f;

const int LEVEL_WIDTH = 1280;
const int LEVEL_HEIGHT = 720;

//speed in pixels/second
#define SCROLL_SPEED 300
#define SPEED 300

#define speedPlayer 10
#define FPS 30
#define framedelay 1000/FPS
#define cutdelay 150

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
void startmusicMenu();

int statusGame=0;
/*
SDL surface is merely a struct that represents image data in memory.
A texture corresponds to image data loaded into the graphic's hardware's memory.
So a surface is in regular memory, and a texture is in this separate VRAM.
*/

int main(int argc, char* args[]){ 
    int sobe, desce;
    int esquerda,direita;

    inicializar();  
    startRenderer();
    //PLAYER------------
    //carrega imagem na memoria do pc,provavelmente na RAM "mario.png"
    SDL_Surface *surfPlayer = IMG_Load("recursos/ranger_m.png");
    if(surfPlayer==NULL){
        printf("Deu merda ao criar surfPlayer! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(render);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }    
    //carrega imagem na memoria de video,VRAM
    SDL_Texture *texturePlayer = SDL_CreateTextureFromSurface(render, surfPlayer);
    if(texturePlayer==NULL){
        printf("Deu merda ao criar texturePlayer! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(render);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    SDL_Rect rectPlayer;
    rectPlayer.x = (SCREEN_WIDTH/2)   - (rectPlayer.w/2);
    rectPlayer.y = (SCREEN_HEIGHT /2) - (rectPlayer.h/2);
    rectPlayer.w = 110;
    rectPlayer.h = 120;
    
    SDL_Rect rectPlayerSprite = {0,72,33,36};

    // h/height altura
    // w/width largura

    //---------------------
    
    //BACKGROUND-----------
    SDL_Surface *surfBACKGROUND = IMG_Load("recursos/grande.png");
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

    SDL_Rect rectBackground = {1000,1000,1280,720};

    //MENU-------------------
    SDL_Surface *surfMENU = IMG_Load("recursos/menu.png");
    if(surfMENU==NULL){
        printf("Deu merda ao criar surfMENU! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(render);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }    
    SDL_Texture *texMENU = SDL_CreateTextureFromSurface(render, surfMENU);
    if(texMENU==NULL){
        printf("Deu merda ao criar texMENU! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(render);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Rect rectMenu = {0,0,1280,720};


    //------------------------






    
    //---------------------
    //ENEMY1----------
    SDL_Surface *surfEnemy = IMG_Load("recursos/mage_f.png");
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
    SDL_Rect rectEnemy1 ={900, 160, 110,120};
    SDL_Rect spriteEnemy = {0,72,33,36};
    //+22
    
    //-----------------
    //enemy2-----------
    SDL_Surface *surfEnemy2 = IMG_Load("recursos/enemy2.png");
    if(surfEnemy2==NULL){
        printf("Deu merda ao criar surfEnemy2! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(render);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }    
    SDL_Texture *texEnemy2 = SDL_CreateTextureFromSurface(render, surfEnemy2);
    //SDL_FreeSurface(surfEnemy2);
    if(texEnemy2==NULL){
        printf("Deu merda ao criar texEnemy2! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(render);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    SDL_Rect rectEnemy2;
    rectEnemy2.x = 300;
    rectEnemy2.y = 200;
    rectEnemy2.w = 100;
    rectEnemy2.h = 100;
    
    
    //-----------------
    SDL_Surface *surfSprite = IMG_Load("recursos/8xCUTSCENE.png");
    SDL_Texture *texSprite = SDL_CreateTextureFromSurface(render, surfSprite);
    SDL_Rect rectScene;
    rectScene.x = 0;
    rectScene.y = 0;
    rectScene.h = SCREEN_HEIGHT;
    rectScene.w = SCREEN_WIDTH;
    SDL_Surface *screen = NULL;
    SDL_Rect recorte = { 0 , 0, 258, 146};
    
    

    
    //set to 1 when window close button is pressed
    int close_requested = 0;
        
    uint32_t framestart;
    uint32_t frameTime,frameTimeSprite;
    int startMMenu=0;
    startAudio();
    //nobreu();

    //animation loop
    printf("Iniciou o JOGO\n");
    while (close_requested == 0){
        
        //process events
        SDL_Event evento;
        SDL_Event evento2;

        if(statusGame==0){
            if(startMMenu==0){
                startmusicMenu();
                ++startMMenu;
            }
            
            framestart = SDL_GetTicks();
            while(SDL_PollEvent(&evento2)){                
                switch (evento2.type){
                    case SDL_QUIT:
                        close_requested = 1;
                    break;
                    
                    case SDL_KEYDOWN:
                    //pressiona wasd ou setas para andar
                        switch (evento2.key.keysym.sym){
                            case SDLK_1:
                                Mix_FadeOutChannel(-1, 1000);
                                statusGame=1;
                                SDL_FreeSurface(surfMENU);
                                SDL_DestroyTexture(texMENU);
                                break;
                            case SDLK_2:
                                Mix_FadeOutChannel(-1, 1000);
                                statusGame=1;
                                SDL_FreeSurface(surfMENU);
                                SDL_DestroyTexture(texMENU);
                                break;
                            case SDLK_3:
                                Mix_FadeOutChannel(-1, 1000);
                                statusGame=1;
                                SDL_FreeSurface(surfMENU);
                                SDL_DestroyTexture(texMENU);
                                break;
                            case SDLK_4:
                                Mix_FadeOutChannel(-1, 1000);
                                statusGame=1;
                                SDL_FreeSurface(surfMENU);
                                SDL_DestroyTexture(texMENU);
                                break;
                            case SDLK_END:
                                SDL_FreeSurface(surfMENU);
                                SDL_DestroyTexture(texMENU);
                                close_requested = 1;
                                break;
                        }
                    break;           
                        }
            }




            frameTime = SDL_GetTicks() - framestart;
            SDL_RenderClear(render);
            SDL_RenderCopy(render, texMENU, NULL, NULL);
            SDL_RenderPresent(render);
            if(framedelay > frameTime){
                SDL_Delay((framedelay) - frameTime);
            }
        }

        else if(statusGame==1){
            framestart = SDL_GetTicks();
                if(recorte.x<=7998 && recorte.y == 0){
                    recorte.x += 258;
                    if(recorte.x>7998){
                        recorte.x = 0;
                        recorte.y += 146;
                    }
                }
                else if(recorte.y == 146){
                    recorte.x += 258;
                }
                if(recorte.x >= 7998 && recorte.y == 146){
                        statusGame = 2;
                        SDL_DestroyTexture(texSprite);
                }
            
            
            
            frameTime = SDL_GetTicks() - framestart;
            SDL_RenderClear(render);
            SDL_RenderCopy(render, texSprite, &recorte, &rectScene);
            SDL_RenderPresent(render);
            if(framedelay > frameTime){
                SDL_Delay((cutdelay) - frameTime);
            }
        }


        else if(statusGame==2){
            framestart = SDL_GetTicks();
            while (SDL_PollEvent(&evento)){                
                switch (evento.type){
                    case SDL_QUIT:
                        close_requested = 1;
                    break;
                    
                    case SDL_KEYDOWN:
                    //pressiona wasd ou setas para andar
                        switch (evento.key.keysym.sym){
                            case SDLK_UP:
                                sobe=1;
                                break;
                            case SDLK_LEFT:
                                esquerda=1;
                                break;
                            case SDLK_DOWN:
                                desce=1;
                                break;
                            case SDLK_RIGHT:
                                direita=1;
                                break;
                            case SDLK_END:
                                close_requested = 1;
                                break;
                        }
                    break;
                    
                    case SDL_KEYUP:
                        switch (evento.key.keysym.sym){
                            case SDLK_UP:
                                sobe=0;
                                break;
                            case SDLK_LEFT:
                                esquerda=0;
                                break;
                            case SDLK_DOWN:
                                desce=0;
                                break;
                            case SDLK_RIGHT:
                                direita=0;
                                break;
                        }
                        break;
                    /*
                    case SDL_WINDOWEVENT:
                        switch (evento.window.event){
                        case SDL_WINDOWEVENT_RESIZED:
                            width = evento.window.data1;
                            height = evento.window.data2;
                            int escala = (double)width/(double)height;
                            if(ESCALA != escala){
                                if(escala>ESCALA){
                                    height = (1.f / ESCALA)* width;
                                }
                                else{
                                    width = ESCALA * height;
                                }
                                printf("Definindo tamanho de tela para %d:%d, na escala: %f\n", 
                                    width, height, (double)width/(double)height);
                            }
                            SCREEN_WIDTH = width;
                            SCREEN_HEIGHT = height;
                            SDL_SetWindowSize(window,width,height);
                            break;                    

                        }
                        break;
                        */
                }
                //fim event loop
                //colisao com "janela"
                /*
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
                */
                if(rectBackground.y -rectPlayerSprite.h >= SCREEN_HEIGHT*5){
                    desce=0;
                }
                if(rectBackground.y <= 0){
                    sobe=0;
                }
                if(rectBackground.x + rectPlayerSprite.w>= SCREEN_WIDTH*5){
                    direita=0;
                }
                if(rectBackground.x <= 0){
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
                //locomocao/sprite player
                frameTimeSprite = SDL_GetTicks() - framestart;
                if(sobe==1){
                    //rectPlayer.y -=speedPlayer;
                    rectBackground.y -=speedPlayer;
                    rectPlayerSprite.y = 0;
                    if(rectPlayerSprite.x <=66){
                        rectPlayerSprite.x += 33;
                    }
                    if(rectPlayerSprite.x>66){
                        rectPlayerSprite.x = 0; 
                    }
                    if(framedelay > frameTime){
                        //SDL_Delay((framedelay) - frameTimeSprite);
                    }
                }
                else if(desce==1){
                    //rectPlayer.y +=speedPlayer;
                    rectBackground.y +=speedPlayer;
                    rectPlayerSprite.y = 72;
                    if(rectPlayerSprite.x <=66){
                        rectPlayerSprite.x += 33;
                    }
                    if(rectPlayerSprite.x>66){
                        rectPlayerSprite.x = 0; 
                    }
                    if(framedelay > frameTime){
                        //SDL_Delay((framedelay) - frameTimeSprite);
                    }
                }
                else if(esquerda==1){
                    //rectPlayer.x -=speedPlayer;
                    rectBackground.x -=speedPlayer;
                    rectPlayerSprite.y = 108;
                    if(rectPlayerSprite.x <=66){
                        rectPlayerSprite.x += 33;
                    }
                    if(rectPlayerSprite.x>66){
                        rectPlayerSprite.x = 0; 
                    }
                    if(framedelay > frameTime){
                        //SDL_Delay((framedelay) - frameTimeSprite);
                    }
                }
                else if(direita==1){
                    //rectPlayer.x +=speedPlayer;
                    rectBackground.x +=speedPlayer;
                    rectPlayerSprite.y = 36;
                    if(rectPlayerSprite.x <=66){
                        rectPlayerSprite.x += 33;
                    }
                    if(rectPlayerSprite.x>66){
                        rectPlayerSprite.x = 0; 
                    }
                    if(framedelay > frameTime){
                        //SDL_Delay((framedelay) - frameTimeSprite);
                    }
                }
                //fim locomocao

                






                frameTime = SDL_GetTicks() - framestart;
                SDL_RenderClear(render);
                SDL_RenderCopy(render, texBACKGROUND, &rectBackground, NULL);
                SDL_RenderCopy(render, texturePlayer, &rectPlayerSprite, &rectPlayer);
                SDL_RenderCopy(render, texEnemy, &spriteEnemy, &rectEnemy1);
                SDL_RenderCopy(render, texEnemy2, NULL, &rectEnemy2);
                SDL_RenderPresent(render);
                if(framedelay > frameTime){
                    SDL_Delay((framedelay) - frameTime);
                }
                
                    

            }     
    
        }
    }
        SDL_FreeSurface(surfPlayer);
        SDL_FreeSurface(surfBACKGROUND);
        //SDL_FreeSurface(surfMENU);
        SDL_FreeSurface(surfEnemy2);
        SDL_FreeSurface(surfEnemy);
        SDL_DestroyTexture(texturePlayer);
        SDL_DestroyTexture(texBACKGROUND);
        //SDL_DestroyTexture(texMENU);
        SDL_DestroyTexture(texEnemy);
        SDL_DestroyTexture(texEnemy2);
        SDL_DestroyRenderer(render);
        
    
    
	finalizar();
    return 0;
}
//------------------------------------------------------------------------------------//
void texture_and_surfaces(){

}
int inicializar(){
    SDL_Init(SDL_INIT_EVERYTHING);

        if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
            printf("Deu merda para iniciar o SDL! SDL_Error: %s\n", SDL_GetError());
            SDL_Quit();
            return 1;
        }
        printf("Iniciou o SDL\n");
        Uint32 flagsCreateWindow = SDL_WINDOW_SHOWN | SDL_WINDOW_MOUSE_CAPTURE | SDL_WINDOW_RESIZABLE | SDL_RENDERER_PRESENTVSYNC /* | SDL_WINDOW_MAXIMIZED | SDL_WINDOW_FULLSCREEN_DESKTOP*/;
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
void startmusicMenu(){
    printf("Toca Menu\n");
    //salva musica para carregar/tocar depois
    Mix_Chunk *musicMenu = Mix_LoadWAV("recursos/musicMenu.mp3");
    if(musicMenu==NULL){
        printf("erro musicMenu: %s\n", Mix_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
    Mix_Volume(-1,MIX_MAX_VOLUME/2);
    Mix_PlayChannel(-1,musicMenu,-1);
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

