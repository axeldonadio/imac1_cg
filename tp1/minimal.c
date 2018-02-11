#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>

/* Dimensions de la fenêtre */
static unsigned int WINDOW_WIDTH = 400;
static unsigned int WINDOW_HEIGHT = 400;

/* Nombre de bits par pixel de la fenêtre */
static const unsigned int BIT_PER_PIXEL = 32;

/* Nombre minimal de millisecondes separant le rendu de deux images */
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

static unsigned int NB_BANDES = 5;


static const unsigned char COLORS[] = {
     0, 0, 0 ,
     255, 0, 0 ,
     0, 255, 0 ,
     0, 0, 255 ,
     255, 255, 0 ,
     255, 0, 255 ,
     0, 255, 255 ,
     255, 255, 255 
};

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

void deletePoints(PointList* list) {
  while(*list) {
    Point* next = (*list)->next;
    free(*list);
    *list = next;
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

void deletePrimitives(PrimitiveList* list) {
  while(*list) {
    Primitive* next = (*list)->next;
    deletePoints(&(*list)->points);
    free(*list);
    *list = next;
  }
}

void drawPrimitives(PrimitiveList list) {
  while(list) {
    glBegin(list->primitiveType);
        drawPoints(list->points);
    glEnd();
    list = list->next;
  }
}

void projection(){
    SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_RESIZABLE);
    glViewport(0,0,WINDOW_WIDTH,WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1., 1., -1., 1.);
}

void couleur(){
    int i =0;
    for(i=0; i < NB_BANDES; i++){
        glColor3ubv(COLORS + i * 3);

        glBegin(GL_QUADS);
            glVertex2f(-1 + i * 2.f / NB_BANDES, -1);
            glVertex2f(-1 + (i+2) * 2.f / NB_BANDES, -1);
            glVertex2f(-1 + (i+2) * 2.f, 1);
            glVertex2f(-1 + i * 2.f / NB_BANDES, 1);
        glEnd(); 
    }
}

int main(int argc, char** argv) {
    int loop = 1;
    int mode = 0;
    int selection;
    int color = 1;
    PrimitiveList primitives = NULL;
    addPrimitive(allocPrimitive(GL_POINTS), &primitives);
    

    /* Initialisation de la SDL */
    if(-1 == SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
        return EXIT_FAILURE;
    }
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,0);
    /* Ouverture d'une fenêtre et création d'un contexte OpenGL */
    if(NULL == SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_RESIZABLE)) {
        fprintf(stderr, "Impossible d'ouvrir la fenetre. Fin du programme.\n");
        return EXIT_FAILURE;
    }
    
    /* Titre de la fenêtre */
    SDL_WM_SetCaption("Ouh la belle fenêtre", NULL);
    

    /* Boucle d'affichage */
    while(loop) {
        
        /* Récupération du temps au début de la boucle */
        Uint32 startTime = SDL_GetTicks();
        
        /* Placer ici le code de dessin */ 
    	glColor3ub(70,70,255);
        glPointSize(5.0);
        glLineWidth(5.0);
        glClear(GL_COLOR_BUFFER_BIT);
        if(mode == 1) {
            couleur();
        } else {
            drawPrimitives(primitives);
        }
        
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
                    //glClearColor((float)(e.button.x%WINDOW_WIDTH)/WINDOW_WIDTH, (float)(e.button.y%WINDOW_HEIGHT)/WINDOW_HEIGHT, 0,1);
                    if(mode == 1) {
                            color = e.button.x * NB_BANDES / WINDOW_WIDTH;
                    }
                    else{
                        addPointToList(allocPoint(-1 + 2. * e.button.x / WINDOW_WIDTH, - (-1 + 2. * e.button.y / WINDOW_HEIGHT), COLORS[color * 3], COLORS[color * 3 + 1], COLORS[color * 3 + 2]), &primitives->points);
                    }
                    /*switch(selection){
                        case 'p':
                            glBegin(GL_POINTS);
                                glVertex2f(-1 + 2. * e.button.x / WINDOW_WIDTH, -(-1 + 2. * e.button.y / WINDOW_HEIGHT));
                            glEnd();
                            break;
                        case 'l':
                            if(x1 == 0 && y1 == 0){
                                x1 = -1 + 2. * e.button.x / WINDOW_WIDTH;
                                y1 = -(-1 + 2. * e.button.y / WINDOW_HEIGHT);
                            }
                            else if( (x1 && y1) && (x2 == 0 && y2 == 0)){
                                x2 = -1 + 2. * e.button.x / WINDOW_WIDTH;
                                y2 = -(-1 + 2. * e.button.y / WINDOW_HEIGHT);
                                glBegin(GL_LINES);
                                    glVertex2f(x1, y1);
                                    glVertex2f(x2, y2);
                                glEnd();
                                x1 = 0;
                                y1 = 0;
                                x2 = 0;
                                y2 = 0;
                            }
                            break;
                        case 't':
                            if(x1 == 0 && y1 == 0){
                                x1 = -1 + 2. * e.button.x / WINDOW_WIDTH;
                                y1 = -(-1 + 2. * e.button.y / WINDOW_HEIGHT);
                            }
                            else if( (x1 && y1) && (x2 == 0 && y2 == 0)){
                                x2 = -1 + 2. * e.button.x / WINDOW_WIDTH;
                                y2 = -(-1 + 2. * e.button.y / WINDOW_HEIGHT);
                            }

                            else if( (x1 && y1) && (x2 && y2) && (x3 ==0 && y3 ==0)){
                                x3 = -1 + 2. * e.button.x / WINDOW_WIDTH;
                                y3 = -(-1 + 2. * e.button.y / WINDOW_HEIGHT);
                                glBegin(GL_TRIANGLES);
                                    glVertex2f(x1, y1);
                                    glVertex2f(x2, y2);
                                    glVertex2f(x3, y3);
                                glEnd();
                                x1 = 0;
                                y1 = 0;
                                x2 = 0;
                                y2 = 0;
                                x3 = 0;
                                y3 = 0;
                            }

                            break;
                        
                        default:
                            break;
                    }*/
                    break;

                /* Touche clavier */
                case SDL_KEYDOWN:
                    printf("touche pressée (code = %d)\n", e.key.keysym.sym);
                    selection = e.key.keysym.sym;
                    if( selection == SDLK_SPACE) {
                        mode = 1;
                    }
                    break;
                case SDL_KEYUP:
                    if(selection == SDLK_SPACE) {
                        mode = 0;
                    }
                    else{
                        switch(selection) {
                          case 'p':
                            addPrimitive(allocPrimitive(GL_POINTS), &primitives);
                            break;
                          case 'l':
                            addPrimitive(allocPrimitive(GL_LINES), &primitives);
                            break;
                          case 't':
                            addPrimitive(allocPrimitive(GL_TRIANGLES), &primitives);
                            break;
                          default:
                            break;
                        }
                    }
                break;
		    /* Déplacement souris 
		    case SDL_MOUSEMOTION:
                glClearColor((float)(e.button.x%WINDOW_WIDTH)/WINDOW_WIDTH, (float)(e.button.y%WINDOW_HEIGHT)/WINDOW_HEIGHT, 0,1);
                break;*/
            case SDL_VIDEORESIZE:
                WINDOW_WIDTH = e.resize.w;
                WINDOW_HEIGHT = e.resize.h;
                projection();
                if(selection == SDLK_SPACE){
                  mode = 1;
                }
            break;

            default:
                break;
            }

 		if(selection == 'q'){
            deletePrimitives(&primitives);
			SDL_Quit();
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
    deletePrimitives(&primitives);
    SDL_Quit();

    return EXIT_SUCCESS;
}
