//# include "iGraphics.h"
#include<windows.h>
#include<bits/stdc++.h>

using namespace std;
//#include "NNNswift.h";
#include "qswift.h";


/********************************* Definitions ****************************************/

// General definitions

//Color Definitions

#define RED Point(255,0,0)
#define GREEN Point(0,255,0)
#define BLUE Point(0,0,255)
#define YELLOW Point(255,255,0)
#define CYAN Point(0,255,255)
#define VIOLET Point(255,0,255)
#define GREY Point(128,128,128)
#define NAVY Point(0,0,128)
#define SILVER Point(192,192,192)
#define MAROON Point(128,0,0)
#define OLIVE Point(128,128,0)
#define WHITE Point(255,255,255)
#define BLACK Point(0,0,0)
#define PINK Point(255,20,147)
#define ORANGE Point(255,128,0)
#define LIGHT_GREY Point(192,192,192)
#define DARK_GREY Point(96,96,96)
#define LIGHT_GREEN Point(204,255,204)
#define DARK_GREEN Point(0,153,0)
#define LIGHT_CYAN Point(204,255,255)
#define LIGHT_RED Point(255,204,204)
#define CHOCOLATE Point(153,76,0)

/**************************** Structure for Slider ****************************/

int sliderHeight = 60;
int sliderWidth = 280;
int basebarWidth = (18*sliderWidth)/20;
int basebarHeight = sliderHeight/8;
int *slider_variable; // the variable that will be affected by slider

void handle_slider_click(Point where, Div *d);
void handle_slider_apply_click(Point Where, Div *d);

struct SliderDiv{

    Div *bg;
    Div *baseBar;
    Div *pointer;
    Div *minValueBox;
    Div *maxValueBox;
    Div *nowValueBox;
    Div *applyBtn;

    int maxValue;
    int minValue;
    int nowValue;

    void createSliderDiv(int ax, int ay, int minval, int maxval, int nowval)
    {
        //printf("Here\n\n");

        nowValue = nowval;
        maxValue = maxval;
        minValue = minval;

        bg = new Div();
        bg->positioning = constants.DIV_RELATIVE;
        bg->a = Point(ax,ay);
        bg->b = Point(sliderWidth + 85, sliderHeight);
        bg->filled = 1;
        bg->color = WHITE;

        root->append_child(bg);

        baseBar = new Div();
        baseBar->positioning = constants.DIV_RELATIVE;
        baseBar->a = Point((sliderWidth - basebarWidth) / 2 , (sliderHeight - basebarHeight) / 2 + 14 );
        baseBar->b = Point(basebarWidth, basebarHeight );
        baseBar->color = DARK_GREY;
        baseBar->click = handle_slider_click;

        bg->append_child(baseBar);

        int pointerX = (((nowValue - minValue)*basebarWidth/(maxValue - minValue))) + (sliderWidth - basebarWidth) / 2;
        int pointerY = baseBar->a.y + basebarHeight/2;

        pointer = new Div();
        pointer->type = constants.DIV_CIRCLE;
        //pointer->positioning = constants.DIV_RELATIVE;
        pointer->R = 9;
        pointer->color = BLUE;
        pointer->filled = 1;
        pointer->a = bg->a + Point(pointerX,pointerY);

        bg->append_child(pointer);

        minValueBox = new Div();
        minValueBox->positioning = constants.DIV_RELATIVE;
        minValueBox->a = Point(baseBar->a.x,basebarHeight);
        minValueBox->b = Point(50,25);
        minValueBox->color = DARK_GREY;
        itoa(minValue, minValueBox->text, 10);
        minValueBox->filled = 1;

        bg->append_child(minValueBox);

        maxValueBox = new Div();
        maxValueBox->positioning = constants.DIV_RELATIVE;
        maxValueBox->a = Point(baseBar->a.x + baseBar->b.x - 50,basebarHeight);
        maxValueBox->b = Point(50,25);
        maxValueBox->color = DARK_GREY;
        itoa(maxValue, maxValueBox->text, 10);
        maxValueBox->filled = 1;

        bg->append_child(maxValueBox);

        nowValueBox = new Div();
        nowValueBox->positioning = constants.DIV_RELATIVE;
        nowValueBox->a = Point(baseBar->a.x + (baseBar->b.x)/2 - 25,basebarHeight);
        nowValueBox->b = Point(50,25);
        nowValueBox->color = LIGHT_GREEN;
        nowValueBox->text_color = BLACK;
        itoa(nowValue, nowValueBox->text, 10);
        nowValueBox->filled = 1;

        bg->append_child(nowValueBox);

        applyBtn = new Div();
        applyBtn->positioning = constants.DIV_RELATIVE;
        applyBtn->a = Point(sliderWidth + 5,5);
        applyBtn->b = Point(sliderWidth/4,sliderHeight-10);
        applyBtn->color = CYAN - Point(10,10,10);
        applyBtn->filled = 1;
        applyBtn->text_color = BLACK;
        applyBtn->click = handle_slider_apply_click;
        strcpy(applyBtn->text,"Apply");

        bg->append_child(applyBtn);

    }

