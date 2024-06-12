#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#define LARG_JANELA 1280
#define ALTURA_JANELA 720
#define LARG_PESSOA 60
#define ALTURA_PESSOA 90
#define LARG_OBJETO 20
#define ALTURA_OBJETO 70
#define LARG_RUA 400
#define ALTURA_RUA 800
#define ESPACAMENTO_RUA 30
#define NUM_OBJETOS 2

int pistaPersonagem;
int pessoaX;
int score;
bool gameover;
bool Tela_Instrucoes;
bool gameStarted;
float objectSpeed = 4.5;
float deslocamentoLinhaCentral = 0.0;

//Vinicius
//função responsavel pela lógica dos mosquitos e da colisão
typedef struct {
    int x;
    int y;
    int lane;
    bool active;
} Object;

//Vinicius
//limitação da quantidade de mosquitos criados por vez
Object objects[NUM_OBJETOS];

//Alan
//função responsável por escrever os textos e suas especificações na tela de jogo 
void EscreverTexto(float x, float y, const char *text) {
    glColor3f(1.0, 1.0, 1.0);
    glRasterPos2f(x, y);
    int length = strlen(text);
    int i;
    for (i = 0; i < length; i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
    }
}

//Gabriel
//função responsável por desenhar o protagonista na tela do jogo
void DesenhaPessoa() {
    glColor3f(0.302f, 0.231f, 0.188f); // Cor da pele

    // Cabelo 
    glColor3f(0.0f, 0.0f, 0.0f); // Preto
    float hairWidth = LARG_PESSOA * 0.4; 
    float hairX = pessoaX + (LARG_PESSOA - hairWidth) / 2; // Centralizar o cabelo
    glRectf(hairX, 20 + ALTURA_PESSOA - 10, hairX + hairWidth, 20 + ALTURA_PESSOA);

    // Cabeça 
    glColor3f(0.302f, 0.231f, 0.188f); // Cor da pele
    float headWidth = LARG_PESSOA * 0.4; // 
    float headX = pessoaX + (LARG_PESSOA - headWidth) / 2; // Centralizar a cabeça
    glRectf(headX, 20 + ALTURA_PESSOA - 30, headX + headWidth, 20 + ALTURA_PESSOA - 10);

    // Tronco 
    glColor3f(0.0f, 0.0f, 1.0f); // Cor da camisa
    float shirtWidth = LARG_PESSOA * 0.5f; // 
    float shirtX = pessoaX + (LARG_PESSOA - shirtWidth) / 2; // Centralizar a camisa
    glRectf(shirtX, 20 + ALTURA_PESSOA / 2, shirtX + shirtWidth, 20 + ALTURA_PESSOA - 20);

    // Pernas
    glColor3f(0.0f, 0.0f, 0.0f); // Cor das calças
    glRectf(pessoaX + LARG_PESSOA / 4, 20, pessoaX + LARG_PESSOA / 2, 20 + ALTURA_PESSOA / 2);
    glRectf(pessoaX + LARG_PESSOA / 2, 20, pessoaX + 3 * LARG_PESSOA / 4, 20 + ALTURA_PESSOA / 2);
    // Braços (ajustar posição para encostar na camisa)
    glColor3f(0.302f, 0.231f, 0.188f); // Cor da pele
    glRectf(shirtX - 10, 20 + ALTURA_PESSOA / 2, shirtX, 20 + ALTURA_PESSOA - 20);
    glRectf(shirtX + shirtWidth, 20 + ALTURA_PESSOA / 2, shirtX + shirtWidth + 10, 20 + ALTURA_PESSOA - 20);

}

//Alan
//função para desenhar a divisão do terreno baldio
void DesenhaEntreTerrenos(){
	glColor3f(0.327f, 0.163f, 0.045f);
	glRectf(LARG_RUA,0 , LARG_RUA+10,LARG_JANELA);
	glRectf(LARG_RUA*2+30,0 , LARG_RUA*2+40,LARG_JANELA);
}

