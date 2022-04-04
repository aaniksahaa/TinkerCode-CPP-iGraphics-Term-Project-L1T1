#include<windows.h>
#include<bits/stdc++.h>
using namespace std;
#include "qswift.h";
/********************************* Globals ****************************************/
void (*search_pointer)(void);
int delay = 50;

/********************************* Thread Handles *********************************/
HANDLE search_handle, search_update_handle;

/********************************* Elements ***************************************/

//Sidebar Menu
Div *menu = new Div();
Div *selected = new Div();
Div *back = new Div();
Div *description = new Div();

Div *menu_binary_search = new Div();
Div *menu_linear_search = new Div();

Div *search_btn = new Div();
Div *sort_btn = new Div();

Div *randomize_btn = new Div();
Div *search_input = new Div();

Div *search_array[30];
//Sort Visual

/******************************** User input **************************/
int search_array_size = 30;
int search_array_start = 350;
int search_array_width = 30;
/******************************** Searching Functions *******************************/
int search_array_sorted = 0;
void linear_search() {
	int x=atoi(search_input->text);
	for(int i=0;i<search_array_size;i++) {
		Yellow(search_array[i]);
		Sleep(delay*3);
		if(x == atoi(search_array[i]->text)) {
			Green(search_array[i]);
			break;
		}
		Red(search_array[i]);
		Sleep(delay*3);
	}
}
void Binary_search() {
	if(!search_array_sorted) return;
	int x=atoi(search_input->text);
	int l=0, r=search_array_size-1;
	while(l<r) {
		int m=(l+r)/2;
		Yellow(search_array[m]);
		Sleep(delay*5);
		if(atoi(search_array[m]->text) < x) l=m+1;
		else r=m;
		Red(search_array[m]);
		Sleep(delay*3);
	}
	if(atoi(search_array[l]->text) == x)
    Green(search_array[l]);
}
/******************************** Thread functions ********************************/

DWORD WINAPI search_update_thread() {

}
DWORD WINAPI search_thread() {
	search_pointer();
}

/******************************** Button Controls *********************************/
void randomize_btn_click() {
	for(int i=0;i<search_array_size;i++) {
		char number[5];
		int rnd=rand()%100;
		itoa(rnd, number, 10);
		search_array[i]=new Div();
		search_array[i]->a=Point(search_array_start+search_array_width*i,400);
		search_array[i]->b=Point(search_array_start+search_array_width*(i+1),400 + search_array_width);
		strcpy(search_array[i]->text,number);
		search_array[i]->color=Point(255,0,0);
		root->append_child(search_array[i]);
	}
	for(int i=search_array_size+1;i<20;i++) {
		search_array[i]->hidden=1;
	}
	search_array_sorted = 0;
}
void sort_btn_click() {
	for(int i=0;i<search_array_size;i++) {
		for(int j=1;j<search_array_size;j++) {
			if(atoi(search_array[j-1]->text) > atoi(search_array[j]->text)) {
				swap(search_array[j-1]->text,search_array[j]->text);
			}
		}
	}
	search_array_sorted = 1;
}
void search_btn_click() {
	search_handle = CreateThread(NULL, 0, search_thread, NULL, 0, NULL);
}
void menu_linear_search_click() {
	selected->bg_color=Point(0,0,0);
	selected = menu_linear_search;
	selected->bg_color=Point(110,0,0);
	search_pointer=linear_search;
	strcpy(description->text,"Linear Search is a searching algorithm that searches for the given element sequentially from the beginning of the array. The array need not be be sorted. Worst case time complexity is O(n).");
}
void menu_binary_search_click() {
	selected->bg_color=Point(0,0,0);
	selected = menu_binary_search;
	selected->bg_color=Point(110,0,0);
	search_pointer=Binary_search;
	strcpy(description->text,"Binary Search is a searching algorithm used in a sorted array by repeatedly dividing the search interval in half. The idea of binary search is to use the information that the array is sorted and reduce the time complexity to O(Log n). ");
}

void initialize(int width,int height, char title[]) {

	root->a=Point(0,0);
	root->b=Point(1400,720);
	root->color=Point(0,0,0);

	description->a=Point(1100,0);
	description->b=Point(1350,670);
	description->text_align=constants.ALIGN_LEFT | constants.ALIGN_UP;
	description->color=Point(0,0,0);
	root->append_child(description);

	menu->a=Point(0,0);
	menu->b=Point(300,720);
	root->append_child(menu);

	back->positioning=constants.DIV_RELATIVE;
	back->a=Point(0, 650);
	back->b=Point(300,70);
	strcpy(back->text," < Back ");
	back->text_align=constants.ALIGN_LEFT | constants.ALIGN_CENTER_Y;
	menu->append_child(back);

	// select bubble sort in the beginning
	menu_linear_search_click();

	menu_linear_search->a=Point(0,550);
	menu_linear_search->b=Point(300,100);
	strcpy(menu_linear_search->text,"Linear Search");
	menu_linear_search->positioning=constants.DIV_RELATIVE;
	menu_linear_search->click = menu_linear_search_click;
	menu->append_child(menu_linear_search);


	menu_binary_search->a=Point(0,450);
	menu_binary_search->b=Point(300,100);
	menu_binary_search->positioning=constants.DIV_RELATIVE;
	strcpy(menu_binary_search->text,"Binary Search");
	menu_binary_search->click = menu_binary_search_click;
	menu->append_child(menu_binary_search);

	randomize_btn->a=Point(400,100);
	randomize_btn->b=Point(500,150);
	strcpy(randomize_btn->text,"Randomize");
	randomize_btn->click=randomize_btn_click;
	randomize_btn->line_width = 3;
	root->append_child(randomize_btn);

	sort_btn->a=Point(550,100);
	sort_btn->b=Point(650,150);
	strcpy(sort_btn->text,"Sort");
	sort_btn->click=sort_btn_click;
	sort_btn->line_width = 3;
	root->append_child(sort_btn);

	search_input->a=Point(700,100);
	search_input->b=Point(800,150);
	strcpy(search_input->text,"");
	search_input->type |= constants.DIV_INPUT;
	search_input->color = Point(255,0,0);
	search_input->bg_color = Point(255,255,255);
	search_input->text_color = Point(0,0,0);
	search_input->line_width = 1;
	root->append_child(search_input);

	search_btn->a=Point(850,100);
	search_btn->b=Point(950,150);
	strcpy(search_btn->text,"Search");
	search_btn->line_width = 3;
	search_btn->click=search_btn_click;
	root->append_child(search_btn);

	randomize_btn_click();

	//update_handle = CreateThread(NULL, 0, update_thread, NULL, 0, NULL);
    iInitialize(width, height, title);
}
/******************************** iGraphics functions ************************************/
void iDraw() {
	iClear();
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
    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP) {

	}
    if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {

   	}
}
/*
	function iKeyboard() is called whenever the user hits a key in keyboard.
	key- holds the ASCII value of the key pressed.
*/
void iKeyboard(unsigned char key) {
  handle_key(key);
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
    initialize(1400, 720, "Searching Algorithms");
    return 0;
}