    void updateSliderDiv(int ax, int ay, int minval, int maxval, int nowval)
    {
        //printf("Here\n\n");

        nowValue = nowval;
        maxValue = maxval;
        minValue = minval;

        bg->positioning = constants.DIV_RELATIVE;
        bg->a = Point(ax,ay);
        bg->b = Point(sliderWidth + 85, sliderHeight);
        bg->filled = 1;
        bg->color = WHITE;

        baseBar->positioning = constants.DIV_RELATIVE;
        baseBar->a = Point((sliderWidth - basebarWidth) / 2 , (sliderHeight - basebarHeight) / 2 + 14 );
        baseBar->b = Point(basebarWidth, basebarHeight );
        baseBar->color = DARK_GREY;
        baseBar->click = handle_slider_click;

        int pointerX = (((nowValue - minValue)*basebarWidth/(maxValue - minValue))) + (sliderWidth - basebarWidth) / 2;
        int pointerY = baseBar->a.y + basebarHeight/2;

        pointer->type = constants.DIV_CIRCLE;
        //pointer->positioning = constants.DIV_RELATIVE;
        pointer->R = 9;
        pointer->color = BLUE;
        pointer->filled = 1;
        pointer->a = bg->a + Point(pointerX,pointerY);


        minValueBox->positioning = constants.DIV_RELATIVE;
        minValueBox->a = Point(baseBar->a.x,basebarHeight);
        minValueBox->b = Point(50,25);
        minValueBox->color = DARK_GREY;
        itoa(minValue, minValueBox->text, 10);
        minValueBox->filled = 1;


        maxValueBox->positioning = constants.DIV_RELATIVE;
        maxValueBox->a = Point(baseBar->a.x + baseBar->b.x - 50,basebarHeight);
        maxValueBox->b = Point(50,25);
        maxValueBox->color = DARK_GREY;
        itoa(maxValue, maxValueBox->text, 10);
        maxValueBox->filled = 1;


        nowValueBox->positioning = constants.DIV_RELATIVE;
        nowValueBox->a = Point(baseBar->a.x + (baseBar->b.x)/2 - 25,basebarHeight);
        nowValueBox->b = Point(50,25);
        nowValueBox->color = LIGHT_GREEN;
        nowValueBox->text_color = BLACK;
        itoa(nowValue, nowValueBox->text, 10);
        nowValueBox->filled = 1;


        applyBtn->positioning = constants.DIV_RELATIVE;
        applyBtn->a = Point(sliderWidth + 5,5);
        applyBtn->b = Point(sliderWidth/4,sliderHeight-10);
        applyBtn->color = CYAN - Point(10,10,10);
        applyBtn->filled = 1;
        applyBtn->text_color = BLACK;
        applyBtn->click = handle_slider_apply_click;
        strcpy(applyBtn->text,"Apply");

    }

