#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
//#include "customlib.h"


//VERSÃO
#define version "v0.1.7" 
/*
    gcc -o game game.c -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -Wall 
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
int imunidadePlayer = 0;
int delayFruta = 0;
int statusColisao = 1;
int bossDead = false;
int i,j,k;
int pontuacaoPlayer=0;
int frutaInvCount = 0;

//speed in pixels/second
#define SPEED 300

double speedPlayer = 10;
#define FPS 23
#define framedelay 30
#define cutdelay 150
#define cutdelay2 500

#define valueImunidadeHit FPS

#define mapx 7680
#define mapy 4320

#define nEnemies 37
#define nBalao1 0
#define nBalao2 1
#define nFruits 16
#define nWalls 110
#define nHearts 12
//cada meio coracao

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
int colisao(SDL_Rect,SDL_Rect);
SDL_Rect matarEnemy(SDL_Rect*,int );
SDL_Rect pegarFrutas(SDL_Rect*);
bool colisaoMatar(SDL_Rect,SDL_Rect);
Mix_Chunk startmusicMenu();
SDL_bool colisionKill;
SDL_Rect *rectFullLife;
SDL_Rect *rectplusOneLife;
int *lifeCount;
void fullLife();
void plusOneLife();
void consumirFruta();
void receiveDamage();
void playHowTo();
void fadeAudio();
int startAudio();
void playGameplay();

int main(int argc, char* args[]){
    int statusGame=0,mageStatus=0;
    int sobe=false, desce=false, esquerda=false,direita=false,atacar=0,lastside,statusRank=0,statusCreditos=0;
    int enemyMove=-1,enemyMoveMinotaur=-1,minotaurAtack=false, falar_npc=false,bossLife=15;
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

    inicializar();  
    startRenderer();
    //LIFE--------------
    int PlayerLife=nHearts;
    lifeCount=&PlayerLife;
    SDL_Surface *surfHeart = IMG_Load("recursos/coracao.png");
    SDL_Texture *texHeart = SDL_CreateTextureFromSurface(render, surfHeart);
    SDL_Rect rectHeart = {5,0,200,30};
    SDL_Rect rectHeartSprite = {1292,0,108,17};
    rectFullLife=&rectHeartSprite;
    //------------------

    //Inventario Frutas
    SDL_Surface *surfInvFruta = IMG_Load("recursos/barradefruta.png");
    SDL_Texture *texInvFruta = SDL_CreateTextureFromSurface(render, surfInvFruta);
    SDL_Rect rectInvFruta = {5,35,200,50};
    SDL_Rect rectInvFrutaSprite = {0,0,72,24};
    rectplusOneLife=&rectInvFrutaSprite;
    //-----------------

    //PLAYER------------
    SDL_Surface *surfPlayer = IMG_Load("recursos/possible_new_character_sprite.png");
    if(surfPlayer==NULL){
        printf("Deu merda ao criar surfPlayer! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(render);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    SDL_Texture *texturePlayer = SDL_CreateTextureFromSurface(render, surfPlayer);
    if(texturePlayer==NULL){
        printf("Deu merda ao criar texturePlayer! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(render);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    SDL_Rect rectPlayer;
    rectPlayer.w = 60;
    rectPlayer.h = 60;
    rectPlayer.x = (SCREEN_WIDTH/2)   - (rectPlayer.w/2);
    rectPlayer.y = (SCREEN_HEIGHT /2) - (rectPlayer.h/2);
    SDL_Rect rectPlayerSprite = {0,128,64,64};
    SDL_Rect rectPlayerAttack = {rectPlayer.x-21,rectPlayer.y-16,100,100};

    SDL_Surface *surfPlayerAttack = IMG_Load("recursos/rosa.png");
    SDL_Texture *texPlayerAttack = SDL_CreateTextureFromSurface(render, surfPlayerAttack);
    SDL_Surface *surfDEBUG = IMG_Load("recursos/azul.png");
    SDL_Texture *texDEBUG = SDL_CreateTextureFromSurface(render, surfDEBUG);
    //---------------------

    //MAGE START GAME
    SDL_Surface *surfMAGE_START_Game = IMG_Load("recursos/mago_f_start.png");
    SDL_Texture *texMAGE_START_Game = SDL_CreateTextureFromSurface(render, surfMAGE_START_Game);
    SDL_Rect rectMAGE_START_Game = {(SCREEN_WIDTH/2)+ (rectPlayer.w/2)+50,(SCREEN_HEIGHT /2) - (rectPlayer.h/2)-50,64,86};
    SDL_Rect rectMAGE_START_GameSprite = {0,0,32,43};

    //---------------------
    //balao 01
    surfBALAO_01[0] = IMG_Load("recursos/balao_1.png");
    texBALAO_01[0] = SDL_CreateTextureFromSurface(render, surfBALAO_01[0]);
    //---------------------

    //balao 02 do mago start
    surfBALAO_02[0] = IMG_Load("recursos/balao_2.png");
    texBALAO_02[0] = SDL_CreateTextureFromSurface(render, surfBALAO_02[0]);
    rectBALAO_02[0].x = rectMAGE_START_Game.x + (rectMAGE_START_Game.w/3);
    rectBALAO_02[0].y = rectMAGE_START_Game.y - 32 ;
    rectBALAO_02[0].w = 32;
    rectBALAO_02[0].h = 32;

    //---------------------
    //MSG FINAL-----------
    SDL_Surface *surfENDGAME = IMG_Load("recursos/endgame.png");
    SDL_Texture *texENDGAME = SDL_CreateTextureFromSurface(render, surfENDGAME);

    //BACKGROUND-----------
    SDL_Surface *surfBACKGROUND = IMG_Load("recursos/El_mapa.png");
    SDL_Texture *texBACKGROUND = SDL_CreateTextureFromSurface(render, surfBACKGROUND);
    SDL_Rect rectBackground = {5772,228,SCREEN_WIDTH,SCREEN_HEIGHT};

    //BACKGROUNDsecret-----------
    SDL_Surface *surfBACKGROUNDsecret = IMG_Load("recursos/El_mapa_misterioso.png");
    SDL_Texture *texBACKGROUNDsecret = SDL_CreateTextureFromSurface(render, surfBACKGROUNDsecret);
    SDL_Rect rectBackgroundsecret = {5772,228,SCREEN_WIDTH,SCREEN_HEIGHT};
    rectBackgroundsecret=rectBackground;
    //Dead-----------
    SDL_Surface *surfDead = IMG_Load("recursos/morto.png");
    SDL_Texture *texDead = SDL_CreateTextureFromSurface(render, surfDead);
    
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
    //SDL_Surface *surfFRUTAS[nFruits];
    SDL_Texture *texFRUTAS[nFruits];
    SDL_Rect rectFRUTAS[nFruits];
    SDL_Surface *surfFruta_01 = IMG_Load("recursos/frutas/frutas_01.png");
    SDL_Surface *surfFruta_02 = IMG_Load("recursos/frutas/frutas_02.png");
    SDL_Surface *surfFruta_03 =IMG_Load("recursos/frutas/frutas_03.png");
    //FRUITS-------------------
    for(i=0;i<nFruits;i++){
        rectFRUTAS[i].w = 30;
        rectFRUTAS[i].h = 30;
    }

    texFRUTAS[0] = SDL_CreateTextureFromSurface(render, surfFruta_01);
    rectFRUTAS[0].x = 1332-rectBackground.x; rectFRUTAS[0].y = 782-rectBackground.y;

    texFRUTAS[1] = SDL_CreateTextureFromSurface(render, surfFruta_02);
    rectFRUTAS[1].x = 1872-rectBackground.x; rectFRUTAS[1].y = 488-rectBackground.y;

    texFRUTAS[2] = SDL_CreateTextureFromSurface(render, surfFruta_03);
    rectFRUTAS[2].x = 5837-rectBackground.x; rectFRUTAS[2].y = 966-rectBackground.y;

    texFRUTAS[3] = SDL_CreateTextureFromSurface(render, surfFruta_01);
    rectFRUTAS[3].x = 5741-rectBackground.x; rectFRUTAS[3].y = 780-rectBackground.y;

    texFRUTAS[4] = SDL_CreateTextureFromSurface(render, surfFruta_02);
    rectFRUTAS[4].x = 6158-rectBackground.x; rectFRUTAS[4].y = 200-rectBackground.y;

    texFRUTAS[5] = SDL_CreateTextureFromSurface(render, surfFruta_03);
    rectFRUTAS[5].x = 1613-rectBackground.x; rectFRUTAS[5].y = 3367-rectBackground.y;

    texFRUTAS[6] = SDL_CreateTextureFromSurface(render, surfFruta_01);
    rectFRUTAS[6].x = 3663-rectBackground.x; rectFRUTAS[6].y = 4088-rectBackground.y;

    texFRUTAS[7] = SDL_CreateTextureFromSurface(render, surfFruta_02);
    rectFRUTAS[7].x = 3111-rectBackground.x; rectFRUTAS[7].y = 3988-rectBackground.y;

    texFRUTAS[8] = SDL_CreateTextureFromSurface(render, surfFruta_03);
    rectFRUTAS[8].x = 1761-rectBackground.x; rectFRUTAS[8].y = 1884-rectBackground.y;

    texFRUTAS[9] = SDL_CreateTextureFromSurface(render, surfFruta_01);
    rectFRUTAS[9].x = 7022-rectBackground.x; rectFRUTAS[9].y = 400-rectBackground.y;

    texFRUTAS[10] = SDL_CreateTextureFromSurface(render, surfFruta_02);
    rectFRUTAS[10].x = 3319-rectBackground.x; rectFRUTAS[10].y = 2938-rectBackground.y;

    texFRUTAS[11] = SDL_CreateTextureFromSurface(render, surfFruta_03);
    rectFRUTAS[11].x = 3371-rectBackground.x; rectFRUTAS[11].y = 1884-rectBackground.y;

    texFRUTAS[12] = SDL_CreateTextureFromSurface(render, surfFruta_01);
    rectFRUTAS[12].x = 4823-rectBackground.x; rectFRUTAS[12].y = 3613-rectBackground.y;

    texFRUTAS[13] = SDL_CreateTextureFromSurface(render, surfFruta_02);
    rectFRUTAS[13].x = 4796-rectBackground.x; rectFRUTAS[13].y = 3986-rectBackground.y;

    texFRUTAS[14] = SDL_CreateTextureFromSurface(render, surfFruta_03);
    rectFRUTAS[14].x = 5638-rectBackground.x; rectFRUTAS[14].y = 3534-rectBackground.y;

    texFRUTAS[15] = SDL_CreateTextureFromSurface(render, surfFruta_01);
    rectFRUTAS[15].x = 5089-rectBackground.x; rectFRUTAS[15].y = 3350-rectBackground.y;

    //LOADING-----------------
    SDL_Surface *surfloading = IMG_Load("recursos/loadingscreen.png");
    SDL_Texture *texloading = SDL_CreateTextureFromSurface(render, surfloading);
    SDL_Rect rectloading = {0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
    SDL_Rect rectloadingrecorte = {0,0,960,540};

    //EA PLAN   -----------------
    SDL_Surface *surEA_PLAN = IMG_Load("recursos/ea_plan.png");
    SDL_Texture *texEA_PLAN = SDL_CreateTextureFromSurface(render, surEA_PLAN);
    SDL_Rect rectEA_PLAN = {0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
    SDL_Rect rectEA_PLANsprite = {0,0,SCREEN_WIDTH,SCREEN_HEIGHT};

    //TUTORIAL-----------------
    SDL_Surface *surTutorial = IMG_Load("recursos/tutorial.png");
    SDL_Texture *texTutorial = SDL_CreateTextureFromSurface(render, surTutorial);    
    //-------------------------------

    //SDL_Rect rectloading = {0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
    //---------------------
    //ENEMY1----------
    SDL_Surface *surfGreenEnemy = IMG_Load("recursos/green_enemy.png");
    //SDL_Texture *texGreenEnemy = SDL_CreateTextureFromSurface(render, surfGreenEnemy);
    SDL_Surface *surfRedEnemy = IMG_Load("recursos/red_enemy.png");
    //SDL_Texture *texRedEnemy = SDL_CreateTextureFromSurface(render, surfRedEnemy);
    SDL_Surface *surfTopEnemy = IMG_Load("recursos/top_enemy.png");
    //SDL_Texture *texTopEnemy = SDL_CreateTextureFromSurface(render, surfTopEnemy);
    SDL_Surface *surfPurpleEnemy = IMG_Load("recursos/purple_enemy.png");
    //SDL_Texture *texPurpleEnemy = SDL_CreateTextureFromSurface(render, surfPurpleEnemy);
    for(i=0;i<nEnemies-1;i++){
        rectspriteEnemies[i].x = 0;
        rectspriteEnemies[i].y = 94;
        rectspriteEnemies[i].w = 60;
        rectspriteEnemies[i].h = 47;
        rectEnemies[i].w = 180;
        rectEnemies[i].h = 141;
    }
    //reeeeeeeeeeeeeeeeeeeeeeeeed---------------------------------------------------------
    texEnemies[0] = SDL_CreateTextureFromSurface(render, surfRedEnemy);
    rectEnemies[0].x = 1060-rectBackground.x; rectEnemies[0].y = 1487-rectBackground.y;
    texEnemies[1] = SDL_CreateTextureFromSurface(render, surfRedEnemy);
    rectEnemies[1].x = 1344-rectBackground.x; rectEnemies[1].y = 1410-rectBackground.y;
    texEnemies[2] = SDL_CreateTextureFromSurface(render, surfRedEnemy);
    rectEnemies[2].x = 1277-rectBackground.x; rectEnemies[2].y = 1364-rectBackground.y;
    texEnemies[3] = SDL_CreateTextureFromSurface(render, surfRedEnemy);
    rectEnemies[3].x = 1213-rectBackground.x; rectEnemies[3].y = 1763-rectBackground.y;
    texEnemies[4] = SDL_CreateTextureFromSurface(render, surfRedEnemy);
    rectEnemies[4].x = 1207-rectBackground.x; rectEnemies[4].y = 1964-rectBackground.y;
    texEnemies[5] = SDL_CreateTextureFromSurface(render, surfRedEnemy);
    rectEnemies[5].x = 1084-rectBackground.x; rectEnemies[5].y = 2129-rectBackground.y;
    texEnemies[6] = SDL_CreateTextureFromSurface(render, surfRedEnemy);
    rectEnemies[6].x = 733-rectBackground.x; rectEnemies[6].y = 2165-rectBackground.y;
    texEnemies[7] = SDL_CreateTextureFromSurface(render, surfRedEnemy);
    rectEnemies[7].x = 580-rectBackground.x; rectEnemies[7].y = 2087-rectBackground.y;
    texEnemies[8] = SDL_CreateTextureFromSurface(render, surfRedEnemy);
    rectEnemies[8].x = 640-rectBackground.x; rectEnemies[8].y = 1892-rectBackground.y;
    texEnemies[9] = SDL_CreateTextureFromSurface(render, surfRedEnemy);
    rectEnemies[9].x = 550-rectBackground.x; rectEnemies[9].y = 1775-rectBackground.y;
    texEnemies[10] = SDL_CreateTextureFromSurface(render, surfRedEnemy);
    rectEnemies[10].x = 634-rectBackground.x; rectEnemies[10].y = 1601-rectBackground.y;
    texEnemies[11] = SDL_CreateTextureFromSurface(render, surfRedEnemy);
    rectEnemies[11].x = 793-rectBackground.x; rectEnemies[11].y = 1490-rectBackground.y;
    texEnemies[12] = SDL_CreateTextureFromSurface(render, surfRedEnemy);
    rectEnemies[12].x = 526-rectBackground.x; rectEnemies[12].y = 1421-rectBackground.y;
    //GREEEEEEEEEEEEEEEN-------------------------------------------------------------------
    texEnemies[13] = SDL_CreateTextureFromSurface(render, surfGreenEnemy);
    rectEnemies[13].x = 1029-rectBackground.x; rectEnemies[13].y = 2643-rectBackground.y;
    texEnemies[14] = SDL_CreateTextureFromSurface(render, surfGreenEnemy);
    rectEnemies[14].x = 2690-rectBackground.x; rectEnemies[14].y = 2751-rectBackground.y;
    texEnemies[15] = SDL_CreateTextureFromSurface(render, surfGreenEnemy);
    rectEnemies[15].x = 1500-rectBackground.x; rectEnemies[15].y = 2862-rectBackground.y;
    texEnemies[16] = SDL_CreateTextureFromSurface(render, surfGreenEnemy);
    rectEnemies[16].x = 777-rectBackground.x; rectEnemies[16].y = 2748-rectBackground.y;
    texEnemies[17] = SDL_CreateTextureFromSurface(render, surfGreenEnemy);
    rectEnemies[17].x = 549-rectBackground.x; rectEnemies[17].y = 2862-rectBackground.y;
    texEnemies[18] = SDL_CreateTextureFromSurface(render, surfGreenEnemy);
    rectEnemies[18].x = 621-rectBackground.x; rectEnemies[18].y = 3759-rectBackground.y;
    texEnemies[19] = SDL_CreateTextureFromSurface(render, surfGreenEnemy);
    rectEnemies[19].x = 651-rectBackground.x; rectEnemies[19].y = 3924-rectBackground.y;
    texEnemies[20] = SDL_CreateTextureFromSurface(render, surfGreenEnemy);
    rectEnemies[20].x = 846-rectBackground.x; rectEnemies[20].y = 4023-rectBackground.y;
    texEnemies[21] = SDL_CreateTextureFromSurface(render, surfGreenEnemy);
    rectEnemies[21].x = 930-rectBackground.x; rectEnemies[21].y = 394-rectBackground.y;
    //PURPLEEEEEEEEEEEEEEEEEEE-------------------------------------------------------------
    texEnemies[22] = SDL_CreateTextureFromSurface(render, surfPurpleEnemy);
    rectEnemies[22].x = 2544-rectBackground.x; rectEnemies[22].y = 3375-rectBackground.y;
    texEnemies[23] = SDL_CreateTextureFromSurface(render, surfPurpleEnemy);
    rectEnemies[23].x = 2889-rectBackground.x; rectEnemies[23].y = 3447-rectBackground.y;
    texEnemies[24] = SDL_CreateTextureFromSurface(render, surfPurpleEnemy);
    rectEnemies[24].x = 2907-rectBackground.x; rectEnemies[24].y = 3651-rectBackground.y;
    texEnemies[25] = SDL_CreateTextureFromSurface(render, surfPurpleEnemy);
    rectEnemies[25].x = 3258-rectBackground.x; rectEnemies[25].y = 3408-rectBackground.y;
    texEnemies[26] = SDL_CreateTextureFromSurface(render, surfPurpleEnemy);
    rectEnemies[26].x = 3258-rectBackground.x; rectEnemies[26].y = 3726-rectBackground.y;
    texEnemies[27] = SDL_CreateTextureFromSurface(render, surfPurpleEnemy);
    rectEnemies[27].x = 3591-rectBackground.x; rectEnemies[27].y = 3942-rectBackground.y;
    texEnemies[28] = SDL_CreateTextureFromSurface(render, surfPurpleEnemy);
    rectEnemies[28].x = 3786-rectBackground.x; rectEnemies[28].y = 3663-rectBackground.y;
    texEnemies[29] = SDL_CreateTextureFromSurface(render, surfPurpleEnemy);
    rectEnemies[29].x = 4194-rectBackground.x; rectEnemies[29].y = 3513-rectBackground.y;
    texEnemies[30] = SDL_CreateTextureFromSurface(render, surfPurpleEnemy);
    rectEnemies[30].x = 3876-rectBackground.x; rectEnemies[30].y = 3384-rectBackground.y;
    texEnemies[31] = SDL_CreateTextureFromSurface(render, surfPurpleEnemy);
    rectEnemies[31].x = 4227-rectBackground.x; rectEnemies[31].y = 3429-rectBackground.y;
    //tooooooooooooooop
    texEnemies[32] = SDL_CreateTextureFromSurface(render, surfTopEnemy);
    rectEnemies[32].x = 5484-rectBackground.x; rectEnemies[32].y = 3400-rectBackground.y;
    texEnemies[33] = SDL_CreateTextureFromSurface(render, surfTopEnemy);
    rectEnemies[33].x = 5127-rectBackground.x; rectEnemies[33].y = 3395-rectBackground.y;
    texEnemies[34] = SDL_CreateTextureFromSurface(render, surfTopEnemy);
    rectEnemies[34].x = 4715-rectBackground.x; rectEnemies[34].y = 3401-rectBackground.y;
    texEnemies[35] = SDL_CreateTextureFromSurface(render, surfTopEnemy);
    rectEnemies[35].x = 4469-rectBackground.x; rectEnemies[35].y = 3398-rectBackground.y;
    SDL_Surface *surfMinotaur = IMG_Load("recursos/minotauro.png");
    texEnemies[36] = SDL_CreateTextureFromSurface(render, surfMinotaur);
    rectEnemies[36].x = 6669-rectBackground.x; rectEnemies[36].y = 3292-rectBackground.y; rectEnemies[36].w = 235; rectEnemies[36].h = 163;
    rectspriteEnemies[36].x = 8;rectspriteEnemies[36].y = 154; rectspriteEnemies[36].w = 40; rectspriteEnemies[36].h = 37;
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
    //HOW HAPPENED------
    SDL_Surface *surfHOW_HAPPENED = IMG_Load("recursos/beforeCutscene.png");
    SDL_Texture *texHOW_HAPPENED = SDL_CreateTextureFromSurface(render, surfHOW_HAPPENED);

    //DIALOG_01
    SDL_Surface *surfDialog_01 = IMG_Load("recursos/Dialog_01.png");
    SDL_Texture *texDialog_01 = SDL_CreateTextureFromSurface(render, surfDialog_01);
    SDL_Rect rectDialog_01 = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_Rect rectDialog_01Sprite = { 0 , 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    //----------------
    
    //WALLS----------------------------------------------------------------------------------------------------------------
    SDL_Rect rectWALL[nWalls];
    rectWALL[0].x = 6210-rectBackground.x; rectWALL[0].y = 141-rectBackground.y; rectWALL[0].w = 103; rectWALL[0].h = 810;    
    rectWALL[1].x = 6210-rectBackground.x; rectWALL[1].y = 678-rectBackground.y; rectWALL[1].w = 447; rectWALL[1].h = 273;
    rectWALL[2].x = 672-rectBackground.x; rectWALL[2].y = 678-rectBackground.y; rectWALL[2].w = 447; rectWALL[2].h = 273;
    rectWALL[3].x = 7125-rectBackground.x; rectWALL[3].y = 142-rectBackground.y; rectWALL[3].w = 104; rectWALL[3].h = 810;
    rectWALL[4].x = 6619-rectBackground.x; rectWALL[4].y = 410-rectBackground.y; rectWALL[4].w = 40; rectWALL[4].h = 163;
    rectWALL[5].x = 6781-rectBackground.x; rectWALL[5].y = 409-rectBackground.y; rectWALL[5].w = 40; rectWALL[5].h = 163;
    rectWALL[6].x = 6631-rectBackground.x; rectWALL[6].y = 137-rectBackground.y; rectWALL[6].w = 196; rectWALL[6].h = 372;
    rectWALL[7].x = 6526-rectBackground.x; rectWALL[7].y = 137-rectBackground.y; rectWALL[7].w = 382; rectWALL[7].h = 311;
    rectWALL[8].x = 6208-rectBackground.x; rectWALL[8].y = 137-rectBackground.y; rectWALL[8].w = 1022; rectWALL[8].h = 149;
    rectWALL[9].x = 5890-rectBackground.x; rectWALL[9].y = 269-rectBackground.y; rectWALL[9].w = 396; rectWALL[9].h = 185;
    rectWALL[10].x = 6206-20-rectBackground.x; rectWALL[10].y = 687-rectBackground.y; rectWALL[10].w = 479; rectWALL[10].h = 168;
    rectWALL[11].x = 6752+20-rectBackground.x; rectWALL[11].y = 691-rectBackground.y; rectWALL[11].w = 479; rectWALL[11].h = 268;
    rectWALL[12].x = 6400-rectBackground.x; rectWALL[12].y = 320-rectBackground.y; rectWALL[12].w = 32; rectWALL[12].h = 64;
    rectWALL[13].x = 5374-rectBackground.x; rectWALL[13].y = 934-rectBackground.y; rectWALL[13].w = 171; rectWALL[13].h = 216;
    rectWALL[14].x = 4671-rectBackground.x; rectWALL[14].y = 814-rectBackground.y; rectWALL[14].w = 171; rectWALL[14].h = 216;
    rectWALL[15].x = 3879-rectBackground.x; rectWALL[15].y = 737-rectBackground.y; rectWALL[15].w = 781; rectWALL[15].h = 366;
    rectWALL[16].x = 3654-rectBackground.x; rectWALL[16].y = 386-rectBackground.y; rectWALL[16].w = 781; rectWALL[16].h = 366;
    rectWALL[17].x = 4644-rectBackground.x; rectWALL[17].y = 528-rectBackground.y; rectWALL[17].w = 96; rectWALL[17].h = 217;
    rectWALL[18].x = 3358-rectBackground.x; rectWALL[18].y = 906-rectBackground.y; rectWALL[18].w = 171; rectWALL[18].h = 216;
    rectWALL[19].x = 3008-rectBackground.x; rectWALL[19].y = 784-rectBackground.y; rectWALL[19].w = 171; rectWALL[19].h = 216;
    rectWALL[20].x = 3136-rectBackground.x; rectWALL[20].y = 364-rectBackground.y; rectWALL[20].w = 171; rectWALL[20].h = 216;
    rectWALL[21].x = 4672-rectBackground.x; rectWALL[21].y = 803-rectBackground.y; rectWALL[21].w = 171; rectWALL[21].h = 223;
    rectWALL[22].x = 2720-rectBackground.x; rectWALL[22].y = 711-rectBackground.y; rectWALL[22].w = 171; rectWALL[22].h = 223;
    rectWALL[23].x = 2654-rectBackground.x; rectWALL[23].y = 240-rectBackground.y; rectWALL[23].w = 171; rectWALL[23].h = 223;
    rectWALL[24].x = 2465-rectBackground.x; rectWALL[24].y = 903-rectBackground.y; rectWALL[24].w = 171; rectWALL[24].h = 223;
    rectWALL[25].x = 2402-rectBackground.x; rectWALL[25].y = 522-rectBackground.y; rectWALL[25].w = 171; rectWALL[25].h = 223;
    rectWALL[26].x = 2465-rectBackground.x; rectWALL[26].y = 891-rectBackground.y; rectWALL[26].w = 171; rectWALL[26].h = 223;
    rectWALL[27].x = 2015-rectBackground.x; rectWALL[27].y = 864-rectBackground.y; rectWALL[27].w = 171; rectWALL[27].h = 223;
    rectWALL[28].x = 2045-rectBackground.x; rectWALL[28].y = 486-rectBackground.y; rectWALL[28].w = 171; rectWALL[28].h = 223;
    rectWALL[29].x = 1793-rectBackground.x; rectWALL[29].y = 258-rectBackground.y; rectWALL[29].w = 171; rectWALL[29].h = 223;
    rectWALL[30].x = 1664-rectBackground.x; rectWALL[30].y = 645-rectBackground.y; rectWALL[30].w = 171; rectWALL[30].h = 223;
    //janela------
    rectWALL[31].x = 0-rectBackground.x; rectWALL[31].y = 0-rectBackground.y; rectWALL[31].w = 7684; rectWALL[31].h = 144;
    rectWALL[32].x = 7391-rectBackground.x; rectWALL[32].y = 0-rectBackground.y; rectWALL[32].w = 293; rectWALL[32].h = 4331;
    rectWALL[33].x = 0-rectBackground.x; rectWALL[33].y = 4134-rectBackground.y; rectWALL[33].w = 7684; rectWALL[33].h = 197;
    rectWALL[34].x = 0-rectBackground.x; rectWALL[34].y = 0-rectBackground.y; rectWALL[34].w = 288; rectWALL[34].h = 4331;
    //--------------
    rectWALL[35].x = 241-rectBackground.x; rectWALL[35].y = 808-rectBackground.y; rectWALL[35].w = 1266; rectWALL[35].h = 326;
    rectWALL[36].x = 241-rectBackground.x; rectWALL[36].y = 326-40-rectBackground.y; rectWALL[36].w = 1266; rectWALL[36].h = 440;
    rectWALL[37].x = 1664-rectBackground.x; rectWALL[37].y = 645+40-rectBackground.y; rectWALL[37].w = 171; rectWALL[37].h = 223;
    rectWALL[38].x = 893-rectBackground.x; rectWALL[38].y = 708-rectBackground.y; rectWALL[38].w = 413; rectWALL[38].h = 163;
    rectWALL[39].x = 0-rectBackground.x; rectWALL[39].y = 1089-rectBackground.y; rectWALL[39].w = 1726; rectWALL[39].h = 311;
    rectWALL[40].x = 0-rectBackground.x; rectWALL[40].y = 1089-rectBackground.y; rectWALL[40].w = 467; rectWALL[40].h = 1729;
    rectWALL[41].x = 0-rectBackground.x; rectWALL[41].y = 1089-rectBackground.y; rectWALL[41].w = 467; rectWALL[41].h = 1729;
    rectWALL[42].x = 0-rectBackground.x; rectWALL[42].y = 2337-rectBackground.y; rectWALL[42].w = 982; rectWALL[42].h = 237;
    rectWALL[43].x = 0-rectBackground.x; rectWALL[43].y = 2337-rectBackground.y; rectWALL[43].w = 890; rectWALL[43].h = 343;
    rectWALL[44].x = 0-rectBackground.x; rectWALL[44].y = 2337-rectBackground.y; rectWALL[44].w = 524; rectWALL[44].h = 491;
    rectWALL[45].x = 0-rectBackground.x; rectWALL[45].y = 2337-rectBackground.y; rectWALL[45].w = 342; rectWALL[45].h = 679;
    rectWALL[46].x = 542-rectBackground.x; rectWALL[46].y = 3067-rectBackground.y; rectWALL[46].w = 358; rectWALL[46].h = 553;
    rectWALL[47].x = 638-rectBackground.x; rectWALL[47].y = 2885-rectBackground.y; rectWALL[47].w = 358; rectWALL[47].h = 893;
    rectWALL[48].x = 0-rectBackground.x; rectWALL[48].y = 1089-rectBackground.y; rectWALL[48].w = 467; rectWALL[48].h = 1729;
    rectWALL[49].x = 792-rectBackground.x; rectWALL[49].y = 2817-rectBackground.y; rectWALL[49].w = 492; rectWALL[49].h = 945;
    rectWALL[50].x = 1096-rectBackground.x; rectWALL[50].y = 2817-rectBackground.y; rectWALL[50].w = 188-18; rectWALL[50].h = 1027;
    rectWALL[51].x = 1251-rectBackground.x; rectWALL[51].y = 3683-rectBackground.y; rectWALL[51].w = 49-13; rectWALL[51].h = 193;
    rectWALL[52].x = 1351+20-rectBackground.x; rectWALL[52].y = 3719+20-rectBackground.y; rectWALL[52].w = 635; rectWALL[52].h = 479;
    //embaixo ponte do final agua
    rectWALL[53].x = 1641+30-rectBackground.x; rectWALL[53].y = 3531+15-rectBackground.y; rectWALL[53].w = 635; rectWALL[53].h = 479;
    
    rectWALL[54].x = 957-rectBackground.x; rectWALL[54].y = 3051-rectBackground.y; rectWALL[54].w = 635-25; rectWALL[54].h = 621-20;
    
    rectWALL[55].x = 1177-rectBackground.x; rectWALL[55].y = 3051-rectBackground.y; rectWALL[55].w = 417-37; rectWALL[55].h = 91;
    rectWALL[56].x = 1073-rectBackground.x; rectWALL[56].y = 3015-rectBackground.y; rectWALL[56].w = 417-10; rectWALL[56].h = 91;
    
    rectWALL[57].x = 1041-rectBackground.x; rectWALL[57].y = 2963-rectBackground.y; rectWALL[57].w = 417; rectWALL[57].h = 91;
    
    rectWALL[58].x = 1177-rectBackground.x; rectWALL[58].y = 3051-rectBackground.y; rectWALL[58].w = 417-25; rectWALL[58].h = 91;
    
    rectWALL[59].x = 941-rectBackground.x; rectWALL[59].y = 2899-rectBackground.y; rectWALL[59].w = 417; rectWALL[59].h = 91;
     
    rectWALL[60].x = 1647+15-rectBackground.x; rectWALL[60].y = 1975-rectBackground.y; rectWALL[60].w = 225; rectWALL[60].h = 1503-20;
    rectWALL[61].x = 1720-rectBackground.x; rectWALL[61].y = 2777-rectBackground.y; rectWALL[61].w = 452; rectWALL[61].h = 572;
    rectWALL[62].x = 1569-rectBackground.x; rectWALL[62].y = 2475-rectBackground.y; rectWALL[62].w = 545; rectWALL[62].h = 316;
    rectWALL[63].x = 1449-rectBackground.x; rectWALL[63].y = 2394-rectBackground.y; rectWALL[63].w = 584; rectWALL[63].h = 316;
    rectWALL[64].x = 1317-rectBackground.x; rectWALL[64].y = 2328-rectBackground.y; rectWALL[64].w = 728; rectWALL[64].h = 316;
    
    rectWALL[65].x = 1095-rectBackground.x; rectWALL[65].y = 2274-rectBackground.y; rectWALL[65].w = 884; rectWALL[65].h = 304;
    rectWALL[66].x = 1464-rectBackground.x; rectWALL[66].y = 1965-rectBackground.y; rectWALL[66].w = 482; rectWALL[66].h = 409;
    rectWALL[67].x = 1464-rectBackground.x; rectWALL[67].y = 1512-rectBackground.y; rectWALL[67].w = 272; rectWALL[67].h = 862;
    rectWALL[67].x = 1383-rectBackground.x; rectWALL[67].y = 1512-rectBackground.y; rectWALL[67].w = 353; rectWALL[67].h = 148;
    rectWALL[68].x = 1485-rectBackground.x; rectWALL[68].y = 1601-rectBackground.y; rectWALL[68].w = 238; rectWALL[68].h = 536;
    rectWALL[69].x = 1540-rectBackground.x; rectWALL[69].y = 3414-rectBackground.y; rectWALL[69].w = 202; rectWALL[69].h = 42+6;
    rectWALL[70].x = 2184-rectBackground.x; rectWALL[70].y = 3476-rectBackground.y; rectWALL[70].w = 586; rectWALL[70].h = 855;
    rectWALL[71].x = 2055-rectBackground.x; rectWALL[71].y = 3523-rectBackground.y; rectWALL[71].w = 282; rectWALL[71].h = 148;
    rectWALL[72].x = 1926-rectBackground.x; rectWALL[72].y = 3583-rectBackground.y; rectWALL[72].w = 282; rectWALL[72].h = 148;
    rectWALL[73].x = 2614-rectBackground.x; rectWALL[73].y = 3761-rectBackground.y; rectWALL[73].w = 328; rectWALL[73].h = 570;
    rectWALL[74].x = 2738-rectBackground.x; rectWALL[74].y = 3997-rectBackground.y; rectWALL[74].w = 328; rectWALL[74].h = 334;
    rectWALL[75].x = 2950-rectBackground.x; rectWALL[75].y = 4087-rectBackground.y; rectWALL[75].w = 328; rectWALL[75].h = 244;
    rectWALL[76].x = 3082-rectBackground.x; rectWALL[76].y = 3519-rectBackground.y; rectWALL[76].w = 146; rectWALL[76].h = 164;
    rectWALL[77].x = 3302-rectBackground.x; rectWALL[77].y = 3511-rectBackground.y; rectWALL[77].w = 164; rectWALL[77].h = 164;
    rectWALL[78].x = 3558-rectBackground.x; rectWALL[78].y = 3483-rectBackground.y; rectWALL[78].w = 146; rectWALL[78].h = 164;
    rectWALL[79].x = 3368-rectBackground.x; rectWALL[79].y = 3707-rectBackground.y; rectWALL[79].w = 146; rectWALL[79].h = 164;
    rectWALL[80].x = 3368-rectBackground.x; rectWALL[80].y = 3959-rectBackground.y; rectWALL[80].w = 146; rectWALL[80].h = 164;
    rectWALL[81].x = 3080-rectBackground.x; rectWALL[81].y = 3771-rectBackground.y; rectWALL[81].w = 146; rectWALL[81].h = 164;
    rectWALL[82].x = 3688-rectBackground.x; rectWALL[82].y = 3779-rectBackground.y; rectWALL[82].w = 146; rectWALL[82].h = 164;
    rectWALL[83].x = 3816-rectBackground.x; rectWALL[83].y = 3995-rectBackground.y; rectWALL[83].w = 146; rectWALL[83].h = 164;
    rectWALL[84].x = 3976-rectBackground.x; rectWALL[84].y = 3779-rectBackground.y; rectWALL[84].w = 146; rectWALL[84].h = 164;
    rectWALL[85].x = 3912-rectBackground.x; rectWALL[85].y = 3487-rectBackground.y; rectWALL[85].w = 146; rectWALL[85].h = 164;
    rectWALL[86].x = 4138-rectBackground.x; rectWALL[86].y = 3997-rectBackground.y; rectWALL[86].w = 146; rectWALL[86].h = 164;
    rectWALL[87].x = 3082-rectBackground.x; rectWALL[87].y = 3519-rectBackground.y; rectWALL[87].w = 146; rectWALL[87].h = 164;
    rectWALL[88].x = 4200-rectBackground.x; rectWALL[88].y = 3647-rectBackground.y; rectWALL[88].w = 146; rectWALL[88].h = 164;
    rectWALL[89].x = 4390-rectBackground.x; rectWALL[89].y = 3483-rectBackground.y; rectWALL[89].w = 146; rectWALL[89].h = 164;
    rectWALL[90].x = 4360-rectBackground.x; rectWALL[90].y = 3837-rectBackground.y; rectWALL[90].w = 146; rectWALL[90].h = 164;
    rectWALL[91].x = 4290-rectBackground.x; rectWALL[91].y = 4071-rectBackground.y; rectWALL[91].w = 66; rectWALL[91].h = 90;
    rectWALL[92].x = 3838-rectBackground.x; rectWALL[92].y = 3849-rectBackground.y; rectWALL[92].w = 66; rectWALL[92].h = 90;
    rectWALL[93].x = 3231-rectBackground.x; rectWALL[93].y = 3847-rectBackground.y; rectWALL[93].w = 66; rectWALL[93].h = 90;
    //casa do inicio
    rectWALL[94].x = 5375-rectBackground.x; rectWALL[94].y = 832-rectBackground.y; rectWALL[94].w = 353; rectWALL[94].h = 320;
    rectWALL[95].x = 1744-rectBackground.x; rectWALL[95].y = 1948-rectBackground.y; rectWALL[95].w = 444; rectWALL[95].h = 1320;
    rectWALL[96].x = 2336-rectBackground.x; rectWALL[96].y = 2018-rectBackground.y; rectWALL[96].w = 5348; rectWALL[96].h = 87;
    rectWALL[97].x = 2280-rectBackground.x; rectWALL[97].y = 2636-rectBackground.y; rectWALL[97].w = 4100; rectWALL[97].h = 236;
    rectWALL[98].x = 5997-rectBackground.x; rectWALL[98].y = 2545-rectBackground.y; rectWALL[98].w = 366; rectWALL[98].h = 808;
    rectWALL[99].x = 5997-rectBackground.x; rectWALL[99].y = 3461-rectBackground.y; rectWALL[99].w = 366; rectWALL[99].h = 808;
    rectWALL[100].x = 5826-rectBackground.x; rectWALL[100].y = 3677-rectBackground.y; rectWALL[100].w = 537; rectWALL[100].h = 592;
    rectWALL[101].x = 6342-rectBackground.x; rectWALL[101].y = 3687-rectBackground.y; rectWALL[101].w = 597; rectWALL[101].h = 540;
    rectWALL[102].x = 6936-rectBackground.x; rectWALL[102].y = 3291-rectBackground.y; rectWALL[102].w = 748; rectWALL[102].h = 945;
    rectWALL[103].x = 6360-rectBackground.x; rectWALL[103].y = 2631-rectBackground.y; rectWALL[103].w = 550; rectWALL[103].h = 459;
    rectWALL[104].x = 6913-rectBackground.x; rectWALL[104].y = 2601-rectBackground.y; rectWALL[104].w = 405; rectWALL[104].h = 600;
    rectWALL[105].x = 7384-rectBackground.x; rectWALL[105].y = 2445-rectBackground.y; rectWALL[105].w = 300; rectWALL[105].h = 1886;
    rectWALL[106].x = 6543-rectBackground.x; rectWALL[106].y = 1944-rectBackground.y; rectWALL[106].w = 1141; rectWALL[106].h = 258;
    rectWALL[107].x = 5706-rectBackground.x; rectWALL[107].y = 2130-rectBackground.y; rectWALL[107].w = 472; rectWALL[107].h = 468;
    rectWALL[108].x = 4605-rectBackground.x; rectWALL[108].y = 3458-rectBackground.y; rectWALL[108].w = 296; rectWALL[108].h = 186;
    rectWALL[109].x = 4577-rectBackground.x; rectWALL[109].y = 3808-rectBackground.y; rectWALL[109].w = 296; rectWALL[109].h = 186;

    //não funciona
    //SDL_Rect barco_final = {6195-rectBackground.x,2135-rectBackground.y,373,495};
    SDL_Rect barco_praia1 = {3262-rectBackground.x,2907-rectBackground.y,135,63};
    SDL_Rect barco_final = {6422-rectBackground.x,1283-rectBackground.y,323,288};


    //---------------------------------------------------------------------------------------------------------------------
    SDL_Rect rectPlayerPosMap = { rectPlayer.x-rectBackground.x, rectPlayer.y-rectBackground.y, rectPlayer.w, rectPlayer.h};
    int close_requested = 0;
        
    uint32_t framestart;
    uint32_t frameTime,frameTimeSprite,frameTimeSpriteAtack;
    int startMMenu=0,startMCutscene=0;
    startAudio();
    Mix_Chunk *bomba = Mix_LoadWAV("recursos/bomba.mp3");
    Mix_VolumeChunk(bomba,MIX_MAX_VOLUME/4);
    Mix_Chunk *teclado = Mix_LoadWAV("recursos/teclado.mp3");
    Mix_VolumeChunk(teclado,MIX_MAX_VOLUME);
    printf("Iniciou o JOGO\n");
    time_t timeStart= time(NULL);
    time_t EAPLAN,timePresent;
    while (close_requested == false){
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
                            case SDLK_ESCAPE:
                                statusRank=false;
                                statusCreditos=false;
                                break;
                            case SDLK_RETURN:
                                if(rectCOIN.y==171){
                                    Mix_FadeOutChannel(-1, 1000);
                                    printf("Inicia CUTSCENE\n");
                                    SDL_Delay(700);
                                    playHowTo();
                                    statusGame=6;
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
                SDL_RenderClear(render);
                SDL_RenderCopy(render, texHOW_HAPPENED, NULL, NULL);
                SDL_RenderPresent(render);
                SDL_Delay(1200);
                fadeAudio();
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
            SDL_RenderClear(render);
            SDL_RenderCopy(render, texloading, NULL, NULL);
            SDL_RenderPresent(render);
            SDL_Delay(3000);
            statusGame=3;
            playGameplay();
        }

        else if(statusGame==3){
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
                            case SDLK_e:
                                falar_npc=true;
                                break;
                            case SDLK_v:
                                consumirFruta();
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
                            case SDLK_e:
                                falar_npc=false;
                                break;
                        }
                        break;
                    }
                }
                
                if(colisaoMatar(rectPlayer,barco_praia1)==true && imunidadePlayer == 0){
                    pontuacaoPlayer+=100;
                }
                //fim event loop
                //inicio colisao com paredes
                if(statusColisao==1)
                for(i=0;i<nWalls;i++){
                    if(colisao(rectPlayer, rectWALL[i])==0)
                        sobe=false;
                    if(colisao(rectPlayer, rectWALL[i])==1)
                        desce=false;
                    if(colisao(rectPlayer, rectWALL[i])==2)
                        direita=false;
                    if(colisao(rectPlayer, rectWALL[i])==3)
                        esquerda=false;   
                }
                //fim colisao com paredes
                //inicio dar vida
                for(i=0;i<nFruits;i++){
                    colisionKill = SDL_HasIntersection(&rectPlayer,&rectFRUTAS[i]);
                    if(colisionKill==SDL_TRUE ){
                        plusOneLife(rectFRUTAS[i]);
                        //fullLife(rectFRUTAS[i]);
                        pegarFrutas(&rectFRUTAS[i]);
                        
                    }
                }
                //fim dar vida

                //colisao player com outros inimigos                
                for(i=0;i<nEnemies;i++){
                    //printf("COLIDINDO\n");
                    if(colisao(rectPlayer, rectEnemies[i])==0){
                        sobe=false;
                        /*KNOCKBACK NOT WORKING AT ALL
                        rectPlayer.y+=speedPlayer*2;
                        rectPlayerAttack.y+=speedPlayer*2;
                        rectBackground.y-=speedPlayer*2;
                        */
                        receiveDamage();
                    }
                    if(colisao(rectPlayer, rectEnemies[i])==1){
                        desce=false;
                        receiveDamage();                        
                    }
                    if(colisao(rectPlayer, rectEnemies[i])==2){
                        direita=false;
                        receiveDamage();                        
                    }
                    if(colisao(rectPlayer, rectEnemies[i])==3){
                        esquerda=false;
                        receiveDamage();
                    }
                    if((colisao(rectPlayer,rectMAGE_START_Game)!=-1)){
                        //dialog_01
                        if(colisao(rectPlayer,rectMAGE_START_Game)==0){
                            sobe=false;
                        }
                        if(colisao(rectPlayer, rectMAGE_START_Game)==1){
                            desce=false;
                        }
                        if(colisao(rectPlayer, rectMAGE_START_Game)==2){
                            direita=false;
                        }
                        if(colisao(rectPlayer, rectMAGE_START_Game)==3){
                            esquerda=false;
                        }
                        if(falar_npc==true){
                            falar_npc=false;
                            rectMAGE_START_GameSprite.x = 32;
                            if(mageStatus!=0){
                                rectDialog_01Sprite.x += SCREEN_WIDTH;
                            }
                            mageStatus+=1;
                        }
                        if(mageStatus==5){

                            rectBALAO_02[0].w = 0;
                            rectBALAO_02[0].h = 0;
                            rectDialog_01Sprite.x = 8000;
                            matarEnemy(&rectMAGE_START_Game,-1);
                            //nao contar os 10 pontos
                            pontuacaoPlayer-=10;
                        }
                    }
                    //sumir/matar com inimigos
                    colisionKill = SDL_HasIntersection(&rectPlayerAttack,&rectEnemies[i]);
                    if(colisionKill==SDL_TRUE && atacar==true && i<nEnemies-1){
                        matarEnemy(&rectEnemies[i],i);
                    }
                    if(colisionKill==SDL_TRUE && atacar==true && i==nEnemies-1){
                        bossLife--;
                    }if(colisionKill==SDL_TRUE && atacar==true && i==nEnemies-1 && bossLife==0){
                        matarEnemy(&rectEnemies[nEnemies-1],nEnemies-1);
                        bossDead=true;
                    }
                }
                //-------------------------

                if(imunidadePlayer>0)
                    imunidadePlayer--;
                
                if(delayFruta>0)
                    delayFruta--;

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
                //colisao inimigos parede
                for(i=0;i<nEnemies;i++){
                    for(j=0;j<nWalls;j++){
                        if(colisao(rectEnemies[i],rectWALL[j])==0)
                            enemyColision[i][0]==true;
                        if(colisao(rectEnemies[i],rectWALL[j])==1)
                            enemyColision[i][1]==true;
                        if(colisao(rectEnemies[i],rectWALL[j])==2)
                            enemyColision[i][2]==true;
                        if(colisao(rectEnemies[i],rectWALL[j])==3)
                            enemyColision[i][3]==true; 
                    }                          
                }
                //fim colisao inimigos parede
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
                        rectPlayerSprite.y = 256;
                        if(rectPlayerSprite.x <=320){
                            rectPlayerSprite.x += 64;
                        }
                        if(rectPlayerSprite.x>320){
                            rectPlayerSprite.x = 0;
                            rectPlayerSprite.y = 0;
                            atacar=0;
                        }
                        if(framedelay > frameTimeSpriteAtack){
                            //SDL_Delay((framedelay) - frameTimeSprite);
                        }
                    }
                    //desce
                    else if(lastside==1){
                        rectPlayerSprite.y = 384;
                        if(rectPlayerSprite.x <= 320){
                            rectPlayerSprite.x += 64;
                        }
                        if(rectPlayerSprite.x>320){
                            rectPlayerSprite.x = 0;
                            rectPlayerSprite.y = 128;
                            atacar=0;
                        }
                        if(framedelay > frameTimeSpriteAtack){
                            //SDL_Delay((framedelay) - frameTimeSprite);
                        }
                    }
                    //esquerda
                    else if(lastside==3){
                        rectPlayerSprite.y = 320;
                        if(rectPlayerSprite.x <=320){
                            rectPlayerSprite.x += 64;
                        }
                        if(rectPlayerSprite.x>320){
                            rectPlayerSprite.x = 0;
                            rectPlayerSprite.y = 64;
                            atacar=0;
                        }
                        if(framedelay > frameTimeSpriteAtack){
                            //SDL_Delay((framedelay) - frameTimeSprite);
                        }
                    }
                    //direita
                    else if(lastside==2){
                        rectPlayerSprite.y = 448;
                        if(rectPlayerSprite.x<=320){
                            rectPlayerSprite.x += 64;
                        }
                        if(rectPlayerSprite.x>320){
                            rectPlayerSprite.x = 0;
                            rectPlayerSprite.y = 192;
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
                    rectPlayerSprite.y = 0;
                    if(rectPlayerSprite.x <=512){
                        rectPlayerSprite.x += 64;
                    }
                    if(rectPlayerSprite.x>512){
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
                    for(i=0;i<nFruits;i++){
                        rectFRUTAS[i].y += speedPlayer;
                    }
                    for(i=0;i<nWalls;i++){
                        rectWALL[i].y += speedPlayer;
                    }
                }
                else if(desce==true){
                    for(i=0;i<nEnemies;i++){
                        rectEnemies[i].y -= speedPlayer;
                    }
                    rectPlayerPosMap.y +=speedPlayer;
                    rectBackground.y +=speedPlayer;
                    rectPlayerSprite.y = 128;
                    if(rectPlayerSprite.x <=512){
                        rectPlayerSprite.x += 64;
                    }
                    if(rectPlayerSprite.x>512){
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
                    for(i=0;i<nFruits;i++){
                        rectFRUTAS[i].y -= speedPlayer;
                    }
                    for(i=0;i<nWalls;i++){
                        rectWALL[i].y -= speedPlayer;
                    }
                }
                else if(esquerda==true){
                    for(i=0;i<nEnemies;i++){
                        rectEnemies[i].x += speedPlayer;
                    }
                    rectPlayerPosMap.x -=speedPlayer;
                    rectBackground.x -=speedPlayer;
                    rectPlayerSprite.y = 64;
                    if(rectPlayerSprite.x <=512){
                        rectPlayerSprite.x += 64;
                    }
                    if(rectPlayerSprite.x>512){
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
                    for(i=0;i<nFruits;i++){
                        rectFRUTAS[i].x += speedPlayer;
                    }
                    for(i=0;i<nWalls;i++){
                        rectWALL[i].x += speedPlayer;
                    }
                }
                else if(direita==true){
                    for(i=0;i<nEnemies;i++){
                        rectEnemies[i].x -= speedPlayer;
                    }
                    rectPlayerPosMap.x +=speedPlayer;
                    rectBackground.x +=speedPlayer;
                    rectPlayerSprite.y = 192;
                    if(rectPlayerSprite.x <=512){
                        rectPlayerSprite.x += 64;
                    }
                    if(rectPlayerSprite.x>512){
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
                    for(i=0;i<nFruits;i++){
                        rectFRUTAS[i].x -= speedPlayer;
                    }
                    for(i=0;i<nWalls;i++){
                        rectWALL[i].x -= speedPlayer;
                    }
                }
                //fim locomocao
                //inicio enemy move, 50% andar 50% parado
                for(i=0;i<nEnemies;i++){
                    enemyMove = rand() % 128+1;
                    if(i<nEnemies-1){
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
                    if(i==nEnemies-1){
                            enemyMoveMinotaur = rand() % 8+1;
                            
                            if(enemyMove<=4 || minotaurAtack==true){
                                if(rectspriteEnemies[nEnemies-1].x<=432){
                                    rectspriteEnemies[nEnemies-1].x += 47;
                                    minotaurAtack=true;
                                }
                                if(rectspriteEnemies[nEnemies-1].x>432){
                                    rectspriteEnemies[nEnemies-1].x=0;
                                    minotaurAtack=false;
                                }
                            }
                    }
                }
                //--------------------------------------
                if(PlayerLife==0){
                    SDL_Delay(1000);
                    statusGame=7;
                }
                if(bossDead==true){
                    statusGame=8;
                }

                frameTime = SDL_GetTicks() - framestart;
                SDL_RenderClear(render);
                SDL_RenderCopy(render, texBACKGROUND, &rectBackground, NULL);
                if(bossDead==true)
                    SDL_RenderCopy(render, texBACKGROUNDsecret, &rectBackground, NULL);
                SDL_RenderCopy(render, texturePlayer, &rectPlayerSprite, &rectPlayer);
                for(i=0;i<nFruits;i++)
                    SDL_RenderCopy(render, texFRUTAS[i], NULL, &rectFRUTAS[i]);
                SDL_RenderCopy(render, texMAGE_START_Game, &rectMAGE_START_GameSprite, &rectMAGE_START_Game);
                SDL_RenderCopy(render, texBALAO_02[0], NULL, &rectBALAO_02[0]);
                for(i=0;i<nEnemies;i++){
                    SDL_RenderCopy(render, texEnemies[i], &rectspriteEnemies[i], &rectEnemies[i]);
                }
                if(mageStatus!=0 && mageStatus!=5)
                    SDL_RenderCopy(render, texDialog_01, &rectDialog_01Sprite, &rectDialog_01);
                SDL_RenderCopy(render,texHeart,&rectHeartSprite,&rectHeart);
                SDL_RenderCopy(render, texInvFruta,&rectInvFrutaSprite,&rectInvFruta);
                
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
        else if(statusGame==6){
                while (SDL_PollEvent(&evento)){
                switch (evento.type){
                    case SDL_QUIT:
                        close_requested = true;
                    break;
                    case SDL_KEYDOWN:
                        switch (evento.key.keysym.sym){
                            case SDLK_RETURN:
                                statusGame=4;
                                break;
                            case SDLK_v:
                                statusGame=4;
                                break;
                            case SDLK_e:
                                statusGame=4;
                                break;
                            case SDLK_SPACE:
                                statusGame=4;
                                break;
                            case SDLK_END:
                                close_requested=true;
                                break;
                            
                        }
                    break;
                    }
                }
                SDL_RenderClear(render);
                SDL_RenderCopy(render, texTutorial, NULL, NULL);
                SDL_RenderPresent(render);
                
                    
        }
        //menu morreu
        
        else if(statusGame==7){
            while (SDL_PollEvent(&evento)){
                switch (evento.type){
                    case SDL_QUIT:
                        close_requested = true;
                    break;
                    case SDL_KEYDOWN:
                        switch (evento.key.keysym.sym){
                            case SDLK_SPACE:
                                close_requested=true;
                                break;
                            
                        }
                    break;
                    }
                }
                SDL_RenderClear(render);
                SDL_RenderCopy(render, texDead, NULL, NULL);
                SDL_RenderPresent(render);
        }
        else if(statusGame==8){
            while (SDL_PollEvent(&evento)){
                switch (evento.type){
                    case SDL_QUIT:
                        close_requested = true;
                    break;
                    case SDL_KEYDOWN:
                        switch (evento.key.keysym.sym){
                            case SDLK_SPACE:
                                close_requested=true;
                                break;
                            
                        }
                    break;
                    }
                }
                SDL_RenderClear(render);
                SDL_RenderCopy(render, texENDGAME, NULL, NULL);
                SDL_RenderPresent(render);
        }
        
    }
        for(i=0;i<nEnemies;i++){
            SDL_DestroyTexture(texEnemies[i]);
        }
        for(i=0;i<nFruits;i++){
            SDL_DestroyTexture(texFRUTAS[i]);
        }
        for(i=0;i>nBalao1;i++){
            SDL_FreeSurface(surfBALAO_01[i]);
            SDL_DestroyTexture(texBALAO_01[i]);
        }for(i=0;i>nBalao2;i++){
            SDL_FreeSurface(surfBALAO_02[i]);
            SDL_DestroyTexture(texBALAO_02[i]);
        }
        SDL_FreeSurface(surfDialog_01);
        SDL_DestroyTexture(texDialog_01); 
        SDL_FreeSurface(surfStory);
        SDL_DestroyTexture(texStory);       
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
    char tempo[50], pontuacaoPlayerString[50];
    sprintf(tempo,"Você jogou por %d segundos",timeOpened);
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, version, tempo, window);
    sprintf(pontuacaoPlayerString,"Voce fez %d pontos",pontuacaoPlayer);
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, version, pontuacaoPlayerString, window);
	finalizar();
    return 0;
}

//------------------------------------------------------------------------------------//

void consumirFruta(){
    printf("antes vida = %d\n", *lifeCount);
    if(*lifeCount<nHearts && frutaInvCount<=4 && frutaInvCount>0 && delayFruta==0){
        if(*lifeCount%2==0){
            frutaInvCount-=1;
            *lifeCount+=2;
            rectplusOneLife->x-= 72;
            rectFullLife->x += 216;
            delayFruta=30;
        }
        else if(*lifeCount%2!=0 && *lifeCount!=11){
            frutaInvCount-=1;
            *lifeCount+=2;
            rectplusOneLife->x-= 72;
            rectFullLife->x += 216;
            delayFruta=30;
        }
        else if(*lifeCount==11){
            frutaInvCount-=1;
            *lifeCount+=1;
            rectplusOneLife->x-= 72;
            rectFullLife->x += 108;
            delayFruta=30;
        }
    }
    printf("depois vida = %d\n", *lifeCount);
}

void plusOneLife(){
    if(*lifeCount<nHearts){
        *lifeCount+=2;
        if(rectFullLife->x<1292)
            rectFullLife->x += 216;
    }
    if(*lifeCount>=nHearts && frutaInvCount<4){
        frutaInvCount+=1;
        rectplusOneLife->x +=72;
        
    }
    printf("depois vida = %d\n", *lifeCount);
}
void receiveDamage(){
    if(imunidadePlayer==0){
    printf("damage antes vida = %d\n", *lifeCount);

        *lifeCount-=1;
        rectFullLife->x -= 108;
        imunidadePlayer=valueImunidadeHit;

    printf("damage depois vida = %d\n", *lifeCount);
    }
}

void fullLife(){
    *lifeCount=nHearts;
    rectFullLife->x = 1292;
}

SDL_Rect matarEnemy(SDL_Rect* enemy,int i){
    enemy->x = 10000;
    enemy->y = 10000;
    pontuacaoPlayer+=10;
    printf("Matou/sumiu inimigo : %d\n", i);
    return *enemy;
}

SDL_Rect pegarFrutas(SDL_Rect* fruta){
    fruta->x = 10000;
    fruta->y = 10000;
    pontuacaoPlayer+=3;
    printf("Pegou fruta\n");
    return *fruta;
}

bool colisaoMatar(SDL_Rect ent_1,SDL_Rect ent_2){

    if( ent_1.x+ent_1.w>=ent_2.w && 
        ent_2.x+ent_2.w>= ent_1.x && 
        ent_1.y+ent_1.h>=ent_2.y && 
        ent_2.y+ent_2.h>=ent_1.y){
        return true;
    }
    else
        return false;
}

int colisao(SDL_Rect ent_1,SDL_Rect ent_2){
    int colide=-1;
    if((ent_1.y - speedPlayer <= ent_2.y + ent_2.h) && (ent_1.y + ent_1.h >= ent_2.y + ent_2.h) && ((ent_1.x >=ent_2.x && ent_1.x <= ent_2.x + ent_2.w) || (ent_1.x + ent_1.w >= ent_2.x && ent_1.x + ent_1.w <= ent_2.x + ent_2.w)  )){
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
void playHowTo(){
    Mix_Chunk *howto = Mix_LoadWAV("recursos/howtoplay.mp3");
    Mix_Volume(-1,MIX_MAX_VOLUME/6);
    Mix_PlayChannel(-1,howto,-1);
}

void fadeAudio(){
    Mix_FadeOutChannel(-1, 1000);
}

void playGameplay(){
    Mix_Chunk *gameplay = Mix_LoadWAV("recursos/gameplay.mp3");
    Mix_Volume(-1,MIX_MAX_VOLUME/5);
    Mix_PlayChannel(-1,gameplay,-1);
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

