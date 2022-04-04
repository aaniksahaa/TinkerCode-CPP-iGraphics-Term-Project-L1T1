#include<windows.h>
#include<bits/stdc++.h>
using namespace std;
#include "swift.h";
/********************************* Globals ****************************************/
Div *main_menu = new Div();
Div *menu = new Div();
Div *mm_sortings = new Div();
Div *mm_searching = new Div();
Div *mm_graphs = new Div();
Div *mm_geometry = new Div();
Div *mm_nt = new Div();
Div *mm_circuit = new Div();
Div *mm_logic = new Div();
Div *mm_toh = new Div();
Div *reset_btn = new Div();
Div *step_btn = new Div();

Div *sort_btn = new Div();
Div *simulation_steps[10];

sliderx *sld;
sliderx *speed_sld;

Div *back = new Div();
void back_click();

void clear_root() {
	root->clear();
	root=new Div();
	root->a=Point(0,0);
	root->b=Point(1400,720);
	root->color=Point(0,0,0);

	menu->clear();
	menu=new Div();
	menu->a=Point(0,0);
	menu->b=Point(300,720);
}
void select_step(int x) {
	int i=0;
	while(simulation_steps[i]!=NULL) {
		simulation_steps[i]->color=WHITE;
		simulation_steps[i]->text_color=WHITE;
		i++;
	}
	if(x == -1) return;
	simulation_steps[x]->color=GREEN;
	simulation_steps[x]->text_color=GREEN;
}
int step = 1;
int global_step = 0;
int STEP = 0;
void pause_step() {
	while(global_step < step and STEP) Sleep(10);
	if(STEP) step++;
}

/*

									SORTINGS

*/

/********************************* Globals ****************************************/
void (*sort_pointer)(int,int);
int delay = 200;
int tile_start = 400;
int tile_width = 30;

/********************************* Thread Handles *********************************/
HANDLE simulate_handle = NULL, update_handle = NULL;

/********************************* Elements ***************************************/

//Sidebar Menu
Div *selected = new Div();
Div *pause_btn=new Div;
Div *description = new Div();

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
Div *simulate_btn = new Div();
/******************************** User input **************************/
int tile_count = 20;
int max_tile_count = 20;

/******************************** Thread functions ********************************/
DWORD WINAPI sort_thread() {
	sort_pointer(0, tile_count-1);
	for(int i=0;i<tile_count;i++) {
		tiles[i]->color=Point(0,255,0);
		tiles[i]->bg_color=Point(0,255,0);

		Sleep(delay);
	}
}
DWORD WINAPI update_thread() { // this thread must work very fast :) :) :)
// no sleep
	while(update_handle!=NULL) {
		char s[1000]="";
		for(int i=0;i<tile_count;i++) {
			tiles[i]->hidden=0;
			char number[100];
			itoa(tiles[i]->b.y - tiles[i]->a.y,number,10);
			strcat(s, number);
			char comma[]=", ";
			strcat(s, comma);
		}
		for(int i=tile_count;i<max_tile_count;i++) tiles[i]->hidden=1;
		strcpy(array_display->text,s);
	}
}
/******************************** Sorting Functions *******************************/
void select(int i) {
	tiles[i]->color=Point(255,255,0);
	tiles[i]->bg_color=Point(255,255,0);
}
void select2(int i) {
	tiles[i]->color=Point(0,0,255);
	tiles[i]->bg_color=Point(0,0,255);
}
void unselect(int i) {
	tiles[i]->color=Point(255,0,0);
	tiles[i]->bg_color=Point(255,0,0);
}
void sorted(int i) {
	tiles[i]->color=Point(0,255,0);
	tiles[i]->bg_color=Point(0,255,0);
}

char *bubble_sort_steps[]={"Comparing two elements","Swapping"};
void bubble_sort(int l, int r) {
	for(int i=0;i<tile_count;i++) {
		for(int j=i+1;j<tile_count;j++) {
			select_step(0);
			pause_step();
			Yellow(tiles[i]);
			Yellow(tiles[j]);
			Sleep(delay);
			pause_step();
			if(tiles[i]->b.y > tiles[j]->b.y) {
				Blue(tiles[i]);
				Blue(tiles[j]);
				select_step(1);
				Sleep(delay);
				swap(tiles[i]->b.y, tiles[j]->b.y);
				Sleep(delay);
				White(tiles[i]);
				White(tiles[j]);
			}
			else {
				White(tiles[i]);
				White(tiles[j]);
				Sleep(delay);
			}

		}
	}
}
char *selection_sort_steps[]={"Comparing","Updating minimum element","Swapping"};
void selection_sort(int l,int r) {
	for(int i=0;i<tile_count;i++) {
		int mn = INT_MAX, mn_ = -1;
		for(int j=i;j<tile_count;j++) {
			select_step(0);
			Yellow(tiles[j]);
			pause_step();
			Sleep(delay);
			if(tiles[j]->b.y < mn) {
				select_step(1);
				Blue(tiles[j]);
				mn = tiles[j]->b.y;
				mn_ = j;
				pause_step();
				Sleep(delay);
			}
			White(tiles[j]);
			Sleep(delay);
		}
		Red(tiles[i]);
		Red(tiles[mn_]);
		select_step(2);
		swap(tiles[i]->b.y, tiles[mn_]->b.y);
		pause_step();
		Sleep(2*delay);
		White(tiles[i]);
		White(tiles[mn_]);
		Sleep(delay);
	}
}
char *insertion_sort_steps[]={"Comparing","Swapping"};
void insertion_sort(int l,int r) {
    for (int i = 1; i < tile_count; i++)
    {
        int key = tiles[i]->b.y;
        int j = i - 1;
        Yellow(tiles[i]);
        select_step(0);
        pause_step();
        Sleep(delay*2);
        /* Move elements of arr[0..i-1], that are
        greater than key, to one position ahead
        of their current position */
        while (j >= 0 && tiles[j]->b.y > key)
        {
        	Blue(tiles[j]);
        	select_step(1);
            tiles[j + 1]->b.y = tiles[j]->b.y;
            pause_step();
            Sleep(delay);
            Red(tiles[j]);
            Red(tiles[j+1]);
            Sleep(delay);
            White(tiles[j+1]);
            White(tiles[j]);
			j = j - 1;
        }
        tiles[j+1]->b.y = key;
        Pink(tiles[j+1]);
        Sleep(delay);
        White(tiles[i]);
		White(tiles[j+1]);
    }
}
char *merge_sort_steps[]={"Dividing range","Merging"};
void merge_sort(int l, int r) {
	if(l==r) {
		Red(tiles[l]);
		select_step(1);
		pause_step();
		Sleep(delay/2);
		White(tiles[l]);
		Sleep(delay/2);
		return;
	}
	Yellow(tiles[l]);
	Yellow(tiles[r]);
	select_step(0);
	pause_step();
	Sleep(delay);
	White(tiles[l]);
	White(tiles[r]);

	int m=(l+r)/2;
	merge_sort(l,m);
	merge_sort(m+1,r);
	int merged[tile_count];
	int i=l,j=m+1,at=0;

	Blue(tiles[l]);
	Blue(tiles[r]);
	select_step(1);
	pause_step();
	Sleep(delay);
	while(i<=m && j<=r) {
		Red(tiles[i]);
		Red(tiles[j]);
		if(tiles[i]->b.y < tiles[j]->b.y) merged[at++]=tiles[i++]->b.y;
		else merged[at++]=tiles[j++]->b.y;
		Sleep(delay/3);
	}
	while(i<=m) Red(tiles[i]),merged[at++]=tiles[i++]->b.y;
	while(j<=r) Red(tiles[j]),merged[at++]=tiles[j++]->b.y;
	at=0;
	for(i=l;i<=r;i++) {
		tiles[i]->b.y=merged[at++];
		White(tiles[i]);
		Sleep(delay/3);
	}
}

/******************************** Button Controls *********************************/

void sorting_reset_btn_click() {
	TerminateThread(simulate_handle,0);
	simulate_handle = NULL;
	global_step=0;
	step_btn->hidden = 0;
	simulate_btn->hidden = 0;
	STEP = 0;
	select_step(-1);
	step = 1;
	for(int i=0;i<tile_count;i++) {
		tiles[i]->b=Point(tile_start+tile_width*(i+1),400+rand()%300);
		tiles[i]->color=Point(255,0,0);
		tiles[i]->color=WHITE;
	}
}
void sorting_simulate_btn_click() {
	TerminateThread(simulate_handle,0);
	simulate_handle = NULL;

	global_step=0;
	STEP = 0;
	select_step(-1);
	step = 1;

	step_btn->hidden = 1;
	simulate_btn->hidden = 1;

	STEP = 0;
	simulate_handle = CreateThread(NULL, 0, sort_thread, NULL, 0, NULL);
}
void step_btn_click() {
	STEP = 1;
	if(simulate_handle == NULL) {
		simulate_handle = CreateThread(NULL, 0, sort_thread, NULL, 0, NULL);
	}
	simulate_btn->hidden = 1;
	global_step+=1;
}

void menu_bubble_click() {

	selected->bg_color=theme->bg_color;
	selected = menu_bubble;
	selected->bg_color=Point(110,0,0);
	sort_pointer=bubble_sort;
	strcpy(description->text,"Bubble sort, sometimes referred to as sinking sort, is a simple sorting algorithm that repeatedly steps through the list, compares adjacent elements and swaps them if they are in the wrong order. The pass through the list is repeated until the list is sorted. The algorithm, which is a comparison sort, is named for the way smaller or larger elements bubble to the top of the list. ");
	for(int i=0;i<2;i++) {
		simulation_steps[i]->hidden = 0;
		strcpy(simulation_steps[i]->text,bubble_sort_steps[i]);
	}
	for(int i=2;simulation_steps[i]!=NULL;i++) {
		simulation_steps[i]->hidden = 1;
	}
}
void menu_selection_click() {
	selected->bg_color=theme->bg_color;
	selected = menu_selection;
	selected->bg_color=Point(0,110,0);
	sort_pointer = selection_sort;
	for(int i=0;i<3;i++) {
		simulation_steps[i]->hidden = 0;
		strcpy(simulation_steps[i]->text,selection_sort_steps[i]);
	}
	for(int i=3;simulation_steps[i]!=NULL;i++) {
		simulation_steps[i]->hidden = 1;
	}
}
void menu_insertion_click() {
	selected->bg_color=theme->bg_color;
	selected = menu_insertion;
	selected->bg_color=Point(0,0,110);
	sort_pointer = insertion_sort;
	for(int i=0;i<2;i++) {
		simulation_steps[i]->hidden = 0;
		strcpy(simulation_steps[i]->text,insertion_sort_steps[i]);
	}
	for(int i=2;simulation_steps[i]!=NULL;i++) {
		simulation_steps[i]->hidden = 1;
	}
}
void menu_quick_click() {
	selected->bg_color=theme->bg_color;
	selected = menu_quick;
	selected->bg_color=Point(110,110,0);
}
void menu_merge_click() {
	selected->bg_color=theme->bg_color;
	selected = menu_merge;
	selected->bg_color=Point(110,0,110);
	sort_pointer=merge_sort;
	for(int i=0;i<2;i++) {
		simulation_steps[i]->hidden = 0;
		strcpy(simulation_steps[i]->text,merge_sort_steps[i]);
	}
	for(int i=2;simulation_steps[i]!=NULL;i++) {
		simulation_steps[i]->hidden = 1;
	}
}
//

void sorting_slider_updated(Point p, Div *d) {
	int val = atoi(d->text);
	tile_count = val;
	printf("slider = %d\n",val);
}

void speed_slider_updated(Point p, Div *d) {
	int val = atoi(d->text);
	delay = 1000/val;
}

void initialize_sortings() {
	clear_root();

	description->a=Point(1100,300);
	description->b=Point(1350,670);
	//strcpy(description->text,"hello world!#this is multiline#:D");
	description->text_align=constants.ALIGN_LEFT | constants.ALIGN_UP;
//	strcpy(description->img_path,".\\bg.bmp");
//	description->img_pos=Point(0,0);
	root->append_child(description);

	for(int i=0;i<5;i++) {
		simulation_steps[i] = new Div();
		simulation_steps[i]->a=Point(1100,210-i*70);
		simulation_steps[i]->b=Point(1350,280-i*70);
		simulation_steps[i]->text_align=constants.ALIGN_LEFT | constants.ALIGN_CENTER_Y;
		root->append_child(simulation_steps[i]);
	}


	menu->a=Point(0,0);
	menu->b=Point(300,720);
	root->append_child(menu);

	back->positioning=constants.DIV_RELATIVE;
	back->a=Point(0, 650);
	back->b=Point(300,70);
	strcpy(back->text," < Back ");
	back->click=back_click;
	back->text_align=constants.ALIGN_LEFT | constants.ALIGN_CENTER_Y;
	menu->append_child(back);

	// select bubble sort in the beginning
	menu_bubble_click();
	menu_bubble->on_do_focus(NULL);
	focus=menu_bubble;

	menu_bubble->a=Point(0,550);
	menu_bubble->b=Point(300,100);
	strcpy(menu_bubble->text,"Bubble Sort");
	menu_bubble->positioning=constants.DIV_RELATIVE;
	menu_bubble->click = menu_bubble_click;
	menu->append_child(menu_bubble);


	menu_selection->a=Point(0,450);
	menu_selection->b=Point(300,100);
	menu_selection->positioning=constants.DIV_RELATIVE;
	strcpy(menu_selection->text,"Selection Sort");
	menu_selection->click = menu_selection_click;
	menu->append_child(menu_selection);


	menu_insertion->a=Point(0,350);
	menu_insertion->b=Point(300,100);
	menu_insertion->positioning=constants.DIV_RELATIVE;
	strcpy(menu_insertion->text,"Insertion Sort");
	menu_insertion->click = menu_insertion_click;
	menu->append_child(menu_insertion);


	menu_quick->a=Point(0,250);
	menu_quick->b=Point(300,100);
	strcpy(menu_quick->text,"Quick Sort");
	menu_quick->positioning=constants.DIV_RELATIVE;
	menu_quick->click = menu_quick_click;
	menu->append_child(menu_quick);

	menu_merge->a=Point(0,150);
	menu_merge->b=Point(300,100);
	strcpy(menu_merge->text,"Merge Sort");
	menu_merge->positioning=constants.DIV_RELATIVE;
	menu_merge->click = menu_merge_click;
	menu->append_child(menu_merge);

	array_display->a=Point(tile_start,200);
	array_display->b=Point(1050,300);
	array_display->color=Point(0,0,0);
	root->append_child(array_display);

	reset_btn->a=Point(400,50);
	reset_btn->b=Point(500,100);
	strcpy(reset_btn->text,"Reset");
	reset_btn->click=sorting_reset_btn_click;
	root->append_child(reset_btn);

	simulate_btn->a=Point(550,50);
	simulate_btn->b=Point(650,100);
	strcpy(simulate_btn->text,"Simulate");
	simulate_btn->click=sorting_simulate_btn_click;
	root->append_child(simulate_btn);

	step_btn->a=Point(700,50);
	step_btn->b=Point(800,100);
	strcpy(step_btn->text,"Step");
	step_btn->click=step_btn_click;
	root->append_child(step_btn);

	speed_sld = new sliderx(1,50,5);
	speed_sld->slider->a=Point(850,50);
	speed_sld->slider->b=Point(1050,100);
	speed_sld->slider->color=TRANSPARENT;
	speed_sld->slider->click=speed_slider_updated;
	root->append_child(speed_sld->slider);

	sld= new sliderx(1,max_tile_count,tile_count);
	sld->slider->a=Point(850,100);
	sld->slider->b=Point(1050,150);
	sld->slider->color=TRANSPARENT;
	sld->slider->click=sorting_slider_updated;
	root->append_child(sld->slider);

	for(int i=0;i<max_tile_count;i++) {
		tiles[i]=new Div();
		//tiles[i]->filled=1;
		//tiles[i]->gradient=constants.GRADIENT_CENTER;
		tiles[i]->a=Point(tile_start+tile_width*i,400);
		tiles[i]->b=Point(tile_start+tile_width*(i+1),400+rand()%300);
		tiles[i]->filled=1;
		tiles[i]->color=WHITE;
		root->append_child(tiles[i]);
	}
	TerminateThread(update_handle,0);
	update_handle = NULL;
	update_handle = CreateThread(NULL, 0, update_thread, NULL, 0, NULL);
    //iInitialize(width, height, title);
}
/*


							SORTINGS END



*/
/*


			LOGIC START


*/
// Logic Circuit Elements

#define ANDGATE 201
#define ORGATE 202
#define NOTGATE 203
#define NORGATE 204
#define NANDGATE 205
#define XORGATE 206
#define XNORGATE 207
#define BUFFERGATE 208
// General typedef

typedef long double ld;
typedef pair<int, int> pii;

/********************************* Globals ****************************************/

// DC Circuit Design Globals

int menuWidth = 280;
int gateBtnWidth = menuWidth/2 ;

int lbX1 = 30;
int lbY = 30;
int lbWidth = 1050;
int lbHeight = 560;
int lbOffset = 20;
int InputLines = 4;


int wireLogicCount = 0;

int gateCount = 0;

int nowAddingLogic;
int nowSelectedgateIndex = -1;


/******************************* Logic Circuit Globals *********************************/


int gateSize = 50;
int terminalRadius = 7;

int switchSize = 30;
int switchHeight = 550;
int switchX1 = 70;
int switchOffset = 35;

int terminalSize = 30;

Point nowPoint = Point(0,0);



/********************************* Function Prototypes ****************************************/

void handle_gate_click(Point where, Div *d);



/********************************* Thread Handles *********************************/

HANDLE  gate_add_handle, wire_logic_add_handle ;


vector<Div *>selectedTerminals;

void handle_terminal_click(Point where, Div *d)
{
    cout<<"Yes clicked\n\n";
    selectedTerminals.push_back(d);
}

/*********************** Structures for Combined Elements ************************/

void handle_toggle(Point where,Div *d);


struct constLine{

    Div *valueBox;
    Div *line;
    Div *terminal;

    int value;

    void createConstLine(int ax,int ay,int id)
    {
        value = 0;

        valueBox = new Div();
        valueBox->positioning = constants.DIV_RELATIVE;
        valueBox->a = Point(ax-switchSize/2,ay-switchSize/2);
        valueBox->b = Point(switchSize,switchSize);
        valueBox->color = Point(30,30,30);
        valueBox->text_color = WHITE;
        valueBox->filled = 1;
        valueBox->click_anim=0;
        valueBox->data[0] = id;
        itoa(value,valueBox->text,10);
        valueBox->click = handle_toggle;
        root->append_child(valueBox);

        line = new Div();
        line->type = constants.DIV_LINE;
        line->a = Point(ax,ay-switchSize/2);
        line->b = Point(ax,30);
        line->line_width = 5;
        line->color=RED;
        root->append_child(line);

        terminal = new Div();
        terminal->type |= constants.DIV_CIRCLE;
        terminal->a = Point(ax,ay-30);
        terminal->R = terminalRadius;
        //terminal->filled=1;
        terminal->color=BLACK;
        terminal->data[0] = (-2)*(id+1);
        terminal->data[2] = 0; //constant
        terminal->data[3] = 0; // always not occupied
        terminal->click = handle_terminal_click;
        root->append_child(terminal);

    }
    void toggleValue()
    {
        printf("Toggling\n");
        value = 1-value;
        if(value == 1)
            valueBox->color = DARK_GREEN;
        else
            valueBox->color = Point(30,30,30);
        itoa(value,valueBox->text,10);
    }

};