    void changeValue(int newVal)
    {
        nowValue = newVal;

        int pointerX = (((nowValue - minValue)*basebarWidth/(maxValue - minValue))) + (sliderWidth - basebarWidth) / 2;
        printf("%d %d %d %d\n",newVal, minValue, maxValue, pointerX);
        int pointerY = baseBar->a.y + basebarHeight/2;
        pointer->a = bg->a + Point(pointerX,pointerY);

        itoa(nowValue, nowValueBox->text, 10);

    }

    void hide()
    {
        bg->hidden = 1;
        baseBar->hidden = 1;
        pointer->hidden = 1;
        minValueBox->hidden = 1;
        maxValueBox->hidden = 1;
        nowValueBox->hidden = 1;
        applyBtn->hidden = 1;
    }

    void unhide()
    {
        bg->hidden = 0;
        baseBar->hidden = 0;
        pointer->hidden = 0;
        minValueBox->hidden = 0;
        maxValueBox->hidden = 0;
        nowValueBox->hidden = 0;
        applyBtn->hidden = 0;
    }
};

// Declaration of Global Slider

SliderDiv slider_global;

void (*slider_apply_function)();

/********************************* Globals ****************************************/

int screenWidth = 1400;
int screenHeight = 720;

// Tower of Hanoi Global Variables

void (*toh_solve_pointer)(int,int,int,int);
int delay = 1;
int discWidths[50];
int atPole[50];
int discNum = 4;
int firstDiscWidth = 150;
int poleX[50];
int poleMax[50];
int lowerMostDiscAtPole[3];
int howManyAtPole[3];
int poleHeight = 400;
stack<int>discsAtPole[3];
int baseX = 370;
int baseWidth = 700; // dont change
int baseHeight = 150; // y value of upper part of base

int diskDifference = (int)((firstDiscWidth - 10)/(2*(discNum-1)))-1;

int discDepth = 20;

int speedY = 10;
int speedX = 10;

// pole and base depth = 10

/********************************* Function Prototypes ****************************************/

void reposition_btn_click();
void animateDisc(int i, int toPole);

/********************************* Thread Handles *********************************/

HANDLE toh_solve_handle;

/********************************* Elements ***************************************/

//Sidebar Menu

Div *menu = new Div();
Div *selected = new Div();

Div *menu_toh = new Div();
Div *reposition_btn = new Div();
Div *toh_solve_btn = new Div();
Div *success_btn = new Div();
Div *discs[50];
Div *poles[3];
Div *base = new Div();

/******************************** Disc Drawing function ********************************/

void drawDisc(int i, int w, int pole, int height) // height from base
{
    height += baseHeight;
    discs[i]->filled=0;
    discs[i]->a=Point((int)(poleX[pole] - ((w-10)/2)),height); // w should be even
    discs[i]->b=Point(poleX[pole] - ((w-10)/2) + w,height+discDepth);
}

void movedisc(int pole, int toPole)
{
    int topmost = discsAtPole[pole].top();
    animateDisc(topmost, toPole);
}

void animateDisc(int i, int toPole)
{
    int w = discWidths[i];
    int pole = atPole[i];
    int h = discs[i]->b.y;
    while(h < poleHeight+30)
    {
        drawDisc(i,discWidths[i],atPole[i],h);
        h += speedY;
        Sleep(1);
    }
    if(poleX[toPole] > pole)
    {
        int x = discs[i]->a.x;
        while(x < poleX[toPole] - (w-10)/2)
        {
            discs[i]->a.x +=10;
            discs[i]->b.x += 10;
            x += speedX;
            Sleep(2*delay);
        }

    }
    else
    {
        int x = discs[i]->a.x;
        while(x > poleX[toPole] - (w-10)/2 )
        {
            discs[i]->a.x -=10;
            discs[i]->b.x -= 10;
            x -= speedX;
            Sleep(2*delay);
        }
    }

    h = discs[i]->a.y - baseHeight;
    while(h >= poleMax[toPole])
    {
        drawDisc(i,discWidths[i],toPole,h);
        h -= speedY;
        Sleep(1);
    }

    PlaySound(TEXT("click.wav"), NULL , SND_ASYNC);
    Sleep(300);

    PlaySound(NULL,0,0);

    discsAtPole[toPole].push(i);
    discsAtPole[pole].pop();
    atPole[i] = toPole;
    poleMax[pole] -= discDepth;
    poleMax[toPole] += discDepth;
    howManyAtPole[pole]--;
    howManyAtPole[pole]++;
}

