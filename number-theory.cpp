//# include "iGraphics.h"
#include<windows.h>
#include<bits/stdc++.h>
#include<mmsystem.h>

using namespace std;
#include "qswift.h";

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

#define debug(i,j) printf("%d   %d\n\n",i,j)


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

// General Design Global

int screenWidth = 1400;
int screenHeight = 720;

// General Global

void (*solve_pointer)(int,int);

// Prime Sieve Globals

int totalNumSieve = 162;

int numbers_in_a_row = 18;
int upHeight = 550;
int leftX = 360;
int blockSide = 50;
int blockOffset = 5;

int primeFound = 0;

/********************************* Thread Handles *********************************/
HANDLE solve_handle, update_handle;

/********************************* Elements ***************************************/

//Sidebar Menu
Div *menu = new Div();
Div *selected = new Div();

Div *menu_primesieve = new Div();
Div *menu_pascaltriangle = new Div();

//solve Visual
Div *numbers[500];

Div *lines[50][50];
Div *borderCircles[50];
Div *borderOutCircles[50];
Div *bestline = new Div();

//Array Controls

Div *reset_btn = new Div();
Div *solve_btn = new Div();

Div *valueBox = new Div();

//SliderDiv slider_global;

/******************************** Thread functions ********************************/
DWORD WINAPI solve_thread() {
	solve_pointer(1, totalNumSieve);
}

/********************************* Helping Functions *********************************/


/******************************** Solving Functions *******************************/


void primesieve_solve(int l, int N) {

    int i,j;
    vector<int>isPrime(N+1,1);

    numbers[1]->color = LIGHT_RED;

    for(i=2;i<=N;i++)
    {
        if(isPrime[i] == 1)
        {
            PlaySound(TEXT("scan3.wav"), NULL , SND_ASYNC);

            numbers[i]->color = CYAN;

            primeFound++;

            char primeText[200];
            strcpy(primeText,"");
            char primeNum[100];
            itoa(primeFound,primeNum,10);
            strcat(primeText,primeNum);
            strcat(primeText,"     Primes Found  ");
            strcpy( valueBox->text,primeText);

            j = 2*i;
            while(j<=N)
            {
                isPrime[j] = 0;
                numbers[j]->color = LIGHT_RED;
                j = j+i;
                Sleep(30);
            }
        }
        Sleep(50);
    }

    Sleep(700);
    PlaySound(TEXT("click.wav"), NULL , SND_ASYNC);

}

/******************************** Button Controls *********************************/

void sayHi() {
	Div *hi = new Div();
	hi->a=Point(rand()%500, rand()%500);
	hi->b=Point(hi->a.x+100, hi->a.y+100);
	//hi->text="Hi";
	root->append_child(hi);
}
void reset_btn_click() {

    TerminateThread(solve_handle,0);

    primeFound = 0;

    char primeText[200];
	strcpy(primeText,"");
	char primeNum[100];
	itoa(primeFound,primeNum,10);
	strcat(primeText,primeNum);
	strcat(primeText,"  Primes Found  ");
	strcpy( valueBox->text,primeText);

    int j = 0;  // now in row

    for(int i=1;i<=162;i++)
    {
        numbers[i]->hidden = 1;
    }

	for(int i=1;i<=totalNumSieve;i++) {

        numbers[i]->hidden = 0;

        //printf("Okay\n\n");

		numbers[i]->filled=1;
		numbers[i]->positioning = constants.DIV_RELATIVE;
		if(i%numbers_in_a_row == 0)
        {
            numbers[i]->a=Point( leftX + (numbers_in_a_row-1)*( blockSide + blockOffset ), upHeight - j*( blockSide + blockOffset ));
        }
        else
        {
            numbers[i]->a=Point( leftX + ((i%numbers_in_a_row)-1)*( blockSide + blockOffset ), upHeight - j*( blockSide + blockOffset ));
        }
		numbers[i]->b = Point(blockSide,blockSide);
		numbers[i]->color = WHITE;
		numbers[i]->text_color = BLACK;
		char str_num[100];
		itoa(i,str_num,10);
		strcpy(numbers[i]->text,str_num);

		root->append_child(numbers[i]);

		if(i%numbers_in_a_row == 0)
            j++;
	}


}
void solve_btn_click() {

    reset_btn_click();

    DWORD solveId;
	solve_handle = CreateThread(NULL, 0, solve_thread, NULL, 0, NULL);
}

