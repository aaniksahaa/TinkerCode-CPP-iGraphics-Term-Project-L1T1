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

void (*solve_pointer)(int,int);
int initial = 1;
int getOut = 0;
int MAXlineCount = 100;
int lineCount = 0;
int lineNum = 5;
int pointNum = 20;
int delay = 200;
int baseColor = 100;
int pointRadius = 7;

int Ax = 380, Ay = 200, Bx = 1080, By = 600; // A and B are bottom left and upper right points of the domain

vector<pt>a;
vector<pt>t;


/********************************* Thread Handles *********************************/
HANDLE solve_handle, update_handle;

/********************************* Elements ***************************************/

//Sidebar Menu
Div *menu = new Div();
Div *selected = new Div();

Div *menu_convexhull = new Div();
Div *menu_closestpair = new Div();

//solve Visual
Div *points[200];
Div *lines[200][200];
Div *borderCircles[200];
Div *borderOutCircles[200];
Div *bestline = new Div();
//Array Controls

Div *reset_btn = new Div();
Div *solve_btn = new Div();

Div *valueBox = new Div();

/******************************** Thread functions ********************************/
DWORD WINAPI solve_thread() {
	solve_pointer(0, pointNum);
}

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

void sayHi() {
	Div *hi = new Div();
	hi->a=Point(rand()%500, rand()%500);
	hi->b=Point(hi->a.x+100, hi->a.y+100);
	//hi->text="Hi";
	root->append_child(hi);
}
void reset_btn_click() {

    strcpy( valueBox->text, "Minimum Distance" );

    initial = 0;
    TerminateThread(solve_handle,0);
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
void solve_btn_click() {
    initial = 0;
    DWORD solveId;
	solve_handle = CreateThread(NULL, 0, solve_thread, NULL, 0, NULL);
}
void menu_convexhull_click() {


    slider_variable = &pointNum;

    printf("%d\n",pointNum);
    slider_apply_function = reset_btn_click;

    pointNum = 30;

    slider_global.updateSliderDiv(400,630,5,50,30);

    valueBox->a = Point(0,0);
    valueBox->b = Point(0,0);

//    if(!initial)
//        reset_btn_click();

    /*TerminateThread(solve_handle,0);
    bestline->a = Point(0,0);
    bestline->b = Point(0,0);*/

    reset_btn_click();

	selected->bg_color=Point(0,0,0);
	selected = menu_convexhull;
	selected->bg_color=Point(110,0,0);
	solve_pointer=convexhull_solve;
}
void menu_closestpair_click() {

    slider_variable = &pointNum;

    slider_apply_function = reset_btn_click;

    pointNum = 10;

    slider_global.updateSliderDiv(400,630,5,50,10);

    valueBox->a=Point(900,620);
	valueBox->b=Point(1250,700);

    initial = 0;
    /*TerminateThread(solve_handle,0);
    bestline->a = Point(0,0);
    bestline->b = Point(0,0);*/
    reset_btn_click();

	selected->bg_color=Point(0,0,0);
	selected = menu_closestpair;
	selected->bg_color=Point(110,0,0);
	solve_pointer=closestpair;
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

	//printf("Okay\n\n");

	menu_convexhull->a=Point(10,610);
	menu_convexhull->b=Point(300,710);
	strcpy( menu_convexhull->text,"Convex Hull");
	menu_convexhull->click = menu_convexhull_click;
	root->append_child(menu_convexhull);


	menu_closestpair->a=Point(10,510);
	menu_closestpair->b=Point(300,610);
	strcpy( menu_closestpair->text,"Closest Pair of Points");
	menu_closestpair->click = menu_closestpair_click;
	root->append_child(menu_closestpair);

	reset_btn->a=Point(400,100);
	reset_btn->b=Point(500,150);
	strcpy( reset_btn->text,"Reset");
	reset_btn->click=reset_btn_click;
	root->append_child(reset_btn);

	solve_btn->a=Point(550,100);
	solve_btn->b=Point(650,150);
	strcpy( solve_btn->text,"Solve");
	solve_btn->click=solve_btn_click;
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
    bestline->color = YELLOW;
    bestline->line_width = 6;
    root->append_child(bestline);

    slider_global.createSliderDiv(400,630,1,162,162);

    printf("Okay \n");

    // select convexhull solve in the beginning
	menu_convexhull_click();

	//update_handle = CreateThread(NULL, 0, update_thread, NULL, 0, NULL);
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

