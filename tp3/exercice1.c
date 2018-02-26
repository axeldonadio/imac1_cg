#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/* Dimensions de la fenêtre */
static unsigned int WINDOW_WIDTH = 800;
static unsigned int WINDOW_HEIGHT = 600;

/* Nombre de bits par pixel de la fenêtre */
static const unsigned int BIT_PER_PIXEL = 32;

/* Nombre minimal de millisecondes separant le rendu de deux images */
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

static const NBSEG = 60;

void drawSquare(){
  glBegin(GL_QUADS);
  glVertex2f(-2,2);
  glVertex2f(2,2);
  glVertex2f(2,-2);
  glVertex2f(-2,-2);
  glEnd();
}

void drawCircle(){
  int i;
  float angle;
  glBegin(GL_POLYGON);
  for(i=0; i<NBSEG; i++){
    angle = 2*M_PI*i/NBSEG;
    glVertex2f( cos(angle), sin(angle) );
  }
  glEnd();
}

void drawLandMark(){
  glColor3ub(255,0,0);
  glBegin(GL_LINES);
  glVertex2f(-4,0);
  glVertex2f(4,0);
  glEnd();
  glColor3ub(0,255,0);
  glBegin(GL_LINES);
  glVertex2f(0,-3);
  glVertex2f(0,3);
  glEnd();
}

void drawRoundedSquare(){
  glBegin(GL_QUADS);
  glVertex2f(-1,2);
  glVertex2f(1,2);
  glVertex2f(1,-2);
  glVertex2f(-1,-2);
  glEnd();
  glBegin(GL_QUADS);
  glVertex2f(-2,1);
  glVertex2f(2,1);
  glVertex2f(2,-1);
  glVertex2f(-2,-1);
  glEnd();
  glPushMatrix();
  glTranslatef(-1,-1,0);
  drawCircle();
  glPopMatrix();
  glPushMatrix();
  glTranslatef(1,1,0);
  drawCircle();
  glPopMatrix();
  glPushMatrix();
  glTranslatef(-1,1,0);
  drawCircle();
  glPopMatrix();
  glPushMatrix();
  glTranslatef(1,-1,0);
  drawCircle();
  glPopMatrix();
}

GLuint drawFirstArm(){
  GLuint id = glGenLists(1);
  glNewList(id,GL_COMPILE);
    glPushMatrix();
      glScalef(0.5,0.5,0.5);
      glTranslatef(3,0,0);
      drawCircle();
      glPopMatrix();
    glPushMatrix();
      glScalef(1,1,1);
      glTranslatef(-2,0,0);
      drawCircle();
    glPopMatrix();
    glPushMatrix();
      glBegin(GL_QUADS);
      glVertex2f(-2,1);
      glVertex2f(1.5,0.5);
      glVertex2f(1.5,-0.5);
      glVertex2f(-2,-1);
      glEnd();
    glPopMatrix();
  glEndList();
  return id; 
}

GLuint drawSecondArm(){
  GLuint id = glGenLists(1);
  glNewList(id,GL_COMPILE);
    glPushMatrix();
      glScalef(0.2,0.2,0.2);
      glTranslatef(-5.5,0,0);
      drawRoundedSquare();
    glPopMatrix(); 
      glPushMatrix();
      glScalef(0.2,0.2,0.2);
      glTranslatef(5.5,0,0);
      drawRoundedSquare();
    glPopMatrix();
    glPushMatrix();
      glScalef(0.5,0.2,0.5);
      glBegin(GL_QUADS);
      glVertex2f(-2,1);
      glVertex2f(2,1);
      glVertex2f(2,-1);
      glVertex2f(-2,-1);
      glEnd();
    glPopMatrix();
  glEndList();
  return id;
}

GLuint drawThirdArm(){
  GLuint id = glGenLists(1);
    glNewList(id,GL_COMPILE);
    glPushMatrix();
      glScalef(0.5,0.5,0.5);
      glTranslatef(2,0,0);
      drawCircle();
    glPopMatrix();
      glPushMatrix();
      glScalef(0.15,0.15,0.15);
      glTranslatef(-5.5,0,0);
      drawRoundedSquare();
    glPopMatrix();
    glPushMatrix();
      glScalef(0.5,0.2,0.5);
      glBegin(GL_QUADS);
      glVertex2f(-2,1);
      glVertex2f(2,1);
      glVertex2f(2,-1);
      glVertex2f(-2,-1);
      glEnd();
    glPopMatrix();
  glEndList();
  return id;
}



void projection(){
  SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_RESIZABLE);
  glViewport(0,0,WINDOW_WIDTH,WINDOW_HEIGHT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(-4., 4., -3., 3.);
}