//Vinicius
//função responsável por desenhar o mosquito na tela do jogo
void DesenhaMosquito(int index){
    float x = objects[index].x;
    float y = objects[index].y;

    // Corpo
    glColor3f(0.0, 0.0, 0.0); // Cor preta
    glRectf(x, y, x + LARG_OBJETO / 2, y + ALTURA_OBJETO / 2);

    // Linhas no corpo (diminuir a distância entre as linhas)
    glColor3f(1.0, 1.0, 1.0); // Cor branca para contraste
    glRectf(x + 0, y + 25 / 2, x + 20 / 2, y + 28 / 2);
    glRectf(x + 0, y + 45 / 2, x + 20 / 2, y + 48 / 2);
    glRectf(x + 0, y + 65 / 2, x + 20 / 2, y + 68 / 2);

    // Asa esquerda
    glColor3f(0.7, 0.7, 0.7); // Cor cinza clara
    glBegin(GL_POLYGON);
    glVertex2f(x - 20 / 2, y + 60 / 2);
    glVertex2f(x, y + 80 / 2);
    glVertex2f(x, y + 40 / 2);
    glVertex2f(x - 30 / 2, y + 40 / 2);
    glEnd();

    // Asa direita
    glBegin(GL_POLYGON);
    glVertex2f(x + LARG_OBJETO / 2 + 20 / 2, y + 60 / 2);
    glVertex2f(x + LARG_OBJETO / 2, y + 80 / 2);
    glVertex2f(x + LARG_OBJETO / 2, y + 40 / 2);
    glVertex2f(x + LARG_OBJETO / 2 + 30 / 2, y + 40 / 2);
    glEnd();

    // Antenas na parte debaixo
    glColor3f(0.0, 0.0, 0.0); // Cor preta
    glBegin(GL_LINES);
    glVertex2f(x + 0, y);                  // Início da primeira antena
    glVertex2f(x - 10 / 2, y - 20 / 2);    // Fim da primeira antena
    glVertex2f(x + 20 / 2, y);             // Início da segunda antena
    glVertex2f(x + 30 / 2, y - 20 / 2);    // Fim da segunda antena
    glEnd();

    // Bico entre as antenas (retângulo)
    glColor3f(0.0, 0.0, 0.0); // Cor preta
    glRectf(x + 8 / 2, y, x + 12 / 2, y - 25 / 2);
}

//Alan
//função para desenhar o terreno baldio
void DesenhaRua() {
    glColor3f(0.545f, 0.271f, 0.075f);
    int i;
    for (i = 0; i < 3; i++) {
        glBegin(GL_QUADS);
        glVertex2f(i * (LARG_RUA + ESPACAMENTO_RUA), 0);
        glVertex2f(i * (LARG_RUA + ESPACAMENTO_RUA) + LARG_RUA, 0);
        glVertex2f(i * (LARG_RUA + ESPACAMENTO_RUA) + LARG_RUA, ALTURA_RUA);
        glVertex2f(i * (LARG_RUA + ESPACAMENTO_RUA), ALTURA_RUA);
        glEnd();
    }
}

//Alan
//função para desenhar as linhas que aparece em cada rua
void DesenhaLinhasNaEstrada() {
    glColor3f(0.327f, 0.163f, 0.045f);
    glLineWidth(2.0);
    glEnable(GL_LINE_SMOOTH);
    float lineSegmentLength = 15.0;
    float spaceSegmentLength = 200.0;

    int i;
    for (i = 0; i < 3; i++) {
        int laneX = i * (LARG_RUA + ESPACAMENTO_RUA) + LARG_RUA / 2;

        // Desenha as linhas descontínuas
        float currentPosition = deslocamentoLinhaCentral;
        while (currentPosition < ALTURA_JANELA) {
            // Desenha um segmento de linha
            glBegin(GL_LINES);
            glVertex2f(laneX, currentPosition);
            glVertex2f(laneX, currentPosition + lineSegmentLength);
            glEnd();

            // Atualiza a posição atual para o proximo espaço vazio
            currentPosition += lineSegmentLength + spaceSegmentLength;
        }
    }

    glDisable(GL_LINE_SMOOTH);
}

//Vinicius
//função para mostrar a pontuação do jogador na tela
void DesenhaPontuacao() {
	char scoreText[20];
    sprintf(scoreText, "Score: %d", score);
	EscreverTexto(1170, ALTURA_JANELA - 40, scoreText);
}

