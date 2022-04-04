//# include "iGraphics.h"
#include<windows.h>
#include<bits/stdc++.h>

using namespace std;
#include "qswift.h";


/********************************* Definitions ****************************************/

// General definitions

// General Definitions

#define pb push_back
#define mp make_pair
#define all(a) (a).begin(), (a).end()
#define rall(a) (a).rbegin(), (a).rend()
#define fo(i, n) for(int i=0;i<n;i++)
#define getline(s) getline(cin,s)
#define gap " "
#define br "\n"

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
#define LIGHT_GREY Point(224,224,224)
#define DARK_GREY Point(96,96,96)
#define LIGHT_GREEN Point(204,255,204)
#define DARK_GREEN Point(0,153,0)
#define LIGHT_CYAN Point(204,255,255)
#define LIGHT_RED Point(255,204,204)
#define CHOCOLATE Point(153,76,0)

// Circuit Element Definitions

#define RESISTANCE 101
#define WIRE 102
#define AMMETER 103
#define DC_SOURCE 104
#define VOLTMETER 105
#define CAPACITOR 106
#define INDUCTOR 107
#define LED 108

/****************************** Global Structures ******************************/

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

void (*toh_solve_pointer)(int,int,int,int);


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

int menuWidth = 280;
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

HANDLE toh_solve_handle, component_add_handle, rlc_component_add_handle ;

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

Div *menu = new Div();
Div *selected = new Div();

Div *menu_toh = new Div();

Div *breadboard = new Div();

Div *menu_resistance = new Div();
Div *menu_dc_source = new Div();

Div *menu_wire = new Div();
Div *menu_ammeter = new Div();

Div *menu_voltmeter = new Div();
Div *menu_led = new Div();

Div *menu_capacitor = new Div();
Div *menu_inductor = new Div();

Div *reset_btn = new Div();
Div *simulate_btn = new Div();
Div *success_btn = new Div();
Div *update_btn = new Div();
Div *delete_btn = new Div();

Div *discs[50];
Div *poles[3];
Div *base = new Div();
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
DWORD WINAPI toh_thread() {

    double k = 0.003;

    //Ammeters[0].setValue(-100.06754);

	Sleep(500);
	success_btn->a=Point(900,100);
	success_btn->b=Point(1100,150);
    Sleep(5000);

}
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
            Sleep(3000);

            //Ammeters[c].hide();

            break;
        }
		Sleep(100);
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

    fo(i,m)
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

void sayHi() {
	Div *hi = new Div();
	hi->a=Point(rand()%500, rand()%500);
	hi->b=Point(hi->a.x+100, hi->a.y+100);
	//hi->text="Hi";
	root->append_child(hi);
}
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
    printf(" %d Yes here\n\n",*slider_variable);
    slider_apply_function();
    printf(" %d Yes here\n\n",*slider_variable);
}

void update_component()
{
    if( nowSelectedComponentIndex != -1 )
    {
        int index = nowSelectedComponentIndex;
        int dval = slider_global.nowValue;
        double desiredValue = dval;
        Components[index].setValue(desiredValue);
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

void initialize(int width,int height, char title[]) {

	root->a=Point(0,0);
	root->b=Point(1400,720);

	setEverythingDC();

	//Ammeters[0].createAmmeterDiv(500,600,200,50);
	//Voltmeters[0].createVoltmeterDiv(100,600,200,50);
	//Batteries[0].createBatteryDiv(100,200,160,100);

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
    initialize(1400, 720, "DC Circuit Simulation");
    return 0;
}