constLine constLines[5];

int getOutVal(int id);
void key_press(char key);


struct gateDiv{

    Div *bg;

    Div *inT1;
    Div *inT2;
    Div *outT;

    Div *inC1;
    Div *inC2;
    Div *outC;

    Div *outValueBox;

    int typeC;
    int index;

    int childrenId[2];
    int parentId;

    int inVal[2];
    int outVal;

    int px,py;


    void createGateDiv(int type, int ax, int ay, int id)
    {
    	px = ax;
    	py = ay;
        childrenId[0] = -1;
        childrenId[1] = -1;
        parentId = -1;

        inVal[0] = 0;
        inVal[1] = 0;
        outVal = 0;

        typeC = type;
        index = id;
        if( type == ANDGATE )
        {

            bg = new Div();
            bg->positioning = constants.DIV_RELATIVE;
            bg->a = Point(ax-gateSize/2,ay-gateSize/2);
            bg->b = Point(gateSize, gateSize);
            bg->color = RED-Point(60,0,0);
            bg->filled = 1;
            bg->data[0] = id;
            strcpy(bg->text,"AND ");
            root->append_child(bg);

            inT1 = new Div();
            inT1->type = constants.DIV_LINE;
            inT1->color=RED;
            inT1->a = Point(ax-gateSize/2,ay-gateSize/4);
            inT1->b = Point(ax-gateSize/2 - terminalSize,ay-gateSize/4);
            inT1->line_width = 5;
            root->append_child(inT1);

            inT2 = new Div();
            inT2->color=RED;
            inT2->type = constants.DIV_LINE;
            inT2->a = Point(ax-gateSize/2,ay+gateSize/4);
            inT2->b = Point(ax-gateSize/2 - terminalSize,ay+gateSize/4);
            inT2->line_width = 5;
            root->append_child(inT2);

            outT = new Div();
            outT->color=RED;
            outT->type = constants.DIV_LINE;
            outT->a = Point(ax+gateSize/2,ay);
            outT->b = Point(ax+gateSize/2 + terminalSize,ay);
            outT->line_width = 5;
            root->append_child(outT);

            inC1 = new Div();
           // inC1->positioning = constants.DIV_RELATIVE;
            inC1->type |= constants.DIV_CIRCLE;
            inC1->a = inT1->b;
            inC1->R = terminalRadius;
           // inC1->filled = 1;
            inC1->color = BLACK;
            inC1->data[0] = id;
            inC1->data[2] = -1;
            inC1->data[3] = 0; // not occupied
            inC1->data[4] = 0; // index of child

            inC1->click = handle_terminal_click;
            root->append_child(inC1);

            inC2 = new Div();
            inC2->type |= constants.DIV_CIRCLE;
            inC2->a = inT2->b;
            inC2->R = terminalRadius;
           // inC2->filled = 1;
            inC2->color = BLACK;
            inC2->data[0] = id;
            inC2->data[2] = -1;
            inC2->data[3] = 0;
            inC2->data[4] = 1;
            inC2->click = handle_terminal_click;
            root->append_child(inC2);

            outC = new Div();
            outC->type |= constants.DIV_CIRCLE;
            outC->a = outT->b;
            outC->R = terminalRadius;
            outC->data[0] = id;
            outC->color=BLACK;
           // outC->filled=1;
            outC->data[2] = 1;
            outC->data[3] = 0;
            outC->click = handle_terminal_click;
            root->append_child(outC);

            outValueBox = new Div();
            outValueBox->positioning = constants.DIV_RELATIVE;
            outValueBox->a = Point(outC->a.x - switchSize/2, outC->a.y+10);;
            outValueBox->b = Point(switchSize, switchSize);
            outValueBox->color = Point(40,40,40);
            outValueBox->filled = 1;
            itoa(outVal,outValueBox->text,10);
            root->append_child(outValueBox);

        }

        if(type == ORGATE)
        {
            bg = new Div();
            bg->positioning = constants.DIV_RELATIVE;
            bg->a = Point(ax-gateSize/2,ay-gateSize/2);
            bg->b = Point(gateSize, gateSize);
            bg->color = NAVY;
            bg->filled = 1;
            strcpy(bg->text,"OR ");
            bg->data[0] = id;
            root->append_child(bg);

            inT1 = new Div();
            inT1->type = constants.DIV_LINE;
            inT1->a = Point(ax-gateSize/2,ay-gateSize/4);
            inT1->b = Point(ax-gateSize/2 - terminalSize,ay-gateSize/4);
            inT1->line_width = 5;
            root->append_child(inT1);

            inT2 = new Div();
            inT2->type = constants.DIV_LINE;
            inT2->a = Point(ax-gateSize/2,ay+gateSize/4);
            inT2->b = Point(ax-gateSize/2 - terminalSize,ay+gateSize/4);
            inT2->line_width = 5;
            root->append_child(inT2);

            outT = new Div();
            outT->type = constants.DIV_LINE;
            outT->a = Point(ax+gateSize/2,ay);
            outT->b = Point(ax+gateSize/2 + terminalSize,ay);
            outT->line_width = 5;
            root->append_child(outT);

            inC1 = new Div();
            inC1->type |= constants.DIV_CIRCLE;
            inC1->a = inT1->b;
            inC1->R = terminalRadius;
          //  inC1->filled = 1;
            inC1->color = BLACK;
            inC1->data[0] = id;
            inC1->data[2] = -1;
            inC1->data[3] = 0;
            inC1->data[4] = 0;
            inC1->click = handle_terminal_click;
            root->append_child(inC1);

            inC2 = new Div();
            inC2->type |= constants.DIV_CIRCLE;
            inC2->a = inT2->b;
            inC2->R = terminalRadius;
            inC2->filled = 1;
            inC2->color = BLACK;
            inC2->data[0] = id;
            inC2->data[2] = -1;
            inC2->data[3] = 0;
            inC2->data[4] = 1;
            inC2->click = handle_terminal_click;
            root->append_child(inC2);

            outC = new Div();
            outC->type |= constants.DIV_CIRCLE;
            outC->a = outT->b;
            outC->R = terminalRadius;
            outC->click = handle_terminal_click;
            outC->data[0] = id;
            outC->data[2] = 1;
            outC->data[3] = 0;
            root->append_child(outC);

            outValueBox = new Div();
            outValueBox->positioning = constants.DIV_RELATIVE;
            outValueBox->a = Point(outC->a.x - switchSize/2, outC->a.y+10);;
            outValueBox->b = Point(switchSize, switchSize);
            outValueBox->color = Point(40,40,40);
            outValueBox->filled = 1;
            itoa(outVal,outValueBox->text,10);
            root->append_child(outValueBox);
        }

        if( type == NOTGATE )
        {
            bg = new Div();
            bg->positioning = constants.DIV_RELATIVE;
            bg->a = Point(ax-gateSize/2,ay-gateSize/2);
            bg->b = Point(gateSize, gateSize);
            bg->color = CHOCOLATE;
            bg->filled = 1;
            strcpy(bg->text,"NOT ");
            bg->data[0] = id;
            root->append_child(bg);

            inT1 = new Div();
            inT1->type = constants.DIV_LINE;
            inT1->a = Point(ax-gateSize/2,ay);
            inT1->b = Point(ax-gateSize/2 - terminalSize,ay);
            inT1->line_width = 5;
            root->append_child(inT1);

            outT = new Div();
            outT->type = constants.DIV_LINE;
            outT->a = Point(ax+gateSize/2,ay);
            outT->b = Point(ax+gateSize/2 + terminalSize,ay);
            outT->line_width = 5;
            root->append_child(outT);

            inC1 = new Div();
            inC1->type |= constants.DIV_CIRCLE;
            inC1->a = inT1->b;
            inC1->R = terminalRadius;
            inC1->filled = 1;
            inC1->color = BLACK;
            inC1->data[0] = id;
            inC1->data[2] = -1;
            inC1->data[3] = 0;
            inC1->data[4] = 0;
            inC1->click = handle_terminal_click;
            root->append_child(inC1);

            outC = new Div();
            outC->type |= constants.DIV_CIRCLE;
            outC->a = outT->b;
            outC->R = terminalRadius;
            outC->click = handle_terminal_click;
            outC->data[0] = id;
            outC->data[2] = 1;
            outC->data[3] = 0;
            root->append_child(outC);

            outValueBox = new Div();
            outValueBox->positioning = constants.DIV_RELATIVE;
            outValueBox->a = Point(outC->a.x - switchSize/2, outC->a.y+10);;
            outValueBox->b = Point(switchSize, switchSize);
            outValueBox->color = Point(40,40,40);
            outValueBox->filled = 1;
            itoa(outVal,outValueBox->text,10);
            root->append_child(outValueBox);
        }

        if(type == XORGATE)
        {
            bg = new Div();
            bg->positioning = constants.DIV_RELATIVE;
            bg->a = Point(ax-gateSize/2,ay-gateSize/2);
            bg->b = Point(gateSize, gateSize);
            bg->color = Point(140,1,122);
            bg->filled = 1;
            strcpy(bg->text,"XOR ");
            bg->data[0] = id;
            root->append_child(bg);

            inT1 = new Div();
            inT1->type = constants.DIV_LINE;
            inT1->a = Point(ax-gateSize/2,ay-gateSize/4);
            inT1->b = Point(ax-gateSize/2 - terminalSize,ay-gateSize/4);
            inT1->line_width = 5;
            root->append_child(inT1);

            inT2 = new Div();
            inT2->type = constants.DIV_LINE;
            inT2->a = Point(ax-gateSize/2,ay+gateSize/4);
            inT2->b = Point(ax-gateSize/2 - terminalSize,ay+gateSize/4);
            inT2->line_width = 5;
            root->append_child(inT2);

            outT = new Div();
            outT->type = constants.DIV_LINE;
            outT->a = Point(ax+gateSize/2,ay);
            outT->b = Point(ax+gateSize/2 + terminalSize,ay);
            outT->line_width = 5;
            root->append_child(outT);

            inC1 = new Div();
            inC1->type |= constants.DIV_CIRCLE;
            inC1->a = inT1->b;
            inC1->R = terminalRadius;
            inC1->filled = 1;
            inC1->color = BLACK;
            inC1->data[0] = id;
            inC1->data[2] = -1;
            inC1->data[3] = 0;
            inC1->data[4] = 0;
            inC1->click = handle_terminal_click;
            root->append_child(inC1);

            inC2 = new Div();
            inC2->type |= constants.DIV_CIRCLE;
            inC2->a = inT2->b;
            inC2->R = terminalRadius;
            inC2->filled = 1;
            inC2->color = BLACK;
            inC2->data[0] = id;
            inC2->data[2] = -1;
            inC2->data[3] = 0;
            inC2->data[4] = 1;
            inC2->click = handle_terminal_click;
            root->append_child(inC2);

            outC = new Div();
            outC->type |= constants.DIV_CIRCLE;
            outC->a = outT->b;
            outC->R = terminalRadius;
            outC->click = handle_terminal_click;
            outC->data[0] = id;
            outC->data[2] = 1;
            outC->data[3] = 0;
            root->append_child(outC);

            outValueBox = new Div();
            outValueBox->positioning = constants.DIV_RELATIVE;
            outValueBox->a = Point(outC->a.x - switchSize/2, outC->a.y+10);;
            outValueBox->b = Point(switchSize, switchSize);
            outValueBox->color = Point(40,40,40);
            outValueBox->filled = 1;
            itoa(outVal,outValueBox->text,10);
            root->append_child(outValueBox);
        }

        if(typeC == XNORGATE)
        {

        }

        if(type == NANDGATE)
        {
            bg = new Div();
            bg->positioning = constants.DIV_RELATIVE;
            bg->a = Point(ax-gateSize/2,ay-gateSize/2);
            bg->b = Point(gateSize, gateSize);
            bg->color = Point(4,188,192);
            bg->filled = 1;
            strcpy(bg->text,"NAND ");
            bg->data[0] = id;
            root->append_child(bg);

            inT1 = new Div();
            inT1->type = constants.DIV_LINE;
            inT1->a = Point(ax-gateSize/2,ay-gateSize/4);
            inT1->b = Point(ax-gateSize/2 - terminalSize,ay-gateSize/4);
            inT1->line_width = 5;
            root->append_child(inT1);

            inT2 = new Div();
            inT2->type = constants.DIV_LINE;
            inT2->a = Point(ax-gateSize/2,ay+gateSize/4);
            inT2->b = Point(ax-gateSize/2 - terminalSize,ay+gateSize/4);
            inT2->line_width = 5;
            root->append_child(inT2);

            outT = new Div();
            outT->type = constants.DIV_LINE;
            outT->a = Point(ax+gateSize/2,ay);
            outT->b = Point(ax+gateSize/2 + terminalSize,ay);
            outT->line_width = 5;
            root->append_child(outT);

            inC1 = new Div();
            inC1->type |= constants.DIV_CIRCLE;
            inC1->a = inT1->b;
            inC1->R = terminalRadius;
            inC1->filled = 1;
            inC1->color = BLACK;
            inC1->data[0] = id;
            inC1->data[2] = -1;
            inC1->data[3] = 0;
            inC1->data[4] = 0;
            inC1->click = handle_terminal_click;
            root->append_child(inC1);

            inC2 = new Div();
            inC2->type |= constants.DIV_CIRCLE;
            inC2->a = inT2->b;
            inC2->R = terminalRadius;
            inC2->filled = 1;
            inC2->color = BLACK;
            inC2->data[0] = id;
            inC2->data[2] = -1;
            inC2->data[3] = 0;
            inC2->data[4] = 1;
            inC2->click = handle_terminal_click;
            root->append_child(inC2);

            outC = new Div();
            outC->type |= constants.DIV_CIRCLE;
            outC->a = outT->b;
            outC->R = terminalRadius;
            outC->click = handle_terminal_click;
            outC->data[0] = id;
            outC->data[2] = 1;
            outC->data[3] = 0;
            root->append_child(outC);

            outValueBox = new Div();
            outValueBox->positioning = constants.DIV_RELATIVE;
            outValueBox->a = Point(outC->a.x - switchSize/2, outC->a.y+10);;
            outValueBox->b = Point(switchSize, switchSize);
            outValueBox->color = Point(40,40,40);
            outValueBox->filled = 1;
            itoa(outVal,outValueBox->text,10);
            root->append_child(outValueBox);
        }

        if(type == NORGATE)
        {
            bg = new Div();
            bg->positioning = constants.DIV_RELATIVE;
            bg->a = Point(ax-gateSize/2,ay-gateSize/2);
            bg->b = Point(gateSize, gateSize);
            bg->color = ORANGE - Point(20,20,20);
            bg->filled = 1;
            strcpy(bg->text,"NOR ");
            bg->data[0] = id;
            root->append_child(bg);

            inT1 = new Div();
            inT1->type = constants.DIV_LINE;
            inT1->a = Point(ax-gateSize/2,ay-gateSize/4);
            inT1->b = Point(ax-gateSize/2 - terminalSize,ay-gateSize/4);
            inT1->line_width = 5;
            root->append_child(inT1);

            inT2 = new Div();
            inT2->type = constants.DIV_LINE;
            inT2->a = Point(ax-gateSize/2,ay+gateSize/4);
            inT2->b = Point(ax-gateSize/2 - terminalSize,ay+gateSize/4);
            inT2->line_width = 5;
            root->append_child(inT2);

            outT = new Div();
            outT->type = constants.DIV_LINE;
            outT->a = Point(ax+gateSize/2,ay);
            outT->b = Point(ax+gateSize/2 + terminalSize,ay);
            outT->line_width = 5;
            root->append_child(outT);

            inC1 = new Div();
            inC1->type |= constants.DIV_CIRCLE;
            inC1->a = inT1->b;
            inC1->R = terminalRadius;
            inC1->filled = 1;
            inC1->color = BLACK;
            inC1->data[0] = id;
            inC1->data[2] = -1;
            inC1->data[3] = 0;
            inC1->data[4] = 0;
            inC1->click = handle_terminal_click;
            root->append_child(inC1);

            inC2 = new Div();
            inC2->type |= constants.DIV_CIRCLE;
            inC2->a = inT2->b;
            inC2->R = terminalRadius;
            inC2->filled = 1;
            inC2->color = BLACK;
            inC2->data[0] = id;
            inC2->data[2] = -1;
            inC2->data[3] = 0;
            inC2->data[4] = 1;
            inC2->click = handle_terminal_click;
            root->append_child(inC2);

            outC = new Div();
            outC->type |= constants.DIV_CIRCLE;
            outC->a = outT->b;
            outC->R = terminalRadius;
            outC->click = handle_terminal_click;
            outC->data[0] = id;
            outC->data[2] = 1;
            outC->data[3] = 0;
            root->append_child(outC);

            outValueBox = new Div();
            outValueBox->positioning = constants.DIV_RELATIVE;
            outValueBox->a = Point(outC->a.x - switchSize/2, outC->a.y+10);;
            outValueBox->b = Point(switchSize, switchSize);
            outValueBox->color = Point(40,40,40);
            outValueBox->filled = 1;
            itoa(outVal,outValueBox->text,10);
            root->append_child(outValueBox);
        }
        bg->click_anim=0;
        inT1->color=RED;
        inC1->filled=0;
        if(type!=NOTGATE) inT2->color=RED, inC2->filled=0;
    	outT->color=RED;
    	inC1->color=BLACK;
    	if(type!=NOTGATE) inC2->color=BLACK;
    	outC->color=BLACK;
    	outC->filled=0;
        printf("Done");
        bg->key_pressed = key_press;
    }
    void updateGateDiv(int type, int ax, int ay, int id)
    {
    	px=ax;
    	py=ay;
        typeC = type;
        index = id;
        if( type != NOTGATE )
        {
            bg->a = Point(ax-gateSize/2,ay-gateSize/2);
            bg->b = Point(gateSize, gateSize);

            inT1->a = Point(ax-gateSize/2,ay-gateSize/4);
            inT1->b = Point(ax-gateSize/2 - terminalSize,ay-gateSize/4);

            inT2->a = Point(ax-gateSize/2,ay+gateSize/4);
            inT2->b = Point(ax-gateSize/2 - terminalSize,ay+gateSize/4);

            outT->a = Point(ax+gateSize/2,ay);
            outT->b = Point(ax+gateSize/2 + terminalSize,ay);

            inC1->a = inT1->b;
            inC2->a = inT2->b;
            outC->a = outT->b;

            outValueBox->a = Point(outC->a.x - switchSize/2, outC->a.y+10);;
            outValueBox->b = Point(switchSize, switchSize);
        }
        else
        {
            bg->a = Point(ax-gateSize/2,ay-gateSize/2);
            bg->b = Point(gateSize, gateSize);

            inT1->a = Point(ax-gateSize/2,ay);
            inT1->b = Point(ax-gateSize/2 - terminalSize,ay);

            outT->a = Point(ax+gateSize/2,ay);
            outT->b = Point(ax+gateSize/2 + terminalSize,ay);

            inC1->a = inT1->b;

            outC->a = outT->b;

            outValueBox->a = Point(outC->a.x - switchSize/2, outC->a.y+10);;
            outValueBox->b = Point(switchSize, switchSize);
        }
    }