//Gabriel
//função para mostrar a mensagem de endgame caso o jogador colida com os mosquitos
void DesenhaFimJogo() {
	char scoreText[20];
    EscreverTexto(LARG_JANELA / 2 - 80, ALTURA_JANELA / 2, "FIM DE JOGO");

    sprintf(scoreText, "Score: %d", score);
	EscreverTexto(LARG_JANELA / 2 - 40, ALTURA_JANELA / 2 - 60, scoreText);
}

//Alan
//função para mostrar as dicas de combate contra a dengue na tela inicial do jogo
void DesenhaDicas() {
    
    EscreverTexto(LARG_JANELA / 2 - 175, ALTURA_JANELA / 2 + 240, "Dicas para combater o Aedes Aegypti:");
    
    EscreverTexto(20, ALTURA_JANELA / 2 + 150, "1. Elimine a agua parada");
    EscreverTexto(20, ALTURA_JANELA / 2 + 120, "2. Use telas nas janelas de casa");
    EscreverTexto(20, ALTURA_JANELA / 2 + 90, "3. Use repelente");
    EscreverTexto(20, ALTURA_JANELA / 2 + 60, "4. Mantenha-se Informado");
   
   
    EscreverTexto(20,50, "Criado por:");
    EscreverTexto(20,20, "Alan Heverton, Gabriel Palha, Vinícius");
    
}

//Gabriel, Alan e Vinicius
//função para a logica do jogo(criar objetos, aumento de velocidade, colisao, contagem de pontos)
void mecanica(int value) {
    if (!gameover && gameStarted) {
        // atualiza os objetos
        int i;
        for (i = 0; i < NUM_OBJETOS; i++) {
            Object *object = &objects[i];
            if (object->active) {
                object->y -= objectSpeed; //velocidade dos objetos

                // Verifica colisao
                if (object->x < pessoaX + LARG_PESSOA &&
                    object->x + LARG_OBJETO > pessoaX &&
                    object->y < 20 + LARG_PESSOA &&
                    object->y + ALTURA_OBJETO > 20) {
                    gameover = true;
                    break;
                }

                // Olha se o objeto esta fora da tela
                if (object->y < -ALTURA_OBJETO) {
                    object->active = false;
                    score++;
                }
            }
        }
        if (!(score % 8)) {
            objectSpeed += 0.03; //Aumenta a velocidade ao poucos
            //deslocamentoLinhaCentral -= objectSpeed;
        }
        deslocamentoLinhaCentral -= objectSpeed;

        // Cria novos objetos
        for (i = 0; i < NUM_OBJETOS; i++) {
            if (!objects[i].active) {
                int randomLane = rand() % 3;

                // Verifica se a pista esta ocupada
                bool laneOccupied = false;
                int j;
                for (j = 0; j < NUM_OBJETOS; j++) {
                    if (objects[j].active && objects[j].lane == randomLane) {
                        laneOccupied = true;
                        break;
                    }
                }

                // Se a pista ja tiver um objeto, procura outra pista
                if (laneOccupied) {
                    randomLane = (randomLane + 1) % 3; // Muda para a proxima pista
                    while (randomLane != objects[i].lane) {
                        laneOccupied = false;
                        for (j = 0; j < NUM_OBJETOS; j++) {
                            if (objects[j].active && objects[j].lane == randomLane) {
                                laneOccupied = true;
                                break;
                            }
                        }
                        if (!laneOccupied) {
                            break;
                        }
                        randomLane = (randomLane + 1) % 3; // Muda para a proxima pista
                    }
                }

                objects[i].active = true;
                objects[i].x = randomLane * (LARG_RUA + ESPACAMENTO_RUA) + (LARG_RUA - LARG_OBJETO) / 2;
                objects[i].y = ALTURA_JANELA;
                objects[i].lane = randomLane;
                break;
            }
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, mecanica, 0);
}

//Vinicius
//função para mostrar a tela de inicio do jogo
void DesenhaTelaInicio() {
    glClearColor(0.545f, 0.271f, 0.075f, 1.0f); 
   
    glClear(GL_COLOR_BUFFER_BIT);
    

    EscreverTexto(LARG_JANELA / 2 - 170, ALTURA_JANELA / 2 + 60, "Jogo: Mosquito Challenge!");
    EscreverTexto(LARG_JANELA / 2 - 170, ALTURA_JANELA / 2 , "Clique na tela para ver o menu");
    
    EscreverTexto(20, 50, "Criado por:");
    EscreverTexto(20, 20, "Alan Heverton, Gabriel Palha e Pedro Vinicius");
}

//Alan, Gabriel e Vinicius
//funcao para a logica do que será mostrado na tela de acordo com os estados do jogo
void Tela() {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, LARG_JANELA, 0, ALTURA_JANELA);
	
	if (!gameStarted && !Tela_Instrucoes && !gameover) {
        DesenhaTelaInicio();
    }
	
    if (!gameover && gameStarted) {
        //deslocamentoLinhaCentral -= objectSpeed;
        DesenhaRua();
        DesenhaLinhasNaEstrada();
        DesenhaEntreTerrenos();
        int i;
        for (i = 0; i < NUM_OBJETOS; i++) {
            if (objects[i].active) {
                DesenhaMosquito(i);
            }
        }
        DesenhaPessoa();
        DesenhaPontuacao();
    }
	if (gameover) {
        DesenhaFimJogo();
    }
	if (Tela_Instrucoes) {
        DesenhaDicas();
    }

    glutSwapBuffers();
}

