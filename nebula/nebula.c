#include <libGFX.h>
#include <libgui_parse.h>
#include <SDL/SDL_gfxPrimitives.h>

void ProcessEvent(void);
unsigned int ProcessGenericKeys(void);
void AffichePaletteDialogue(SDL_Surface *screen,int x,int y,int dr,int dg,int db);
void DrawForm(FORM *form,SDL_Surface *screen);
void UpdateForm(FORM *Form,int NumObj);

//Structure de la souris

typedef struct MOUSESTATE
	{
	unsigned char	etat;
	unsigned int	mx,my;
	unsigned char	bouton;
	}MouseState;

typedef struct KEYBSTATE
	{
	Uint8		*keystate;	//tableau de 255 car
	unsigned int	keymodstate;	//entier pour les touches de control
	unsigned char	etat;		//appuye / relache
	}KeybState;

KeybState	*keybstate;
MouseState	*mousestate;
		    
void InitKeyboard(void)
    {
    keybstate=(KeybState*)malloc(sizeof(KeybState));    
    }
void CloseKeyboard(void)
    {
    free(keybstate);
    }
void InitMouse(void)
    {
    mousestate=(MouseState*)malloc(sizeof(MouseState));
    }
void CloseMouse(void)
    {
    free(mousestate);
    }
void InitEvent(void)
    {
    InitKeyboard();
    InitMouse();
    }
void CloseEvent(void)
    {
    CloseKeyboard();
    CloseMouse();
    }
    
/*static void print_modifiers(void)
{
	int mod;
	printf(" modifiers:");
	mod = SDL_GetModState();
	if(!mod) {
		printf(" (none)");
		return;
	}
	if(mod & KMOD_LSHIFT)
		printf(" LSHIFT");
	if(mod & KMOD_RSHIFT)
		printf(" RSHIFT");
	if(mod & KMOD_LCTRL)
		printf(" LCTRL");
	if(mod & KMOD_RCTRL)
		printf(" RCTRL");
	if(mod & KMOD_LALT)
		printf(" LALT");
	if(mod & KMOD_RALT)
		printf(" RALT");
	if(mod & KMOD_LMETA)
		printf(" LMETA");
	if(mod & KMOD_RMETA)
		printf(" RMETA");
	if(mod & KMOD_NUM)
		printf(" NUM");
	if(mod & KMOD_CAPS)
		printf(" CAPS");
	if(mod & KMOD_MODE)
		printf(" MODE");
}
*/
static void PrintKey(SDL_keysym *sym, int pressed)
{
	/* Print the keycode, name and state */
	if ( sym->sym ) {
		printf("Key %s:  %d-%s ", pressed ?  "pressed" : "released",
					sym->sym, SDL_GetKeyName(sym->sym));
	} else {
		printf("Unknown Key (scancode = %d) %s ", sym->scancode,
					pressed ?  "pressed" : "released");
	}

	/* Print the translated character, if one exists */
//	if ( sym->unicode ) {
//		/* Is it a control-character? */
//		if ( sym->unicode < ' ' ) {
//			printf(" (^%c)", sym->unicode+'@');
//		} else {
//#ifdef UNICODE
//			printf(" (%c)", sym->unicode);
//#else
//			/* This is a Latin-1 program, so only show 8-bits */
//			if ( !(sym->unicode & 0xFF00) )
//				printf(" (%c)", sym->unicode);
//#endif
//		}
//	}
//	print_modifiers();
	printf("\n");
}

int main(int argc, char *argv[])
	{
	SDL_Event	event;
	Uint8		bouton=0;
	unsigned int	done,i;
	SDL_Surface	*Img;

	GFXInit(10,"Nebula - Alpha version");
	
	InitEvent();
	
	//Gestion du clavier
	/* Enable auto repeat for keyboard input */
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY,
	                    SDL_DEFAULT_REPEAT_INTERVAL);

//	Img=GFXLoadTGA("./gfx2.tga");
//	GFXRcCopy(Img,0,0,Img->w,Img->h,SDLscreen,0,0);
//
//	AffichePaletteDialogue(SDLscreen,100,120,1,1,-1);
		
	GFXRefreshScreen();
	/* Watch keystrokes */
	done = 0;
	while ( !done )
		{
		ProcessEvent();
		ProcessGenericKeys();
    		
		if(keybstate->keystate[SDLK_ESCAPE]==1)
		    {
		    // Pour le debuggage
		    fprintf(stderr,"ESC\t");
		    done=1;
		    }

		// Test les touches
		// Palette de couleur
		if(keybstate->keystate[SDLK_TAB]==1)
		    {
		    AffichePaletteDialogue(SDLscreen,100,120,1,1,-1);
		    }
		
		// 
		if((mousestate->mx<10) && (mousestate->my<10) && (mousestate->etat==1)) done=1;
		if(mousestate->etat==1) printf("bouton toujours appuye deplacement X : %d\t Y : %d\t\r",mousestate->mx,mousestate->my);
		}