    void setInVal()
    {
        if(childrenId[0] <= -2)
        {
            int kk = childrenId[0];
            kk = -1 - (kk/2);
            inVal[0] = constLines[kk].value;
        }


        if(childrenId[1] <= -2)
        {
            int kk = childrenId[1];
            kk = -1 - (kk/2);
            inVal[1] = constLines[kk].value;
        }


        if(childrenId[0] >= 0)
        {
            inVal[0] = getOutVal(childrenId[0]);
        }

        if(childrenId[1] >= 0)
        {
            inVal[1] = getOutVal(childrenId[1]);
        }
    }

    void setOutVal()
    {
        if(typeC == ANDGATE)
            outVal = inVal[0] & inVal[1];
        if(typeC == ORGATE)
            outVal = inVal[0] | inVal[1];
        if(typeC == NOTGATE)
            outVal = !(inVal[0]);
        if(typeC == NANDGATE)
            outVal = !(inVal[0] & inVal[1]);
        if(typeC == NORGATE)
            outVal = !(inVal[0] | inVal[1]);
        if(typeC == XORGATE)
            outVal = inVal[0]^inVal[1];

        if(outVal == 1)
            outValueBox->color = DARK_GREEN;
        else
            outValueBox->color = Point(30,30,30);

        itoa(outVal,outValueBox->text,10);

    }

    void hide()
    {


    }

};



/********************************* Elements ***************************************/

//Sidebar Menu

Div *logic_board = new Div();

Div *menu_andgate = new Div();
Div *menu_notgate = new Div();

Div *menu_wire_logic = new Div();
Div *menu_nandgate = new Div();

Div *menu_norgate = new Div();
Div *menu_led = new Div();

Div *menu_xorgate = new Div();
Div *menu_orgate = new Div();

Div *reset_btn_logic = new Div();
Div *simulate_btn_logic = new Div();

Div *delete_logic_btn = new Div();

// Logic gates


gateDiv Gates[500];
 Div *wires[500];

void key_press(char key) {
	auto last = Gates[focus->data[0]];
	int x = last.px, y=last.py;
	if(key == 100) x-=10;
	else if(key == 101) y+=10;
	else if(key == 102) x+=10;
	else if(key == 103) y-=10;
	Gates[last.index].updateGateDiv(last.typeC, x, y, last.index);
}

int getOutVal(int id)
{
    return Gates[id].outVal;
}


void handle_toggle(Point where,Div *d)
{
    int id = d->data[0];
    printf("Here\n\n");
    constLines[id].toggleValue();
}

/******************************** gate control Functions **********************/

void handle_gate_click(Point where, Div *d)
{

}

/******************************** Thread functions ********************************/
vector<pair<Div*,Div*>> Wires;

DWORD WINAPI wire_logic_add_thread() {
    for(int i=0;;)
    {
        if(selectedTerminals.size()>=2)
        {
            printf("Wire Terminals Found");
            int c = wireLogicCount;

            int id1 = selectedTerminals[0]->data[0]; // gate index
            int id2 = selectedTerminals[1]->data[0]; // gate index

            int t1 = selectedTerminals[0]->data[2];
            int t2 = selectedTerminals[1]->data[2];

            int o1 = selectedTerminals[0]->data[3];
            int o2 = selectedTerminals[1]->data[3];

            int n1 = selectedTerminals[0]->data[4];
            int n2 = selectedTerminals[1]->data[4];

            int ax = selectedTerminals[0]->a.x;
            int ay = selectedTerminals[0]->a.y;

            int bx = selectedTerminals[1]->a.x;
            int by = selectedTerminals[1]->a.y;

            if(o1 == 1 || o2 == 1) {
            	selectedTerminals.clear();
            	continue;
			}
            if(t1 == 0)
            {
                if(t2==-1)
                {
                    wires[c] = new Div();
                    wires[c]->type = constants.DIV_LINE;
                    wires[c]->line_width = 5;
                    wires[c]->a = Point(ax,by);
                    wires[c]->b = Point(bx,by);
                    wires[c]->color=RED;
                    //root->append_child(wires[c]);

                    selectedTerminals[1]->data[3] = 1; //occupy

					Div *joint = new Div();

					joint->a=Point(ax,by);
					Wires.push_back({     joint, selectedTerminals[1]});

					Gates[id2].childrenId[n2] = id1;
                }
            }

            else if(t2 == 0)
            {
                if(t1==-1)
                {
                    wires[c] = new Div();
                    wires[c]->type = constants.DIV_LINE;
                    wires[c]->line_width = 5;
                    wires[c]->a = Point(bx,ay);
                    wires[c]->color=RED;
                    wires[c]->b = Point(ax,ay);
                //    root->append_child(wires[c]);

                    selectedTerminals[0]->data[3] = 1;
					Div *joint = new Div();
					joint->a=Point(bx,ay);
					Wires.push_back({     joint, selectedTerminals[0]});

                    Gates[id1].childrenId[n1] = id2;
                }
            }


            else if(t1 == -1)
            {
                if(t2 == 1)
                {
                    wires[c] = new Div();
                    wires[c]->type = constants.DIV_LINE;
                    wires[c]->line_width = 5;
                    wires[c]->a = selectedTerminals[0]->a;
                    wires[c]->b = selectedTerminals[1]->a;
                    wires[c]->color=RED;
				//	root->append_child(wires[c]);

					Wires.push_back({selectedTerminals[0], selectedTerminals[1]});

                    selectedTerminals[0]->data[3] = 1;
                    selectedTerminals[1]->data[3] = 1;

                    Gates[id1].childrenId[n1] = id2;
                }
            }
            else if(t2 == -1)
            {
                if(t1 == 1)
                {
                    wires[c] = new Div();
                    wires[c]->type = constants.DIV_LINE;
                    wires[c]->line_width = 5;
                    wires[c]->a = selectedTerminals[0]->a;
                    wires[c]->color=RED;
                    wires[c]->b = selectedTerminals[1]->a;
                   // root->append_child(wires[c]);

                    selectedTerminals[0]->data[3] = 1;
                    selectedTerminals[1]->data[3] = 1;

					Wires.push_back({selectedTerminals[0], selectedTerminals[1]});
                    Gates[id2].childrenId[n2] = id1;
                }
            }

            else
            {
            	selectedTerminals.clear();
                continue;
            }

            wireLogicCount++;
            selectedTerminals.clear();
        	continue;
        }
        Sleep(100);
    }

}

DWORD WINAPI gate_add_thread() {

	for(int i=0;i<5000;i++)
	{
        if(nowPoint.x > 0)
        {
            printf("Found\n\n");

            int c = gateCount;

            Point color = DARK_GREEN;

            if(nowAddingLogic == ANDGATE)
            {
                Gates[c].createGateDiv(ANDGATE,nowPoint.x,nowPoint.y,c);
            }

            if(nowAddingLogic == ORGATE)
            {

                Gates[c].createGateDiv(ORGATE,nowPoint.x,nowPoint.y,c);
            }

            if(nowAddingLogic == NANDGATE)
            {
                Gates[c].createGateDiv(NANDGATE,nowPoint.x,nowPoint.y,c);
            }

            if(nowAddingLogic == NORGATE)
            {

                Gates[c].createGateDiv(NORGATE,nowPoint.x,nowPoint.y,c);
            }

            if(nowAddingLogic == XORGATE)
            {
                Gates[c].createGateDiv(XORGATE,nowPoint.x,nowPoint.y,c);
            }

            if(nowAddingLogic == XNORGATE)
            {

                Gates[c].createGateDiv(XNORGATE,nowPoint.x,nowPoint.y,c);
            }
            if(nowAddingLogic == NOTGATE)
            {

                Gates[c].createGateDiv(NOTGATE,nowPoint.x,nowPoint.y,c);
            }


            gateCount++;

            printf("\n%d\n",gateCount);
            Sleep(3000);

            nowPoint.x = 0;
            nowPoint.y = 0;

            break;
        }
		Sleep(100);
	}
}
/******************************** Solving Functions *******************************/



void solve_logic_circuit()
{


}


/******************************* Solve Button Functions ************************/

void solve_logic_circuit_btn_click()
{
    cout<<"\n\n";
    int i,j;

    for(i=0;i<gateCount;i++)
    {
        cout<<i<<"  "<<Gates[i].childrenId[0]<<"  "<<Gates[i].childrenId[1]<<"\n\n";
    }

    for(i=0;i<gateCount;i++)
    {
        for(j=0;j<gateCount;j++)
        {
            Gates[j].setInVal();
            Gates[j].setOutVal();
        }
    }
}

/******************************** Button Controls *********************************/

void setEverythingLogic();

void handle_board_click(Point where)
{
    nowPoint.x = where.x;
    nowPoint.y = where.y;
}

void reset_btn_logic_click() {

	Wires.clear();

    TerminateThread(gate_add_handle,0);
    //TerminateThread(wire_logic_add_handle,0);

    wireLogicCount = 0;
    gateCount = 0;
    root->children.clear();

    setEverythingLogic();

}


void menu_wire_logic_click()
{
    selectedTerminals.clear();
    DWORD toh_solveId;
    TerminateThread(gate_add_handle,0);
    //TerminateThread(wire_logic_add_handle,0);
    nowAddingLogic = WIRE;
	wire_logic_add_handle = CreateThread(NULL, 0, wire_logic_add_thread, NULL, 0, NULL);
}

void menu_nandgate_click()
{
    DWORD toh_solveId;
    TerminateThread(gate_add_handle,0);
    //TerminateThread(wire_logic_add_handle,0);
    nowAddingLogic = NANDGATE;
    nowPoint.x = 0;
    nowPoint.y = 0;
	gate_add_handle = CreateThread(NULL, 0, gate_add_thread, NULL, 0, NULL);
}

void menu_norgate_click()
{
    DWORD toh_solveId;
    TerminateThread(gate_add_handle,0);
    //TerminateThread(wire_logic_add_handle,0);
    nowAddingLogic = NORGATE;
    nowPoint.x = 0;
    nowPoint.y = 0;
	gate_add_handle = CreateThread(NULL, 0, gate_add_thread, NULL, 0, NULL);
}

void menu_notgate_click()
{
    DWORD toh_solveId;
    TerminateThread(gate_add_handle,0);
    //TerminateThread(wire_logic_add_handle,0);
    nowAddingLogic = NOTGATE;
    nowPoint.x = 0;
    nowPoint.y = 0;
	gate_add_handle = CreateThread(NULL, 0, gate_add_thread, NULL, 0, NULL);
}

void menu_andgate_click()
{
    DWORD toh_solveId;
    TerminateThread(gate_add_handle,0);
    //TerminateThread(wire_logic_add_handle,0);
    nowAddingLogic = ANDGATE;
    nowPoint.x = 0;
    nowPoint.y = 0;
	gate_add_handle = CreateThread(NULL, 0, gate_add_thread, NULL, 0, NULL);
}

void menu_orgate_click()
{
    DWORD toh_solveId;
    TerminateThread(gate_add_handle,0);
  //  TerminateThread(wire_logic_add_handle,0);
    nowAddingLogic = ORGATE;
    nowPoint.x = 0;
    nowPoint.y = 0;
	gate_add_handle = CreateThread(NULL, 0, gate_add_thread, NULL, 0, NULL);
}

void menu_xorgate_click()
{
    DWORD toh_solveId;
    TerminateThread(gate_add_handle,0);
    //TerminateThread(wire_logic_add_handle,0);
    nowAddingLogic = XORGATE;
    nowPoint.x = 0;
    nowPoint.y = 0;
	gate_add_handle = CreateThread(NULL, 0, gate_add_thread, NULL, 0, NULL);
}

void delete_logic_btn_click()
{

}

HANDLE table_handle;
Div *table_btn_logic = new Div();

DWORD WINAPI table_thread()
{
    cout<<"Here\n\n";

    FILE *fp;

    fp = fopen("logic_output.txt","a+");

    fprintf(fp,"\n\nMask");

    int occ = 1;

    for(int j=0;j<gateCount;j++)
        {
            if(Gates[j].outC->data[3] == 0)
            {
                fprintf(fp,"\tOutput %d",occ);
                occ++;
            }
        }

    fprintf(fp,"\n");
    for(int i=0;i<(1<<InputLines);i++)
    {
        int mask = 1;
        for(int j=0;j<InputLines;j++)
        {
            int state = constLines[j].value;
            if(mask&i)
            {
                fprintf(fp,"%d",1);
                if(state==0)
                {
                    constLines[j].toggleValue();
                }
            }
            else
            {
                fprintf(fp,"%d",0);
                if(state==1)
                {
                    constLines[j].toggleValue();
                }
            }
            mask<<=1;
        }
        solve_logic_circuit_btn_click();

        for(int j=0;j<gateCount;j++)
        {
            if(Gates[j].outC->data[3] == 0)
            {
                int out = Gates[j].outVal;
                printf("\n\nThis is the output %d\n\n",out);
                fprintf(fp,"\t%4d",out);
            }
        }

        fprintf(fp,"\n");

        Sleep(250);
    }
    fprintf(fp,"\n\n");
    fclose(fp);

}
void table_logic_circuit_btn_click()
{
    table_handle = CreateThread(NULL, 0, table_thread, NULL, 0, NULL);
}


void setEverythingLogic()
{
    menu->a=Point(1390-menuWidth,10);
	menu->b=Point(1390,710);
	root->append_child(menu);

	int menuX1 = 1390-menuWidth;
	int menuX2 = menuX1 + gateBtnWidth;

	// Draw logicBoard


	logic_board->a = Point(lbX1,lbY);
	logic_board->b = Point(lbX1 + lbWidth,lbY + lbHeight);
	logic_board->color = Point(234,234,234);
	logic_board->filled = 1;
	logic_board->click_anim = 0;
	root->append_child(logic_board);
	logic_board->click = handle_board_click;



	menu_wire_logic->a=Point(menuX1,610-1*gateBtnWidth);
	menu_wire_logic->b=Point(menuX2,610-0*menuWidth);
	strcpy( menu_wire_logic->text,"Wire");
	menu_wire_logic->bg_color=BLACK;
	menu_wire_logic->click = menu_wire_logic_click;
	root->append_child(menu_wire_logic);

	menu_andgate->a=Point(menuX2,610-1*gateBtnWidth);
	menu_andgate->b=Point(1390,610-0*gateBtnWidth);
	strcpy( menu_andgate->text,"AND Gate");
	menu_andgate->bg_color=BLACK;
	menu_andgate->click = menu_andgate_click;
	root->append_child(menu_andgate);

	menu_orgate->a=Point(menuX1,610-2*gateBtnWidth);
	menu_orgate->b=Point(menuX2,610-1*gateBtnWidth);
	strcpy( menu_orgate->text,"OR Gate");
	menu_orgate->click = menu_orgate_click;
	menu_orgate->bg_color=BLACK;
	root->append_child(menu_orgate);

	menu_nandgate->a=Point(menuX2,610-2*gateBtnWidth);
	menu_nandgate->b=Point(1390,610-1*gateBtnWidth);
	strcpy(menu_nandgate->text,"NAND Gate");
	menu_nandgate->click = menu_nandgate_click;
	menu_nandgate->bg_color=BLACK;
	root->append_child(menu_nandgate);

	menu_norgate->a=Point(menuX1,610-3*gateBtnWidth);
	menu_norgate->b=Point(menuX2,610-2*gateBtnWidth);
	strcpy( menu_norgate->text,"NOR Gate");
	menu_norgate->click = menu_norgate_click;
	menu_norgate->bg_color=BLACK;
	root->append_child(menu_norgate);

	menu_led->a=Point(menuX2,610-3*gateBtnWidth);
	menu_led->b=Point(1390,610-2*gateBtnWidth);
	strcpy( menu_led->text,"LED");
	menu_led->bg_color=BLACK;
	//menu_wire_logic->click = menu_wire_logic;
	root->append_child(menu_led);

	menu_xorgate->a=Point(menuX1,610-4*gateBtnWidth);
	menu_xorgate->b=Point(menuX2,610-3*gateBtnWidth);
	strcpy( menu_xorgate->text,"XOR Gate");
	menu_xorgate->click = menu_xorgate_click;
	menu_xorgate->bg_color=BLACK;
	root->append_child(menu_xorgate);

	menu_notgate->a=Point(menuX2,610-4*gateBtnWidth);
	menu_notgate->b=Point(1390,610-3*gateBtnWidth);
	strcpy( menu_notgate->text,"NOT Gate");
	menu_notgate->bg_color=BLACK;
	menu_notgate->click = menu_notgate_click;
	root->append_child(menu_notgate);

	reset_btn_logic->a=Point(550,630);
    reset_btn_logic->b=Point(670,680);
    strcpy( reset_btn_logic->text,"Reset All");
    reset_btn_logic->click=reset_btn_logic_click;
    root->append_child(reset_btn_logic);

    simulate_btn_logic->a=Point(930,630);
    simulate_btn_logic->b=Point(1030,680);
    strcpy( simulate_btn_logic->text,"Simulate");
    simulate_btn_logic->click = solve_logic_circuit_btn_click;
    root->append_child(simulate_btn_logic);

    table_btn_logic->a=Point(690,630);
    table_btn_logic->b=Point(890,680);
    strcpy( table_btn_logic->text,"Generate Table");
    table_btn_logic->click = table_logic_circuit_btn_click;
    root->append_child(table_btn_logic);


	delete_logic_btn->a=Point(490,630);
	delete_logic_btn->b=Point(590,680);
	delete_logic_btn->bg_color=BLACK;
	strcpy( delete_logic_btn->text,"Delete");
	delete_logic_btn->click = delete_logic_btn_click;
	//root->append_child(delete_logic_btn);

	for(int i=0;i<InputLines;i++)
    {
        constLines[i].createConstLine(switchX1+switchOffset*i,switchHeight,i);
    }

}

void initialize_logic() {

	clear_root();

	setEverythingLogic();

}
/*


			LOGIC END



*/
/*


			DC CIRCUIT START


*/

// Circuit Structures

struct weight{
    int c;
    double val;
};

/****************************** Global Typedef ******************************/

// General typedef

typedef long double ld;
typedef pair<int, int> pii;

/********************************* Globals ****************************************/

// Tower of Hanoi Global Variables
int dummyVar;

// General Constants

const int inf = 1e18;
const int MAXN = 2e5+5;
const int mod = 1e9+7;
const ld pi = atan2(0, -1);
const ld eps = 1e-4;