/******************************** Thread functions ********************************/
DWORD WINAPI toh_thread() {

	toh_solve_pointer(discNum,0,1,2);

	Sleep(500);
	success_btn->a=Point(900,60);
	success_btn->b=Point(1100,110);
    PlaySound(TEXT("click.wav"), NULL , SND_ASYNC);
    Sleep(5000);

}

/******************************** solving Functions *******************************/
void toh(int n, int s, int h, int d) {
    if(n==1)
        movedisc(s,d);
    else
    {
        toh(n-1,s,d,h);
        movedisc(s,d);
        toh(n-1,h,s,d);
    }
}

/******************************** Button Controls *********************************/

void sayHi() {
	Div *hi = new Div();
	hi->a=Point(rand()%500, rand()%500);
	hi->b=Point(hi->a.x+100, hi->a.y+100);
	//hi->text="Hi";
	root->append_child(hi);
}
void reposition_btn_click() {
    success_btn->a=Point(0,0);
	success_btn->b=Point(0,0);
    TerminateThread(toh_solve_handle,0);
    //Sleep(3000);
    for(int i=0;i<3;i++)
    {
        poleMax[i] = 0;
        lowerMostDiscAtPole[i] = 0;
        howManyAtPole[i] = 0;
    }

    for(int i=0;i<20;i++)
    {
        discs[i]->hidden = 1;
    }

	for(int i=0;i<discNum;i++) {
	    int diskDifference = (int)((firstDiscWidth - 10)/(2*(discNum-1)))-1;
        discs[i]->hidden = 0;
        discWidths[i] = firstDiscWidth - diskDifference*i;
        int w = discWidths[i];
		drawDisc(i,w,0,discDepth*i);
		poleMax[0] += discDepth;
		howManyAtPole[0]++;
		discsAtPole[0].push(i);
		atPole[i] = 0;

	}
}
void toh_solve_btn_click() {

    reposition_btn_click();
    Sleep(100);
    DWORD toh_solveId;
	toh_solve_handle = CreateThread(NULL, 0, toh_thread, NULL, 0, NULL);
}
void menu_toh_click() {

    slider_variable = &discNum;

    slider_apply_function = reposition_btn_click;

    slider_global.updateSliderDiv(400,630,2,15,4);

    reposition_btn_click();

	selected->bg_color=Point(0,0,0);
	selected = menu_toh;
	selected->bg_color=Point(110,0,0);
	toh_solve_pointer=toh;
}

//
/****************************** Slider Handle Functions *******************************/

// These must be below slider_apply_function

void handle_slider_click(Point where, Div *d)
{
    int maxV, minV , X;

    X = slider_global.bg->a.x + slider_global.baseBar->a.x;
    maxV = slider_global.maxValue;
    minV = slider_global.minValue;

    int desiredValue = (where.x - X)*(maxV-minV)/basebarWidth + minV ;
    slider_global.changeValue(desiredValue);
}

void handle_slider_apply_click(Point Where, Div *d)
{
    *slider_variable = slider_global.nowValue;
    slider_apply_function();
    printf(" %d Yes here\n\n",*slider_variable);
}


