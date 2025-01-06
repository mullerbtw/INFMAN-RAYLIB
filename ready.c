#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#define MATRIXLINES 15
#define MATRIXCOLUMNS 200
#define NUMBOMBS 5
#define PLAY 1
#define LEADERBOARD 2
#define EXIT 3
#define NUMJOG 5

const int screenWidth = 1200;
const int screenHeight = 600;
const int gameWidth = 6400;

typedef struct
{
	Texture2D texture;
	Vector2 movement;
	unsigned numTiles;
	float width;
	int speed;
	int gravity;
	Vector2 position;
	Rectangle frameRec;
} MEGAMAN;

typedef struct
{
	Texture2D texture;
	Vector2 position;
	Rectangle frameRec;
} TIROCOLISAO;

typedef struct
{
	Texture2D texture;
	Vector2 position;
	Rectangle frameRec;
} ENEMY;

typedef struct
{
	Texture2D texture;
	Vector2 position;
	Rectangle frameRec;
} BOX;

typedef struct
{
	Texture2D texture;
} BACKGROUND;

typedef struct
{
	Texture2D texture;
} SPIKE;

typedef struct
{
	Texture2D texture;
	Vector2 position;
} TIRO;

typedef struct
{
    Texture2D texture;
    Vector2 position;
    Rectangle frameRec;
} CHECKPOINT;

typedef struct
{
    char nomeJog[12];
    int pontuacao;
} POSICOES;

void binaryFileRead(POSICOES leaderboard [NUMJOG])
{
    FILE *fp;
    fp = fopen("top_scores.bin", "rb");

    if(fp != NULL)
    {
        if(fread(leaderboard, sizeof(POSICOES), NUMJOG, fp) != NUMJOG)
            printf("ERRO NA LEITURA!");
        else
            printf("ARQUIVO LIDO COM SUCESSO!");
    }
    fclose(fp);
}

void binaryFilePrint(POSICOES leaderboard[])
{
    InitWindow(screenWidth, screenHeight, "LEADERBOARD");
    SetTargetFPS(60);
    
    while (!WindowShouldClose())
    {
        ClearBackground(DARKBLUE);
        
        DrawText
        (
            leaderboard [0].nomeJog,
            screenWidth / 2 - MeasureText(leaderboard [0].nomeJog, 10),
            50,
            10,
            WHITE
        );
        
        DrawText
        (
            TextFormat("%i", leaderboard [0].pontuacao),
            screenWidth / 2 + 20,
            50,
            10,
            WHITE
        );
        
        DrawText
        (
            leaderboard [1].nomeJog,
            screenWidth / 2 - MeasureText(leaderboard [1].nomeJog, 10),
            150,
            10, 
            WHITE
        );
        
        DrawText
        (
            TextFormat("%i", leaderboard [1].pontuacao),
            screenWidth / 2 + 20,
            150,
            10,
            WHITE
        );
        
        DrawText
        (
            leaderboard [2].nomeJog,
            screenWidth / 2 - MeasureText(leaderboard [2].nomeJog, 10),
            250,
            10,
            WHITE
        );
        
        DrawText
        (
            TextFormat("%i", leaderboard [2].pontuacao),
            screenWidth / 2 + 20,
            250,
            10,
            WHITE
        );
        
        DrawText
        (
            leaderboard [3].nomeJog,
            screenWidth / 2 - MeasureText(leaderboard [3].nomeJog, 10),
            350,
            10,
            WHITE
        );
        
        DrawText
        (
            TextFormat("%i", leaderboard [3].pontuacao),
            screenWidth / 2 + 20,
            350,
            10,
            WHITE
        );
        
        DrawText
        (
            leaderboard [4].nomeJog,
            screenWidth / 2 - MeasureText(leaderboard [4].nomeJog, 10),
            450,
            10,
            WHITE
        );
        
        DrawText
        (
            TextFormat("%i", leaderboard [4].pontuacao),
            screenWidth / 2 + 20,
            450,
            10,
            WHITE
        );
        
        DrawText
        (
            leaderboard [5].nomeJog,
            screenWidth / 2 - MeasureText(leaderboard [5].nomeJog, 10),
            550,
            10,
            WHITE
        );
        
        DrawText
        (
            TextFormat("%i", leaderboard [5].pontuacao),
            screenWidth / 2 + 20,
            550,
            10,
            WHITE
        );
        
        EndDrawing();
    }
    
    CloseWindow();
}