const double EPS = 1e-9;
const int INF = 1e7; // it doesn't actually have to be infinity or a big number

// Circuit Constants

const double intRes = 0.0001;
const double tinyR = 0.0001;
const double bigR = 10000;
const double LedRes = 10;
const double tiny = 0.00001;

// DC Circuit Design Globals

int componentBtnWidth = menuWidth/2 ;

int bbX1 = 30;
int bbY = 150;
int bbWidth;
int bbHeight;
int bbOffset = 20;


set<pair<int,int>>selectedPins;
int oneSel, twoSel, threeSel ;

int waitForHowMany;
int waitingDone;

int allWireWidth = 6;
Point allWireColor = DARK_GREEN;

int batteryWidth = 160;
int batteryHeight = 100;
int ammeterWidth = 200;
int ammeterHeight = 50;
int voltmeterWidth = 200;
int voltmeterHeight = 50;
int resistanceWidth = 60;
int resistanceHeight = 25;
int capacitorWidth = 60;
int capacitorHeight = 50;

int wireCount = 0;
int batteryCount = 0;
int ammeterCount = 0;
int voltmeterCount = 0;
int resistanceCount = 0;

int componentCount = 0;

int nowAdding;
int nowSelectedComponentIndex = -1;

// pole and base depth = 10

/*********************************** DC Circuit Global Variables *******************************/

// DC Circuit Globals

int nodeNumDC = 132, componentNumDC;
double input_DC[1000][4];
vector<double>ans_DC_V;

/********************************* Function Prototypes ****************************************/

void reposition_btn_click();
void animateDisc(int i, int toPole);
void handle_component_click(Point where, Div *d);

/********************************* Helping Functions *********************************/