void initialize(int width,int height, char title[]) {

	root->a=Point(0,0);
	root->b=Point(screenWidth,screenHeight);

	menu->a=Point(10,10);
	menu->b=Point(300,screenHeight - 10);
	root->append_child(menu);

	menu_toh->a=Point(10,610);
	menu_toh->b=Point(300,710);
	strcpy( menu_toh->text,"Tower Of Hanoi");
	menu_toh->click = menu_toh_click;
	root->append_child(menu_toh);

	reposition_btn->a=Point(400,60);
	reposition_btn->b=Point(600,110);
	strcpy( reposition_btn->text,"Reposition Discs");
	reposition_btn->click=reposition_btn_click;
	root->append_child(reposition_btn);

	toh_solve_btn->a=Point(700,60);
	toh_solve_btn->b=Point(800,110);
	strcpy( toh_solve_btn->text,"Solve");
	toh_solve_btn->click=toh_solve_btn_click;
	root->append_child(toh_solve_btn);

	success_btn->a=Point(0,0);
	success_btn->b=Point(0,0);
	success_btn->color = Point(0,255,0);
	strcpy( success_btn->text,"Task Completed !");
	root->append_child(success_btn);

	base->a=Point(baseX,baseHeight - 10);
	base->b=Point(baseX+baseWidth,baseHeight);
	base->filled = 1;
	base->color = YELLOW;
	root->append_child(base);

	for(int i=0;i<3;i++)
    {
        poleMax[i] = 0;
        lowerMostDiscAtPole[i] = 0;
        howManyAtPole[i] = 0;
    }

	for(int i=0;i<3;i++) {
		poles[i]=new Div();
		poles[i]->filled=1;
		poles[i]->a=Point(153+baseX+200*i,baseHeight);
		poles[i]->b=Point(153+baseX+10+200*i,baseHeight+poleHeight);
		poleX[i] = poles[i]->a.x;
		root->append_child(poles[i]);
	}

	for(int i=0;i<20;i++) {

        //discs[i]->hidden = 1;
        discWidths[i] = firstDiscWidth - diskDifference*i;
        int w = discWidths[i];
		discs[i]=new Div();
		discs[i]->color = CYAN;
		discs[i]->filled = 0;
		discs[i]->gradient = constants.GRADIENT_CENTER;
		discs[i]->gradient_step = 1;
		drawDisc(i,w,0,discDepth*i);
		poleMax[0] += discDepth;
		howManyAtPole[0]++;
		discsAtPole[0].push(i);
		atPole[i] = 0;
		root->append_child(discs[i]);
	}

	slider_global.createSliderDiv(400,630,1,162,162);

	// select toh in the beginning
	menu_toh_click();

    iInitialize(width, height, title);

}
/******************************** iGraphics functions ************************************/
void iDraw() {
	Draw(root);
}
/*
	function iMouseMove() is called when the user presses and drags the mouse.
	(mx, my) is the position where the mouse pointer is.
*/
void iMouseMove(int mx, int my) {
    //printf("x = %d, y= %d\n",mx,my);
    //place your codes here
}
/*
	function iMouse() is called when the user presses/releases the mouse.
	(mx, my) is the position where the mouse pointer is.
*/
void iMouse(int button, int state, int mx, int my)
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
       handle_click(root,mx,my);
    }
    if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {

   	}
}
/*
	function iKeyboard() is called whenever the user hits a key in keyboard.
	key- holds the ASCII value of the key pressed.
*/
void iKeyboard(unsigned char key) {

}
/*
	function iSpecialKeyboard() is called whenver user hits special keys like-
	function keys, home, end, pg up, pg down, arraows etc. you have to use
	appropriate constants to detect them. A list is:
	GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
	GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11, GLUT_KEY_F12,
	GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN, GLUT_KEY_PAGE UP,
	GLUT_KEY_PAGE DOWN, GLUT_KEY_HOME, GLUT_KEY_END, GLUT_KEY_INSERT
*/
void iSpecialKeyboard(unsigned char key) {
    if(key == GLUT_KEY_END) {
        exit(0);
    }
    //place your codes for other keys here
}
int main() {
    initialize(screenWidth, screenHeight, "solveing Algorithms");
    return 0;
}

