#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>

/* Dimensions de la fenêtre */
static unsigned int WINDOW_WIDTH = 800;
static unsigned int WINDOW_HEIGHT = 600;

/* Nombre de bits par pixel de la fenêtre */
static const unsigned int BIT_PER_PIXEL = 32;

/* Nombre minimal de millisecondes separant le rendu de deux images */
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

typedef struct Point {
  float x, y; 
  unsigned char r, g, b; 
  struct Point* next; 
} Point, *PointList;

typedef struct primitive {
  GLenum primitiveType; 
  PointList points; 
  struct primitive* next;
} Primitive, *PrimitiveList;

Point* allocPoint(float x, float y, unsigned char r, unsigned char g, unsigned char b) {
  Point* tmp = (Point*) malloc(sizeof(Point));
  if(!tmp) {
    return NULL;
  }
  tmp->x = x;
  tmp->y = y;
  tmp->r = r;
  tmp->g = g;
  tmp->b = b;
  tmp->next = NULL;
  return tmp;
}

void addPointToList(Point* point, PointList* list) {
  if(*list == NULL) {
    *list = point;
  } else {
    addPointToList(point, &(*list)->next);
  }
}

void drawPoints(PointList list) {
  while(list) {
    glColor3ub(list->r, list->g, list->b);
    glVertex2f(list->x, list->y);
    list = list->next;
  }
}

Primitive* allocPrimitive(GLenum primitiveType) {
  Primitive* tmp = (Primitive*) malloc(sizeof(Primitive));
  if(!tmp) {
    return NULL;
  }
  tmp->primitiveType = primitiveType;
  tmp->points = NULL;
  tmp->next = NULL;
  return tmp;
}

void addPrimitive(Primitive* primitive, PrimitiveList* list) {
  primitive->next = *list;
  *list = primitive;
}

void drawPrimitives(PrimitiveList list) {
  while(list) {
    glBegin(list->primitiveType);
        drawPoints(list->points);
    glEnd();
    list = list->next;
  }
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
  
  /* Titre de la fenêtre */
  SDL_WM_SetCaption("Exercice 1", NULL);
  
  /* Boucle d'affichage */
  int loop = 1;
  int selection;
  PrimitiveList primitives = NULL;
  addPrimitive(allocPrimitive(GL_POINTS), &primitives);

  while(loop) {
    /* Récupération du temps au début de la boucle */
    Uint32 startTime = SDL_GetTicks();
    
    /* Placer ici le code de dessin */
    glColor3ub(70,70,255);
    glPointSize(5.0);
    glLineWidth(5.0);
    glClear(GL_COLOR_BUFFER_BIT);

    drawPrimitives(primitives);
    /* Echange du front et du back buffer : mise à jour de la fenêtre */
    SDL_GL_SwapBuffers();
    
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
        case SDL_MOUSEBUTTONUP:
          printf("clic en (%d, %d)\n", e.button.x, e.button.y);
          printf("selection : %d\n", selection);
          addPointToList(allocPoint(-1 + 2. * e.button.x / WINDOW_WIDTH, - (-1 + 2. * e.button.y / WINDOW_HEIGHT), 255, 255, 255), &primitives->points);
      	break;
          
        /* Touche clavier */
        case SDL_KEYDOWN:
          printf("touche pressée (code = %d)\n", e.key.keysym.sym);
          selection = e.key.keysym.sym;
          switch(selection) {
              case 'p':
                addPrimitive(allocPrimitive(GL_POINTS), &primitives);
                break;
              case 'l':
                addPrimitive(allocPrimitive(GL_LINE_LOOP), &primitives);
                break;
              case 't':
                addPrimitive(allocPrimitive(GL_TRIANGLES), &primitives);
                break;
              default:
                break;
            }
          break;
          
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