//	SDL_FreeSurface(Img);
	CloseEvent();
	GFXClose();
	return(0);
	}
unsigned int ProcessGenericKeys(void)
    {
    unsigned int result=0;

    if(keybstate->keystate[SDLK_RETURN]==1)
	{
	if(SDL_GetModState() & KMOD_ALT) SDL_WM_ToggleFullScreen(SDLscreen);
	}

    return result;
    }
void ProcessEvent(void)
    {
    SDL_Event	event;
    int		val=0;
    Uint8	bouton;
    
    SDL_PollEvent(&event);
    keybstate->keystate=SDL_GetKeyState(NULL);

    switch (event.type)
	{
	case SDL_KEYDOWN:
	    keybstate->keystate[event.key.keysym.sym]=1;
	    break;
	case SDL_KEYUP:
	    keybstate->keystate[event.key.keysym.sym]=0;
	    break;
	case SDL_MOUSEBUTTONDOWN:
	    bouton=SDL_GetMouseState(NULL,NULL);
	    mousestate->mx=event.button.x;
	    mousestate->my=event.button.y;
	    mousestate->etat=1;
	    mousestate->bouton=0;
	    bouton=1;
	    break;
	case SDL_MOUSEBUTTONUP:
	    bouton=SDL_GetMouseState(NULL,NULL);
	    mousestate->mx=event.button.x;
	    mousestate->my=event.button.y;
	    mousestate->etat=0;
	    mousestate->bouton=0;
	    bouton=0;
	    break;
	case SDL_MOUSEMOTION:
	    mousestate->mx=event.motion.x;
	    mousestate->my=event.motion.y;
	    break;
	case SDL_QUIT:
	    val = 1;
	    break;
	default:
	    break;
	}
    }

void AffichePaletteDialogue(SDL_Surface *screen,int x,int y,int dr,int dg,int db)
    {
    // Variables pour le calcul des couleurs
    int r,g,b,sr=0,sg=0,sb=0;
    int i,j;
    int PalStartX,PalStartY;
    unsigned char done=0;
    
    // Variables pour le formulaire
    FORM *Form;
    
    Form=(FORM*)ReadForm("./palettedialogue.rsc");
    
    PalStartX=(640/2)-(Form->TailleX/2);
    PalStartY=(480/2)-(Form->TailleY/2);
    rectangleRGBA(screen,PalStartX,PalStartY,PalStartX+Form->TailleX,PalStartY+Form->TailleY,0,0,255,255);

    // Dessin de la palette
    for (i=0;i<256;i++)
	{
	r=sr;g=sg;b=sb;
	for(j=0;j<256;j++)
	    {
	    pixelRGBA(screen,(10+PalStartX)+j,(10+PalStartY)+i,r,g,b,255);
	    if(dr==1) r+=1;if(dg==1) g+=1;if(db==1) b+=1;
	    }
	if(dr==-1) sr+=1;if(dg==-1) sg+=1;if(db==-1) sb+=1;
	}

    // Dessin des boites de couleur
    for(i=0;i<Form->NbObjR;i++)
	{
	boxRGBA(screen,Form->Obj[i].PosX+PalStartX,Form->Obj[i].PosY+PalStartY,
		Form->Obj[i].PosX+Form->Obj[i].TailleX+PalStartX,
		Form->Obj[i].PosY+Form->Obj[i].TailleY+PalStartY,255,0,0,255);
	}
    DrawForm(Form,screen);

    GFXRefreshScreen();

    // Test des touches
    while(done!=1)
	{
	ProcessEvent();
	ProcessGenericKeys();
    		
	// Test les touches
	// Palette de couleur
	if(keybstate->keystate[SDLK_SPACE]==1)
	    {
	    memset(SDLscreen->pixels,0,640*480*4);
	    GFXRefreshScreen();
	    done=1;
	    }
	}	
    FreeForm(Form);
    }

void DrawForm(FORM *form,SDL_Surface *screen)
    {
    int mx,my;
    
    // On cherche a positionner au milieu de l'ecran
    mx=(screen->w/2)-(form->TailleX/2);
    my=(screen->h/2)-(form->TailleY/2);
    
    // On dessine d'abord la fenetre principale
    // Dessin du cadre principal + bord superieur de la fenetre
    // - Dessin du cadre superieur
    //rectangleRGBA(screen,mx,my-20,mx+form->TailleX,my+5,128,0,0,255);
    
    // - Dessin du fond en gris !
    //boxRGBA(screen,mx,my,mx+form->TailleX,my+form->TailleY,180,180,180,255);
    //Mem_Pbox(mx,my,mx+form->TailleX,my+form->TailleY,180,180,180,screen);
    Mem_Box(100,0,170,170,0,0,180,screen);
    Mem_Pixel(160,79,180,180,255,screen);
    fprintf(stderr,"mx : %d\nmy : %d\n",mx,my);
    // Dessin des autres elements ....
    
    }
    
void UpdateForm(FORM *Form,int NumObj)
    {
    
    }