//Alan
//funçao para definir as teclas que são utilizadas na movimentação e seleção
void specialKeyboard(int key, int x, int y) {
    if (!Tela_Instrucoes && gameStarted) {
        switch (key) {
            case GLUT_KEY_LEFT:
                if (pistaPersonagem > 0)
                    pistaPersonagem--;
                break;
            case GLUT_KEY_RIGHT:
                if (pistaPersonagem < 2)
                    pistaPersonagem++;
                break;
        }

        pessoaX = pistaPersonagem * (LARG_RUA + ESPACAMENTO_RUA) + (LARG_RUA - LARG_PESSOA) / 2;
    }
}

//Vinicius
//função contendo a lógica para todos os casos selecionados do menu
void menu(int choice) {
    switch (choice) {
        case 1:
            gameStarted = true;
            gameover = false;
            Tela_Instrucoes = false;
            score = 0;
            objectSpeed = 5.0;
            deslocamentoLinhaCentral = 0.0;
            int i;
            for (i = 0; i < NUM_OBJETOS; i++) {
                objects[i].active = false;
            }
            break;
        case 2:
            Tela_Instrucoes = true;
            gameStarted = false;
            gameover = false;
            break;
        case 3:
        	Tela_Instrucoes = false;
            gameStarted = false;
            gameover = false;
        	break;
    }
}

//Gabriel
//função responsável por criar o menu
void createMenu() {
    glutCreateMenu(menu);
    glutAddMenuEntry("Iniciar Jogo", 1);
    glutAddMenuEntry("Dicas para combater o Aedes Aegypti", 2);
    glutAddMenuEntry("Menu", 3);
    glutAttachMenu(GLUT_LEFT_BUTTON);
}

//Gabriel
//função responsável por iniciar o jogo
void inicialize(){
	srand(time(NULL));

    pistaPersonagem = 1;
    pessoaX = pistaPersonagem * (LARG_RUA + ESPACAMENTO_RUA) + (LARG_RUA - LARG_PESSOA) / 2;
    score = 0;
    gameover = false;
    Tela_Instrucoes = false;
    gameStarted = false;

    int i;
    for (i = 0; i < NUM_OBJETOS; i++) {
        objects[i].active = false;
    }
}

//função principal que chama as outras funções:
int main() {
    
	inicialize();
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(130,90);
    glutInitWindowSize(LARG_JANELA, ALTURA_JANELA);
    glutCreateWindow("Mosquito Challenge");
    glutDisplayFunc(Tela);
    glutSpecialFunc(specialKeyboard);
    glutTimerFunc(0, mecanica, 0);
    glClearColor(0.0, 0.0, 0.0, 0.0);

    createMenu();

    glutMainLoop();
    return 0;
}
