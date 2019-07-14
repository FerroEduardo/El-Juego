//ENEMY1----------
    //SDL_Surface *surfGreenEnemy = IMG_Load("recursos/green_enemy.png");
    //SDL_Texture *texGreenEnemy = SDL_CreateTextureFromSurface(render, surfGreenEnemy);
    //SDL_Surface *surfRedEnemy = IMG_Load("recursos/red_enemy.png");
    //SDL_Texture *texRedEnemy = SDL_CreateTextureFromSurface(render, surfRedEnemy);
    //SDL_Surface *surfTopEnemy = IMG_Load("recursos/top_enemy.png");
    //SDL_Texture *texTopEnemy = SDL_CreateTextureFromSurface(render, surfTopEnemy);
    //SDL_Surface *surfPurpleEnemy = IMG_Load("recursos/Purple_enemy.png");
    //SDL_Texture *texPurpleEnemy = SDL_CreateTextureFromSurface(render, surfPurpleEnemy);
    for(i=0;i<nEnemies;i++){
        rectspriteEnemies[i].x = 0;
        rectspriteEnemies[i].y = 94;
        rectspriteEnemies[i].w = 60;
        rectspriteEnemies[i].h = 47;
        rectEnemies[i].w = 180;
        rectEnemies[i].h = 141;
    }
    //reeeeeeeeeeeeeeeeeeeeeeeeed---------------------------------------------------------
    surfEnemies[0] = IMG_Load("recursos/red_enemy.png");
    texEnemies[0] = SDL_CreateTextureFromSurface(render, surfEnemies[0]);
    rectEnemies[0].x = 1060-rectBackground.x; rectEnemies[0].y = 1487-rectBackground.y;
    surfEnemies[1] = IMG_Load("recursos/red_enemy.png");
    texEnemies[1] = SDL_CreateTextureFromSurface(render, surfEnemies[1]);
    rectEnemies[1].x = 1344-rectBackground.x; rectEnemies[1].y = 1410-rectBackground.y;
    surfEnemies[2] = IMG_Load("recursos/red_enemy.png");
    texEnemies[2] = SDL_CreateTextureFromSurface(render, surfEnemies[2]);
    rectEnemies[2].x = 1277-rectBackground.x; rectEnemies[2].y = 1364-rectBackground.y;
    surfEnemies[3] = IMG_Load("recursos/red_enemy.png");
    texEnemies[3] = SDL_CreateTextureFromSurface(render, surfEnemies[3]);
    rectEnemies[3].x = 1213-rectBackground.x; rectEnemies[3].y = 1763-rectBackground.y;
    surfEnemies[4] = IMG_Load("recursos/red_enemy.png");
    texEnemies[4] = SDL_CreateTextureFromSurface(render, surfEnemies[4]);
    rectEnemies[4].x = 1207-rectBackground.x; rectEnemies[4].y = 1964-rectBackground.y;
    surfEnemies[5] = IMG_Load("recursos/red_enemy.png");
    texEnemies[5] = SDL_CreateTextureFromSurface(render, surfEnemies[5]);
    rectEnemies[5].x = 1084-rectBackground.x; rectEnemies[5].y = 2129-rectBackground.y;
    surfEnemies[6] = IMG_Load("recursos/red_enemy.png");
    texEnemies[6] = SDL_CreateTextureFromSurface(render, surfEnemies[6]);
    rectEnemies[6].x = 733-rectBackground.x; rectEnemies[6].y = 2165-rectBackground.y;
    surfEnemies[7] = IMG_Load("recursos/red_enemy.png");
    texEnemies[7] = SDL_CreateTextureFromSurface(render, surfEnemies[7]);
    rectEnemies[7].x = 580-rectBackground.x; rectEnemies[7].y = 2087-rectBackground.y;
    surfEnemies[8] = IMG_Load("recursos/red_enemy.png");
    texEnemies[8] = SDL_CreateTextureFromSurface(render, surfEnemies[8]);
    rectEnemies[8].x = 640-rectBackground.x; rectEnemies[8].y = 1892-rectBackground.y;
    surfEnemies[9] = IMG_Load("recursos/red_enemy.png");
    texEnemies[9] = SDL_CreateTextureFromSurface(render, surfEnemies[9]);
    rectEnemies[9].x = 550-rectBackground.x; rectEnemies[9].y = 1775-rectBackground.y;
    surfEnemies[10] = IMG_Load("recursos/red_enemy.png");
    texEnemies[10] = SDL_CreateTextureFromSurface(render, surfEnemies[10]);
    rectEnemies[10].x = 634-rectBackground.x; rectEnemies[10].y = 1601-rectBackground.y;
    surfEnemies[11] = IMG_Load("recursos/red_enemy.png");
    texEnemies[11] = SDL_CreateTextureFromSurface(render, surfEnemies[11]);
    rectEnemies[11].x = 793-rectBackground.x; rectEnemies[11].y = 1490-rectBackground.y;
    surfEnemies[12] = IMG_Load("recursos/red_enemy.png");
    texEnemies[12] = SDL_CreateTextureFromSurface(render, surfEnemies[12]);
    rectEnemies[12].x = 526-rectBackground.x; rectEnemies[12].y = 1421-rectBackground.y;
    //GREEEEEEEEEEEEEEEN-------------------------------------------------------------------
    surfEnemies[13] = IMG_Load("recursos/green_enemy.png");
    texEnemies[13] = SDL_CreateTextureFromSurface(render, surfEnemies[13]);
    rectEnemies[13].x = 1029-rectBackground.x; rectEnemies[13].y = 2643-rectBackground.y;
    surfEnemies[14] = IMG_Load("recursos/green_enemy.png");
    texEnemies[14] = SDL_CreateTextureFromSurface(render, surfEnemies[14]);
    rectEnemies[14].x = 2690-rectBackground.x; rectEnemies[14].y = 2751-rectBackground.y;
    surfEnemies[15] = IMG_Load("recursos/green_enemy.png");
    texEnemies[15] = SDL_CreateTextureFromSurface(render, surfEnemies[15]);
    rectEnemies[15].x = 1500-rectBackground.x; rectEnemies[15].y = 2862-rectBackground.y;
    surfEnemies[16] = IMG_Load("recursos/green_enemy.png");
    texEnemies[16] = SDL_CreateTextureFromSurface(render, surfEnemies[16]);
    rectEnemies[16].x = 777-rectBackground.x; rectEnemies[16].y = 2748-rectBackground.y;
    surfEnemies[17] = IMG_Load("recursos/green_enemy.png");
    texEnemies[17] = SDL_CreateTextureFromSurface(render, surfEnemies[17]);
    rectEnemies[17].x = 549-rectBackground.x; rectEnemies[17].y = 2862-rectBackground.y;
    surfEnemies[18] = IMG_Load("recursos/green_enemy.png");
    texEnemies[18] = SDL_CreateTextureFromSurface(render, surfEnemies[18]);
    rectEnemies[18].x = 621-rectBackground.x; rectEnemies[18].y = 3759-rectBackground.y;
    surfEnemies[19] = IMG_Load("recursos/green_enemy.png");
    texEnemies[19] = SDL_CreateTextureFromSurface(render, surfEnemies[19]);
    rectEnemies[19].x = 651-rectBackground.x; rectEnemies[19].y = 3924-rectBackground.y;
    surfEnemies[20] = IMG_Load("recursos/green_enemy.png");
    texEnemies[20] = SDL_CreateTextureFromSurface(render, surfEnemies[20]);
    rectEnemies[20].x = 846-rectBackground.x; rectEnemies[20].y = 4023-rectBackground.y;
    surfEnemies[21] = IMG_Load("recursos/green_enemy.png");
    texEnemies[21] = SDL_CreateTextureFromSurface(render, surfEnemies[21]);
    rectEnemies[21].x = 930-rectBackground.x; rectEnemies[21].y = 394-rectBackground.y;

















    //OLD--------------------------------------------------------------
    surfEnemies[0] = IMG_Load("recursos/green_enemy.png");
    texEnemies[0] = SDL_CreateTextureFromSurface(render, surfEnemies[0]);
    rectEnemies[0].x = 3148-rectBackground.x; rectEnemies[0].y = 644-rectBackground.y; rectEnemies[0].w = 180; rectEnemies[0].h = 141;
    //enemy2-----------
    surfEnemies[1] = IMG_Load("recursos/red_enemy.png");
    texEnemies[1] = SDL_CreateTextureFromSurface(render, surfEnemies[1]);
    rectEnemies[1].x = 583-rectBackground.x; rectEnemies[1].y = 1248-rectBackground.y; rectEnemies[1].w = 180; rectEnemies[1].h = 141;

    
    //-----------------
    //ENEMY3----------
    
    surfEnemies[2] = IMG_Load("recursos/purple_enemy.png");
    texEnemies[2] = SDL_CreateTextureFromSurface(render, surfEnemies[2]);
    rectEnemies[2].x = 5259-rectBackground.x; rectEnemies[2].y = 1257-rectBackground.y; rectEnemies[2].w = 180; rectEnemies[2].h = 141;


    //enemy4-----------
    surfEnemies[3] = IMG_Load("recursos/top_enemy.png");
    texEnemies[3] = SDL_CreateTextureFromSurface(render, surfEnemies[3]);
    rectEnemies[3].x = 4953-rectBackground.x; rectEnemies[3].y = 1251-rectBackground.y; rectEnemies[3].w = 180; rectEnemies[3].h = 141;
    //-----------------

    //enemy5-----------
    /*
    surfEnemies[4] = IMG_Load("recursos/minotauro.png");
    texEnemies[4] = SDL_CreateTextureFromSurface(render, surfEnemies[4]);
    rectEnemies[4].x = 894-rectBackground.x; rectEnemies[4].y = 1782-rectBackground.y; rectEnemies[4].w = 400; rectEnemies[4].h = 370;
    rectspriteEnemies[4].x = 8;rectspriteEnemies[4].y = 154; rectspriteEnemies[4].w = 40; rectspriteEnemies[4].h = 37;
    */
    surfEnemies[4] = IMG_Load("recursos/minotauro_invertido.png");
    texEnemies[4] = SDL_CreateTextureFromSurface(render, surfEnemies[4]);
    rectEnemies[4].x = 894-rectBackground.x; rectEnemies[4].y = 1782-rectBackground.y; rectEnemies[4].w = 400; rectEnemies[4].h = 370;
    //50++
    //-----------------