#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_ttf.h"
//#include "customlib.h"


//VERSÃO
#define version "v0.1.3" 
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
double SCREEN_WIDTH = 1280;
//altura 480
float SCREEN_HEIGHT = 720;
//escala 16:9
double ESCALA = 16.f/9.f;

int LEVEL_WIDTH = 1280;
int LEVEL_HEIGHT = 720;

int i,j,k;

//speed in pixels/second
#define SPEED 300

double speedPlayer = 10;
#define FPS 23
#define framedelay 30
#define cutdelay 150
#define cutdelay2 500

#define mapx 7680
#define mapy 4320

#define nEnemies 5
#define nBalao1 0
#define nBalao2 1

//variaveis globais
SDL_Window *window = NULL;
SDL_Renderer *render = NULL;
SDL_Surface *surfPlayer = NULL;
SDL_Texture *texturePlayer = NULL;

//nem que seja só para o player
typedef struct{
    int life;
    int posX;
    int posY;
    int posXSprite;
    int posYSprite;
    SDL_Surface *surfEntity;
    SDL_Texture *texEntity;
    SDL_Rect ;
}entityStats;



int inicializar();
int finalizar();
void nobreu();
int startAudio();
int startRenderer();
int colisao(SDL_Rect,SDL_Rect);
SDL_Rect matarEnemy(SDL_Rect*,int );
Mix_Chunk startmusicMenu();


/*
SDL surface is merely a struct that represents image data in memory.
A texture corresponds to image data loaded into the graphic's hardware's memory.
So a surface is in regular memory, and a texture is in this separate VRAM.
*/

