#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
//#include "customlib.h"


//VERSÃO
#define version "v0.1.01" 
/*
    gcc -o game game.c -lSDL2 -lSDL2_image -lSDL2_mixer -Wall 
-Wno-switch remove todos os warns relacionados ao Wswitch(n tem mais esse erro)
compila e abre se nao tiver erro
    gcc -o game game.c -lSDL2 -lSDL2_image -lSDL2_mixer -Wall && ./game
SDL_Image, SDL_Mixer, SDL_Ttf, SDL_Gfx
http://www.lazyfoo.net/tutorials/SDL/01_hello_SDL/index2.php
https://www.youtube.com/watch?v=yFLa3ln16w0
*/

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

int i;

//speed in pixels/second
#define SPEED 300

#define speedPlayer 10
#define FPS 23
#define framedelay 30
#define cutdelay 150
#define cutdelay2 500

#define timeToClose 180

#define mapx 7680
#define mapy 4320

#define nEnemies 4

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
Mix_Chunk startmusicMenu();


/*
SDL surface is merely a struct that represents image data in memory.
A texture corresponds to image data loaded into the graphic's hardware's memory.
So a surface is in regular memory, and a texture is in this separate VRAM.
*/

int main(int argc, char* args[]){
    int statusGame=0;
    int sobe, desce, esquerda,direita,atacar=0,lastside,statusRank=0,statusCreditos=0;
    int enemyMove;
    SDL_Surface *surfEnemies[nEnemies];
    SDL_Texture *texEnemies[nEnemies];
    SDL_Rect rectEnemies[nEnemies];
    SDL_Rect rectspriteEnemies[nEnemies];


    inicializar();  
    startRenderer();
    //PLAYER------------
    //carrega imagem na memoria do pc,provavelmente na RAM "mario.png"
    SDL_Surface *surfPlayer = IMG_Load("recursos/main_character_moves.png");
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
    //rectPlayer.x = (SCREEN_WIDTH/2)   + (rectPlayer.w/2);
    //rectPlayer.y = (SCREEN_HEIGHT /2) - (rectPlayer.h/2);
    rectPlayer.w = 60;
    rectPlayer.h = 86;
    rectPlayer.x = (SCREEN_WIDTH/2)   - (rectPlayer.w/2);
    rectPlayer.y = (SCREEN_HEIGHT /2) - (rectPlayer.h/2);

    
    SDL_Rect rectPlayerSprite = {56,72,28,36};
    
    // h/height altura
    // w/width largura

    //---------------------
    
    //BACKGROUND-----------
    SDL_Surface *surfBACKGROUND = IMG_Load("recursos/teste_mapa.png");
    SDL_Texture *texBACKGROUND = SDL_CreateTextureFromSurface(render, surfBACKGROUND);
    SDL_Rect rectBackground = {(SCREEN_WIDTH/2)   + (rectPlayer.w/2),(SCREEN_HEIGHT /2) - (rectPlayer.h/2),SCREEN_WIDTH,SCREEN_HEIGHT};

    //RANK-----------
    SDL_Surface *surfRANK = IMG_Load("recursos/RANKS.png");
    SDL_Texture *texRANK = SDL_CreateTextureFromSurface(render, surfRANK);
    
    //CREDITOS-----------
    SDL_Surface *surfCREDITS = IMG_Load("recursos/CREDITS.png");
    SDL_Texture *texCREDITS = SDL_CreateTextureFromSurface(render, surfCREDITS);

    //MENU-------------------
    SDL_Surface *surfMENU = IMG_Load("recursos/menu_without_coin.png");
    SDL_Texture *texMENU = SDL_CreateTextureFromSurface(render, surfMENU);
    //SDL_Rect rectMenu = {0,0,SCREEN_WIDTH,SCREEN_HEIGHT};

    //COIN-------------------
    SDL_Surface *surfCOIN = IMG_Load("recursos/coin.png");
    SDL_Texture *texCOIN = SDL_CreateTextureFromSurface(render, surfCOIN);
    SDL_Rect rectCOIN = {839,171,72,90};


    //------------------------
    //LOADING-----------------
    SDL_Surface *surfloading = IMG_Load("recursos/sprite_loading.png");
    SDL_Texture *texloading = SDL_CreateTextureFromSurface(render, surfloading);
    SDL_Rect rectloading = {0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
    SDL_Rect rectloadingrecorte = {0,0,960,540};

    //EA PLAN   -----------------
    SDL_Surface *surEA_PLAN = IMG_Load("recursos/ea_plan.png");
    SDL_Texture *texEA_PLAN = SDL_CreateTextureFromSurface(render, surEA_PLAN);
    SDL_Rect rectEA_PLAN = {0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
    SDL_Rect rectEA_PLANsprite = {0,0,SCREEN_WIDTH,SCREEN_HEIGHT};


    //SDL_Rect rectloading = {0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
    //---------------------
    //ENEMY1----------
    
    surfEnemies[0] = IMG_Load("recursos/green_enemy.png");
    texEnemies[0] = SDL_CreateTextureFromSurface(render, surfEnemies[0]);
    rectEnemies[0].x = 900; rectEnemies[0].y = 160; rectEnemies[0].w = 180; rectEnemies[0].h = 141;
    rectspriteEnemies[0].x = 0;rectspriteEnemies[0].y = 94; rectspriteEnemies[0].w = 60; rectspriteEnemies[0].h = 47;
    
    //-----------------
    //enemy2-----------
    surfEnemies[1] = IMG_Load("recursos/red_enemy.png");
    texEnemies[1] = SDL_CreateTextureFromSurface(render, surfEnemies[1]);
    rectEnemies[1].x = 300; rectEnemies[1].y = 200; rectEnemies[1].w = 180; rectEnemies[1].h = 141;
    rectspriteEnemies[1].x = 0;rectspriteEnemies[1].y = 94; rectspriteEnemies[1].w = 60; rectspriteEnemies[1].h = 47;

    
    //-----------------
    //ENEMY3----------
    
    surfEnemies[2] = IMG_Load("recursos/purple_enemy.png");
    texEnemies[2] = SDL_CreateTextureFromSurface(render, surfEnemies[2]);
    rectEnemies[2].x = 500; rectEnemies[2].y = 600; rectEnemies[2].w = 180; rectEnemies[2].h = 141;
    rectspriteEnemies[2].x = 0;rectspriteEnemies[2].y = 94; rectspriteEnemies[2].w = 60; rectspriteEnemies[2].h = 47;


    //enemy4-----------
    surfEnemies[3] = IMG_Load("recursos/top_enemy.png");
    texEnemies[3] = SDL_CreateTextureFromSurface(render, surfEnemies[3]);
    rectEnemies[3].x = 500; rectEnemies[3].y = 900; rectEnemies[3].w = 180; rectEnemies[3].h = 141;
    rectspriteEnemies[3].x = 0;rectspriteEnemies[3].y = 94; rectspriteEnemies[3].w = 60; rectspriteEnemies[3].h = 47; 
    //-----------------


    //CUTSCENE
    SDL_Surface *surfScene = IMG_Load("recursos/8xCUTSCENE.png");
    SDL_Texture *texScene = SDL_CreateTextureFromSurface(render, surfScene);
    SDL_Rect rectScene = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_Rect recorte = { 0 , 0, 258, 146};
    //----------------

    //GAME-STORY------

    SDL_Surface *surfStory = IMG_Load("recursos/game_story.png");
    SDL_Texture *texStory = SDL_CreateTextureFromSurface(render, surfStory);
    SDL_Rect rectStory = {0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
    
    //--------------
    
    SDL_Rect rectPlayerPosMap = { rectPlayer.x-rectBackground.x, rectPlayer.y-rectBackground.y, rectPlayer.w, rectPlayer.h};
    
    //set to 1 when window close button is pressed
    int close_requested = 0;
    int menuOption = 0;
        
    uint32_t framestart;
    uint32_t frameTime,frameTimeSprite,frameTimeSpriteAtack;
    int startMMenu=0,startMCutscene=0;
    startAudio();
    Mix_Chunk *bomba = Mix_LoadWAV("recursos/bomba.mp3");
    Mix_VolumeChunk(bomba,MIX_MAX_VOLUME/4);
    Mix_Chunk *teclado = Mix_LoadWAV("recursos/teclado.mp3");
    Mix_VolumeChunk(teclado,MIX_MAX_VOLUME);
    //animation loop
    printf("Iniciou o JOGO\n");
    time_t timeStart= time(NULL);
    time_t EAPLAN,timePresent;
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
                            case SDLK_UP:
                                if(rectCOIN.y==171)
                                    rectCOIN.y=539;                                
                                else if(rectCOIN.y==291)
                                    rectCOIN.y=171;                                
                                else if(rectCOIN.y==410)
                                    rectCOIN.y=291;
                                else if(rectCOIN.y==539)
                                    rectCOIN.y=410;
                                break;
                            case SDLK_DOWN:
                                if(rectCOIN.y==171)
                                    rectCOIN.y=291;                                
                                else if(rectCOIN.y==291)
                                    rectCOIN.y=410;                                
                                else if(rectCOIN.y==410)
                                    rectCOIN.y=539;
                                else if(rectCOIN.y==539)
                                    rectCOIN.y=171;
                                break;
                            case SDLK_RETURN2:
                                if(rectCOIN.y==171){
                                    Mix_FadeOutChannel(-1, 1000);
                                    printf("Inicia CUTSCENE\n");
                                    SDL_Delay(700);
                                    statusGame=4;
                                }
                                    
                                else if(rectCOIN.y==291){
                                    statusRank=1;
                                }
                                                         
                                else if(rectCOIN.y==410){
                                    statusCreditos=1;
                                }
                                
                                else if(rectCOIN.y==539){
                                    printf("Fechar JOGO\n");
                                    close_requested = 1;
                                    break;
                                }
                                break;
                            case SDLK_ESCAPE:
                                statusRank=0;
                                statusCreditos=0;
                                break;
                            case SDLK_RETURN:
                                if(rectCOIN.y==171){
                                    Mix_FadeOutChannel(-1, 1000);
                                    printf("Inicia CUTSCENE\n");
                                    SDL_Delay(700);
                                    statusGame=4;
                                }
                                    
                                else if(rectCOIN.y==291){
                                    statusRank=1;
                                }
                                                         
                                else if(rectCOIN.y==410){
                                    statusCreditos=1;
                                }
                                
                                else if(rectCOIN.y==539){
                                    printf("Fechar JOGO\n");
                                    close_requested = 4;
                                    break;
                                }
                                
                                break;
                            
                            case SDLK_END:
                                printf("Fechar JOGO\n");
                                //SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Obrigado", "Obrigado por não jogar o jogo", window);
                                close_requested = 1;
                                break;
                            case SDLK_0:
                                Mix_FadeOutChannel(-1, 1000);
                                Mix_CloseAudio();
                                startAudio();
                                statusGame=5;
                                break;
                        }
                    break;           
                        }
            }




            frameTime = SDL_GetTicks() - framestart;
            SDL_RenderClear(render);
            SDL_RenderCopy(render, texMENU, NULL, NULL);
            SDL_RenderCopy(render,texCOIN, NULL,&rectCOIN);
            if(statusRank==1&&statusCreditos==0)
                SDL_RenderCopy(render, texRANK, NULL, NULL);
            if(statusCreditos==1&&statusRank==0)
                SDL_RenderCopy(render, texCREDITS, NULL, NULL);
            SDL_RenderPresent(render);
            if(framedelay > frameTime){
                SDL_Delay((framedelay) - frameTime);
            }
        }
        else if(statusGame==4){
            if(rectStory.x<=2560){
                SDL_RenderClear(render);
                SDL_RenderCopy(render, texStory, &rectStory, NULL);
                SDL_RenderPresent(render);
                rectStory.x += 1280;
                SDL_Delay(4000);
            }
            else{
                SDL_Delay(700);
                statusGame=1;
            }
        }
        
        
        
        //CUTSCENE
        else if(statusGame==1){
            
            framestart = SDL_GetTicks();
            if(startMCutscene==0){
                Mix_PlayChannel(4,teclado,1);
                ++startMCutscene;
            }
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
                if(recorte.x >= 4902 && recorte.y == 146){
                    if(startMCutscene==1){
                        Mix_FadeOutChannel(4,1);
                        Mix_PlayChannel(1,bomba,1);
                        startMCutscene++;
                    }
                }
                if(recorte.x >= 7998 && recorte.y == 146){
                    Mix_FadeOutChannel(-1, 1000);
                    Mix_CloseAudio();
                    startAudio();
                    //SDL_Delay(500);
                    printf("Inicia JOGO\n");
                    statusGame = 2;
                }
            
            
            
            frameTime = SDL_GetTicks() - framestart;
            SDL_RenderClear(render);
            SDL_RenderCopy(render, texScene, &recorte, &rectScene);
            SDL_RenderPresent(render);
            if(framedelay > frameTime){
                SDL_Delay((cutdelay) - frameTime);
            }
        }

        else if(statusGame==2){
                framestart = SDL_GetTicks();
                if(rectloadingrecorte.x<=6720)
                    rectloadingrecorte.x+=960;
                else if(rectloadingrecorte.x>6720&&rectloadingrecorte.y==0){
                    rectloadingrecorte.y=540;
                    rectloadingrecorte.x=0;
                }
                SDL_RenderClear(render);
                SDL_RenderCopy(render, texloading, &rectloadingrecorte, &rectloading);
                SDL_RenderPresent(render);
                if(1000 /*fps */ > frameTime){
                    SDL_Delay((cutdelay2) - frameTime);
                }
                
                if(rectloadingrecorte.x > 6720 && rectloadingrecorte.y==540){
                    statusGame=3;
                    EAPLAN=time(NULL);
                }
                    
        }

        else if(statusGame==3){
            timePresent=time(NULL);
            //printf("tempo atual %ld ",timePresent-EAPLAN);
            if(timePresent-EAPLAN>=timeToClose){
                statusGame=5;
            }
            //SDL_Rect rectPlayerPosMap = { rectPlayer.x-rectBackground.x, rectPlayer.y-rectBackground.y, rectPlayer.w, rectPlayer.h};
            //printf("X GLOBAL: %d, Y GLOBAL: %d\n", rectPlayerPosMap.x,rectPlayerPosMap.y);
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
                            case SDLK_j:
                                statusGame=5;
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
                }
                }
                //fim event loop
                //colisao com "janela"
                if(rectBackground.y + rectBackground.h + speedPlayer >= mapy){
                    desce=0;
                }
                if(rectBackground.y - speedPlayer <= 0 ){
                    sobe=0;
                }
                if(rectBackground.x + rectBackground.w + speedPlayer >= mapx){
                    direita=0;
                }
                if(rectBackground.x - speedPlayer <= 0){
                    esquerda=0;
                }
                
                
                //fim colisao com janela
                //colisao player com outro rect
                

                //fim colisao de player com outro rect


                //fim colisao rect
                //locomocao/sprite player
                frameTimeSprite = SDL_GetTicks() - framestart;
                if(sobe==1){
                    for(i=0;i<nEnemies;i++){
                        rectEnemies[i].y += speedPlayer;
                    }
                    //rectPlayer.y -=speedPlayer;
                    rectPlayerPosMap.y -=speedPlayer;
                    rectBackground.y -=speedPlayer;
                    rectPlayerSprite.y = 0;
                    if(rectPlayerSprite.x <=56){
                        rectPlayerSprite.x += 28;
                    }
                    if(rectPlayerSprite.x>56){
                        rectPlayerSprite.x = 0; 
                    }
                    if(framedelay > frameTime){
                        SDL_Delay((framedelay) - frameTimeSprite);
                    }
                }
                else if(desce==1){
                    for(i=0;i<nEnemies;i++){
                        rectEnemies[i].y -= speedPlayer;
                    }
                    //rectPlayer.y +=speedPlayer;
                    rectPlayerPosMap.y +=speedPlayer;
                    rectBackground.y +=speedPlayer;
                    rectPlayerSprite.y = 72;
                    if(rectPlayerSprite.x <=56){
                        rectPlayerSprite.x += 28;
                    }
                    if(rectPlayerSprite.x>56){
                        rectPlayerSprite.x = 0; 
                    }
                    if(framedelay > frameTime){
                        SDL_Delay((framedelay) - frameTimeSprite);
                    }
                }
                else if(esquerda==1){
                    for(i=0;i<nEnemies;i++){
                        rectEnemies[i].x += speedPlayer;
                    }
                    //rectPlayer.x -=speedPlayer;
                    rectPlayerPosMap.x -=speedPlayer;
                    rectBackground.x -=speedPlayer;
                    rectPlayerSprite.y = 108;
                    if(rectPlayerSprite.x <=56){
                        rectPlayerSprite.x += 28;
                    }
                    if(rectPlayerSprite.x>56){
                        rectPlayerSprite.x = 0; 
                    }
                    if(framedelay > frameTime){
                        SDL_Delay((framedelay) - frameTimeSprite);
                    }
                }
                else if(direita==1){
                    for(i=0;i<nEnemies;i++){
                        rectEnemies[i].x -= speedPlayer;
                    }
                    //rectPlayer.x +=speedPlayer;
                    rectPlayerPosMap.x +=speedPlayer;
                    rectBackground.x +=speedPlayer;
                    rectPlayerSprite.y = 36;
                    if(rectPlayerSprite.x <=56){
                        rectPlayerSprite.x += 28;
                    }
                    if(rectPlayerSprite.x>56){
                        rectPlayerSprite.x = 0; 
                    }
                    if(framedelay > frameTime){
                        SDL_Delay((framedelay) - frameTimeSprite);
                    }
                }
                //fim locomocao



                //inicio enemy move, 50% andar 50% parado
                for(i=0;i<nEnemies;i++){
                    enemyMove = rand() % 32+1;
                    if(enemyMove==1){
                        //direita
                        rectEnemies[i].x +=speedPlayer;
                        rectspriteEnemies[i].y = 141;
                        if(rectspriteEnemies[i].x <=180){
                            rectspriteEnemies[i].x += 60;
                        }
                        if(rectspriteEnemies[i].x>180){
                            rectspriteEnemies[i].x = 0; 
                        }
                        if(framedelay > frameTime){
                            SDL_Delay((framedelay) - frameTimeSprite);
                        }
                    }
                    if(enemyMove==2){
                        //esquerda
                        rectEnemies[i].x -=speedPlayer;

                        rectspriteEnemies[i].y = 47;
                        if(rectspriteEnemies[i].x <=180){
                            rectspriteEnemies[i].x += 60;
                        }
                        if(rectspriteEnemies[i].x>180){
                            rectspriteEnemies[i].x = 0; 
                        }
                        if(framedelay > frameTime){
                            SDL_Delay((framedelay) - frameTimeSprite);
                        }
                    }
                    if(enemyMove==3){
                        //desce
                        rectEnemies[i].y +=speedPlayer;
                        rectspriteEnemies[i].y = 94;
                        if(rectspriteEnemies[i].x <=180){
                            rectspriteEnemies[i].x += 60;
                        }
                        if(rectspriteEnemies[i].x>180){
                            rectspriteEnemies[i].x = 0; 
                        }
                        if(framedelay > frameTime){
                            SDL_Delay((framedelay) - frameTimeSprite);
                        }
                        
                    }
                    if(enemyMove==4){
                        //sobe
                        rectEnemies[i].y -=speedPlayer;
                        rectspriteEnemies[i].y = 0;
                        if(rectspriteEnemies[i].x <=180){
                            rectspriteEnemies[i].x += 60;
                        }
                        if(rectspriteEnemies[i].x>180){
                            rectspriteEnemies[i].x = 0; 
                        }
                        if(framedelay > frameTime){
                            SDL_Delay((framedelay) - frameTimeSprite);
                        }
                        
                        
                    }
                }
                //--------------------------------------

                






                frameTime = SDL_GetTicks() - framestart;
                SDL_RenderClear(render);
                SDL_RenderCopy(render, texBACKGROUND, &rectBackground, NULL);
                SDL_RenderCopy(render, texturePlayer, &rectPlayerSprite, &rectPlayer);
                for(i=0;i<nEnemies;i++){
                    SDL_RenderCopy(render, texEnemies[i], &rectspriteEnemies[i], &rectEnemies[i]);
                }
                SDL_RenderPresent(render);
                if(framedelay > frameTime){
                    SDL_Delay((framedelay) - frameTime);
                }
                
                    

                 
    
        }
        
        else if(statusGame==5){
            framestart = SDL_GetTicks();
            while (SDL_PollEvent(&evento2)){
                switch (evento2.type){
                    case SDL_QUIT:
                        close_requested = 1;
                    break;
                    
                    case SDL_KEYDOWN:
                    //pressiona wasd ou setas para andar
                        switch (evento2.key.keysym.sym){
                            case SDLK_SPACE:
                                if(rectEA_PLANsprite.x==0){
                                    rectEA_PLANsprite.x=1280;
                                    SDL_Delay(1000);
                                }
                                else if(rectEA_PLANsprite.x==1280){
                                    close_requested = 1;
                                }
                                break;
                        }
                    break;                   
                    
                }
            }



            frameTime = SDL_GetTicks() - framestart;
            SDL_RenderClear(render);
            SDL_RenderCopy(render, texEA_PLAN, &rectEA_PLANsprite, &rectEA_PLAN);
            SDL_RenderPresent(render);
            if(framedelay > frameTime){
                SDL_Delay((framedelay) - frameTime);
            }
        }
        
    }
        for(i=0;i<nEnemies;i++){
            SDL_FreeSurface(surfEnemies[i]);
            SDL_DestroyTexture(texEnemies[i]);
        }
        SDL_FreeSurface(surfScene);
        SDL_DestroyTexture(texScene);
        SDL_FreeSurface(surEA_PLAN);
        SDL_DestroyTexture(texEA_PLAN);
        SDL_FreeSurface(surfloading);
        SDL_DestroyTexture(texloading);
        SDL_FreeSurface(surfCOIN);
        SDL_DestroyTexture(texCOIN);
        SDL_FreeSurface(surfMENU);
        SDL_DestroyTexture(texMENU);
        SDL_FreeSurface(surfCREDITS);
        SDL_DestroyTexture(texCREDITS);
        SDL_FreeSurface(surfRANK);
        SDL_DestroyTexture(texRANK);
        SDL_FreeSurface(surfPlayer);
        SDL_DestroyTexture(texturePlayer);
        SDL_FreeSurface(surfBACKGROUND);
        SDL_DestroyTexture(texBACKGROUND);
        SDL_DestroyRenderer(render);
    
    time_t timeStop= time(NULL);
    int timeOpened = difftime(timeStop,timeStart);
    char tempo[50];
    sprintf(tempo,"Você jogou por %d segundos",timeOpened);
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, version, tempo,  window);
    //printf("FICOU ABERTO %d segundos\n", timeOpened);
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
        Uint32 flagsCreateWindow = SDL_WINDOW_SHOWN | SDL_RENDERER_PRESENTVSYNC /* | SDL_WINDOW_MAXIMIZED*/ | SDL_WINDOW_FULLSCREEN_DESKTOP;
        window = SDL_CreateWindow(version, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, flagsCreateWindow );
        if(window == NULL){
            printf("Deu merda na janela! SDL_Error: %s\n", SDL_GetError());
            SDL_Quit();
            return 1;
        }
        SDL_Surface *surfIcon = IMG_Load("recursos/icon.png");
        SDL_SetWindowIcon(window,surfIcon);
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

int startAudio(){
    //inicia "driver" de audio
    printf("Iniciou AUDIO\n");
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 5, 1024);
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024)==-1) {
        printf("Merda no Mix_OpenAudio: %s\n", Mix_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    return 0;
}
Mix_Chunk startmusicMenu(){
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
    return *musicMenu;
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