int menu()
{
   InitWindow(screenWidth, screenHeight, "MENU");

   Texture2D title = LoadTexture("INFMAN.png");
   
   SetTargetFPS(60);
   
   while(!WindowShouldClose())
   {
      BeginDrawing();

      ClearBackground(DARKBLUE);

      DrawTexture(title, screenWidth / 2 - 426 / 2, 100, RAYWHITE);
      DrawText("PLAY", screenWidth / 2.0f - MeasureText("PLAY", 30) / 2.0f, 280, 30, RAYWHITE);
      DrawText("LEADERBOARD", screenWidth / 2.0f - MeasureText("LEADERBOARD", 30) / 2.0f , 360, 30, RAYWHITE);
      DrawText("EXIT", screenWidth / 2.0f - MeasureText("EXIT", 30) / 2.0f, 440, 30, RAYWHITE);

      Rectangle playRec = (Rectangle)
      {
         screenWidth / 2.0f - MeasureText("PLAY", 30) / 2.0f,
         280,
         MeasureText("PLAY", 30),
         30
      };

      Rectangle leaderboardRec = (Rectangle)
      {
         screenWidth / 2.0f - MeasureText("LEADERBOARD", 30) / 2.0f,
         360,
         MeasureText("LEADERBOARD", 30),
         30
      };

      Rectangle exitRec = (Rectangle)
      {
         screenWidth / 2.0f - MeasureText("EXIT", 30) / 2.0f,
         440,
         MeasureText("EXIT", 30),
         30
      };

      if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
      {
         if(CheckCollisionPointRec(GetMousePosition(), playRec))
         // if button pressed on 'play' area
         {
            return PLAY;
         }

         if(CheckCollisionPointRec(GetMousePosition(), leaderboardRec))
         // if button pressed on 'leaderboard' area
         {
            return LEADERBOARD;
            // only load .bin file if leaderboard chosen
         }

         if(CheckCollisionPointRec(GetMousePosition(), exitRec))
         // if button pressed on 'exit' area
         {
            return EXIT;
         }
      }
      EndDrawing();
   }
   UnloadTexture(title);
   CloseWindow();
   return 0;
}