int main(int argc, char* args[]){
    int statusGame=0,mageStatus=0;
    int sobe=false, desce=false, esquerda=false,direita=false,atacar=0,lastside,statusRank=0,statusCreditos=0;
    int enemyMove=-1,enemyMoveMinotaur=-1,minotaurAtack=false, falar_npc=false;
    SDL_Surface *surfEnemies[nEnemies];
    SDL_Texture *texEnemies[nEnemies];
    SDL_Rect rectEnemies[nEnemies];
    SDL_Rect rectspriteEnemies[nEnemies];
    SDL_Surface *surfBALAO_01[1];
    SDL_Texture *texBALAO_01[1];
    SDL_Surface *surfBALAO_02[1];
    SDL_Texture *texBALAO_02[1];
    SDL_Rect rectBALAO_01[1];
    SDL_Rect rectBALAO_02[1];
    int enemyColision[nEnemies][4];
    for(i=0;i<nEnemies;i++){
        enemyColision[i][0]=false;
        enemyColision[i][1]=false;
        enemyColision[i][2]=false;
        enemyColision[i][3]=false;
    }

    SDL_StartTextInput();
    inicializar();  
    startRenderer();
    //PLAYER------------
    //carrega imagem na memoria do pc,provavelmente na RAM "mario.png"
    SDL_Surface *surfPlayer = IMG_Load("recursos/player_new.png");
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
    rectPlayer.w = 110;
    rectPlayer.h = 110;
    rectPlayer.x = (SCREEN_WIDTH/2)   - (rectPlayer.w/2);
    rectPlayer.y = (SCREEN_HEIGHT /2) - (rectPlayer.h/2);
    SDL_Rect rectPlayerSprite = {0,0,112,112};
    
    // h/height altura
    // w/width largura
    //---------------------
    //MAGE START GAME
    SDL_Surface *surfMAGE_START_Game = IMG_Load("recursos/mago_f_start.png");
    SDL_Texture *texMAGE_START_Game = SDL_CreateTextureFromSurface(render, surfMAGE_START_Game);
    SDL_Rect rectMAGE_START_Game = {(SCREEN_WIDTH/2)+ (rectPlayer.w/2)+50,(SCREEN_HEIGHT /2) - (rectPlayer.h/2)-50,128,172};
    SDL_Rect rectMAGE_START_GameSprite = {0,0,32,43};

    //---------------------
    
    //balao 01
    surfBALAO_01[0] = IMG_Load("recursos/balao_1.png");
    texBALAO_01[0] = SDL_CreateTextureFromSurface(render, surfBALAO_01[0]);
    /* ver posicao do balao
    rectBALAO_01[0].x = rectMAGE_START_Game.x + (rectMAGE_START_Game.w/4);
    rectBALAO_01[0].y = rectMAGE_START_Game.y - 172 ;
    rectBALAO_01[0].w = 64;
    rectBALAO_01[0].h = 86;
     */

    //---------------------

    //balao 02 do mago start
    surfBALAO_02[0] = IMG_Load("recursos/balao_2.png");
    texBALAO_02[0] = SDL_CreateTextureFromSurface(render, surfBALAO_02[0]);
    rectBALAO_02[0].x = rectMAGE_START_Game.x + (rectMAGE_START_Game.w/3);
    rectBALAO_02[0].y = rectMAGE_START_Game.y - 86 ;
    rectBALAO_02[0].w = 64;
    rectBALAO_02[0].h = 86;

    //---------------------

    //BACKGROUND-----------
    SDL_Surface *surfBACKGROUND = IMG_Load("recursos/01_el_mapa.png");
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
    rectEnemies[1].x = 300; rectEnemies[1].y = 350; rectEnemies[1].w = 180; rectEnemies[1].h = 141;
    rectspriteEnemies[1].x = 0;rectspriteEnemies[1].y = 94; rectspriteEnemies[1].w = 60; rectspriteEnemies[1].h = 47;

    
    //-----------------
    //ENEMY3----------
    
    surfEnemies[2] = IMG_Load("recursos/purple_enemy.png");
    texEnemies[2] = SDL_CreateTextureFromSurface(render, surfEnemies[2]);
    rectEnemies[2].x = 500; rectEnemies[2].y = 1200; rectEnemies[2].w = 180; rectEnemies[2].h = 141;
    rectspriteEnemies[2].x = 0;rectspriteEnemies[2].y = 94; rectspriteEnemies[2].w = 60; rectspriteEnemies[2].h = 47;


    //enemy4-----------
    surfEnemies[3] = IMG_Load("recursos/top_enemy.png");
    texEnemies[3] = SDL_CreateTextureFromSurface(render, surfEnemies[3]);
    rectEnemies[3].x = 250; rectEnemies[3].y = 900; rectEnemies[3].w = 180; rectEnemies[3].h = 141;
    rectspriteEnemies[3].x = 0;rectspriteEnemies[3].y = 94; rectspriteEnemies[3].w = 60; rectspriteEnemies[3].h = 47; 
    //-----------------

    //enemy5-----------
    surfEnemies[4] = IMG_Load("recursos/minotauro.png");
    texEnemies[4] = SDL_CreateTextureFromSurface(render, surfEnemies[4]);
    rectEnemies[4].x = 1500; rectEnemies[4].y = 570; rectEnemies[4].w = 400; rectEnemies[4].h = 370;
    rectspriteEnemies[4].x = 8;rectspriteEnemies[4].y = 154; rectspriteEnemies[4].w = 40; rectspriteEnemies[4].h = 37;
    //50++
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
    //nobreu();
    Mix_Chunk *bomba = Mix_LoadWAV("recursos/bomba.mp3");
    Mix_VolumeChunk(bomba,MIX_MAX_VOLUME/4);
    Mix_Chunk *teclado = Mix_LoadWAV("recursos/teclado.mp3");
    Mix_VolumeChunk(teclado,MIX_MAX_VOLUME);
    //animation loop
    printf("Iniciou o JOGO\n");
    time_t timeStart= time(NULL);
    time_t EAPLAN,timePresent;
    while (close_requested == false){
        
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
                        close_requested = true;
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
                                    statusRank=true;
                                }
                                                         
                                else if(rectCOIN.y==410){
                                    statusCreditos=true;
                                }
                                
                                else if(rectCOIN.y==539){
                                    printf("Fechar JOGO\n");
                                    close_requested = true;
                                    break;
                                }
                                break;
                            case SDLK_ESCAPE:
                                statusRank=false;
                                statusCreditos=false;
                                break;
                            case SDLK_RETURN:
                                if(rectCOIN.y==171){
                                    Mix_FadeOutChannel(-1, 1000);
                                    printf("Inicia CUTSCENE\n");
                                    SDL_Delay(700);
                                    statusGame=4;
                                }
                                    
                                else if(rectCOIN.y==291){
                                    statusRank=true;
                                }
                                                         
                                else if(rectCOIN.y==410){
                                    statusCreditos=true;
                                }
                                
                                else if(rectCOIN.y==539){
                                    printf("Fechar JOGO\n");
                                    close_requested = true;
                                    break;
                                }
                                
                                break;
                            
                            case SDLK_END:
                                printf("Fechar JOGO\n");
                                //SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Obrigado", "Obrigado por não jogar o jogo", window);
                                close_requested = true;
                                break;
                            case SDLK_0:
                                Mix_FadeOutChannel(-1, 1000);
                                statusGame=3;
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
            //timePresent=time(NULL);
            //printf("tempo atual %ld ",timePresent-EAPLAN);
            if(timePresent-EAPLAN>=45){
                //statusGame=5;
            }
            //SDL_Rect rectPlayerPosMap = { rectPlayer.x-rectBackground.x, rectPlayer.y-rectBackground.y, rectPlayer.w, rectPlayer.h};
            //printf("X GLOBAL: %d, Y GLOBAL: %d\n", rectPlayerPosMap.x,rectPlayerPosMap.y);
            framestart = SDL_GetTicks();
            while (SDL_PollEvent(&evento)){                
                switch (evento.type){
                    case SDL_QUIT:
                        close_requested = true;
                    break;
                    
                    case SDL_KEYDOWN:
                    //pressiona wasd ou setas para andar
                        switch (evento.key.keysym.sym){
                            case SDLK_UP:
                                sobe=true;
                                lastside=0;
                                atacar=false;
                                break;
                            case SDLK_LEFT:
                                esquerda=true;
                                lastside=3;
                                atacar=false;
                                break;
                            case SDLK_DOWN:
                                desce=true;
                                lastside=1;
                                atacar=false;
                                break;
                            case SDLK_RIGHT:
                                direita=true;
                                lastside=2;
                                atacar=false;
                                break;
                            case SDLK_SPACE:
                                atacar=1;
                                break;
                            case SDLK_RETURN:
                                falar_npc=true;
                                break;
                            case SDLK_END:
                                close_requested = true;
                                break;
                        }
                    break;
                    
                    case SDL_KEYUP:
                        switch (evento.key.keysym.sym){
                            case SDLK_UP:
                                sobe=false;
                                break;
                            case SDLK_LEFT:
                                esquerda=false;
                                break;
                            case SDLK_DOWN:
                                desce=false;
                                break;
                            case SDLK_RIGHT:
                                direita=false;
                                break;
                            case SDLK_RETURN:
                                falar_npc=false;
                                break;
                        }
                        break;
                    }
                }
                //fim event loop
                //colisao com "janela"
                if(rectBackground.y + rectBackground.h + speedPlayer >= mapy){
                    desce=false;
                }
                if(rectBackground.y - speedPlayer <= 0 ){
                    sobe=false;
                }
                if(rectBackground.x + rectBackground.w + speedPlayer >= mapx){
                    direita=false;
                }
                if(rectBackground.x - speedPlayer <= 0){
                    esquerda=false;
                }
                
                
                //fim colisao com janela
                //colisao player com outros inimigos
                for(i=0;i<nEnemies;i++){
                    if(colisao(rectPlayer, rectEnemies[i])==0){
                        sobe=false;
                    }
                    if(colisao(rectPlayer, rectEnemies[i])==1){
                        desce=false;
                    }
                    if(colisao(rectPlayer, rectEnemies[i])==2){
                        direita=false;
                    }
                    if(colisao(rectPlayer, rectEnemies[i])==3){
                        esquerda=false;
                    }
                    if((colisao(rectPlayer,rectMAGE_START_Game)==0||colisao(rectPlayer,rectMAGE_START_Game)==1||colisao(rectPlayer,rectMAGE_START_Game)==2||colisao(rectPlayer,rectMAGE_START_Game)==3)){
                        if(mageStatus==0 && falar_npc==true){
                            rectMAGE_START_GameSprite.x = 32;
                            mageStatus=1;
                            rectBALAO_02[0].w = 0;
                            rectBALAO_02[0].h = 0;
                        }
                        else if(mageStatus==1 && falar_npc==true){

                            //avancar fala
                            
                            
                        }
                        /*
                        else if(terminou fala)
                        fora desse loop de colisao
                            if(rectMAGE_START_GameSprite.x >= 32){
                                rectMAGE_START_GameSprite.x += 32;
                                rectMAGE_START_Game.x += speedPlayer*2;
                            }

                        */

                    }
                    //sumir/matar com inimigos
                    if(colisao(rectPlayer, rectEnemies[i])!=-1 && atacar==true){
                        matarEnemy(&rectEnemies[i],i);
                        
                    }
                }
                //-------------------------
                //colisao inimigos com eles mesmos
                for(i=0;i<nEnemies;i++)
                    for(j=0;j<nEnemies;j++)
                        if(j!=i){
                            
                            if(colisao(rectEnemies[i],rectEnemies[j])==0)
                                enemyColision[i][0]==true;
                            if(colisao(rectEnemies[i],rectEnemies[j])==1)
                                enemyColision[i][1]==true;
                            if(colisao(rectEnemies[i],rectEnemies[j])==2)
                                enemyColision[i][2]==true;
                            if(colisao(rectEnemies[i],rectEnemies[j])==3)
                                enemyColision[i][3]==true;
                            
                        }
                //-----------------------------
                //colisao inimigos com player
                for(i=0;i<nEnemies;i++){
                    if(colisao(rectEnemies[i],rectPlayer)==0)
                        enemyColision[i][0]==true;
                    if(colisao(rectEnemies[i],rectPlayer)==1)
                        enemyColision[i][1]==true;
                    if(colisao(rectEnemies[i],rectPlayer)==2)
                        enemyColision[i][2]==true;
                    if(colisao(rectEnemies[i],rectPlayer)==3)
                        enemyColision[i][3]==true;                            
                }
                //-------------
                




                //fim colisao rect

                /*
                last side
                sobe = 0
                desce = 1
                direita = 2
                esquerda = 3
                */
                //inicio atacar player
                frameTimeSpriteAtack = SDL_GetTicks() - framestart;
                //sobe
                if(atacar==true){
                    //sobe
                    if(lastside==0){
                        rectPlayerSprite.y = 784;
                        if(rectPlayerSprite.x <=224){
                            rectPlayerSprite.x += 112;
                        }
                        if(rectPlayerSprite.x>224){
                            rectPlayerSprite.x = 0;
                            rectPlayerSprite.y = 336;
                            atacar=0;
                        }
                        if(framedelay > frameTimeSpriteAtack){
                            //SDL_Delay((framedelay) - frameTimeSprite);
                        }
                    }
                    //desce
                    else if(lastside==1){
                        rectPlayerSprite.y = 448;
                        if(rectPlayerSprite.x <=224){
                            rectPlayerSprite.x += 112;
                        }
                        if(rectPlayerSprite.x>224){
                            rectPlayerSprite.x = 0;
                            rectPlayerSprite.y = 0;
                            atacar=0;
                        }
                        if(framedelay > frameTimeSpriteAtack){
                            //SDL_Delay((framedelay) - frameTimeSprite);
                        }
                    }
                    //esquerda
                    else if(lastside==3){
                        rectPlayerSprite.y = 672;
                        if(rectPlayerSprite.x <=224){
                            rectPlayerSprite.x += 112;
                        }
                        if(rectPlayerSprite.x>224){
                            rectPlayerSprite.x = 0;
                            rectPlayerSprite.y = 112;
                            atacar=0;
                        }
                        if(framedelay > frameTimeSpriteAtack){
                            //SDL_Delay((framedelay) - frameTimeSprite);
                        }
                    }
                    //direita
                    else if(lastside==2){
                        rectPlayerSprite.y = 560;
                        if(rectPlayerSprite.x<=224){
                            rectPlayerSprite.x += 112;
                        }
                        if(rectPlayerSprite.x>224){
                            rectPlayerSprite.x = 0;
                            rectPlayerSprite.y = 224;
                            atacar=0;
                        }
                        if(framedelay > frameTimeSpriteAtack){
                            //SDL_Delay((framedelay) - frameTimeSprite);
                        }
                    }
                }
                //fim atacar player


                //locomocao/sprite player
                frameTimeSprite = SDL_GetTicks() - framestart;
                if(sobe==true){
                    for(i=0;i<nEnemies;i++){
                        rectEnemies[i].y += speedPlayer;
                    }
                    rectPlayerPosMap.y -=speedPlayer;
                    rectBackground.y -=speedPlayer;
                    rectPlayerSprite.y = 336;
                    if(rectPlayerSprite.x <=448){
                        rectPlayerSprite.x += 112;
                    }
                    if(rectPlayerSprite.x>448){
                        rectPlayerSprite.x = 0; 
                    }
                    if(framedelay > frameTime){
                        SDL_Delay((framedelay) - frameTimeSprite);
                    }
                    rectMAGE_START_Game.y += speedPlayer;
                    for(i=0;i<nBalao1;i++){
                        rectBALAO_01[i].y += speedPlayer;
                    }
                    for(i=0;i<nBalao2;i++){
                        rectBALAO_02[i].y += speedPlayer;
                    }
                }
                else if(desce==true){
                    for(i=0;i<nEnemies;i++){
                        rectEnemies[i].y -= speedPlayer;
                    }
                    rectPlayerPosMap.y +=speedPlayer;
                    rectBackground.y +=speedPlayer;
                    rectPlayerSprite.y = 0;
                    if(rectPlayerSprite.x <=448){
                        rectPlayerSprite.x += 112;
                    }
                    if(rectPlayerSprite.x>448){
                        rectPlayerSprite.x = 0; 
                    }
                    if(framedelay > frameTime){
                        SDL_Delay((framedelay) - frameTimeSprite);
                    }
                    rectMAGE_START_Game.y -= speedPlayer;
                    for(i=0;i<nBalao1;i++){
                        rectBALAO_01[i].y -= speedPlayer;
                    }
                    for(i=0;i<nBalao2;i++){
                        rectBALAO_02[i].y -= speedPlayer;
                    }
                }
                else if(esquerda==true){
                    for(i=0;i<nEnemies;i++){
                        rectEnemies[i].x += speedPlayer;
                    }
                    rectPlayerPosMap.x -=speedPlayer;
                    rectBackground.x -=speedPlayer;
                    rectPlayerSprite.y = 112;
                    if(rectPlayerSprite.x <=560){
                        rectPlayerSprite.x += 112;
                    }
                    if(rectPlayerSprite.x>560){
                        rectPlayerSprite.x = 0; 
                    }
                    if(framedelay > frameTime){
                        SDL_Delay((framedelay) - frameTimeSprite);
                    }
                    rectMAGE_START_Game.x += speedPlayer;
                    for(i=0;i<nBalao1;i++){
                        rectBALAO_01[i].x += speedPlayer;
                    }
                    for(i=0;i<nBalao2;i++){
                        rectBALAO_02[i].x += speedPlayer;
                    }
                }
                else if(direita==true){
                    for(i=0;i<nEnemies;i++){
                        rectEnemies[i].x -= speedPlayer;
                    }
                    rectPlayerPosMap.x +=speedPlayer;
                    rectBackground.x +=speedPlayer;
                    rectPlayerSprite.y = 224;
                    if(rectPlayerSprite.x <=560){
                        rectPlayerSprite.x += 112;
                    }
                    if(rectPlayerSprite.x>560){
                        rectPlayerSprite.x = 0; 
                    }
                    if(framedelay > frameTime){
                        SDL_Delay((framedelay) - frameTimeSprite);
                    }
                    rectMAGE_START_Game.x -= speedPlayer;
                    for(i=0;i<nBalao1;i++){
                        rectBALAO_01[i].x -= speedPlayer;
                    }
                    for(i=0;i<nBalao2;i++){
                        rectBALAO_02[i].x -= speedPlayer;
                    }
                }
                //fim locomocao
                
                


                //inicio enemy move, 50% andar 50% parado
                for(i=0;i<nEnemies;i++){
                    //enemyMove = rand() % 32+1;
                    if(i<4){
                        if(enemyMove==1 && enemyColision[i][2]==false){
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
                        if(enemyMove==2 && enemyColision[i][3]==false){
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
                        if(enemyMove==3 && enemyColision[i][1]==false){
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
                        if(enemyMove==4 && enemyColision[i][0]==false){
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
                    if(i==4){
                            enemyMoveMinotaur = rand() % 32+1;
                            if(enemyMove<=4 || minotaurAtack==true){
                                if(rectspriteEnemies[4].x<=432){
                                    rectspriteEnemies[4].x += 47;
                                    minotaurAtack=true;
                                }
                                if(rectspriteEnemies[4].x>432){
                                    rectspriteEnemies[4].x=0;
                                    minotaurAtack=false;
                                }
                                if(framedelay > frameTime){
                                    SDL_Delay((15) - frameTimeSprite);
                                }
                            }
                    }
                }
                //--------------------------------------

                






                frameTime = SDL_GetTicks() - framestart;
                SDL_RenderClear(render);
                SDL_RenderCopy(render, texBACKGROUND, &rectBackground, NULL);
                SDL_RenderCopy(render, texturePlayer, &rectPlayerSprite, &rectPlayer);
                SDL_RenderCopy(render, texMAGE_START_Game, &rectMAGE_START_GameSprite, &rectMAGE_START_Game);
                SDL_RenderCopy(render, texBALAO_02[0], NULL, &rectBALAO_02[0]);
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
                        close_requested = true;
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
                                    close_requested = true;
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
        SDL_DestroyTexture(texMAGE_START_Game);
        SDL_FreeSurface(surfMAGE_START_Game);
        SDL_DestroyRenderer(render);
    
    time_t timeStop= time(NULL);
    int timeOpened = difftime(timeStop,timeStart);
    char tempo[50];
    sprintf(tempo,"Você jogou por %d segundos",timeOpened);
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, version, tempo,  window);
    //printf("FICOU ABERTO %d segundos\n", timeOpened);
    SDL_StopTextInput();
	finalizar();
    return 0;
}

/*
Magostart:
    (texto centralizado)
    Bruxa:
        Você parece novo aqui.
    Bruxa:
        Então, um ser desconhecido esta prestes a destruir nossa vila/cidade/regiao
    Bruxa:
        e você parece ser capaz de derrota-lo.
    Bruxa:
        Você parece não falar muito. De qualquer forma,
    Bruxa:
        siga as setas que encontrar e chegara ao seu destino.



*/
//------------------------------------------------------------------------------------//

SDL_Rect matarEnemy(SDL_Rect* enemy,int i){
    enemy->x = 6000;
    enemy->y = 3000;
    printf("Matou/sumiu inimigo : %d\n", i);
    return *enemy;
}


int colisao(SDL_Rect ent_1,SDL_Rect ent_2){
    int colide=-1;
    if((ent_1.y - speedPlayer <= ent_2.y + ent_2.h) && (ent_1.y + ent_1.h >= ent_2.y + ent_2.h) && ((ent_1.x >=ent_2.x && ent_1.x <= ent_2.x + ent_2.w) || (ent_1.x + ent_1.w >= ent_2.x && ent_1.x + ent_1.w <= ent_2.x + ent_2.w))){
        //sobe=0
        colide = 0;
        //printf("Colisao no topo do rect\n");
    }
    if((ent_1.y + ent_1.h + speedPlayer >= ent_2.y) && (ent_1.y + ent_1.h <= ent_2.y) && ((ent_1.x >=ent_2.x && ent_1.x <= ent_2.x + ent_2.w) || (ent_1.x + ent_1.w >= ent_2.x && ent_1.x + ent_1.w <= ent_2.x + ent_2.w))){
        //desce=0
        colide = 1;
        //printf("Colisao na base do rect\n");
    }
    if((ent_1.x +ent_1.w + speedPlayer >= ent_2.x) && (ent_1.x + ent_1.w <= ent_2.x) && (ent_1.y <= ent_2.y + ent_2.h && ent_1.y + ent_1.h >= ent_2.y)){
        //direita=0
        colide = 2;
        //printf("Colisao na direita do rect\n");
    }
    if((ent_1.x - speedPlayer <= ent_2.x + ent_2.w) && (ent_1.x >= ent_2.x + ent_2.w) && (ent_1.y <= ent_2.y + ent_2.h && ent_1.y + ent_1.h >= ent_2.y)){
        //esquerda=0
        colide = 3;
        //printf("Colisao na esquerda do rect\n");
    }
    
    return colide;
}
/*

if (dPlayer.y >= dEsqueleto.y && dPlayer.y <= dEsqueleto.y + dEsqueleto.h && Imunidade == 0){
		if(dPlayer.x + dPlayer.w >= dEsqueleto.x && dPlayer.x <= dEsqueleto.x + dEsqueleto.w){
			player.Vida -= 5;
			Imunidade = 100;
		}
	}
	if (dPlayer.x >= dEsqueleto.x && dPlayer.x <= dEsqueleto.x + dEsqueleto.w && Imunidade == 0){
		if(dPlayer.y + dPlayer.h >= dEsqueleto.y && dPlayer.y <= dEsqueleto.y + dEsqueleto.h){
			player.Vida -= 5;
			Imunidade = 100;
		}
}

*/

int inicializar(){
    SDL_Init(SDL_INIT_EVERYTHING);

        if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
            printf("Deu merda para iniciar o SDL! SDL_Error: %s\n", SDL_GetError());
            SDL_Quit();
            return 1;
        }
        printf("Iniciou o SDL\n");
        Uint32 flagsCreateWindow = SDL_WINDOW_SHOWN | SDL_RENDERER_PRESENTVSYNC | SDL_WINDOW_FULLSCREEN_DESKTOP;
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

