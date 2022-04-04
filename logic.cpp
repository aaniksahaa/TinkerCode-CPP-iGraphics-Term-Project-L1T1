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

// Logic Circuit Elements

#define ANDGATE 201
#define ORGATE 202
#define NOTGATE 203
#define NORGATE 204
#define NANDGATE 205
#define XORGATE 206
#define XNORGATE 207
#define BUFFERGATE 208


/****************************** Global Typedef ******************************/

// General typedef

typedef long double ld;
typedef pair<int, int> pii;

/********************************* Globals ****************************************/



int dummyVar;

// General Constants

const int inf = 1e18;
const int MAXN = 2e5+5;
const int mod = 1e9+7;
const ld pi = atan2(0, -1);
const ld eps = 1e-4;

const double EPS = 1e-9;
const int INF = 1e7; // it doesn't actually have to be infinity or a big number


// DC Circuit Design Globals

int menuWidth = 280;
int gateBtnWidth = menuWidth/2 ;

int lbX1 = 30;
int lbY = 30;
int lbWidth = 1050;
int lbHeight = 560;
int lbOffset = 20;



int wireLogicCount = 0;

int gateCount = 0;

int nowAddingLogic;
int nowSelectedgateIndex = -1;


/******************************* Logic Circuit Globals *********************************/


int gateSize = 50;
int terminalRadius = 5;

int switchSize = 30;
int switchHeight = 550;
int switchX1 = 70;
int switchOffset = 35;

int inputLines = 2;

int terminalSize = 30;

Point nowPoint = Point(0,0);



/********************************* Function Prototypes ****************************************/

void handle_gate_click(Point where, Div *d);



/********************************* Thread Handles *********************************/

