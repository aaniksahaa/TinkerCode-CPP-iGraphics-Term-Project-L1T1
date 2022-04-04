# include "iGraphics.h"
#include<windows.h>
#include<bits/stdc++.h>

using namespace std;
#include "swift.h";
/********************************* Globals ****************************************/
void (*sort_pointer)(int,int);
int delay = 50;
/********************************* Thread Handles *********************************/
HANDLE sort_handle, update_handle;

/********************************* Elements ***************************************/

//Sidebar Menu
Div *menu = new Div();
Div *selected = new Div();

Div *menu_bubble = new Div();
Div *menu_selection = new Div();
Div *menu_insertion = new Div();
Div *menu_quick = new Div();
Div *menu_merge = new Div();
//Sort Visual
Div *tiles[50];
//Array Controls
Div *array_display = new Div();
Div *randomize_btn = new Div();
Div *sort_btn = new Div();

/******************************** Thread functions ********************************/
DWORD WINAPI sort_thread() {
	sort_pointer(0, 19);
	for(int i=0;i<20;i++) {
		tiles[i]->color=Point(0,255,0);
		tiles[i]->bgColor=Point(0,255,0);
		Sleep(delay);
	}
}
DWORD WINAPI update_thread() {
	while(1) {
		string s="Array: ";
		for(int i=0;i<20;i++) {
			s+=to_string((int)(tiles[i]->b.y - tiles[i]->a.y));
			s+=", ";
		}
		array_display->text=s;
		Sleep(delay);
	}
}
/******************************** Sorting Functions *******************************/
void bubble_sort(int l, int r) {
	for(int i=0;i<20;i++) {
		for(int j=i+1;j<20;j++) {
			if(tiles[i]->b.y > tiles[j]->b.y) {

                PlaySound(TEXT("scan3.wav"), NULL , SND_ASYNC);
				tiles[i]->color=Point(255,255,0);
				tiles[i]->bgColor=Point(255,255,0);
				tiles[j]->color=Point(255,255,0);
				tiles[j]->bgColor=Point(255,255,0);
				Sleep(2*delay);

				swap(tiles[i]->b.y, tiles[j]->b.y);
				Sleep(2*delay);
				tiles[i]->color=Point(255,0,0);
				tiles[j]->color=Point(255,0,0);
				tiles[i]->bgColor=Point(255,0,0);
				tiles[j]->bgColor=Point(255,0,0);
			}
		}
	}
}

void merge_sort(int l, int r) {
	if(l==r) {
		tiles[l]->color=Point(0,0,255);
		Sleep(delay);
		return;
	}
	int m=(l+r)/2;
	merge_sort(l,m);
	merge_sort(m+1,r);
	int merged[20];
	int i=l,j=m+1,at=0;
	while(i<=m && j<=r) {
		tiles[i]->color=Point(255,255,0);
		tiles[j]->color=Point(255,255,0);
		if(tiles[i]->b.y < tiles[j]->b.y) merged[at++]=tiles[i++]->b.y;
		else merged[at++]=tiles[j++]->b.y;
	}
	while(i<=m) tiles[i]->color=Point(255,255,0),merged[at++]=tiles[i++]->b.y;
	while(j<=r) tiles[j]->color=Point(255,255,0),merged[at++]=tiles[j++]->b.y;
	at=0;
	for(i=l;i<=r;i++) {
		tiles[i]->b.y=merged[at++];
		tiles[i]->color=Point(255,0,0);
		Sleep(delay);
	}
}
/******************************** Button Controls *********************************/

void sayHi() {
	Div *hi = new Div();
	hi->a=Point(rand()%500, rand()%500);
	hi->b=Point(hi->a.x+100, hi->a.y+100);
	hi->text="Hi";
	root->appendChild(hi);
}
void randomize_btn_click() {
	for(int i=0;i<20;i++) {
		tiles[i]->b=Point(430+30*i,400+rand()%300);
		tiles[i]->color=Point(255,0,0);
	}
}
void sort_btn_click() {
    DWORD sortId;
	sort_handle = CreateThread(NULL, 0, sort_thread, NULL, 0, NULL);
}
void menu_bubble_click() {
	selected->bgColor=Point(0,0,0);
	selected = menu_bubble;
	selected->bgColor=Point(110,0,0);
	sort_pointer=bubble_sort;
}
void menu_selection_click() {
	selected->bgColor=Point(0,0,0);
	selected = menu_selection;
	selected->bgColor=Point(110,0,0);
}
void menu_insertion_click() {
	selected->bgColor=Point(0,0,0);
	selected = menu_insertion;
	selected->bgColor=Point(110,0,0);
}
void menu_quick_click() {
	selected->bgColor=Point(0,0,0);
	selected = menu_quick;
	selected->bgColor=Point(110,0,0);
}
void menu_merge_click() {
	selected->bgColor=Point(0,0,0);
	selected = menu_merge;
	selected->bgColor=Point(110,0,0);
	sort_pointer=merge_sort;
}
//


void initialize(int width,int height, char title[]) {

	root->a=Point(10,10);
	root->b=Point(1190,710);

	menu->a=Point(10,10);
	menu->b=Point(300,710);
	root->appendChild(menu);

	// select bubble sort in the beginning
	menu_bubble_click();

	menu_bubble->a=Point(10,610);
	menu_bubble->b=Point(300,710);
	menu_bubble->text="Bubble Sort";
	menu_bubble->click = menu_bubble_click;
	root->appendChild(menu_bubble);


	menu_selection->a=Point(10,510);
	menu_selection->b=Point(300,610);
	menu_selection->text="Selection Sort";
	menu_selection->click = menu_selection_click;
	root->appendChild(menu_selection);


	menu_insertion->a=Point(10,410);
	menu_insertion->b=Point(300,510);
	menu_insertion->text="Insertion Sort";
	menu_insertion->click = menu_insertion_click;
	root->appendChild(menu_insertion);


	menu_quick->a=Point(10,310);
	menu_quick->b=Point(300,410);
	menu_quick->text="Quick Sort";
	menu_quick->click = menu_quick_click;
	root->appendChild(menu_quick);

	menu_merge->a=Point(10,210);
	menu_merge->b=Point(300,310);
	menu_merge->text="Merge Sort";
	menu_merge->click = menu_merge_click;
	root->appendChild(menu_merge);

	array_display->a=Point(400,200);
	array_display->b=Point(710,300);
	array_display->color=Point(0,0,0);
	array_display->text="Array: ";
	array_display->click=sayHi;
	root->appendChild(array_display);

	randomize_btn->a=Point(400,100);
	randomize_btn->b=Point(500,150);
	randomize_btn->text="Randomize";
	randomize_btn->click=randomize_btn_click;
	root->appendChild(randomize_btn);

	sort_btn->a=Point(600,100);
	sort_btn->b=Point(700,150);
	sort_btn->text="Sort";
	sort_btn->click=sort_btn_click;
	root->appendChild(sort_btn);

	for(int i=0;i<20;i++) {
		tiles[i]=new Div();
		tiles[i]->filled=1;
		tiles[i]->a=Point(400+30*i,400);
		tiles[i]->b=Point(430+30*i,400+rand()%300);
		root->appendChild(tiles[i]);
	}

	update_handle = CreateThread(NULL, 0, update_thread, NULL, 0, NULL);
    iInitialize(width, height, title);
}
/******************************** iGraphics functions ************************************/
void iDraw() {
	Draw();
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
       handleClick(root,mx,my);
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
    initialize(1400, 720, "Sorting Algorithms");
    return 0;
}