int main(int argc, char** argv) {
  /* Initialisation de la SDL */
  if(-1 == SDL_Init(SDL_INIT_VIDEO)) {
    fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
    return EXIT_FAILURE;
  }
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,0);
  /* Ouverture d'une fenêtre et création d'un contexte OpenGL */
  if(NULL == SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_GL_DOUBLEBUFFER)) {
    fprintf(stderr, "Impossible d'ouvrir la fenetre. Fin du programme.\n");
    return EXIT_FAILURE;
  }
  glMatrixMode(GL_MODELVIEW);
  /* Titre de la fenêtre */
  SDL_WM_SetCaption("Exercice 1", NULL);
  
  /* Boucle d'affichage */
  int loop = 1;
  int selection;
  int alpha;
  GLuint first = drawFirstArm();
  GLuint second = drawSecondArm();
  GLuint third = drawThirdArm();

  while(loop) {
    /* Récupération du temps au début de la boucle */
    Uint32 startTime = SDL_GetTicks();
    
    /* Placer ici le code de dessin */
    glColor3ub(70,70,255);
    glPointSize(5.0);
    glLineWidth(5.0);
    glClear(GL_COLOR_BUFFER_BIT);
    drawLandMark();
      //drawCircle();
      /*glRotatef(alpha, 0.0, 0.0, 1.0);
        drawSquare();
      glRotatef(-alpha, 0.0, 0.0, 1.0);*/
    //drawRoundedSquare();
    /*glPushMatrix();
      glTranslatef(-2.2,-1,0);
      glRotatef(45,0,0,1);
      drawFirstArm();
    glPopMatrix();
     glPushMatrix();
      glRotatef(-10,0,0,1);
      drawSecondArm();
    glPopMatrix();
    glPushMatrix();
      glTranslatef(1.8,0.3,0);
      glRotatef(35,0,0,1);
      drawThirdArm();
    glPopMatrix();
    glPushMatrix();
      glTranslatef(0.2,0.3,0);
      glRotatef(150,0,0,1);
      drawThirdArm();
    glPopMatrix();
    glPushMatrix();
      glTranslatef(1.3,0.6,0);
      glRotatef(70,0,0,1);
      drawThirdArm();
    glPopMatrix();*/

    glPushMatrix();
      glTranslatef(-2.2,-1,0);
      glRotatef(45,0,0,1);
      glCallList(first);
    glPopMatrix();
     glPushMatrix();
      glRotatef(-10,0,0,1);
      glCallList(second);
    glPopMatrix();
    glPushMatrix();
      glTranslatef(1.8,0.3,0);
      glRotatef(35,0,0,1);
      glCallList(third);
    glPopMatrix();
    glPushMatrix();
      glTranslatef(0.2,0.3,0);
      glRotatef(150,0,0,1);
      glCallList(third);
    glPopMatrix();
    glPushMatrix();
      glTranslatef(1.3,0.6,0);
      glRotatef(70,0,0,1);
      glCallList(third);
    glPopMatrix();

    
    /*drawFirstArm();
    drawSecondArm();
    drawThirdArm();*/

    /* Echange du front et du back buffer : mise à jour de la fenêtre */
    SDL_GL_SwapBuffers();
    projection();
    
    /* Boucle traitant les evenements */
    SDL_Event e;
    while(SDL_PollEvent(&e)) {
      /* L'utilisateur ferme la fenêtre : */
      if(e.type == SDL_QUIT) {
        loop = 0;
        break;
      }
      
      /* Quelques exemples de traitement d'evenements : */
      switch(e.type) {
        /* Clic souris */
        case SDL_MOUSEMOTION:
        alpha = (e.button.x * 360)/WINDOW_WIDTH;
        break;
        case SDL_MOUSEBUTTONUP:
        printf("clic en (%d, %d)\n", e.button.x, e.button.y);
        printf("selection : %d\n", selection);
        break;

        /* Touche clavier */
        case SDL_KEYDOWN:
        printf("touche pressée (code = %d)\n", e.key.keysym.sym);
        selection = e.key.keysym.sym;

        default:
        break;

      }
    }
    
    /* Calcul du temps écoulé */
    Uint32 elapsedTime = SDL_GetTicks() - startTime;
    /* Si trop peu de temps s'est écoulé, on met en pause le programme */
    if(elapsedTime < FRAMERATE_MILLISECONDS) {
      SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
    }
  }
  


  /* Liberation des ressources associées à la SDL */ 
  SDL_Quit();
  
  return EXIT_SUCCESS;
}