void reverse(char* str, int len)
{
    int i = 0, j = len - 1, temp;
    while (i < j) {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}

// Converts a given integer x to string str[].
// d is the number of digits required in the output.
// If d is more than the number of digits in x,
// then 0s are added at the beginning.
int intToStr(int x, char str[], int d)
{
    int i = 0;
    while (x) {
        str[i++] = (x % 10) + '0';
        x = x / 10;
    }

    // If number of digits required is more, then
    // add 0s at the beginning
    while (i < d)
        str[i++] = '0';

    reverse(str, i);
    str[i] = '\0';
    return i;
}

// Converts a double number to a string.
void dtoa(double n, char* res, int afterpoint)
{
    if(n<0)
        n = -n;  // Minus Sign Implementation should be added
    // Extract integer part
    int ipart = (int)n;

    // Extract floating part
    double fpart = n - (double)ipart;

    // convert integer part to string
    int i = intToStr(ipart, res, 0);

    // check for display option after point
    if (afterpoint != 0) {
        res[i] = '.'; // add dot

        // Get the value of fraction part upto given no.
        // of points after dot. The third parameter
        // is needed to handle cases like 233.007
        fpart = fpart * pow(10, afterpoint);

        intToStr((int)fpart, res + i + 1, afterpoint);
    }
}

void writeValue(Div* any, double value, int d)
{

    char temp[100];
    dtoa(value,temp,d);
    strcpy(any->text , temp);

    //printf("\n\nOkay\n\n");
}

/********************************* Thread Handles *********************************/

HANDLE component_add_handle, rlc_component_add_handle ;

/*********************** Structures for Combined Elements ************************/

struct ComponentDivDC{

    Div *bg;
    Div *valueBox;
    Div *indicator;
    Div *plust;
    Div *minust;
    Div *plusWire;
    Div *minusWire;
    Div *wire;
    //Div *stripes[50];

    Point plusEnd;
    Point minusEnd;
    Point a;
    Point b;
    Point color = allWireColor ;

    double value;

    int typeC;

    int disabled = 0;

    int node1;
    int node2;
    int index;
    int width = allWireWidth ;

    void createComponentDivDC(int type, int ax, int ay, int bx, int by, Point p1, Point p2, int id)
    {
        typeC = type;
        /*bg = new Div();
        plust = new Div();
        minust = new Div();
        plusWire = new Div();
        minusWire = new Div();
        valueBox = new Div();
        indicator = new Div();
        wire = new Div();*/

        if( type == AMMETER )
        {
            index = id;

            int w = ammeterWidth;
            int h = ammeterHeight;

            value = 0;


            bg = new Div();
            bg->a = Point(ax,ay);
            bg->b = bg->a + Point(w,h);
            bg->color = YELLOW;
            bg->filled = 1;
            bg->data[0] = index;
            bg->click = handle_component_click ;

            root->append_child(bg);


            plust = new Div();
            plust->positioning = constants.DIV_RELATIVE;
            plust->a = Point(7*w/16,0);
            plust->b = Point(7,-10);
            plust->filled = 1;
            plust->color = RED;
            bg->append_child(plust);


            minust = new Div();
            minust->positioning = constants.DIV_RELATIVE;
            minust->a = Point(9*w/16,0);
            minust->b = Point(7,-10);
            minust->filled = 1;
            minust->color = BLUE;
            bg->append_child(minust);



            //plusEnd = plust->a;
            //minusEnd = minust->a;

            //plusEnd = Point(0,0);
            //minusEnd = Point(0,0);


            plusWire = new Div();
            //plusWire->positioning = constants.DIV_RELATIVE;
            plusWire->type = constants.DIV_LINE;
            plusWire->a = Point(ax,ay) + ( plust->a + plust->a + plust->b )  / 2 ;

            plusEnd = p1;

            plusWire->b = plusEnd;
            plusWire->line_width = allWireWidth;
            plusWire->color = RED;
            bg->append_child(plusWire);


            minusWire = new Div();
            //minusWire->positioning = constants.DIV_RELATIVE;
            minusWire->type = constants.DIV_LINE;
            minusWire->a = Point(ax,ay) + ( minust->a + minust->a + minust->b )  / 2 ;

            minusEnd = p2;

            minusWire->b = minusEnd;
            minusWire->line_width = allWireWidth;
            minusWire->color = BLUE;
            bg->append_child(minusWire);

            valueBox = new Div();
            valueBox->positioning = constants.DIV_RELATIVE;
            valueBox->a = Point(5,5);
            valueBox->b = Point(3*w/4,h-10);
            valueBox->filled = 1;
			valueBox->click_anim=0;
            valueBox->data[0] = index;
            valueBox->click = handle_component_click ;

            valueBox->color = LIGHT_CYAN;

            //writeValue();

            /*char temp[100];
            dtoa(value,temp,5);
            strcpy(valueBox->text , temp);*/

            writeValue(valueBox,value,5);

            valueBox->text_color = BLACK;
            bg->append_child(valueBox);

            indicator = new Div();
            indicator->positioning = constants.DIV_RELATIVE;
            indicator->a = Point(3*w/4 + 10,5);
            indicator->b = Point(w/6,h-10);
            indicator->filled = 1;
            strcpy(indicator->text , "A");
            indicator->text_color = BLACK;
            indicator->color = LIGHT_CYAN;
            bg->append_child(indicator);

        }

        if(type == WIRE)
        {
            index = id;
            value = 1; // dummy

            wire = new Div();
            wire->type = constants.DIV_LINE;
            wire->a = p1;
            wire->b = p2;
            wire->line_width = width;
            wire->color = color;
            root->append_child(wire);
        }

        if( type == VOLTMETER )
        {
            value = 0;

            index = id;

            int w = voltmeterWidth;
            int h = voltmeterHeight;

            bg = new Div();
            bg->a = Point(ax,ay);
            bg->b = bg->a + Point(w,h);
            bg->color = YELLOW;
            bg->data[0] = index;
            bg->click = handle_component_click ;
            bg->filled = 1;

            root->append_child(bg);

            plust = new Div();
            plust->positioning = constants.DIV_RELATIVE;
            plust->a = Point(7*w/16,0);
            plust->b = Point(7,-10);
            plust->filled = 1;
            plust->color = RED;
            bg->append_child(plust);

            minust = new Div();
            minust->positioning = constants.DIV_RELATIVE;
            minust->a = Point(9*w/16,0);
            minust->b = Point(7,-10);
            minust->filled = 1;
            minust->color = BLUE;
            bg->append_child(minust);

            plusWire = new Div();
            //plusWire->positioning = constants.DIV_RELATIVE;
            plusWire->type = constants.DIV_LINE;
            plusWire->a = Point(ax,ay) + ( plust->a + plust->a + plust->b )  / 2 ;

            plusEnd = p1;

            plusWire->b = plusEnd;
            plusWire->line_width = allWireWidth;
            plusWire->color = RED;
            bg->append_child(plusWire);

            minusWire = new Div();
            //minusWire->positioning = constants.DIV_RELATIVE;
            minusWire->type = constants.DIV_LINE;
            minusWire->a = Point(ax,ay) + ( minust->a + minust->a + minust->b )  / 2 ;

            minusEnd = p2;

            minusWire->b = minusEnd;
            minusWire->line_width = allWireWidth;
            minusWire->color = BLUE;
            bg->append_child(minusWire);

            valueBox = new Div();
            valueBox->positioning = constants.DIV_RELATIVE;
            valueBox->a = Point(5,5);
            valueBox->b = Point(3*w/4,h-10);
            valueBox->filled = 1;
            valueBox->color = LIGHT_GREEN;
            valueBox->data[0] = index;
            valueBox->click = handle_component_click ;

            //writeValue();

            writeValue(valueBox,value,5);

            //setDivValue(valueBox,value);

            valueBox->text_color = BLACK;
            bg->append_child(valueBox);

            indicator = new Div();
            indicator->positioning = constants.DIV_RELATIVE;
            indicator->a = Point(3*w/4 + 10,5);
            indicator->b = Point(w/6,h-10);
            indicator->filled = 1;
            strcpy(indicator->text , "V");
            indicator->text_color = BLACK;
            indicator->color = LIGHT_GREEN;
            bg->append_child(indicator);
        }

        if(type == DC_SOURCE)
        {
            index = id;

            value = 15;

            int w = batteryWidth;
            int h = batteryHeight;

            bg = new Div();
            bg->a = Point(ax,ay);
            bg->b = bg->a + Point(w,h);
            bg->color = RED;
            bg->data[0] = index;
            bg->click = handle_component_click ;
            bg->filled = 1;
            //bg->click = disableAmmeter ;
            root->append_child(bg);

            plust = new Div();
            plust->positioning = constants.DIV_RELATIVE;
            plust->a = Point(w,3*h/8);
            plust->b = Point(7,5);
            plust->filled = 1;
            plust->color = RED;
            bg->append_child(plust);

            minust = new Div();
            minust->positioning = constants.DIV_RELATIVE;
            minust->a = Point(w,5*h/8);
            minust->b = Point(7,5);
            minust->filled = 1;
            minust->color = BLUE;
            bg->append_child(minust);

            plusWire = new Div();
            //plusWire->positioning = constants.DIV_RELATIVE;
            plusWire->type = constants.DIV_LINE;
            plusWire->a = Point(ax,ay) + ( plust->a + plust->a + plust->b )  / 2 ;

            plusEnd = p1;

            plusWire->b = plusEnd;
            plusWire->line_width = allWireWidth;
            plusWire->color = RED;
            bg->append_child(plusWire);

            minusWire = new Div();
            //minusWire->positioning = constants.DIV_RELATIVE;
            minusWire->type = constants.DIV_LINE;
            minusWire->a = Point(ax,ay) + ( minust->a + minust->a + minust->b )  / 2 ;

            minusEnd = p2;

            minusWire->b = minusEnd;
            minusWire->line_width = allWireWidth;
            minusWire->color = BLUE;
            bg->append_child(minusWire);

            valueBox = new Div();
            valueBox->positioning = constants.DIV_RELATIVE;
            valueBox->a = Point(0,0);
            valueBox->b = Point(3*w/4,h);
            valueBox->filled = 1;
            valueBox->color = ORANGE;
            valueBox->data[0] = index;
            valueBox->click = handle_component_click ;
valueBox->click_anim=0;
            writeValue(valueBox,value,0);

            valueBox->text_color = BLACK;
            bg->append_child(valueBox);
        }

        if(typeC == RESISTANCE)
        {
            index = id;

            value = 50;

            int w = resistanceWidth;
            int h = resistanceHeight;

            bg = new Div();
            bg->a = Point(ax,ay);
            bg->b = bg->a + Point(w,h);
            bg->color = RED;
            bg->data[0] = index;
            bg->click = handle_component_click ;
            bg->filled = 1;
            //bg->click = disableAmmeter ;
            root->append_child(bg);

            plust = new Div();
            plust->positioning = constants.DIV_RELATIVE;
            plust->a = Point(w,h/2-3);
            plust->b = Point(7,5);
            plust->filled = 1;
            plust->color = DARK_GREEN;
            //plust->hidden = 1;
            bg->append_child(plust);

            minust = new Div();
            minust->positioning = constants.DIV_RELATIVE;
            minust->a = Point(0,h/2-3);
            minust->b = Point(-7,5);
            minust->filled = 1;
            minust->color = DARK_GREEN;
            //minust->hidden = 1;
            bg->append_child(minust);

            plusWire = new Div();
            //plusWire->positioning = constants.DIV_RELATIVE;
            plusWire->type = constants.DIV_LINE;
            plusWire->a = Point(ax-7,ay+resistanceHeight/2+2) ;

            plusEnd = p1;

            plusWire->b = plusEnd;
            plusWire->line_width = allWireWidth;
            plusWire->color = DARK_GREEN;
            bg->append_child(plusWire);

            minusWire = new Div();
            //minusWire->positioning = constants.DIV_RELATIVE;
            minusWire->type = constants.DIV_LINE;
            minusWire->a = Point(ax+w+7,ay+resistanceHeight/2+2) ;

            minusEnd = p2;

            minusWire->b = minusEnd;
            minusWire->line_width = allWireWidth;
            minusWire->color = DARK_GREEN;
            bg->append_child(minusWire);

            valueBox = new Div();
            valueBox->positioning = constants.DIV_RELATIVE;
            valueBox->a = Point(0,0);
            valueBox->b = Point(3*w/4,h);
            valueBox->filled = 1;
            valueBox->color = ORANGE;
            valueBox->data[0] = index;
            valueBox->click = handle_component_click ;
valueBox->click_anim=0;
            writeValue(valueBox,value,0);

            valueBox->text_color = BLACK;
            bg->append_child(valueBox);
        }

        if(type == INDUCTOR)
        {
            index = id;

            value = 50;

            int w = resistanceWidth;
            int h = resistanceHeight;

            bg = new Div();
            bg->a = Point(ax,ay);
            bg->b = bg->a + Point(w,h);
            bg->color = CHOCOLATE;
            bg->data[0] = index;
            bg->click = handle_component_click ;
            bg->filled = 1;
            //bg->click = disableAmmeter ;
            root->append_child(bg);

            plust = new Div();
            plust->positioning = constants.DIV_RELATIVE;
            plust->a = Point(w,h/2-3);
            plust->b = Point(7,5);
            plust->filled = 1;
            plust->color = DARK_GREEN;
            //plust->hidden = 1;
            bg->append_child(plust);

            minust = new Div();
            minust->positioning = constants.DIV_RELATIVE;
            minust->a = Point(0,h/2-3);
            minust->b = Point(-7,5);
            minust->filled = 1;
            minust->color = DARK_GREEN;
            //minust->hidden = 1;
            bg->append_child(minust);

            plusWire = new Div();
            //plusWire->positioning = constants.DIV_RELATIVE;
            plusWire->type = constants.DIV_LINE;
            plusWire->a = Point(ax-7,ay+resistanceHeight/2+2) ;

            plusEnd = p1;

            plusWire->b = plusEnd;
            plusWire->line_width = allWireWidth;
            plusWire->color = DARK_GREEN;
            bg->append_child(plusWire);

            minusWire = new Div();
            //minusWire->positioning = constants.DIV_RELATIVE;
            minusWire->type = constants.DIV_LINE;
            minusWire->a = Point(ax+w+7,ay+resistanceHeight/2+2) ;

            minusEnd = p2;

            minusWire->b = minusEnd;
            minusWire->line_width = allWireWidth;
            minusWire->color = DARK_GREEN;
            bg->append_child(minusWire);

            valueBox = new Div();
            valueBox->positioning = constants.DIV_RELATIVE;
            valueBox->a = Point(0,0);
            valueBox->b = Point(w,h);
            valueBox->filled = 1;
            valueBox->color = CHOCOLATE;
            valueBox->data[0] = index;
            valueBox->click = handle_component_click ;

            writeValue(valueBox,value,0);

            valueBox->text_color = WHITE;
            bg->append_child(valueBox);

            /*for(int j=0;j<5;j++)
            {
                stripes[j] = new Div();
                stripes[j]->type = constants.DIV_LINE;
                stripes[j]->color = Point(255,215,0);
                stripes[j]->filled = 1;
                stripes[j]->positioning = constants.DIV_RELATIVE;
                stripes[j]->a = Point(10*j , 0);
                stripes[j]->b = Point(3 ,25);
                //stripes[j]->line_width = 5;
                bg->append_child(stripes[j]);
            }*/

            //strcpy( bg->img_path , "inductor3.bmp");
            //bg->img_pos = Point(0,0);
        }

        if(type == CAPACITOR)
        {
            index = id;

            value = 50;

            int w = capacitorWidth;
            int h = capacitorHeight;

            bg = new Div();
            bg->a = Point(ax,ay);
            bg->b = bg->a + Point(w,h);
            bg->color = GREY;
            bg->data[0] = index;
            bg->click = handle_component_click ;
            bg->filled = 1;
            //bg->click = disableAmmeter ;
            root->append_child(bg);

            plust = new Div();
            plust->positioning = constants.DIV_RELATIVE;
            plust->a = Point(w/4 + 3,0);
            plust->b = Point(-7,-5);
            plust->filled = 1;
            plust->color = GREY;
            //plust->hidden = 1;
            bg->append_child(plust);

            minust = new Div();
            minust->positioning = constants.DIV_RELATIVE;
            minust->a = Point(3*w/4 - 3,0);
            minust->b = Point(7,-5);
            minust->filled = 1;
            minust->color = GREY;
            //minust->hidden = 1;
            bg->append_child(minust);

            plusWire = new Div();
            //plusWire->positioning = constants.DIV_RELATIVE;
            plusWire->type = constants.DIV_LINE;
            plusWire->a = Point(ax,ay) + ( plust->a + plust->a + plust->b )  / 2 ;

            plusEnd = p1;

            plusWire->b = plusEnd;
            plusWire->line_width = allWireWidth;
            plusWire->color = DARK_GREEN;
            bg->append_child(plusWire);

            minusWire = new Div();
            //minusWire->positioning = constants.DIV_RELATIVE;
            minusWire->type = constants.DIV_LINE;
            minusWire->a = Point(ax,ay) + ( minust->a + minust->a + minust->b )  / 2 ;

            minusEnd = p2;

            minusWire->b = minusEnd;
            minusWire->line_width = allWireWidth;
            minusWire->color = DARK_GREEN;
            bg->append_child(minusWire);

            valueBox = new Div();
            valueBox->positioning = constants.DIV_RELATIVE;
            valueBox->a = Point(0,0);
            valueBox->b = Point(3*w/4,h);
            valueBox->filled = 1;
            valueBox->color = DARK_GREY;
            valueBox->data[0] = index;
            valueBox->click = handle_component_click ;

            writeValue(valueBox,value,0);

            valueBox->text_color = WHITE;
            bg->append_child(valueBox);
        }
        printf("Done");
    }

    void setValue(double p)
    {
        int N = 20;
        double diff = p/N;
        double temp = 0;
        for(int i=0;i<N;i++)
        {
            temp += diff;
            writeValue(valueBox,temp,5);
            Sleep(1);
        }
        value = p;
        if(typeC == RESISTANCE || typeC == CAPACITOR || typeC == INDUCTOR || typeC == DC_SOURCE)
        {
            writeValue(valueBox,value,0);
        }
        else
            writeValue(valueBox,value,5);
    }
    void hide()
    {
        if(typeC == WIRE)
        {
            wire->hidden = 1;
        }
        else if(typeC == DC_SOURCE || typeC == RESISTANCE || typeC == CAPACITOR || typeC == INDUCTOR )
        {
            bg->hidden = 1;
            plusWire->hidden = 1;
            minusWire->hidden = 1;
            valueBox->hidden = 1;
            plust->hidden = 1;
            minust->hidden = 1;
        }
        else
        {
            bg->hidden = 1;
            plusWire->hidden = 1;
            minusWire->hidden = 1;
            valueBox->hidden = 1;
            indicator->hidden = 1;
            plust->hidden = 1;
            minust->hidden = 1;
        }
    }

};



/********************************* Elements ***************************************/

//Sidebar Menu


Div *breadboard = new Div();

Div *menu_resistance = new Div();
Div *menu_dc_source = new Div();

Div *menu_wire = new Div();
Div *menu_ammeter = new Div();

Div *menu_voltmeter = new Div();
Div *menu_capacitor = new Div();
Div *menu_inductor = new Div();

Div *update_btn = new Div();
Div *delete_btn = new Div();

Div *pins[100][100];



/*WireDiv Wires[100];
AmmeterDiv Ammeters[100];
VoltmeterDiv Voltmeters[100];
BatteryDiv Batteries[100];*/

ComponentDivDC Components[500];


/******************************** Component control Functions **********************/

void handle_component_click(Point where, Div *d)
{
    int index = d->data[0];
    printf("\n%d\n",index);
    if(Components[index].disabled == 0)
    {
        printf("\n%d\n",index);
        nowSelectedComponentIndex = index;
    }
    // Otherwise Ignore Click
}

/******************************** Thread functions ********************************/
DWORD WINAPI wire_add_thread() {

	for(int i=0;i<5000;i++)
	{
        if(selectedPins.size() >= waitForHowMany)
        {
            printf("Found\n\n");
            waitingDone = 1 ;

            std::set<pair<int,int>>::iterator it;
            it = selectedPins.begin();

            int ax = (*it).first;
            int ay = (*it).second;

            advance(it,1);

            int bx = (*it).first;
            int by = (*it).second;

            int c = componentCount;

            Point a = pins[ax][ay]->a;
            Point b = pins[bx][by]->a;
            //Wires[c].width = 7;
            //Point color = DARK_GREEN;
            //root->append_child(Wires[c]);

            //Wires[c].createWireDiv(a,b,7,color);

            Components[c].createComponentDivDC( WIRE, 0,0,0,0,a,b,c);

            componentCount++;

            break;
        }
		Sleep(100);
	}
}

DWORD WINAPI component_add_thread() {

	for(int i=0;i<5000;i++)
	{
        if(selectedPins.size() >= waitForHowMany)
        {
            printf("Found\n\n");
            waitingDone = 1 ;

            std::set<pair<int,int>>::iterator it;
            it = selectedPins.begin();

            int ax = (*it).first;
            int ay = (*it).second;

            advance(it,1);

            int bx = (*it).first;
            int by = (*it).second;

            int c = componentCount;

            Point a = pins[ax][ay]->a;
            Point b = pins[bx][by]->a;

            int n1 = pins[ax][ay]->data[2];
            int n2 = pins[bx][by]->data[2];

            printf("%d  %d \n %d \n", ax, ay, n1);
            printf("%d  %d \n %d \n", bx, by, n2);

            Point color = DARK_GREEN;

            if(nowAdding == AMMETER)
            {

                int mid_ax = (a.x + b.x)/2 - ammeterWidth/2 ;
                int amY = bbY + bbHeight + 50;

                Components[c].createComponentDivDC( AMMETER, mid_ax, amY, 0, 0,a,b,c);

            }

            if(nowAdding == VOLTMETER)
            {

                int mid_ax = (a.x + b.x)/2 - voltmeterWidth/2 ;
                int amY = bbY + bbHeight + 50;

                Components[c].createComponentDivDC( VOLTMETER, mid_ax, amY, 0, 0,a,b,c);

            }

            else if(nowAdding == WIRE)
            {
                Components[c].createComponentDivDC( WIRE, 0,0,0,0,a,b,c);
            }

            else if(nowAdding == DC_SOURCE)
            {
                int mid_ax = min(a.x,b.x)-250 ;
                int amY = bbY + bbHeight + 10;

                Components[c].createComponentDivDC( DC_SOURCE, mid_ax, amY, 0, 0,a,b,c);
            }

            Components[c].node1 = n1;
            Components[c].node2 = n2;

            componentCount++;

            printf("\n%d\n",componentCount);
            Sleep(3000);

            //Ammeters[c].hide();

            break;
        }
		Sleep(100);
	}
}

DWORD WINAPI rlc_component_add_thread() {
	while(1)
	{
        if(selectedPins.size() >= waitForHowMany)
        {
            printf("Found\n\n");
            waitingDone = 1 ;

            std::set<pair<int,int>>::iterator it;
            it = selectedPins.begin();

            int ax = (*it).first;
            int ay = (*it).second;

            int bx = ax;
            int by ;

            if(nowAdding == CAPACITOR)
            {
                by = ay+2;
            }
            else
            {
                by = ay+5;
            }

            if( ax==0 || ax==1 || ax==12 || ax==13 || ay>55)
            {
                break;
            }

            int c = componentCount;

            Point a = pins[ax][ay]->a;
            Point b = pins[bx][by]->a;

            int n1 = pins[ax][ay]->data[2];
            int n2 = pins[bx][by]->data[2];

            printf("%d  %d \n %d \n", ax, ay, n1);
            printf("%d  %d \n %d \n", bx, by, n2);

            Point color = DARK_GREEN;

            if(nowAdding == RESISTANCE)
            {
            	printf("hehe");
                int mid_ax = (a.x + b.x)/2 - resistanceWidth/2 ;
                int amY = a.y - 5;

                Components[c].createComponentDivDC( RESISTANCE , mid_ax, amY, 0, 0,a,b,c);
            }

            if(nowAdding == INDUCTOR)
            {
                int mid_ax = (a.x + b.x)/2 - resistanceWidth/2 ;
                int amY = a.y - 5;

                Components[c].createComponentDivDC( INDUCTOR , mid_ax, amY, 0, 0,a,b,c);
            }

            if(nowAdding == CAPACITOR)
            {
                int mid_ax = (a.x + b.x)/2 - resistanceWidth/2 ;
                int amY = a.y + 10;

                Components[c].createComponentDivDC( CAPACITOR , mid_ax, amY, 0, 0,a,b,c);
            }

            Components[c].node1 = n1;
            Components[c].node2 = n2;

            componentCount++;

            printf("\n%d\n",componentCount);
            Sleep(31);

            //Ammeters[c].hide();

            break;
        }
		Sleep(10);
	}
}
/******************************** Solving Functions *******************************/

// Gaussian Elimination

int gauss (vector < vector<double> > a, vector<double> &ans) {

    int n = (int) a.size();
    int m = (int) a[0].size() - 1;

    vector<int> where (m, -1);
    for (int col=0, row=0; col<m && row<n; ++col) {

        int sel = row;
        for (int i=row; i<n; ++i)
            if (abs (a[i][col]) > abs (a[sel][col]))
                sel = i;
        if (abs (a[sel][col]) < EPS)
            continue;


        for (int i=col; i<=m; ++i)
            swap (a[sel][i], a[row][i]);
        where[col] = row;

        for (int i=0; i<n; ++i)
            if (i != row) {
                double c = a[i][col] / a[row][col];
                for (int j=col; j<=m; ++j)
                    a[i][j] -= a[row][j] * c;
            }
        ++row;
    }

    ans.assign (m, 0);
    for (int i=0; i<m; ++i)
        if (where[i] != -1)
            ans[i] = a[where[i]][m] / a[where[i]][i];

    for (int i=0; i<n; ++i) {
        double sum = 0;
        for (int j=0; j<m; ++j)
            sum += ans[j] * a[i][j];
        if (abs (sum - a[i][m]) > EPS)
            return 0;
    }

    for (int i=0; i<m; ++i)
    {
        if (where[i] == -1)
            return INF;
    }

    return 1;
}


void solve_dc_circuit()
{

    // Make Sure, the array input_DC[][] and nodeNumDC and componentNumDC are set before calling this function
    //Input node indexing starts at zero
    // 1 2 V 15 means V1 - V2 = 15
    // If a node voltage cannot be determined, it is defaultly set to zero

    int m,n,p,q,a,b,arr[MAXN],maxm,minm,u,v, gauss_Output;

    double val;

    int c;

    vector<double>temp;
    vector< vector<double> >coeff;
    vector<pair<int,weight>>adj[1000];

    n = nodeNumDC;
    m = componentNumDC;

    for(int i=0;i<m;i++)
    {
        p = input_DC[i][0];
        q = input_DC[i][1];
        c = input_DC[i][2];
        val = input_DC[i][3];

        if(abs(val) <= eps)
            val = tiny;

        if( c == WIRE || c == AMMETER )
        {
            c = RESISTANCE;
            val = tinyR;
        }

        if( c == VOLTMETER )
        {
            c = RESISTANCE;
            val = bigR;
        }

        if( c == LED )
        {
            c = RESISTANCE;
            val = LedRes;
        }

        if( c == INDUCTOR )
        {
            c = RESISTANCE;
            val = tinyR;
        }

        if( c == CAPACITOR )
        {
            c = RESISTANCE;
            val = bigR;
        }

        struct weight w;
        w.c = c;
        w.val = val;

        struct weight mw; //minus w for V only
        mw.c = c;
        mw.val = -val;

        if(c == RESISTANCE )
        {
            adj[p].pb({q,w});
            adj[q].pb({p,w});
        }

        else if(c == DC_SOURCE)
        {
            adj[p].pb({q,w});
            adj[q].pb({p,mw});
        }

    }


    // First eqn, ground zero

    cout<<"\n\nEquations\n\n";
    temp.clear();
    temp.pb(1);
    fo(i,n)
        temp.pb(0);
    coeff.pb(temp);

    // Show Equations

    /*fo(i,n+1)
        cout<<temp[i]<<" ";
    cout<<endl;*/

    // Equations for each of the nodes except Ground

    fo(i,n)
    {
        if(i==0)
            continue;
        temp.clear();
        temp.assign(n+1,0);
        for(auto branch: adj[i])
        {
            q = branch.first;
            c = branch.second.c;
            val = branch.second.val;

            //cout<<endl<<q<<"  "<<c<<gap<<val<<endl;

            if(c == RESISTANCE)
            {
                temp[i] += 1/val;
                temp[q] -= 1/val;
            }
            if(c == DC_SOURCE)
            {
                temp[i] += 1/intRes;
                temp[q] -= 1/intRes;
                temp[n] += val/intRes;
            }
        }
        coeff.pb(temp);

        // Show Equations

        /*fo(i,n+1)
            cout<<temp[i]<<" ";
        cout<<endl;*/
    }

    gauss_Output = gauss(coeff, ans_DC_V);

    if(gauss_Output)
    {
        ans_DC_V[0] = 0;
//        cout<<"\nAnswers:\n\n";
//        fo(i,n)
//            cout<<ans_DC_V[i]<<endl;
    }
    else
    {
        fo(i,n)
            ans_DC_V[i] = 0;
//        cout<<"\nAnswers:\n\n";
//        fo(i,n)
//            cout<<ans_DC_V[i]<<endl;
    }
    cout<<endl;
}


/******************************* Solve Button Functions ************************/

void solve_circuit_btn_click()
{
    componentNumDC = componentCount;

    int j = 0;

    for(int i=0;i<componentCount;i++)
    {
        if( Components[i].disabled == 1 )
        {
            componentNumDC--;
            continue;
        }

        int p = Components[i].node1;
        int q = Components[i].node2;
        int c = Components[i].typeC;
        double val = Components[i].value;

        printf("%d  %d  %d  %lf \n", p,q,c,val );

        input_DC[j][0] = p;
        input_DC[j][1] = q;
        input_DC[j][2] = c;
        input_DC[j][3] = val;

        printf("\n\n%lf  %lf  %lf  %lf \n", input_DC[j][0],input_DC[j][1],input_DC[j][2],input_DC[j][3] );

        j++;

    }

    solve_dc_circuit();

    for(int i=0;i<componentCount;i++)
    {
        if( Components[i].disabled == 1 )
            continue;

        if( Components[i].typeC == VOLTMETER )
        {
            int n1 = Components[i].node1;
            int n2 = Components[i].node2;

            Components[i].setValue(abs(ans_DC_V[n1] - ans_DC_V[n2]));
        }

        if( Components[i].typeC == AMMETER )
        {
            int n1 = Components[i].node1;
            int n2 = Components[i].node2;

            Components[i].setValue(abs(ans_DC_V[n1] - ans_DC_V[n2])/tinyR);
        }

    }

}

/******************************** Button Controls *********************************/

void setEverythingDC();

void reset_btn_click() {

    componentCount = 0;

    root->children.clear();

    setEverythingDC();

}

void highlight(int i, int j)
{
    pins[i][j]->R = 6;
    pins[i][j]->color = GREEN;
}

void unhighlight(int i, int j)
{
    pins[i][j]->R = 3;
    pins[i][j]->color = DARK_GREY;
}

void clear_all_selected()
{
    for(auto p: selectedPins)
    {
        unhighlight(p.first, p.second);
    }
    selectedPins.clear();
}

void menu_wire_click()
{
    DWORD toh_solveId;
    TerminateThread(component_add_handle,0);
    clear_all_selected();
    waitForHowMany = 2;
    waitingDone = 0;
    nowAdding = WIRE;
	component_add_handle = CreateThread(NULL, 0, component_add_thread, NULL, 0, NULL);
}

void menu_ammeter_click()
{
    DWORD toh_solveId;
    TerminateThread(component_add_handle,0);
    clear_all_selected();
    waitForHowMany = 2;
    waitingDone = 0;
    nowAdding = AMMETER;
	component_add_handle = CreateThread(NULL, 0, component_add_thread, NULL, 0, NULL);
}

void menu_voltmeter_click()
{
    DWORD toh_solveId;
    TerminateThread(component_add_handle,0);
    clear_all_selected();
    waitForHowMany = 2;
    waitingDone = 0;
    nowAdding = VOLTMETER;
	component_add_handle = CreateThread(NULL, 0, component_add_thread, NULL, 0, NULL);
}

void menu_dc_source_click()
{
    DWORD toh_solveId;
    TerminateThread(component_add_handle,0);
    clear_all_selected();
    waitForHowMany = 2;
    waitingDone = 0;
    nowAdding = DC_SOURCE;
	component_add_handle = CreateThread(NULL, 0, component_add_thread, NULL, 0, NULL);
}

void menu_resistance_click()
{
	printf("in here");
    DWORD toh_solveId;
    TerminateThread(component_add_handle,0);
    clear_all_selected();
    waitForHowMany = 1;
    waitingDone = 0;
    nowAdding = RESISTANCE;
	rlc_component_add_handle = CreateThread(NULL, 0, rlc_component_add_thread, NULL, 0, NULL);
}

void menu_inductor_click()
{
    DWORD toh_solveId;
    TerminateThread(component_add_handle,0);
    clear_all_selected();
    waitForHowMany = 1;
    waitingDone = 0;
    nowAdding = INDUCTOR;
	rlc_component_add_handle = CreateThread(NULL, 0, rlc_component_add_thread, NULL, 0, NULL);
}

void menu_capacitor_click()
{
    DWORD toh_solveId;
    TerminateThread(component_add_handle,0);
    clear_all_selected();
    waitForHowMany = 1;
    waitingDone = 0;
    nowAdding = CAPACITOR;
	rlc_component_add_handle = CreateThread(NULL, 0, rlc_component_add_thread, NULL, 0, NULL);
}

void delete_btn_click()
{
    int index = nowSelectedComponentIndex;
    printf("Deleting %d \n\n",index);
    Components[index].disabled = 1;
    Components[index].hide();
}

void handle_pin_click(Point where, Div *d)
{
    printf("\n\nOkay\n\n");
    int a = d->data[0];
    int b = d->data[1];

    if(d->data[4] == 0)
    {
        highlight(a,b);
        printf("Selected");
        selectedPins.insert({a,b});
    }
    else
    {
        unhighlight(a,b);
        selectedPins.erase({a,b});
    }

    d->data[4] = 1 - d->data[4] ; // swap between 0 and 1

}
//

/****************************** Slider Handle Functions *******************************/

// These must be below slider_apply_function
void update_component()
{
    if( nowSelectedComponentIndex != -1 )
    {
        int index = nowSelectedComponentIndex;
        int dval = slider_global.nowValue;
        double desiredValue = dval;
        Components[index].setValue(desiredValue);
        nowSelectedComponentIndex=-1;
    }
    else
    {
        printf("No components Selected\n");
    }
}

void here()
{
    printf("Here\n");
}

void setEverythingDC()
{
    menu->a=Point(1390-menuWidth,10);
	menu->b=Point(1390,710);
	root->append_child(menu);

	int menuX1 = 1390-menuWidth;
	int menuX2 = menuX1 + componentBtnWidth;

	// Draw Breadboard

	bbWidth = menuX1 - 2*bbX1;
	bbHeight = bbWidth/3;

	breadboard->a=Point(bbX1,bbY);
	breadboard->b=Point(bbX1 + bbWidth,bbY+bbHeight);
	breadboard->filled = 1;
	breadboard->click_anim = 0;
	breadboard->color = Point(234,234,234);
	//breadboard->text="Wire";
	//menu_wire->click = menu_wire;
	root->append_child(breadboard);

	for(int i=0;i<2;i++)
    {
        for(int j=0;j<10;j++)
        {
            for(int k=0;k<5;k++)
            {
                int diff = (bbWidth - 2*bbOffset)/49;
                pins[i][j*5+k] = new Div();
                pins[i][j*5+k]->a = Point(bbX1+bbOffset+10+diff*(j*5+k),bbY+bbHeight-bbOffset-15*i);
                pins[i][j*5+k]->R = 3;
                pins[i][j*5+k]->filled = 1;
                pins[i][j*5+k]->type |= constants.DIV_CIRCLE;
                pins[i][j*5+k]->type |= constants.DIV_BUTTON;
                pins[i][j*5+k]->color = DARK_GREY;
                pins[i][j*5+k]->click = handle_pin_click;
                pins[i][j*5+k]->data[0] = i;  // array index
                pins[i][j*5+k]->data[1] = j*5+k ;  // array index
                pins[i][j*5+k]->data[2] = i; // node-id
                pins[i][j*5+k]->data[3] = 0; // whether selected
                root->append_child(pins[i][j*5+k]);
            }
        }
    }

    for(int i=2;i<7;i++)
    {
        for(int j=0;j<64;j++)
        {
            int diff = (bbWidth - 2*bbOffset)/63;
            pins[i][j] = new Div();
            pins[i][j]->a = Point(bbX1+bbOffset+1+diff*(j),bbY+bbHeight-bbOffset-15*i-2*bbOffset);
            pins[i][j]->R = 3;
            pins[i][j]->filled = 1;
            pins[i][j]->type |= constants.DIV_CIRCLE;
            pins[i][j]->type |= constants.DIV_BUTTON;
            pins[i][j]->color = DARK_GREY;
            pins[i][j]->click = handle_pin_click;
            pins[i][j]->data[0] = i;  // array index
            pins[i][j]->data[1] = j ;  // array index
            pins[i][j]->data[2] = j+2; // node-id
            pins[i][j]->data[3] = 0; // whether selected
            root->append_child(pins[i][j]);
        }
    }

    for(int i=7;i<12;i++)
    {
        for(int j=0;j<64;j++)
        {
            int diff = (bbWidth - 2*bbOffset)/63;
            pins[i][j] = new Div();
            pins[i][j]->a = Point(bbX1+bbOffset+1+diff*(j),bbY+bbHeight-bbOffset-15*i-3*bbOffset);
            pins[i][j]->R = 3;
            pins[i][j]->filled = 1;
            pins[i][j]->type |= constants.DIV_CIRCLE;
            pins[i][j]->type |= constants.DIV_BUTTON;
            pins[i][j]->color = DARK_GREY;
            pins[i][j]->click = handle_pin_click;
            pins[i][j]->data[0] = i;  // array index
            pins[i][j]->data[1] = j ;  // array index
            pins[i][j]->data[2] = j+66; // node-id
            pins[i][j]->data[3] = 0; // whether selected
            root->append_child(pins[i][j]);
        }
    }

    for(int i=12;i<14;i++)
    {
        for(int j=0;j<10;j++)
        {
            for(int k=0;k<5;k++)
            {
                int diff = (bbWidth - 2*bbOffset)/49;
                pins[i][j*5+k] = new Div();
                pins[i][j*5+k]->a = Point(bbX1+bbOffset+10+diff*(j*5+k),bbY+bbHeight-bbOffset-15*i-5*bbOffset);
                pins[i][j*5+k]->R = 3;
                pins[i][j*5+k]->filled = 1;
                pins[i][j*5+k]->type |= constants.DIV_CIRCLE;
                pins[i][j*5+k]->type |= constants.DIV_BUTTON;
                pins[i][j*5+k]->color = DARK_GREY;
                pins[i][j*5+k]->click = handle_pin_click;
                pins[i][j*5+k]->data[0] = i;  // array index
                pins[i][j*5+k]->data[1] = j*5+k ;  // array index
                pins[i][j*5+k]->data[2] = i+130; // node-id
                pins[i][j*5+k]->data[3] = 0; // whether selected
                root->append_child(pins[i][j*5+k]);
            }
        }
    }

	menu_wire->a=Point(menuX1,610-1*componentBtnWidth);
	menu_wire->b=Point(menuX2,610-0*menuWidth);
	strcpy( menu_wire->text,"Wire");
	menu_wire->click = menu_wire_click;
	root->append_child(menu_wire);

	menu_resistance->a=Point(menuX2,610-1*componentBtnWidth);
	menu_resistance->b=Point(1390,610-0*componentBtnWidth);
	strcpy( menu_resistance->text,"Resistance");
	menu_resistance->type|=constants.DIV_BUTTON;
	menu_resistance->click = menu_resistance_click;
	root->append_child(menu_resistance);

	menu_dc_source->a=Point(menuX1,610-2*componentBtnWidth);
	menu_dc_source->b=Point(menuX2,610-1*componentBtnWidth);
	strcpy( menu_dc_source->text,"DC Source");
	menu_dc_source->click = menu_dc_source_click;
	root->append_child(menu_dc_source);

	menu_ammeter->a=Point(menuX2,610-2*componentBtnWidth);
	menu_ammeter->b=Point(1390,610-1*componentBtnWidth);
	strcpy(menu_ammeter->text,"Ammeter");
	menu_ammeter->click = menu_ammeter_click;
	root->append_child(menu_ammeter);

	menu_voltmeter->a=Point(menuX1,610-3*componentBtnWidth);
	menu_voltmeter->b=Point(menuX2,610-2*componentBtnWidth);
	strcpy( menu_voltmeter->text,"Voltmeter");
	menu_voltmeter->click = menu_voltmeter_click;
	root->append_child(menu_voltmeter);

	menu_led->a=Point(menuX2,610-3*componentBtnWidth);
	menu_led->b=Point(1390,610-2*componentBtnWidth);
	strcpy( menu_led->text,"LED");
	//menu_wire->click = menu_wire;
	root->append_child(menu_led);

	menu_capacitor->a=Point(menuX1,610-4*componentBtnWidth);
	menu_capacitor->b=Point(menuX2,610-3*componentBtnWidth);
	strcpy( menu_capacitor->text,"Capacitor");
	menu_capacitor->click = menu_capacitor_click;
	root->append_child(menu_capacitor);

	menu_inductor->a=Point(menuX2,610-4*componentBtnWidth);
	menu_inductor->b=Point(1390,610-3*componentBtnWidth);
	strcpy( menu_inductor->text,"Inductor");
	menu_inductor->click = menu_inductor_click;
	root->append_child(menu_inductor);

	reset_btn->a=Point(780,630);
	reset_btn->b=Point(880,680);
	strcpy( reset_btn->text,"Reset All");
	reset_btn->click=reset_btn_click;
	root->append_child(reset_btn);

	simulate_btn->a=Point(930,630);
	simulate_btn->b=Point(1030,680);
	strcpy( simulate_btn->text,"Simulate");
	simulate_btn->click = solve_circuit_btn_click;
	root->append_child(simulate_btn);

	update_btn->a=Point(360,630);
	update_btn->b=Point(460,680);
	strcpy( update_btn->text,"Update");
	//update_btn->click = update_btn_click;
	//root->append_child(update_btn);

	delete_btn->a=Point(490,630);
	delete_btn->b=Point(590,680);
	strcpy( delete_btn->text,"Delete");
	delete_btn->click = delete_btn_click;
	root->append_child(delete_btn);


	slider_global.createSliderDiv(20,600,1,162,162);
	slider_global.updateSliderDiv(40,626,1,999,50);
	slider_apply_function = update_component;
	slider_variable = &dummyVar;
}

void initialize_dc_circuit() {
	clear_root();
	setEverythingDC();

	//Ammeters[0].createAmmeterDiv(500,600,200,50);
	//Voltmeters[0].createVoltmeterDiv(100,600,200,50);
	//Batteries[0].createBatteryDiv(100,200,160,100);

	//update_handle = CreateThread(NULL, 0, update_thread, NULL, 0, NULL);
}
/*


			DC CIRCUIT END


*/
/*


			GRAPHS


*/
/********************************* Globals ****************************************/
void (*graph_pointer)(int);
/********************************* Thread Handles *********************************/

/********************************* Elements ***************************************/

Div *menu_dfs = new Div();
Div *menu_bfs = new Div();
Div *menu_dijkstra = new Div();
Div *menu_mst = new Div();
Div *graph_view = new Div();


Div *graph_reset_btn = new Div();
Div *graph_simulate_btn = new Div();

/******************************** Globals *****************************/
int id = 1;
int previous_node = -1;

map<int,set<int>> adj;
int weight[1000][1000];

int vis[10000];
int dist[10000];
int connected[1000][1000];

HANDLE graph_thread_handle;

/******************************** User input **************************/

/******************************** Searching Functions *******************************/

void dfs(int u) {
	if(vis[u]) return;
	vis[u]=1;
	cout<<u<<endl;
	Blue(div_from_id[u]);
	Sleep(delay);
	for(int v: adj[u]) {
		connected[u][v]=1;
		connected[v][u]=1;
		Sleep(delay);
		dfs(v);
		connected[u][v]=0;
		connected[v][u]=0;
		Sleep(delay);
	}
	Red(div_from_id[u]);
	Sleep(delay);
}
void bfs() {
	queue<int> q;
	q.push(adj.begin()->first);
	while(q.size()) {
		int u=q.front();
		q.pop();
		if(vis[u]) continue; // :)
		vis[u]=1;
		Blue(div_from_id[u]);
		Sleep(500);
		for(int v: adj[u]) {
			connected[u][v]=1;
			Sleep(500);
			q.push(v);
			//connected[u][v]=0;
			//Sleep(100);
		}
	}
}
void dijkstra() {
	int source = 1;
	for(auto p: adj) {
		dist[p.first] = 100000;
	}
	dist[adj.begin()->first] = 0;

	priority_queue<pair<int,int>> q;
	q.push({0, adj.begin()->first});

	while(q.size()) {
		int u=q.top().second;
		q.pop();
		//if(vis[u]) continue; // :)
		//vis[u]=1;
		Blue(div_from_id[u]);
		Sleep(500);
		for(int v: adj[u]) {
			if(dist[v] > dist[u] + weight[u][v]) {
				connected[u][v]=1;
				Sleep(500);

				dist[v] = dist[u] + weight[u][v];
				q.push({-dist[v], v});

				connected[u][v]=0;
				Sleep(100);
			}
		}
	}
}
struct edge {
	int u,v,w;
	edge(int u,int v,int w) {
		this->u=u;
		this->v=v;
		this->w=w;
	}
};
int parent[1000];
int Find(int x) {
	if(parent[x] == x) return x;
	return parent[x] = Find(parent[x]);
}
void mst() {
	vector<edge> edge_list;
	for(int u=0;u<100;u++) {
		for(int v=u+1;v<100;v++) {
			if(weight[u][v] <1000 and weight[u][v]!=0)
			edge_list.push_back(edge(u, v, weight[u][v]));
		}
	}
	sort(edge_list.begin(), edge_list.end(), [](edge &a, edge &b) {
		return a.w<b.w;
	});

	for(int i=0;i<1000;i++) parent[i]=i;
	for(auto e: edge_list) {
		connected[e.v][e.v] = 1;
		Sleep(500);
		int u=Find(e.u);
		int v=Find(e.v);
		if(u==v) {
			connected[e.v][e.v] = 0;
			Sleep(500);
			continue;
		}
		parent[u]=v;
		connected[e.u][e.v]=1;
		connected[e.v][e.u]=1;
		Sleep(500);
	}
}
/******************************** Thread functions ********************************/
DWORD WINAPI graph_thread() {
	memset(vis, 0, sizeof vis);
	memset(connected,0,sizeof connected);

	graph_pointer(adj.begin()->first);
}
/******************************** Button Controls *********************************/

void menu_dfs_click() {

	selected->bg_color=Point(0,0,0);
	selected = menu_dfs;
	selected->bg_color=Point(110,0,0);
	graph_pointer=dfs;
	strcpy(description->text,"DFS");
}
void menu_dijkstra_click() {

	selected->bg_color=Point(0,0,0);
	selected = menu_dijkstra;
	selected->bg_color=Point(110,0,0);
	graph_pointer=dijkstra;
	strcpy(description->text,"Dijkstra");
}
void menu_mst_click() {
	selected->bg_color=Point(0,0,0);
	selected = menu_mst;
	selected->bg_color=Point(110,0,0);
	graph_pointer=mst;
	strcpy(description->text,"MST");
}
void menu_bfs_click() {

	selected->bg_color=Point(0,0,0);
	selected = menu_bfs;
	selected->bg_color=Point(110,0,0);
	graph_pointer=bfs;
	strcpy(description->text,"BFS");
}
void graph_simulate_btn_click() {
	graph_thread_handle = CreateThread(NULL,0,graph_thread,NULL,0,NULL);
}

void graph_reset_btn_click() {
	TerminateThread(graph_thread_handle,0);
	for(int i=0;i<graph_view->children.size();i++) {
		Unselect(graph_view->children[i]);
	}
	memset(dist, 0, sizeof dist);
	memset(connected, 0, sizeof connected);
}
void node_right_click(Point where, Div *d) {
	for(int i=0;i<d->parent->children.size();i++) {
		if(d->parent->children[i]->div_id == d->div_id) {
			d->parent->children.erase(d->parent->children.begin() + i);
			break;
		}
	}
	adj[d->div_id].clear();
	for(auto &p: adj) {
		p.second.erase(d->div_id);
	}
	delete d;
}

void node_click(Point where, Div *d) {
	if(previous_node == d->div_id) {
		Unselect(div_from_id[d->div_id]);
		previous_node = -1;
		return;
	}
	if(previous_node == -1) {
		previous_node = d->div_id;
		Green(d);
	}
	else {
		if(adj[previous_node].count(d->div_id)) {
			adj[previous_node].erase(d->div_id);
			adj[d->div_id].erase(previous_node);
			weight[previous_node][d->div_id]=INT_MAX;
			weight[d->div_id][previous_node]=INT_MAX;
		}
		else {
			adj[previous_node].insert(d->div_id);
			adj[d->div_id].insert(previous_node);
			weight[previous_node][d->div_id]=rand()%100;
			weight[d->div_id][previous_node]=rand()%100;
		}
		Unselect(div_from_id[previous_node]);
		previous_node = -1;
	}
}
void node_dragged(Div *d, Point from, Point to) {
	d->a = to;
}
void node_leave_focus(Div *d) {
	if(previous_node != -1 and d->data[1]!=69) {
		Div *selected = div_from_id[previous_node];
		Unselect(selected);
		previous_node = -1;
	}
}
void graph_view_click(Point where, Div *d) {
	Div *node = new Div();
	node->a=where;
	char buf[10];
	itoa(id, buf, 10);
	strcpy(node->text,buf);
	node->data[0] = id++;
	node->dragged = node_dragged;
	node->R=20;
	node->line_width=5;
	node->data[1]=69; //;)
	node->leave_focus=node_leave_focus;
//	node->do_focus=node_do_focus;

	node->get_id(); // set unique id to each nodes
	node->type|=constants.DIV_CIRCLE;
	node->click=node_click;
	node->right_click=node_right_click;
	graph_view->append_child(node);
}
void graph_view_right_click(Point where, Div *d) {

}
void graph_randomize_btn_click() {

}
void initialize_graph() {
	clear_root();

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
	back->click=back_click;
	strcpy(back->text," < Back ");
	back->text_align=constants.ALIGN_LEFT | constants.ALIGN_CENTER_Y;
	menu->append_child(back);

	// select bubble sort in the beginning
	menu_dfs_click();

	menu_dfs->a=Point(0,550);
	menu_dfs->b=Point(300,100);
	strcpy(menu_dfs->text,"Depth First Search");
	menu_dfs->positioning=constants.DIV_RELATIVE;
	menu_dfs->click = menu_dfs_click;
	menu->append_child(menu_dfs);


	menu_bfs->a=Point(0,450);
	menu_bfs->b=Point(300,100);
	menu_bfs->positioning=constants.DIV_RELATIVE;
	strcpy(menu_bfs->text,"Bredth First Search");
	menu_bfs->click = menu_bfs_click;
	menu->append_child(menu_bfs);

	menu_dijkstra->a=Point(0,350);
	menu_dijkstra->b=Point(300,100);
	menu_dijkstra->positioning=constants.DIV_RELATIVE;
	strcpy(menu_dijkstra->text,"Dijsktra");
	menu_dijkstra->click = menu_dijkstra_click;
	menu->append_child(menu_dijkstra);

	menu_mst->a=Point(0,250);
	menu_mst->b=Point(300,100);
	menu_mst->positioning=constants.DIV_RELATIVE;
	strcpy(menu_mst->text,"Minimum Spanning Tree");
	menu_mst->click = menu_mst_click;
	menu->append_child(menu_mst);

	graph_view->a=Point(320,120);
	graph_view->b=Point(1100,680);
	graph_view->color=Point(0,255,0);
	graph_view->click=graph_view_click;
	graph_view->color=BLACK;
	root->append_child(graph_view);

	reset_btn->a=Point(400,50);
	reset_btn->b=Point(500,100);
	strcpy(reset_btn->text,"Reset");
	reset_btn->click=graph_reset_btn_click;
	root->append_child(reset_btn);

	simulate_btn->a=Point(550,50);
	simulate_btn->b=Point(650,100);
	strcpy(simulate_btn->text,"Simulate");
	simulate_btn->click=graph_simulate_btn_click;
	root->append_child(simulate_btn);

	step_btn->a=Point(700,50);
	step_btn->b=Point(800,100);
	strcpy(step_btn->text,"Step");
	step_btn->click=step_btn_click;
	root->append_child(step_btn);


	speed_sld = new sliderx(1,50,5);
	speed_sld->slider->a=Point(850,50);
	speed_sld->slider->b=Point(1050,100);
	speed_sld->slider->color=TRANSPARENT;
	speed_sld->slider->click=speed_slider_updated;
	root->append_child(speed_sld->slider);

	//update_handle = CreateThread(NULL, 0, update_thread, NULL, 0, NULL);
}
/*


			GRAPHS END


*/
/*



							TOWER OF HANOI START


*/
// General definitions

/********************************* Globals ****************************************/

int screenWidth = 1400;
int screenHeight = 720;

// Tower of Hanoi Global Variables

void (*toh_solve_pointer)(int,int,int,int);
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
int baseHeight = 220; // y value of upper part of base

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
            Sleep(delay/20);
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
            Sleep(delay/20);
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
   // Sleep(300);

    //PlaySound(NULL,0,0);

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
	success_btn->a=Point(900,100);
	success_btn->b=Point(1100,150);
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
void initialize_toh() {
	clear_root();

	menu->a=Point(0,0);
	menu->b=Point(300,720);
	root->append_child(menu);


	back->positioning=constants.DIV_RELATIVE;
	back->a=Point(0, 650);
	back->b=Point(300,70);
	back->click=back_click;
	strcpy(back->text," < Back ");
	back->text_align=constants.ALIGN_LEFT | constants.ALIGN_CENTER_Y;
	menu->append_child(back);

	// select toh in the beginning


	menu_toh->a=Point(0,550);
	menu_toh->b=Point(300,100);
	menu_toh->positioning=constants.DIV_RELATIVE;
	strcpy(menu_toh->text, "Tower Of Hanoi");
	menu_toh->click = menu_toh_click;
	menu->append_child(menu_toh);

	reposition_btn->a=Point(400,100);
	reposition_btn->b=Point(600,150);
	strcpy(reposition_btn->text,"Reposition Discs");
	reposition_btn->click=reposition_btn_click;
	root->append_child(reposition_btn);

	toh_solve_btn->a=Point(700,100);
	toh_solve_btn->b=Point(800,150);
	strcpy(toh_solve_btn->text,"Solve");
	toh_solve_btn->click=toh_solve_btn_click;
	root->append_child(toh_solve_btn);

	success_btn->a=Point(0,0);
	success_btn->b=Point(10,010);
	success_btn->color = Point(0,255,0);
//	strcpy(success_btn->text,"Task Comple ted! ");
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
    menu_toh_click();
}
/*


			TOWER OF HANOI END


*/

/*


			SEARCHING START


*/
/********************************* Globals ****************************************/
void (*search_pointer)(void);

/********************************* Thread Handles *********************************/
HANDLE search_handle, search_update_handle;

/********************************* Elements ***************************************/

//Sidebar Menu
Div *menu_binary_search = new Div();
Div *menu_linear_search = new Div();

Div *search_btn = new Div();
Div *searching_reset_btn = new Div();

Div *search_input = new Div();

Div *search_array[30];
//Sort Visual

/******************************** User input **************************/
int max_search_array_size = 30;
int search_array_size = 30;
int search_array_start = 350;
int search_array_width = 30;
/******************************** Searching Functions *******************************/
int search_array_sorted = 0;
void linear_search() {
	int x=atoi(search_input->text);
	for(int i=0;i<search_array_size;i++) {
		Yellow(search_array[i]);
		Sleep(delay);
		if(x == atoi(search_array[i]->text)) {
			Green(search_array[i]);
			break;
		}
		Red(search_array[i]);
		Sleep(delay);
	}
}
void Binary_search() {
	if(!search_array_sorted) {
			return;
			//asdasdasdadasdasdasdasdadsad
	}
	int x=atoi(search_input->text);
	int l=0, r=search_array_size-1;
	while(l<r) {
		int m=(l+r)/2;
		Yellow(search_array[m]);
		Sleep(delay);
		if(atoi(search_array[m]->text) < x) l=m+1;
		else r=m;
		Red(search_array[m]);
		Sleep(delay);
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
void searching_reset_btn_click() {
	TerminateThread(simulate_handle,0);

	global_step=0;
	STEP = 0;
	select_step(-1);
	step = 1;

	step_btn->hidden = 0;
	simulate_btn->hidden = 0;
	sort_btn->hidden = 0;

	for(int i=0;i<max_search_array_size;i++) Unselect(search_array[i]);

	for(int i=0;i<search_array_size;i++) {
		char number[5];
		search_array[i]->hidden=0;
		int rnd=rand()%100;
		itoa(rnd, number, 10);
		strcpy(search_array[i]->text,number);
		search_array[i]->color=Point(255,0,0);
	}
	for(int i=search_array_size;i<max_search_array_size;i++) {
		search_array[i]->hidden=1;
	}
	search_array_sorted = 0;
}
void searching_sort_btn_click() {
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
	TerminateThread(simulate_handle,0);
	simulate_handle = NULL;

	global_step=0;
	STEP = 0;
	select_step(-1);
	step = 1;

	step_btn->hidden = 1;
	simulate_btn->hidden = 1;
	sort_btn->hidden = 1;

	simulate_handle = CreateThread(NULL, 0, search_thread, NULL, 0, NULL);
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
void searching_slider_updated(Point p, Div *d) {
	int val = atoi(d->text);
	search_array_size = val;
	searching_reset_btn_click();
}
void initialize_searching() {

	clear_root();


	description->a=Point(1100,0);
	description->b=Point(1350,670);
	description->text_align=constants.ALIGN_LEFT | constants.ALIGN_UP;
	description->color=Point(0,0,0);
	root->append_child(description);

	root->append_child(menu);


	back->positioning=constants.DIV_RELATIVE;
	back->a=Point(0, 650);
	back->click=back_click;
	back->b=Point(300,70);
	strcpy(back->text," < Back ");
	back->text_align=constants.ALIGN_LEFT | constants.ALIGN_CENTER_Y;
	menu->append_child(back);

	// select bubble sort in the beginning
	menu_linear_search_click();

	//return;

	menu_linear_search->a=Point(0,550);
	menu_linear_search->b=Point(300,100);
	strcpy(menu_linear_search->text,"Linear Search");
	menu_linear_search->positioning=constants.DIV_RELATIVE;
	menu_linear_search->click = menu_linear_search_click;
	menu->append_child(menu_linear_search);

    //return;

	menu_binary_search->a=Point(0,450);
	menu_binary_search->b=Point(300,100);
	menu_binary_search->positioning=constants.DIV_RELATIVE;
	strcpy(menu_binary_search->text,"Binary Search");
	menu_binary_search->click = menu_binary_search_click;
	menu->append_child(menu_binary_search);

	//return;

	search_input->a=Point(400,150);
	search_input->b=Point(500,200);
	strcpy(search_input->text,"");
	search_input->type |= constants.DIV_INPUT;
	search_input->color = Point(255,0,0);
	search_input->bg_color = Point(255,255,255);
	search_input->text_color = Point(0,0,0);
	search_input->line_width = 1;

	//return;

	//root->append_child(search_input);

	//return;

	sort_btn->a=Point(550,150);
	sort_btn->b=Point(650,200);
	strcpy(sort_btn->text,"Sort");
	sort_btn->click=searching_sort_btn_click;
	sort_btn->line_width = 3;
	root->append_child(sort_btn);

	//return;

	sld= new sliderx(1,max_search_array_size,search_array_size);
	sld->slider->a=Point(700,150);
	sld->slider->b=Point(1000,200);
	sld->slider->color=TRANSPARENT;
	sld->slider->click=searching_slider_updated;
	root->append_child(sld->slider);

	reset_btn->a=Point(400,50);
	reset_btn->b=Point(500,100);
	strcpy(reset_btn->text,"Reset");
	reset_btn->click=searching_reset_btn_click;
	root->append_child(reset_btn);

	simulate_btn->a=Point(550,50);
	simulate_btn->b=Point(650,100);
	strcpy(simulate_btn->text,"Simulate");
	simulate_btn->click=search_btn_click;
	root->append_child(simulate_btn);

	step_btn->a=Point(700,50);
	step_btn->b=Point(800,100);
	strcpy(step_btn->text,"Step");
	step_btn->click=step_btn_click;
	root->append_child(step_btn);

	speed_sld = new sliderx(1,50,5);
	speed_sld->slider->a=Point(850,50);
	speed_sld->slider->b=Point(1050,100);
	speed_sld->slider->color=TRANSPARENT;
	speed_sld->slider->click=speed_slider_updated;
	root->append_child(speed_sld->slider);


	for(int i=0;i<max_search_array_size;i++) {
		search_array[i]=new Div();
		search_array[i]->a=Point(search_array_start+search_array_width*i,400);
		search_array[i]->b=Point(search_array_start+search_array_width*(i+1),400 + search_array_width);
		root->append_child(search_array[i]);
	}
	searching_reset_btn_click();
	//update_handle = CreateThread(NULL, 0, update_thread, NULL, 0, NULL);
}
/*


			SEARCHING END


*/
/*


			NUMBER THEORY


*/

/********************************* Globals ****************************************/
// General Global

void (*nt_solve_pointer)(int,int);

// Prime Sieve Globals

int totalNumSieve = 162;

int numbers_in_a_row = 18;
int upHeight = 550;
int leftX = 360;
int blockSide = 50;
int blockOffset = 5;

int primeFound = 0;

/********************************* Thread Handles *********************************/
HANDLE nt_solve_handle, nt_update_handle;

/********************************* Elements ***************************************/

//Sidebar Menu
Div *menu_primesieve = new Div();
Div *menu_pascaltriangle = new Div();

//solve Visual
Div *numbers[200];

//Array Controls

Div *nt_reset_btn = new Div();
Div *nt_solve_btn = new Div();

Div *nt_valueBox = new Div();

//SliderDiv slider_global;

/******************************** Thread functions ********************************/
DWORD WINAPI nt_solve_thread() {
	nt_solve_pointer(1, totalNumSieve);
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
            strcpy(nt_valueBox->text,primeText);

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
void nt_reset_btn_click() {

    TerminateThread(nt_solve_handle,0);

    primeFound = 0;

    char primeText[200];
	strcpy(primeText,"");
	char primeNum[100];
	itoa(primeFound,primeNum,10);
	strcat(primeText,primeNum);
	strcat(primeText,"  Primes Found  ");
	strcpy(nt_valueBox->text,primeText);

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
void nt_solve_btn_click() {

    nt_reset_btn_click();

    DWORD solveId;
	nt_solve_handle = CreateThread(NULL, 0, nt_solve_thread, NULL, 0, NULL);
}

void menu_primesieve_click() {

    slider_variable = &totalNumSieve;

    slider_apply_function = nt_reset_btn_click;

    slider_global.updateSliderDiv(400,630,1,162,162);

    nt_reset_btn_click();

	selected->bg_color=theme->bg_color;
	selected = menu_primesieve;
	selected->bg_color=Point(110,0,0);
	nt_solve_pointer=primesieve_solve;
}

void initialize_nt() {
	clear_root();

	menu->a=Point(0,0);
	menu->b=Point(300,720);
	root->append_child(menu);


	back->positioning=constants.DIV_RELATIVE;
	back->a=Point(0, 650);
	back->b=Point(300,70);
	back->click=back_click;
	strcpy(back->text," < Back ");
	back->text_align=constants.ALIGN_LEFT | constants.ALIGN_CENTER_Y;
	menu->append_child(back);

	// select toh in the beginning


	menu_primesieve->a=Point(0,550);
	menu_primesieve->b=Point(300,100);
	menu_primesieve->positioning=constants.DIV_RELATIVE;
	strcpy(menu_primesieve->text, "Prime Sieve");
	menu_primesieve->click = menu_primesieve_click;
	menu->append_child(menu_primesieve);

	nt_reset_btn->a=Point(400,40);
	nt_reset_btn->b=Point(500,90);
	strcpy( nt_reset_btn->text,"Reset");
	nt_reset_btn->click=nt_reset_btn_click;
	root->append_child(nt_reset_btn);

	nt_solve_btn->a=Point(550,40);
	nt_solve_btn->b=Point(650,90);
	strcpy( nt_solve_btn->text,"Solve");
	nt_solve_btn->click=nt_solve_btn_click;
	root->append_child(nt_solve_btn);

	nt_valueBox->a=Point(1030,630);
	nt_valueBox->positioning = constants.DIV_RELATIVE;
	nt_valueBox->b=Point(300,50);
	nt_valueBox->color = LIGHT_GREEN;
	nt_valueBox->filled = 1;
	nt_valueBox->text_color = BLACK;

	char primeText[200];
	strcpy(primeText,"");
	char primeNum[100];
	itoa(primeFound,primeNum,10);
	strcat(primeText,primeNum);
	strcat(primeText,"  Primes Found  ");
	strcpy(nt_valueBox->text,primeText);

	root->append_child(nt_valueBox);

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

}
/*


		NUMBER THEORY END


*/
/*


			GEOMETRY START


*/

#define debug(i,j) printf("%d   %d\n\n",i,j)

struct pt {
    double x, y;
    int i;
    pt(int p, double a, double b)
    {
        i=p;
        x=a;
        y=b;
    }
};
/********************************* Globals ****************************************/

void (*geometry_solve_pointer)(int,int);
int initial = 1;
int getOut = 0;
int MAXlineCount = 100;
int lineCount = 0;
int lineNum = 5;
int pointNum = 30;
int baseColor = 100;
int pointRadius = 7;

int Ax = 380, Ay = 200, Bx = 1080, By = 600; // A and B are bottom left and upper right points of the domain

vector<pt>a;
vector<pt>t;


/********************************* Thread Handles *********************************/
HANDLE geometry_solve_handle, geometry_update_handle;

/********************************* Elements ***************************************/

//Sidebar Menu
Div *menu_convexhull = new Div();
Div *menu_closestpair = new Div();

//solve Visual
Div *points[200];
Div *lines[200][200];
Div *borderCircles[200];
Div *borderOutCircles[200];
Div *bestline = new Div();
//Array Controls

Div *solve_btn = new Div();

Div *valueBox = new Div();

/******************************** Thread functions ********************************/
DWORD WINAPI geometry_solve_thread() {
	geometry_solve_pointer(0, pointNum);
}

/********************************* Helping Functions *********************************/


// Converts a given integer x to string str[].
// d is the number of digits required in the output.
// If d is more than the number of digits in x,
// then 0s are added at the beginning.
// Converts a double number to a string.
/******************************** solveing Functions *******************************/
void drawLine(int i, int j)
{
    lines[i][j]->a = points[i]->a;
    lines[i][j]->b = points[j]->a;
}

void removeLine(int i, int j)
{
    lines[i][j]->a = Point(0,0);
    lines[i][j]->b = Point(0,0);
}

void borderPoint(int i)
{
    borderCircles[i]->color = WHITE;
    borderOutCircles[i]->color = BLUE;
    borderOutCircles[i]->R = pointRadius+7;
    borderCircles[i]->R = pointRadius + 4;
    points[i]->R = 0;
}

void unborderPoint(int i)
{
    borderOutCircles[i]->R = 0;
    borderCircles[i]->R = 0;
    points[i]->R = pointRadius;
}

void hightlight(int i)
{
    points[i]->R = pointRadius + 4;
}

void unhightlight(int i)
{
    points[i]->R = pointRadius;
}


int orientation(pt a, pt b, pt c) {
    double v = a.x*(b.y-c.y)+b.x*(c.y-a.y)+c.x*(a.y-b.y);
    if (v < 0) return -1; // clockwise
    if (v > 0) return +1; // counter-clockwise
    return 0;
}

bool cw(pt a, pt b, pt c, bool include_collinear) {
    int o = orientation(a, b, c);
    return o < 0 || (include_collinear && o == 0);
}

bool collinear(pt a, pt b, pt c) { return orientation(a, b, c) == 0; }

void convex_hull() {

    printf("In here\n");
    bool include_collinear = false;
    pt p0 = *min_element(a.begin(), a.end(), [](pt a, pt b) {
        return make_pair(a.y, a.x) < make_pair(b.y, b.x);
    });

    points[p0.i]->R = 15;
    points[p0.i]->bg_color = Point(0,255,0);

    sort(a.begin(), a.end(), [&p0](const pt& a, const pt& b) {
        int o = orientation(p0, a, b);
        if (o == 0)
            return (p0.x-a.x)*(p0.x-a.x) + (p0.y-a.y)*(p0.y-a.y)
                < (p0.x-b.x)*(p0.x-b.x) + (p0.y-b.y)*(p0.y-b.y);
        return o < 0;
    });
    if (include_collinear) {
        int i = (int)a.size()-1;
        while (i >= 0 && collinear(p0, a[i], a.back())) i--;
        reverse(a.begin()+i+1, a.end());
    }

    vector<pt> st;
    for (int i = 0; i < (int)a.size(); i++) {
        while (st.size() > 1 && !cw(st[st.size()-2], st.back(), a[i], include_collinear))
        {
            PlaySound(TEXT("scan3.wav"), NULL , SND_ASYNC);
            if(st.size()>1)
            {
                removeLine(st[st.size()-2].i,st[st.size()-1].i);
            }
            if(st.size()>0)
            {
                unhightlight(st[st.size()-1].i);
            }
            st.pop_back();
            Sleep(delay);
        }
        if(st.size()>0)
        {
            drawLine(st[st.size()-1].i,a[i].i);
            hightlight(a[i].i);
        }
        st.push_back(a[i]);
        PlaySound(TEXT("scan3.wav"), NULL , SND_ASYNC);
        Sleep(2*delay);
    }

    drawLine(st[st.size()-1].i,p0.i);

    a = st;
}

void convexhull_solve(int l, int r) {

    int n = pointNum,i,j;
    convex_hull();

}


struct cmp_x {
    bool operator()(const pt & a, const pt & b) const {
        return a.x < b.x || (a.x == b.x && a.y < b.y);
    }
};

struct cmp_y {
    bool operator()(const pt & a, const pt & b) const {
        return a.y < b.y;
    }
};

double mindist;
pair<int, int> best_pair = {0,0};

void upd_ans(const pt & a, const pt & b) {
    PlaySound(TEXT("scan3.wav"), NULL , SND_ASYNC);
    int flag = 1;
    hightlight(a.i);
    hightlight(b.i);
    drawLine(a.i,b.i);
    double dist = sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
    if (dist < mindist) {
        mindist = dist;

        char str[50] = "Minimum Distance  =  ";
        char temp[100];
        dtoa(mindist,temp,6);
        strcat(str,temp);
        strcpy( valueBox->text , str );

        unborderPoint(best_pair.first);
        unborderPoint(best_pair.second);
        best_pair = {a.i, b.i};
        borderPoint(a.i);
        borderPoint(b.i);
        bestline->a = points[a.i]->a;
        bestline->b = points[b.i]->a;
        flag = 0;
    }
    Sleep(2*delay);
    if(flag==1)
    {
        unhightlight(a.i);
        unhightlight(b.i);
    }
    removeLine(a.i,b.i);
}

void rec(int l, int r) {

    hightlight(best_pair.first);
    hightlight(best_pair.second);
    //debug(l,r);

    if (r - l <= 3) {
        for (int i = l; i < r; ++i) {
            for (int j = i + 1; j < r; ++j) {
                upd_ans(a[i], a[j]);
            }
        }
        sort(a.begin() + l, a.begin() + r, cmp_y());
        return;
    }

    int m = (l + r) >> 1;
    int midx = a[m].x;
    rec(l, m);
    rec(m, r);

    //debug(l,r);

    merge(a.begin() + l, a.begin() + m, a.begin() + m, a.begin() + r, t.begin(), cmp_y());

    debug(l,r);

    copy(t.begin(), t.begin() + r - l, a.begin() + l);


    int tsz = 0;
    for (int i = l; i < r; ++i) {
        if (abs(a[i].x - midx) < mindist) {
            for (int j = tsz - 1; j >= 0 && a[i].y - t[j].y < mindist; --j)
                upd_ans(a[i], t[j]);
            t[tsz++] = a[i];
        }
    }
}


void closestpair(int l, int r) {
	t.clear();
	for(int i=0;i<pointNum;i++)
        t.push_back(pt(0,0,0));
    sort(a.begin(), a.end(), cmp_x());
    mindist = 1E20;
    rec(l, r);
    borderPoint(best_pair.first);
    borderPoint(best_pair.second);
    debug(0,0);
}
/******************************** Button Controls *********************************/
void geometry_reset_btn_click() {

    strcpy( valueBox->text, "Minimum Distance" );

    initial = 0;
    TerminateThread(geometry_solve_handle,0);
    bestline->a = Point(0,0);
    bestline->b = Point(0,0);
    getOut = 1;

    a.clear();

    for(int i=0;i<100;i++)
    {
        points[i]->hidden = 1;
    }
    for(int i=0;i<100;i++)
    {
        borderOutCircles[i]->hidden = 1;
    }
    for(int i=0;i<100;i++)
    {
        borderCircles[i]->hidden = 1;
    }

	for(int i=0;i<pointNum;i++) {
        points[i]->hidden = 0;
		points[i]->filled=1;
		points[i]->type = constants.DIV_CIRCLE;
		points[i]->a=Point(Ax+rand()%(Bx-Ax),Ay+rand()%(By-Ay));
		points[i]->R = pointRadius;
		points[i]->color = Point(baseColor+rand()%(250-baseColor),baseColor+rand()%(250-baseColor),baseColor+rand()%(250-baseColor));

        pt k = pt(i,points[i]->a.x+0.0,points[i]->a.y+0.0);
		a.push_back(k);

	}

	for(int i=0;i<pointNum;i++) {
        borderOutCircles[i]->hidden = 0;
		borderOutCircles[i]->filled=1;
		borderOutCircles[i]->type = constants.DIV_CIRCLE;
		borderOutCircles[i]->a=points[i]->a;
		borderOutCircles[i]->R = 0;
		borderOutCircles[i]->color = Point(0,255,0);
	}

	for(int i=0;i<pointNum;i++) {
        borderCircles[i]->hidden = 0;
		borderCircles[i]->filled=1;
		borderCircles[i]->type = constants.DIV_CIRCLE;
		borderCircles[i]->a=points[i]->a;
		borderCircles[i]->R = 0;
		borderCircles[i]->color = Point(0,255,0);
	}

	for(int i=0;i<100;i++)
    {
        for(int j=0;j<100;j++)
        {
            lines[i][j]->type = constants.DIV_LINE;
            lines[i][j]->a = Point(0,0);
            lines[i][j]->b = Point(0,0);
            lines[i][j]->color = Point(255,255,255);
            root->append_child(lines[i][j]);
        }
    }
}
void geometry_solve_btn_click() {
    initial = 0;
    DWORD solveId;
	geometry_solve_handle = CreateThread(NULL, 0, geometry_solve_thread, NULL, 0, NULL);
}
void menu_convexhull_click() {


    slider_variable = &pointNum;

    printf("%d\n",pointNum);
    slider_apply_function = geometry_reset_btn_click;

    slider_global.updateSliderDiv(400,630,5,50,20);

    valueBox->a = Point(0,0);
    valueBox->b = Point(0,0);

//    if(!initial)
//        reset_btn_click();

    /*TerminateThread(solve_handle,0);
    bestline->a = Point(0,0);
    bestline->b = Point(0,0);*/

    geometry_reset_btn_click();

	selected->bg_color=Point(0,0,0);
	selected = menu_convexhull;
	selected->bg_color=Point(110,0,0);
	geometry_solve_pointer=convexhull_solve;
}
void menu_closestpair_click() {

    slider_variable = &pointNum;

    slider_apply_function = geometry_reset_btn_click;

    slider_global.updateSliderDiv(400,630,5,50,20);

    valueBox->a=Point(900,620);
	valueBox->b=Point(1250,700);

    initial = 0;
    /*TerminateThread(solve_handle,0);
    bestline->a = Point(0,0);
    bestline->b = Point(0,0);*/
    geometry_reset_btn_click();

	selected->bg_color=Point(0,0,0);
	selected = menu_closestpair;
	selected->bg_color=Point(110,0,0);
	geometry_solve_pointer=closestpair;
}

void initialize_geometry() {
	clear_root();

	root->append_child(menu);


	back->positioning=constants.DIV_RELATIVE;
	back->a=Point(0, 650);
	back->b=Point(300,70);
	back->click=back_click;
	strcpy(back->text," < Back ");
	back->text_align=constants.ALIGN_LEFT | constants.ALIGN_CENTER_Y;
	menu->append_child(back);


	menu_convexhull->a=Point(0,550);
	menu_convexhull->b=Point(300,100);
	menu_convexhull->positioning=constants.DIV_RELATIVE;
	strcpy(menu_convexhull->text, "Convex Hull");
	menu_convexhull->click = menu_convexhull_click;
	menu->append_child(menu_convexhull);

	menu_closestpair->a=Point(0,450);
	menu_closestpair->b=Point(300,100);
	menu_closestpair->positioning=constants.DIV_RELATIVE;
	strcpy(menu_closestpair->text,"Closest Pair");
	menu_closestpair->click = menu_closestpair_click;
	menu->append_child(menu_closestpair);


	reset_btn->a=Point(400,100);
	reset_btn->b=Point(500,150);
	strcpy( reset_btn->text,"Reset");
	reset_btn->click=geometry_reset_btn_click;
	root->append_child(reset_btn);

	solve_btn->a=Point(550,100);
	solve_btn->b=Point(650,150);
	strcpy( solve_btn->text,"Solve");
	solve_btn->click=geometry_solve_btn_click;
	root->append_child(solve_btn);

	valueBox->a=Point(0,0);
	valueBox->b=Point(0,0);
	strcpy( valueBox->text,"Minimum Distance = ");
	root->append_child(valueBox);

	a.clear();
	for(int i=0;i<100;i++) {

		points[i]=new Div();
		points[i]->filled=1;
		points[i]->type = constants.DIV_CIRCLE;
		points[i]->a=Point(Ax+rand()%(Bx-Ax),Ay+rand()%(By-Ay));
		points[i]->R = pointRadius;
		points[i]->color = Point(baseColor+rand()%(250-baseColor),baseColor+rand()%(250-baseColor),baseColor+rand()%(250-baseColor));
		//points[i]->b=Point(430+30*i,400+rand()%300);

		pt k = pt(i,points[i]->a.x+0.0,points[i]->a.y+0.0);
		points[i]->hidden = 1;
		a.push_back(k);

		root->append_child(points[i]);
	}

	for(int i=0;i<100;i++) {

        borderOutCircles[i] = new Div();
		borderOutCircles[i]->filled=1;
		borderOutCircles[i]->type = constants.DIV_CIRCLE;
		borderOutCircles[i]->a=points[i]->a;
		borderOutCircles[i]->R = 0;
		borderOutCircles[i]->color = Point(0,255,0);

		borderOutCircles[i]->hidden = 1;
		root->append_child(borderOutCircles[i]);
	}

	for(int i=0;i<100;i++) {

        borderCircles[i] = new Div();
		borderCircles[i]->filled=1;
		borderCircles[i]->type = constants.DIV_CIRCLE;
		borderCircles[i]->a=points[i]->a;
		borderCircles[i]->R = 0;
		borderCircles[i]->color = Point(0,255,0);

        borderCircles[i]->hidden = 1;
		root->append_child(borderCircles[i]);
	}

	//printf("Okay \n");

	for(int i=0;i<100;i++)
    {
        for(int j=0;j<100;j++)
        {
            lines[i][j] = new Div();
            lines[i][j]->type = constants.DIV_LINE;
            lines[i][j]->line_width = 5;
            lines[i][j]->a = Point(0,0);
            lines[i][j]->b = Point(0,0);
            lines[i][j]->color = Point(255,255,255);
            root->append_child(lines[i][j]);
        }
    }

    bestline->type = constants.DIV_LINE;
    bestline->line_width = 3;
    bestline->a = Point(0,0);
    bestline->b = Point(0,0);
    bestline->color = Point(255,255,0);
    root->append_child(bestline);

    slider_global.createSliderDiv(400,630,1,162,162);

    printf("Okay \n");

    // select convexhull solve in the beginning
	menu_convexhull_click();

	//update_handle = CreateThread(NULL, 0, update_thread, NULL, 0, NULL);
}
/*


			GEOMETRY END


*/
HANDLE animate_header_thread, animate_chaos_thread;
int at_logic = 0;
void mm_logic_click() {
	TerminateThread(animate_header_thread,0);
	TerminateThread(animate_chaos_thread,0);
	initialize_logic();
	at_logic = 1;
}
void mm_sortings_click() {
	TerminateThread(animate_header_thread,0);
	TerminateThread(animate_chaos_thread,0);
	initialize_sortings();
}
void mm_toh_click() {
	TerminateThread(animate_header_thread,0);
	TerminateThread(animate_chaos_thread,0);
	initialize_toh();
}
void mm_searching_click() {
	TerminateThread(animate_header_thread,0);
	TerminateThread(animate_chaos_thread,0);
	initialize_searching();
}
int at_graphs = 0;
void mm_graphs_click() {
	TerminateThread(animate_header_thread,0);
	TerminateThread(animate_chaos_thread,0);
	initialize_graph();
	at_graphs = 1;
}
void mm_geometry_click() {
	TerminateThread(animate_header_thread,0);
	TerminateThread(animate_chaos_thread,0);
	initialize_geometry();
}
void mm_nt_click() {
	TerminateThread(animate_header_thread,0);
	TerminateThread(animate_chaos_thread,0);
	initialize_nt();
}
void mm_circuit_click() {
	TerminateThread(animate_header_thread,0);
	TerminateThread(animate_chaos_thread,0);
	initialize_dc_circuit();
}

struct Complex {
	double x,y;
	Complex(double X, double Y) {
		x=X;
		y=Y;
	}
	double abs() {
		return sqrt(x*x + y*y);
	}
	Complex operator+(Complex other) {
		return Complex(x+other.x, y+other.y);
	}
	Complex operator*(Complex other) {
		return Complex(x * other.x - y*other.y, x * other.y + y * other.x);
	}
};
bool belongs(Complex c, int iter) {
	Complex z=Complex(0,0);
	int i=0;
	while(z.abs() < 2 and i<iter) {
		z=(z*z)+c;
		i++;
	}
	return i==iter;
}
Div *frac = new Div();

void pixel(double x,double y, Point color) {
	x*=300;
	y*=300;
	Div *d = new Div();
	d->a=Point(x,y);
	d->R=1;
	d->color=color;
	d->positioning=constants.DIV_RELATIVE;
	d->type=constants.DIV_CIRCLE;
	frac->append_child(d);
}
int iii=77;
DWORD WINAPI fractal() {
while(1) {
	double minx = -2, maxx =1, miny=-1,maxy=1;
	double xstep = 0.01, ystep = 0.01;
	double x = minx;
	while(x <= maxx) {
		double y=miny;
		while(y <= maxy) {
			bool res = belongs(Complex(x,y), min(iii, 77));
			if(res) {
				pixel(x,y,RED);
			}
			else pixel(x,y,WHITE);
			y+=ystep;
		}
		x+=xstep;
	}

	Sleep(010000);
	//iii++;
}
}

Div *header = new Div();
Div *chaos = new Div();
Div *balls[50];

DWORD WINAPI animate_header() {
	char text[] = "TINKERC0DE";
	int at = 0;
	int sat=0;
	while(1) {
		char snd[100]="play .\\key_sounds\\";
		char sat_[3]; itoa(sat, sat_, 10);
		strcat(snd, sat_);
		strcat(snd, ".wav");
		sat=rand()%9;

		if(text[at]!=' ')
		mciSendString(snd,NULL,0,0);

		if(rand()%3==0)
		Sleep(rand()%500);
		else
		Sleep(rand()%10);

		char now[28] = "";
		for(int i=0;i<=at;i++) now[i]=text[i];
		strcpy(header->text, now);
		at++;

		int cnt = rand()%5;
		while(cnt--) {
			strcat(header->text,"|");
			Sleep(rand()%100);
			header->text[strlen(header->text)-1]='\0';
			Sleep(rand()%100);
		}
		at%=strlen(text);
		if(at == 0) Sleep(3000);
	}
}
double tt=0;
Point Get() {
	return Point(sin(tt + rand()%100), cos(tt + rand()%50)) * 1000;
}
DWORD WINAPI animate_chaos() {
	while(1) {
		for(int i=0;i<20;i++) {
			balls[i]->a = balls[i]->a + balls[i]->data_points[0];
			//balls[i]->a = Get();

			if(balls[i]->a.x > 1400 or balls[i]->a.x < 0) {
				balls[i]->data_points[0].x = -balls[i]->data_points[0].x;
			}
			if(balls[i]->a.y > 720 or balls[i]->a.y < 0) {
				balls[i]->data_points[0].y = -balls[i]->data_points[0].y;
			}
		}
		Sleep(10);
		tt++;
	}
}
int padding = 20;


void initialize_main() {
	at_graphs = 0;
	at_logic = 0;

	clear_root();
	root->data[0]=45;
	TerminateThread(animate_header_thread,0);
	TerminateThread(animate_chaos_thread,0);

	chaos->clear();
	chaos->a=Point(0,0);
	chaos->b=Point(1400,720);
//	root->append_child(chaos);

	header->a=Point(370, 500);
	header->b=Point(1400, 720);
	header->color=Point(-1,0,0);
	header->font_size=50;
	header->font_width = 5;
	header->text_align=constants.ALIGN_LEFT | constants.ALIGN_CENTER_Y;
	root->append_child(header);

	for(int i=0;i<20;i++) {
		balls[i] = new Div();
		balls[i]->a = Point(rand()%1000, rand()%720);
		balls[i]->type=constants.DIV_CIRCLE;
		balls[i]->R=rand()%10;
		balls[i]->filled=1;
		balls[i]->color=Point(rand()%255, rand()%255, rand()%255);
		balls[i]->data_points[0] = Point(rand()%10, rand()%10);
		balls[i]->data_points[1] = Point(rand()%5, rand()%10);
		chaos->append_child(balls[i]);
	}

	animate_header_thread = CreateThread(NULL,NULL,animate_header,NULL,NULL,NULL);
	animate_chaos_thread = CreateThread(NULL,NULL,animate_chaos,NULL,NULL,NULL);


	mm_sortings->a=Point(100,300);
	mm_sortings->b=Point(300,500);
	mm_sortings->bg_color=Point(20,20,20);
	strcpy(mm_sortings->text, "SORTINGS   ");
	mm_sortings->click = mm_sortings_click;
	mm_sortings->font=constants.FONT_ANIK;
	root->append_child(mm_sortings);

	mm_searching->a=Point(300+padding,300);
	mm_searching->b=Point(500+padding,500);
	mm_searching->bg_color=Point(20,20,20);
	strcpy(mm_searching->text,"SEARCHING   ");
	mm_searching->click = mm_searching_click;
	mm_searching->font=constants.FONT_ANIK;
	root->append_child(mm_searching);


	mm_graphs->a=Point(500+padding*2,300);
	mm_graphs->b=Point(700+padding*2,500);
	mm_graphs->bg_color=BLACK;
	strcpy(mm_graphs->text,"GRAPHS  ");
	mm_graphs->bg_color=Point(20,20,20);
	mm_graphs->click = mm_graphs_click;
	mm_graphs->font=constants.FONT_ANIK;
	root->append_child(mm_graphs);

	mm_logic->a=Point(700+padding*3,300);
	mm_logic->b=Point(900+padding*3,500);
//	strcpy(".\\btn.jpg",mm_logic->img_path);
//	mm_logic->img_pos=Point(0,0);
	mm_logic->bg_color=Point(0,40,0);
	strcpy(mm_logic->text,"LOGIC CIRCUIT  ");
	mm_logic->font=constants.FONT_ANIK;
	mm_logic->click = mm_logic_click;
	root->append_child(mm_logic);

	mm_geometry->a=Point(100,100-padding);
	mm_geometry->b=Point(300,300-padding);
	mm_geometry->bg_color=Point(20,20,20);
	strcpy(mm_geometry->text,"GEOMETRY   ");
	mm_geometry->click = mm_geometry_click;
	mm_geometry->font=constants.FONT_ANIK;
	root->append_child(mm_geometry);

	mm_nt->a=Point(300+padding,100-padding);
	mm_nt->b=Point(500+padding,300-padding);
	mm_nt->bg_color=Point(20,20,20);
	mm_nt->font=constants.FONT_ANIK;
	strcpy(mm_nt->text,"NUMBER THEORY    ");
	mm_nt->click = mm_nt_click;
	root->append_child(mm_nt);

	mm_toh->a=Point(500+padding*2,100-padding);
	mm_toh->b=Point(700+padding*2,300-padding);
	mm_toh->bg_color=Point(20,20,20);
	strcpy(mm_toh->text,"TOWER OF HANOI    ");
	mm_toh->font=constants.FONT_ANIK;
	mm_toh->click = mm_toh_click;
	root->append_child(mm_toh);

	mm_circuit->a=Point(700+padding*3,100-padding);
	mm_circuit->b=Point(900+padding*3,300-padding);
	mm_circuit->bg_color=Point(40,0,0);
	strcpy(mm_circuit->text,"CIRCUIT SIMULATOR  ");
	mm_circuit->click = mm_circuit_click;
	mm_circuit->font=constants.FONT_ANIK;
	root->append_child(mm_circuit);
}

void back_click() {
	TerminateThread(update_handle,0);
	initialize_main();
}
/******************************** iGraphics functions ************************************/
int cnt=1;
void iDraw() {
	iClear();
	char path[100];
	sprintf(path,".\\bg_jpg\\%d.jpg", cnt);
	if(root->data[0] == 45) iShowBMP(0,0,path); // :3
	Draw(root);
	cnt++;
	if(cnt==500) cnt=1;
	//
	if(at_graphs)
 	{
		int conn[100][100];
		memset(conn, 0, sizeof conn);

		for(auto p: adj) {
			for(int x: p.second) {
				if(conn[p.first][x]) continue;
				conn[p.first][x]=1;
				conn[x][p.first]=1;

				Div *from = div_from_id[p.first];
				Div *to = div_from_id[x];
				Div *line = new Div();
				line->a=from->a;
				line->b=to->a;
				char wt[10];
				itoa(weight[p.first][x], wt, 10);

				iText((line->a.x+line->b.x)/2 + 10 , (line->a.y+line->b.y)/2, wt, constants.FONT_DEFAULT);

				line->color=Point(255,255,255);
				line->line_width=5;
				line->type = constants.DIV_LINE;
				if(connected[p.first][x]) {
					line->line_width=10;
					line->color=Point(0,255,0);
				}
				else {

				}
				line->draw();
			}
		}
		for(auto *child: graph_view->children) {
			Div *d = child;
			char W[100];
			itoa(dist[d->div_id], W, 10);
			iText(d->abs_o().x, d->abs_o().y+25,W,constants.FONT_DEFAULT);
		}
	}
	else if(at_logic) {
		for(auto p: Wires) {
			glLineWidth(5);
			iSetColor(255,0,0);
			iLine(p.first->a.x, p.first->a.y, p.second->a.x, p.second->a.y);
			glLineWidth(1);
		}
	}
}
/*
	function iMouseMove() is called when the user presses and drags the mouse.
	(mx, my) is the position where the mouse pointer is.
*/
void iMouseMove(int mx, int my) {
    drag.push_back(Point(mx,my));
    //place your codes here
}
/*
	function iMouse() is called when the user presses/releases the mouse.
	(mx, my) is the position where the mouse pointer is.
*/
void iMouse(int button, int state, int mx, int my)
{
	handle_click(root,mx,my,button,state);

    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
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
	handle_key(key);
    if(key == GLUT_KEY_END) {
        exit(0);
    }
    //place your codes for other keys here
}
int main() {
    initialize_main();
    iInitialize(1400, 720, ":)");
    return 0;
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