int enterPlayerName(char *ptrNomeJog)
{
    ptrNomeJog [12] = '\0'; // último spot está com terminador, virá como parâmetro (?)
    int charCount = 0;
    
    InitWindow(screenWidth, screenHeight, "UPDATE LEADERBOARD");
    SetTargetFPS(60);
    
    while (!WindowShouldClose())
    {   
        int key = GetCharPressed();
       
        while (key > 0 && charCount <= 11)
        {
            if ((char)key == '.')
            {
                return 0;
            }
            ptrNomeJog [charCount] = (char)key;
            ptrNomeJog [charCount + 1] = '\0';
            charCount++;
            key = GetCharPressed();
        }
        
        BeginDrawing();
        ClearBackground(DARKBLUE);
        
        
        DrawText("ENTER YOUR NAME:", (screenWidth / 2) - (MeasureText("ENTER YOUR NAME:", 30) / 2), (screenHeight / 3), 30,  WHITE);
        DrawText(ptrNomeJog, (screenWidth / 2) - ((MeasureText(ptrNomeJog, 30)) / 2), screenHeight / 2, 30, WHITE);
        
        
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}

void binaryFileSave(POSICOES leaderboard[NUMJOG])
{
    FILE *fp;
    fp = fopen("top_score.bin", "wb");
    if(fp != NULL)
    {
        if(fwrite(leaderboard, sizeof(POSICOES), NUMJOG, fp) != NUMJOG)
            printf("ERRO NA ESCRITA!");
        else
            printf("ARQUIVO ESCRITO COM SUCESSO!\n");
    }
    fclose(fp);
}

void drawTiro(TIRO tiro, int correcaoTiro)
{
	DrawTexture
    (
        tiro.texture,
        tiro.position.x - correcaoTiro,
        tiro.position.y,
        WHITE);
}

void atualizaTiro(TIRO *tiro, Vector2 megamanPos)
{
	tiro->position.x = megamanPos.x;
	tiro->position.y = megamanPos.y;
}

void readMatrix(char matrix[][MATRIXCOLUMNS])
{
	FILE *fp;
	if ((fp = fopen("matrix.txt", "r")) != NULL)
	{
		for (int l = 0; l < MATRIXLINES; l++)
		{
			for (int c = 0; c < MATRIXCOLUMNS; c++)
			{
				fscanf(fp, "%c", &matrix[l][c]);
				if (matrix[l][c] == '\n')
					matrix[l][c] = ' ';
			}
		}
		fclose(fp);
	}
}

void initMegamanPosition(MEGAMAN **megaman, char matrix[][MATRIXCOLUMNS])
{
	for (int l = 0; l < MATRIXLINES; l++)
	{
		for (int c = 0; c < MATRIXCOLUMNS; c++)
		{
			if (matrix[l][c] == 'P')
			{
				(*megaman)->position = (Vector2)
                {
                    (c * (*megaman)->texture.width),
                    (l * (*megaman)->texture.height)
                };
			}
		}
	}
}

void initParedePosition(char matrix [][MATRIXCOLUMNS], BOX *parede)
{
	for (int l = 0; l < MATRIXLINES; l++)
	{
		for (int c = 0; c < MATRIXCOLUMNS; c++)
		{
			if (matrix[l][c] == 'D')
			{
				parede->frameRec = (Rectangle)
                {
                    c * parede->texture.width,
                    l * parede->texture.height,
                    parede->texture.width,
                    parede->texture.height
                };
			}
		}
	}
}

void initFloorPosition(char matrix[][MATRIXCOLUMNS], BOX *floor)
{
	for (int l = 0; l < MATRIXLINES; l++)
	{
		for (int c = 0; c < MATRIXCOLUMNS; c++)
		{
			if (matrix[l][c] == 'B')
			{
				floor->frameRec = (Rectangle)
                {
                    c * floor->texture.width,
                    l * floor->texture.height,
                    floor->texture.width,
                    floor->texture.height
                };
			}
		}
	}
}

void initCameraSettings(Camera2D *camera, MEGAMAN *megaman)
{
	camera->offset = (Vector2){0, 0};
	camera->target = (Vector2){megaman->position.x, 0};
	camera->rotation = 0;
	camera->zoom = 1.0;
}

void drawBackground(BACKGROUND *background)
{
	for (int i = 0; i <= 9; i++)
	{
		DrawTexture
        (
            background->texture,
            i * background->texture.width,
            0.0f,
            BLUE
        );
	}
}

int bombDano(TIROCOLISAO *tiroColisao, ENEMY *bomb, int *pontos)
{
	Rectangle tiroRec =
    {
        tiroColisao->position.x,
        tiroColisao->position.y,
        tiroColisao->texture.width,
        tiroColisao->texture.height
    };
	Rectangle bombRec =
    {
        bomb->position.x,
        bomb->position.y,
        bomb->texture.width,
        bomb->texture.height
    };
	if (CheckCollisionRecs(tiroRec, bombRec))
	{
		bomb->position.x = 8888888;
		bomb->position.y = 8888888;
        *pontos += 200;
		return 1;
	}
}

int bombMovement(ENEMY *bomb, MEGAMAN *megaman, int *vidas)
{
	if (megaman->position.x - bomb->position.x < 100)
	{
		// inimigo a direita do megaman
		if (bomb->position.x > megaman->position.x)
		{
			bomb->position.x = bomb->position.x - 1;
		}
		// inimigo a esquerda do megaman
		if (bomb->position.x < megaman->position.x)
		{
			bomb->position.x = bomb->position.x + 1;
		}
		// above or below
		if (bomb->position.y > megaman->position.y)
		{
			bomb->position.y = bomb->position.y - 1;
		}
        
		if (bomb->position.y < megaman->position.y)
		{
			bomb->position.y = bomb->position.y + 1;
		}
		// colisão com inimigo
		if (fabs(bomb->position.x - megaman->position.x) < 10.0f)
		{
			if (fabs(bomb->position.y - megaman->position.y) < 10.0f)
			{
                *vidas = *vidas - 1;
				return 0;
			}
		}
        else
        {
            return 1;
        }
	}
}

void cameraUpdate(Camera2D *camera, MEGAMAN *megaman)
{
	if (megaman->position.x <= screenWidth / 2)
	{
		camera->target.x = 0;
		camera->offset.x = 0;
	}
    
	if (megaman->position.x >= screenWidth / 2)
	{
		camera->target.x = megaman->position.x;
		camera->offset.x = screenWidth / 2 - ((float)megaman->texture.width / 6);
	}
    
	if (megaman->position.x >= (gameWidth - (screenWidth / 2) - (megaman->texture.width / 6)))
	{
		camera->target.x = gameWidth - (screenWidth / 2) - (megaman->texture.width / 6);
	}
}

bool tileMap(char matrix[][MATRIXCOLUMNS], ENEMY *bomb, MEGAMAN *megaman, BOX *floor, BOX *parede, SPIKE *spike, Sound deathSound, int *vidas, bool *acima)
{
	bool isColliding = false;
	for (int l = 0; l < MATRIXLINES; l++)
	{
		for (int c = 0; c < MATRIXCOLUMNS; c++)
		{
			if (matrix[l][c] == 'B')
			{
				Rectangle megamanRect =
                {
                    megaman->position.x,
                    megaman->position.y,
                    megaman->width,
                    megaman->texture.height
                };
                
				Rectangle floorRect =
                {
                    c * floor->texture.width,
                    l * floor->texture.height,
                    floor->texture.width,
                    floor->texture.height
                };
				if (CheckCollisionRecs(megamanRect, floorRect))
				{
                    isColliding = true;
                    
					if (megaman->movement.y > 0)
					{
						megaman->position.y = floorRect.y - megaman->texture.height;
						megaman->movement.y = 0;
                        
                        *acima = false;
                        
					}
                    
					if (megaman->movement.y < 0)
					{
                        *acima = true;
                        
                        megaman->position.y = floorRect.y + floorRect.height;
                        megaman->movement.y = 0;
					}
				}

				DrawTexture
                (
                    floor->texture,
                    (c * floor->texture.width),
                    (l * floor->texture.height),
                    BLUE
                );
			}
            
			if (matrix[l][c] == 'S')
			{
                Rectangle megamanSpikeRect = 
                {
                    megaman->position.x,
                    megaman->position.y,
                    megaman->width,
                    megaman->texture.height
                };
                
                Rectangle spikeRect =
                {
                    c * spike->texture.width,
                    l * spike->texture.height,
                    spike->texture.width,
                    spike->texture.height
                };
                
                DrawTexture
                (
                    spike->texture,
                    (c * spike->texture.width),
                    (l * spike->texture.height),
                    ORANGE
                );
                
                if (CheckCollisionRecs(megamanSpikeRect, spikeRect))
                {
                    PlaySound(deathSound);
                    megaman->position.x = megaman->position.x - 100;
                    
                    *vidas = *vidas - 1;
                    
                }
			}
			if (matrix[l][c] == 'D')
			{
				DrawTexture
                (
                    parede->texture,
                    (c * parede->texture.width),
                    (l * parede->texture.height),
                    BLUE
                );
                
				Rectangle megamanRect =
                {
                    megaman->position.x,
                    megaman->position.y,
                    megaman->width,
                    megaman->texture.height
                };
                
				Rectangle paredeRect =
                {
                    c * parede->texture.width,
                    l * parede->texture.height,
                    parede->texture.width,
                    parede->texture.height
                };
                
				if (CheckCollisionRecs(megamanRect, paredeRect))
				{
					if (megaman->movement.x < 0)
					{
						megaman->position.x = paredeRect.x + paredeRect.width;
					}

					if (megaman->movement.x > 0)
					{
						megaman->position.x = paredeRect.x - megaman->width;
					}
				}
			}
		}
	}
	return isColliding;
}

void initBombPosition(Vector2 bombPositionArray [], ENEMY bomb [], char matrix [][MATRIXCOLUMNS])
{
    int bombCounter = 0;
    
    Texture2D bombTexture = LoadTexture("bomb.png");
    
    
    for (int l = 0; l < MATRIXLINES; l++)
	{
		for (int c = 0; c < MATRIXCOLUMNS; c++)
		{
            if(matrix [l][c] == 'E')
            {
                bombPositionArray[bombCounter] = (Vector2)
                {
                    c * bombTexture.width,
                    l * bombTexture.height
                };
                bombCounter++;
            }        
        }
    }
}

void initBombStructs(ENEMY bomb [NUMBOMBS], Vector2 bombPositionArray [NUMBOMBS])
{
    for (int i = 0; i < NUMBOMBS; i++)
    {
        bomb [i].texture = LoadTexture("bomb.png");
        bomb [i].position = (Vector2)
        {
            bombPositionArray [i].x,
            bombPositionArray [i].y
        };
        bomb [i].frameRec = (Rectangle)
        {
            bomb [i].position.x,
            bomb [i].position.y, 
            bomb [i].texture.width,
            bomb [i].texture.height
        };
    }
}

void initMegamanStruct(MEGAMAN *megaman, char matrix [][MATRIXCOLUMNS])
{
    megaman->texture = (Texture2D)LoadTexture("megamanWalking.png");
	megaman->movement = (Vector2){0, 0};
	megaman->numTiles = (int)3;
	megaman->width = (float)((float)megaman->texture.width / (float)megaman->numTiles);
	megaman->speed = 5;
	megaman->gravity = 1;
	initMegamanPosition(&megaman, matrix);
	megaman->frameRec = (Rectangle)
    {
        megaman->position.x,
        megaman->position.y,
        megaman->width,
        megaman->texture.height
    };
}

bool arrivedAtCheckpoint(MEGAMAN *megaman, CHECKPOINT *checkpoint)
{
    Rectangle megamanRect = (Rectangle)
    {
        megaman->position.x,
        megaman->position.y,
        megaman->width,
        megaman->texture.height
    };
    
    Rectangle checkpointRect = (Rectangle)
    {
        checkpoint->position.x,
        checkpoint->position.y,
        checkpoint->texture.width,
        checkpoint->texture.height
    };
    
    if (CheckCollisionRecs(megamanRect, checkpointRect))
        return true;
    else 
        return false;
}

int gameplay()
{
	char matrix[MATRIXLINES][MATRIXCOLUMNS];
	readMatrix(matrix);
	bool isMegamanJumping = false;
    bool acima = false;
	unsigned frameDelay = 5;
	unsigned frameDelayCounter = 0;
    unsigned endFrameDelay = 100;
    unsigned endFrameDelayCounter = 0;
	int xStartingPosition = 30;
	int correcaoTiro;
    int pontos = 0;
    int vidas = 5;
    int pontosPositionUpdate = 0;
    int vidaPositionUpdate = 0;
    char pontosTexto [20];
    char vidaTexto [20];
    
    
    int megamanPosXTemp = 0;
    int megamanPosYTemp = 0;
    
	InitWindow(screenWidth, screenHeight, "GAMEPLAY");

	MEGAMAN megaman;
    initMegamanStruct(&megaman, matrix);
        
    // i hope this works
    ENEMY bomb [NUMBOMBS];
    Vector2 bombPositionArray [NUMBOMBS];
    initBombPosition(bombPositionArray, bomb, matrix);
    initBombStructs(bomb, bombPositionArray);
    
	BACKGROUND background;
	background.texture = LoadTexture("background.png");
    
    Texture2D gameOver = LoadTexture("blackoutSkull.png");
    Texture2D paredeT = LoadTexture("box.png");
    
	BOX parede;
	initParedePosition(matrix, &parede);
	parede.texture = paredeT;
    
	BOX floor;
	initFloorPosition(matrix, &floor);
	floor.texture = paredeT;
    
	SPIKE spike;
	spike.texture = LoadTexture("spike.png");

	TIRO tiro;
	tiro.texture = LoadTexture("laser.png");
	tiro.position = (Vector2){999999999, 99999999};

	TIROCOLISAO tiroColisao;
	tiroColisao.texture = LoadTexture("colisaolaser.png");
	tiroColisao.position = (Vector2){999999999, 99999999};
	tiroColisao.frameRec = (Rectangle)
    {
        tiroColisao.position.x,
        tiroColisao.position.y,
        tiroColisao.texture.width,
        tiroColisao.texture.height
    };
    
    CHECKPOINT checkpoint;
    checkpoint.texture = LoadTexture("colisao.png");
    checkpoint.position = (Vector2)
    {
        gameWidth - checkpoint.texture.width,
        390
    };
    
	Camera2D camera;
	initCameraSettings(&camera, &megaman);
    
	Vector2 TempTiro = (Vector2){999999999, 99999999};
    
	InitAudioDevice();
	Music musica = LoadMusicStream("musica.mp3");
	Sound tiroSound = LoadSound("tiro.mp3");
    Sound deathSound = LoadSound("manDeathScream.ogg");
    Sound moreteSound = LoadSound("maleVoiceScreamingLoudly.wav");
    Sound jumpSound = LoadSound("jump.mp3");
    
    SetTargetFPS(60);
    
    PlayMusicStream(musica);

    
	while (!WindowShouldClose())
	{
		UpdateMusicStream(musica);
		BeginDrawing();
		ClearBackground(WHITE);
		BeginMode2D(camera);
		drawBackground(&background);
        
        pontosPositionUpdate = 0;
		
        DrawTexture(checkpoint.texture, checkpoint.position.x, checkpoint.position.y, GRAY);
        if (arrivedAtCheckpoint(&megaman, &checkpoint))
        {
            break;
        }
        
        if (vidas < 0)
        {
            vidas = 0;
        }
        
        if (megaman.position.x > screenWidth / 2)
        {
            vidaPositionUpdate = 25 + megaman.position.x - screenWidth / 2 ;
        }
        if (megaman.position.x >= (gameWidth - (screenWidth / 2) - (megaman.texture.width / 6))){
            
            vidaPositionUpdate =  gameWidth - (screenWidth / 2) - (megaman.texture.width / 6) - 575 ;
        }
        

        if (megaman.position.x > screenWidth / 2)
        {
            pontosPositionUpdate = 25 + megaman.position.x - screenWidth / 2 ;
        }
        
        if (megaman.position.x >= (gameWidth - (screenWidth / 2) - (megaman.texture.width / 6)))
        {
            pontosPositionUpdate =  gameWidth - (screenWidth / 2) - (megaman.texture.width / 6) - 575 ;      
        }

        sprintf(pontosTexto, "Pontos: %d", pontos);
        
        DrawText
        (   
            pontosTexto,
            10 + pontosPositionUpdate,
            10,
            20,
            ORANGE
        ); 

        sprintf(vidaTexto, "Vida: %d", vidas);
        DrawText
        (
            vidaTexto,
            300 + vidaPositionUpdate,
            10,
            20,
            ORANGE
        ); 
        
        bool isColliding = tileMap(matrix, &bomb [0], &megaman, &floor, &parede, &spike, deathSound, &vidas, &acima);

		if (isColliding)
		{
			isMegamanJumping = false;
		}
        
		else
		{
			isMegamanJumping = true;
		}
        
		if (IsKeyPressed(KEY_C))
		{
			PlaySound(tiroSound);
		}
        
		if (IsKeyDown(KEY_C))
		{
			atualizaTiro(&tiro, megaman.position);
			drawTiro(tiro, correcaoTiro);
            DrawTextureRec
            (
                tiroColisao.texture,
                tiroColisao.frameRec,
                tiroColisao.position,
                WHITE
            );
			tiroColisao.position.x = tiro.position.x - correcaoTiro;
			tiroColisao.position.y = tiro.position.y;
		}
        
		else
		{
			tiroColisao.position = (Vector2){999999999, 99999999};
			atualizaTiro(&tiro, TempTiro);
		}
        
		if (IsKeyDown(KEY_W) && (isColliding)  && !acima)
		{
            PlaySound(jumpSound);
			megaman.movement.y = -4 * megaman.speed + megaman.gravity;
			DrawTextureRec(megaman.texture, megaman.frameRec, megaman.position, WHITE);
            isMegamanJumping = true;
		}
        
		else if (IsKeyDown(KEY_A))
		{
			correcaoTiro = 130;
			isMegamanJumping = false;
			DrawTextureRec
            (
                megaman.texture,
                megaman.frameRec,
                megaman.position,
                WHITE
            );
			megaman.movement.x = -megaman.speed;
			if (megaman.frameRec.width > 0)
				megaman.frameRec.width = -megaman.frameRec.width;
			frameDelayCounter++;
			if (frameDelayCounter >= frameDelay)
			{
				megaman.frameRec.x += megaman.width;
				frameDelayCounter = 0;
			}
		}
        
		else if (IsKeyDown(KEY_D))
		{
			correcaoTiro = -30;
			isMegamanJumping = false;
			DrawTextureRec(megaman.texture, megaman.frameRec, megaman.position, WHITE);
			megaman.movement.x = megaman.speed;
			if (megaman.frameRec.width < 0)
				megaman.frameRec.width = -megaman.frameRec.width;
			frameDelayCounter++;
			if (frameDelayCounter >= frameDelay)
			{
				megaman.frameRec.x += megaman.width;
				frameDelayCounter = 0;
			}
		}
        
		else if (isColliding)
		{
			isMegamanJumping = false;
			DrawTextureRec
            (
                megaman.texture,
                megaman.frameRec,
                megaman.position,
                WHITE
            );
			megaman.movement.x = 0;
			if (megaman.frameRec.width > 0)
				megaman.frameRec.width = +megaman.frameRec.width;
		}
        
		else
		{
			isMegamanJumping = true;
			DrawTextureRec
            (
                megaman.texture,
                megaman.frameRec,
                megaman.position,
                WHITE
            );
		}
        
		megaman.position = Vector2Add(megaman.position, megaman.movement);
		megaman.movement.y += megaman.gravity;
        
		if (megaman.position.x < xStartingPosition - (megaman.texture.width / 6))
		{
			megaman.position.x = xStartingPosition - (megaman.texture.width / 6);
		}
        
		if (megaman.position.x > gameWidth - (megaman.texture.width / 3))
		{
			megaman.position.x = gameWidth - (megaman.texture.width / 3);
		}
        
		DrawTextureRec
        (
            bomb [0].texture,
            bomb [0].frameRec,
            bomb [0].position,
            WHITE
        );
		DrawTextureRec
        (
            bomb [1].texture,
            bomb [1].frameRec,
            bomb [1].position,
            WHITE
        );
        DrawTextureRec
        (
            bomb [2].texture,
            bomb [2].frameRec,
            bomb [2].position,
            WHITE
        );
        DrawTextureRec
        (
            bomb [3].texture,
            bomb [3].frameRec,
            bomb [3].position,
            WHITE
        );
		DrawTextureRec
        (
            bomb [4].texture,
            bomb [4].frameRec,
            bomb [4].position,
            WHITE
        );
        
        megamanPosXTemp = megaman.position.x;
        megamanPosYTemp = megaman.position.y;
        
		if (bombMovement(&bomb [0], &megaman, &vidas) == 0)
        {
			PlaySound(deathSound);
            megaman.position.x = megaman.position.x - 300;
            bomb[0].position.x = bomb[0].position.x + 300;
            bomb[0].position.y = bomb[0].position.y - 100;
        }
        
		if (bombMovement(&bomb [1], &megaman, &vidas) == 0)
        {		
            PlaySound(deathSound);
            megaman.position.x = megaman.position.x - 100;
            bomb[1].position.x = bomb[0].position.x + 300;
            bomb[1].position.y = bomb[0].position.y - 100;
        }       
        
        if (bombMovement(&bomb [2], &megaman, &vidas) == 0)
        {
            PlaySound(deathSound);
            megaman.position.x = megaman.position.x - 100;
            bomb[2].position.x = bomb[2].position.x + 300;
            bomb[2].position.y = bomb[2].position.y - 100;
        }
        
        if (bombMovement(&bomb [3], &megaman, &vidas) == 0)
		{
            PlaySound(deathSound);
            megaman.position.x = megaman.position.x - 100;
            bomb[3].position.x = bomb[3].position.x + 300;
            bomb[3].position.y = bomb[3].position.y - 100;
        }
        
        if (bombMovement(&bomb [4], &megaman, &vidas) == 0)		
        {	
            PlaySound(deathSound);
            megaman.position.x = megaman.position.x - 100;
            bomb[4].position.x = bomb[4].position.x + 300;
            bomb[4].position.y = bomb[4].position.y - 100;
        }
        
        bombDano(&tiroColisao, &bomb [0], &pontos);
        bombDano(&tiroColisao, &bomb [1], &pontos);
        bombDano(&tiroColisao, &bomb [2], &pontos);
        bombDano(&tiroColisao, &bomb [3], &pontos);
        bombDano(&tiroColisao, &bomb [4], &pontos);

		cameraUpdate(&camera, &megaman);
        
        megamanPosXTemp = megaman.position.x;
        megamanPosYTemp = megaman.position.y;
         
         
        if (vidas == 1)
        {
            deathSound = moreteSound; 
        }
        
        if (vidas == 0)
        {
            break;
        }
        
        EndDrawing();
        
	}
	CloseAudioDevice();
	CloseWindow();
	return pontos; // in order to use in main()
}

int main()
{
    POSICOES leaderboard [NUMJOG];
    binaryFileRead(leaderboard);
    // call necessary binary file function
    int pontos;
    int option = 0;
    char nomeJog [12];

    while (option != 3)
    {
        option = menu();
        
        if(option == EXIT)
            return 0;
        
        binaryFileRead(leaderboard);
        
        if(option == PLAY)
            // call game function / loop
            pontos = gameplay();
            for (int i = 5; i > 0; i--) // CHECK IF LOOP NUMBERING IS RIGHT
            {
                if (pontos > leaderboard[i].pontuacao)
                {
                    leaderboard [i].pontuacao = pontos;
                    enterPlayerName(nomeJog);
                    strcpy(nomeJog, leaderboard [i].nomeJog);
                    binaryFileSave(leaderboard);
                    break;
                }
            }
            
        if(option == LEADERBOARD)
            binaryFilePrint(leaderboard);
    }
    return 0;
}
