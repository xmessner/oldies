#include <libGFX.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <libgui_parse.h>

void DrawForm(FORM *form,SDLsurface *screen);
void UpdateForm(FORM *Form,int NumObj);

void DrawForm(FORM *form,SDLsurface *screen)
    {
    int mx,my;
    
    // On cherche a positionner au milieu de l'ecran
    mx=(screen->w/2)-(form->TailleX/2);
    my=(screen->h/2)-(form->TailleY/2);
    
    // On dessine d'abord la fenetre principale
    // Dessin du cadre principal + bord superieur de la fenetre
    // - Dessin du cadre superieur
    rectangleRGBA(screen,mx,my-20,mx+form->TailleX,20,0,0,128,255);
    // - Dessin du fond en gris !
    boxRGBA(screen,mx,my,mx+form->TailleX,my+form->TailleY,128,128,128,255);
    
    // Dessin des autres elements ....
    
    }
    
void UpdateForm(FORM *Form,int NumObj)
    {
    
    }