void menu_primesieve_click() {

    slider_variable = &totalNumSieve;

    slider_apply_function = reset_btn_click;

    slider_global.updateSliderDiv(400,630,1,162,162);

    reset_btn_click();

	selected->bg_color=Point(0,0,0);
	selected = menu_primesieve;
	selected->bg_color=Point(110,0,0);
	solve_pointer=primesieve_solve;
}



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

    printf("Okay\n\n");

	root->a=Point(0,0);
	root->b=Point(screenWidth,screenHeight);

	//printf("Okay\n\n");

	menu->a=Point(10,10);
	menu->b=Point(300,screenHeight - 10);
	root->append_child(menu);

	// select primesieve solve in the beginning


	menu_primesieve->a=Point(10,610);
	menu_primesieve->b=Point(300,710);
	strcpy( menu_primesieve->text,"Prime Sieve");
	menu_primesieve->click = menu_primesieve_click;
	root->append_child(menu_primesieve);

	reset_btn->a=Point(400,40);
	reset_btn->b=Point(500,90);
	strcpy( reset_btn->text,"Reset");
	reset_btn->click=reset_btn_click;
	root->append_child(reset_btn);

	solve_btn->a=Point(550,40);
	solve_btn->b=Point(650,90);
	strcpy( solve_btn->text,"Solve");
	solve_btn->click=solve_btn_click;
	root->append_child(solve_btn);

	valueBox->a=Point(1030,630);
	valueBox->positioning = constants.DIV_RELATIVE;
	valueBox->b=Point(300,50);
	valueBox->color = LIGHT_GREEN;
	valueBox->filled = 1;
	valueBox->text_color = BLACK;

	char primeText[200];
	strcpy(primeText,"");
	char primeNum[100];
	itoa(primeFound,primeNum,10);
	strcat(primeText,primeNum);
	strcat(primeText,"  Primes Found  ");
	strcpy( valueBox->text,primeText);

	root->append_child(valueBox);

	//printf("Okay\n\n");

	int j = 0;  // now in row

	for(int i=1;i<=162;i++) {


        //printf("Okay\n\n");

		numbers[i]=new Div();
		numbers[i]->filled=1;
		numbers[i]->positioning = constants.DIV_RELATIVE;
		if(i%numbers_in_a_row == 0)
        {
            numbers[i]->a=Point( leftX + (numbers_in_a_row-1)*( blockSide + blockOffset ), upHeight - j*( blockSide + blockOffset ));
        }
        else
        {
            numbers[i]->a=Point( leftX + ((i%numbers_in_a_row)-1)*( blockSide + blockOffset ), upHeight - j*( blockSide + blockOffset ));
        }
		numbers[i]->b = Point(blockSide,blockSide);
		numbers[i]->color = WHITE;
		numbers[i]->text_color = BLACK;
		char str_num[100];
		itoa(i,str_num,10);
		strcpy(numbers[i]->text,str_num);

		root->append_child(numbers[i]);

		if(i%numbers_in_a_row == 0)
            j++;
	}

	slider_global.createSliderDiv(400,630,1,162,162);

    // do not create multiple instances
	// if needed update the global slider in menu functions


	//Sleep(2000);

	//slider_global.changeValue(50);

    //printf("Okay \n");

    menu_primesieve_click();

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
    printf("x = %d, y= %d\n",mx,my);
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
    initialize( screenWidth, screenHeight, "solveing Algorithms");
    return 0;
}