HANDLE  gate_add_handle, wire_logic_add_handle, table_handle;


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
        valueBox->data[0] = id;
        itoa(value,valueBox->text,10);
        valueBox->click = handle_toggle;
        root->append_child(valueBox);

        line = new Div();
        line->type = constants.DIV_LINE;
        line->a = Point(ax,ay-switchSize/2);
        line->b = Point(ax,30);
        line->line_width = 5;
        root->append_child(line);

        terminal = new Div();
        terminal->type |= constants.DIV_CIRCLE;
        terminal->a = Point(ax,ay-30);
        terminal->R = terminalRadius;
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


    void createGateDiv(int type, int ax, int ay, int id)
    {
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
            //inC1->positioning = constants.DIV_RELATIVE;
            inC1->type |= constants.DIV_CIRCLE;
            inC1->a = inT1->b;
            inC1->R = terminalRadius;
            inC1->filled = 1;
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
            outC->data[0] = id;
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
        printf("Done");
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

Div *menu = new Div();
Div *selected = new Div();


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

Div *table_btn_logic = new Div();

Div *delete_logic_btn = new Div();

// Logic gates


gateDiv Gates[500];
 Div *wires[500];


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

DWORD WINAPI wire_logic_add_thread() {
    for(int i=0;i<5000;i++)
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

            cout<<"\no1 = "<<o1<<"  o2 = \n"<<o2;

            if(o1 == 1 || o2 == 1)
                break;

            if(t1 == 0)
            {
                if(t2==-1)
                {
                    wires[c] = new Div();
                    wires[c]->type = constants.DIV_LINE;
                    wires[c]->line_width = 5;
                    wires[c]->a = Point(ax,by);
                    wires[c]->b = Point(bx,by);
                    root->append_child(wires[c]);

                    selectedTerminals[1]->data[3] = 1; //occupy

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
                    wires[c]->b = Point(ax,ay);
                    root->append_child(wires[c]);

                    selectedTerminals[0]->data[3] = 1;

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
                    root->append_child(wires[c]);

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
                    wires[c]->b = selectedTerminals[1]->a;
                    root->append_child(wires[c]);

                    selectedTerminals[0]->data[3] = 1;
                    selectedTerminals[1]->data[3] = 1;

                    Gates[id2].childrenId[n2] = id1;
                }
            }

            else
            {
                break;
            }

            wireLogicCount++;
            break;

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

void solve_logic_circuit_btn_click();

void solve_logic_circuit()
{


}

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
    for(int i=0;i<(1<<inputLines);i++)
    {
        int mask = 1;
        for(int j=0;j<inputLines;j++)
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

void table_logic_circuit_btn_click()
{
    table_handle = CreateThread(NULL, 0, table_thread, NULL, 0, NULL);
}

/******************************** Button Controls *********************************/

void setEverythingLogic();

void handle_board_click(Point where)
{
    nowPoint.x = where.x;
    nowPoint.y = where.y;
}

void reset_btn_logic_click() {

    TerminateThread(gate_add_handle,0);
    TerminateThread(wire_logic_add_handle,0);

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
    TerminateThread(wire_logic_add_handle,0);
    nowAddingLogic = WIRE;
	wire_logic_add_handle = CreateThread(NULL, 0, wire_logic_add_thread, NULL, 0, NULL);
}

void menu_nandgate_click()
{
    DWORD toh_solveId;
    TerminateThread(gate_add_handle,0);
    TerminateThread(wire_logic_add_handle,0);
    nowAddingLogic = NANDGATE;
    nowPoint.x = 0;
    nowPoint.y = 0;
	gate_add_handle = CreateThread(NULL, 0, gate_add_thread, NULL, 0, NULL);
}

void menu_norgate_click()
{
    DWORD toh_solveId;
    TerminateThread(gate_add_handle,0);
    TerminateThread(wire_logic_add_handle,0);
    nowAddingLogic = NORGATE;
    nowPoint.x = 0;
    nowPoint.y = 0;
	gate_add_handle = CreateThread(NULL, 0, gate_add_thread, NULL, 0, NULL);
}

void menu_notgate_click()
{
    DWORD toh_solveId;
    TerminateThread(gate_add_handle,0);
    TerminateThread(wire_logic_add_handle,0);
    nowAddingLogic = NOTGATE;
    nowPoint.x = 0;
    nowPoint.y = 0;
	gate_add_handle = CreateThread(NULL, 0, gate_add_thread, NULL, 0, NULL);
}

void menu_andgate_click()
{
    DWORD toh_solveId;
    TerminateThread(gate_add_handle,0);
    TerminateThread(wire_logic_add_handle,0);
    nowAddingLogic = ANDGATE;
    nowPoint.x = 0;
    nowPoint.y = 0;
	gate_add_handle = CreateThread(NULL, 0, gate_add_thread, NULL, 0, NULL);
}

void menu_orgate_click()
{
    DWORD toh_solveId;
    TerminateThread(gate_add_handle,0);
    TerminateThread(wire_logic_add_handle,0);
    nowAddingLogic = ORGATE;
    nowPoint.x = 0;
    nowPoint.y = 0;
	gate_add_handle = CreateThread(NULL, 0, gate_add_thread, NULL, 0, NULL);
}

void menu_xorgate_click()
{
    DWORD toh_solveId;
    TerminateThread(gate_add_handle,0);
    TerminateThread(wire_logic_add_handle,0);
    nowAddingLogic = XORGATE;
    nowPoint.x = 0;
    nowPoint.y = 0;
	gate_add_handle = CreateThread(NULL, 0, gate_add_thread, NULL, 0, NULL);
}

void delete_logic_btn_click()
{

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
	root->append_child(logic_board);
	logic_board->click = handle_board_click;



	menu_wire_logic->a=Point(menuX1,610-1*gateBtnWidth);
	menu_wire_logic->b=Point(menuX2,610-0*menuWidth);
	strcpy( menu_wire_logic->text,"Wire");
	menu_wire_logic->click = menu_wire_logic_click;
	root->append_child(menu_wire_logic);

	menu_andgate->a=Point(menuX2,610-1*gateBtnWidth);
	menu_andgate->b=Point(1390,610-0*gateBtnWidth);
	strcpy( menu_andgate->text,"AND Gate");
	menu_andgate->click = menu_andgate_click;
	root->append_child(menu_andgate);

	menu_orgate->a=Point(menuX1,610-2*gateBtnWidth);
	menu_orgate->b=Point(menuX2,610-1*gateBtnWidth);
	strcpy( menu_orgate->text,"OR Gate");
	menu_orgate->click = menu_orgate_click;
	root->append_child(menu_orgate);

	menu_nandgate->a=Point(menuX2,610-2*gateBtnWidth);
	menu_nandgate->b=Point(1390,610-1*gateBtnWidth);
	strcpy(menu_nandgate->text,"NAND Gate");
	menu_nandgate->click = menu_nandgate_click;
	root->append_child(menu_nandgate);

	menu_norgate->a=Point(menuX1,610-3*gateBtnWidth);
	menu_norgate->b=Point(menuX2,610-2*gateBtnWidth);
	strcpy( menu_norgate->text,"NOR Gate");
	menu_norgate->click = menu_norgate_click;
	root->append_child(menu_norgate);

	menu_led->a=Point(menuX2,610-3*gateBtnWidth);
	menu_led->b=Point(1390,610-2*gateBtnWidth);
	strcpy( menu_led->text,"LED");
	//menu_wire_logic->click = menu_wire_logic;
	root->append_child(menu_led);

	menu_xorgate->a=Point(menuX1,610-4*gateBtnWidth);
	menu_xorgate->b=Point(menuX2,610-3*gateBtnWidth);
	strcpy( menu_xorgate->text,"XOR Gate");
	menu_xorgate->click = menu_xorgate_click;
	root->append_child(menu_xorgate);

	menu_notgate->a=Point(menuX2,610-4*gateBtnWidth);
	menu_notgate->b=Point(1390,610-3*gateBtnWidth);
	strcpy( menu_notgate->text,"NOT Gate");
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
	strcpy( delete_logic_btn->text,"Delete");
	delete_logic_btn->click = delete_logic_btn_click;
	//root->append_child(delete_logic_btn);

	for(int i=0;i<inputLines;i++)
    {
        constLines[i].createConstLine(switchX1+switchOffset*i,switchHeight,i);
    }

}

void initialize(int width,int height, char title[]) {

	root->a=Point(0,0);
	root->b=Point(1400,720);

	setEverythingLogic();

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
    initialize(1400, 720, "Logic Circuit Simulation");
    return 0;
}



