#include "iGraphics.h"
#define ANI_BTN_CLICK 0

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
#define TRANSPARENT Point(-1,-1,-1)

/********************* Structures *********************/
struct constants {
	int ALIGN_CENTER_X = 1<<1;
	int ALIGN_CENTER_Y = 1<<2;
	int ALIGN_RIGHT	   = 1<<3;
	int ALIGN_LEFT     = 1<<4;
	int ALIGN_UP       = 1<<5;
	int ALIGN_DOWN     = 1<<6;
	
	int FONT_DEFAULT = GLUT_BITMAP_9_BY_15;
	int FONT_ANIK = GLUT_BITMAP_HELVETICA_18;
	
	int DIV_BOX = 1<<1;
	int DIV_LINE = 1<<2;
	int DIV_CIRCLE = 1<<3;
	int DIV_BUTTON = 1<<4;
	int DIV_INPUT = 1<<5;
	
	int DIV_ABSOLUTE = 0;
	int DIV_RELATIVE = 1;
	
	int GRADIENT_NONE = 0;
	int GRADIENT_UP = 1;
	int GRADIENT_DOWN = 2;
	int GRADIENT_RIGHT = 3;
	int GRADIENT_LEFT = 4;
	int GRADIENT_CENTER = 5;
	
} constants;


int DIV_ID = 0;
void dummy() {};

struct Point {
	double x,y,z;
	Point(double a,double b) {
		x=a,y=b;
		z=0;
	}
	Point(double a,double b,double c) {
		x=a,y=b,z=c;
	}
	Point() {
	}
	Point operator+(Point a) {
		return Point(x+a.x, y+a.y, z+a.z);
	}
	Point operator-(Point a) {
		return Point(x-a.x, y-a.y, z-a.z);
	}
	Point operator/(double v) {
		return Point(x/v,y/v,z/v);
	}
	Point operator*(double v) {
		return Point(x*v,y*v,z*v);
	}
	Point unit() {
		return Point(x,y,z)/abs();
	}
	double abs() {
		return sqrt(x*x+y*y+z*z);
	}
};
vector<Point> drag;

struct Theme {
	Point color;
	Point bg_color;
	Point text_color;
	int font = constants.FONT_DEFAULT;
	Theme() {
		color=Point(255,255,255);
		bg_color=Point(-1,0,0);
		text_color=Point(255,255,255);
		font=constants.FONT_ANIK;
	}
};
Theme *theme = new Theme();
int _ID = 0;

void iiText(double x,double y,char *str,void *font, double sz, int width) {
	if(sz == -1) {
		iText(x,y,str,font);
		return;
	}
	glRasterPos3d(x, y, 0);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glLineWidth(width);
    glScalef(sz/100,sz/100,100);
    for( char* p = str; *p; p++)
    {
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, *p);
    }
    glLineWidth(1);
    glPopMatrix();
}

struct Div {
	// extra data for a div
	int data[100];
	Point data_points[100];
	int div_id = -1; // no id in the beginning
	//
	int _id = -1;
	Point a,b,origin;
	// here a is bottom-left and b is top-right point in absolute positioning
	// in relative positioning, a is relative origin and b is width, height
	
	Div *parent; // this will be set by the parent of this div. 
	vector<Div*> children;
	
	// size and shape
	double R=0;
	int type = constants.DIV_BOX | constants.DIV_BUTTON;
	
	// text related
	char text[2000];
	int text_align = constants.ALIGN_CENTER_X | constants.ALIGN_CENTER_Y;
	int font = constants.FONT_DEFAULT;
	int font_size = -1.0;
	int font_width = 1;
	
	void (*dragged)(Div *d = NULL, Point from = Point(0,0), Point to = Point(0,0));
	void (*leave_focus)(Div *from = NULL, Div *to = NULL) = NULL;
	void (*do_focus)(Div *from = NULL, Div *to = NULL) = NULL;
	void (*key_pressed)(char key) = NULL;
	
	// image related
	char img_path[255];
	Point img_pos;
	
	// gradient
	int gradient_step = 2, gradient = constants.GRADIENT_NONE;
	
	// div properties
	Point color;
	Point text_color;
	Point bg_color;
	Point border_color;
	
	bool click_anim = 1;
	bool filled=0;
	bool hidden=0;
	int line_width=0;
	int positioning = constants.DIV_ABSOLUTE;
	Div() {
		color = theme->color;
		click = dummy; // just to avoid RTE
		dragged = dummy;
		right_click = dummy; // like me :)
		leave_focus = dummy;
		do_focus = dummy;
		key_pressed = dummy;
		
		a=Point(0,0);
		b=Point(0,0);
		bg_color=theme->bg_color;
		text_color=theme->text_color;
		font=theme->font;
		_id = _ID++;
		memset(data, -1, sizeof data);
		
		
		data[4] = 0;
		// small hacc
	}
	void clear() {
		children.clear();
		strcpy(img_path, "");
		strcpy(text,"");
	}
	Point abs_a() {
		if(type & constants.DIV_CIRCLE) {
			return abs_o() - Point(R,R);
		}
		if(positioning == constants.DIV_RELATIVE) return this->a + parent->a;
		return this->a;
	}
	Point abs_b() {
		if(type & constants.DIV_CIRCLE) {
			return this->abs_a() + Point(R,R) * 2;
		}
		if(positioning == constants.DIV_RELATIVE) return this->abs_a() + this->b;
		return this->b;
	}
	Point abs_o() {
		if(positioning == constants.DIV_RELATIVE) return this->a + parent->a;
		return this->a;
	}
	
	void (*click)(Point where = Point(0,0),Div *d = NULL) = NULL;
	void (*right_click)(Point where = Point(0,0),Div *d = NULL) = NULL;
	
	void animate(int type);
	void draw() {
		// image
		// background
		// foreground
		// gradient
		// text
		if(hidden) return;
		Point a = abs_a(), b = abs_b();
		
		if(type & constants.DIV_CIRCLE) {
			if(bg_color.x!=-1) {
				iSetColor(bg_color.x,bg_color.y,bg_color.z);
				iFilledCircle(abs_o().x,abs_o().y,R,100);
			}
			
			if(gradient) {
				Point cc = Point(0,0,0);
				for(int i=R;i>=0;i--) {
					cc=cc+Point(255.0/(R+ 1e-5),0,0); // avoid div by 0/
					iSetColor(cc.x,cc.y,cc.z);
					iFilledCircle(abs_o().x, abs_o().y, i,100);
				}
			}
			
			iSetColor(color.x,color.y,color.z);
			glLineWidth(line_width);
			if(filled) iFilledCircle(abs_o().x,abs_o().y,R,100);
			else iCircle(abs_o().x,abs_o().y,R,100);
			glLineWidth(1);

			iSetColor(text_color.x, text_color.y, text_color.z);
			iText(abs_o().x,abs_o().y,text,font);
		}
		else if(type & constants.DIV_LINE) {
            iSetColor(color.x,color.y,color.z);
            glLineWidth(line_width);
			iLine(a.x,a.y,b.x,b.y);
			glLineWidth(1);/*
			double slope=(b.y-a.y)/(b.x-a.x + 1e-5); // avoid division by 0
			double perpendicular_slope = -1.0/(slope + + 1e-5);

			if(abs(slope)<1 && b.x != a.x )
            {
                for(int j=0;j<line_width/2;j++) {
                    iLine(a.x,a.y+j,b.x,b.y+j);
                }
                for(int j=0;j<line_width/2;j++) {
                    iLine(a.x,a.y-j,b.x,b.y-j);
                }
            }
            else
            {
                for(int j=0;j<line_width/2;j++) {
                    iLine(a.x+j,a.y,b.x+j,b.y);
                }
                for(int j=0;j<line_width/2;j++) {
                    iLine(a.x-j,a.y,b.x-j,b.y);
                }
            }*/
        }
		else if(type & constants.DIV_BOX) {
			if(strlen(img_path) != 0) {
				iShowBMP2(100, 200, img_path, 0);
			}
			
			if(bg_color.x != -1) // transparent
			{
				iSetColor(bg_color.x,bg_color.y,bg_color.z);
				iFilledRectangle(a.x,a.y,b.x-a.x,b.y-a.y);
			}
			
			if(gradient == constants.GRADIENT_NONE) {
				// do nothing :)
			}
			else if(gradient == constants.GRADIENT_CENTER) {
				Point a1=a;
				Point b1=b;
				Point cc = Point(0,0,0);
				while(a1.x<=b1.x && a1.y<=b1.y) {
					iSetColor(cc.x,cc.y,cc.z);
					iFilledRectangle(a1.x,a1.y,b1.x-a1.x,b1.y-a1.y);
					cc = cc + color.unit() * max(500.0/(b1.x-a1.x + 1e-5),500.0/(b1.y-a1.y + 1e-5)) * gradient_step;
					b1 = b1 - Point(gradient_step,gradient_step);
					a1 = a1 + Point(gradient_step,gradient_step);
				}
			}
			else if(gradient == constants.GRADIENT_UP || gradient == constants.GRADIENT_DOWN) {
				Point a1=a;
				Point b1=b;
				Point cc = Point(0,0,0);
				for(int i=a1.y;i<=b1.y;i+=gradient_step) {
					cc = cc+color.unit() * (255.0/(b1.y-a1.y + 1e-5)) * gradient_step;
					iSetColor(cc.x,cc.y,cc.z);
					iFilledRectangle(a1.x,a1.y,b1.x-a1.x,b1.y-a1.y);
					if(gradient == constants.GRADIENT_UP)
					a1 = a1 + Point(0,gradient_step);
					else
					b1 = b1 + Point(0,-gradient_step);
				}	
			}
			else if(gradient == constants.GRADIENT_RIGHT || gradient == constants.GRADIENT_LEFT) {
				Point a1=a;
				Point b1=b;
				Point cc = Point(0,0,0);
				for(int i=a1.x;i<=b1.x;i+=gradient_step) {
					cc = cc+color.unit() * (255.0/(b1.x-a1.x + 1e-5)) * gradient_step;
					iSetColor(cc.x,cc.y,cc.z);
					iFilledRectangle(a1.x,a1.y,b1.x-a1.x,b1.y-a1.y);
					if(gradient == constants.GRADIENT_RIGHT)
					a1 = a1 + Point(gradient_step,0);
					else
					b1 = b1 + Point(-gradient_step,0);
				}	
			}
			
			// Line width = border inside the div
			// outside doesnt work properly
			
			if(color.x!=-1) {
				iSetColor(color.x,color.y,color.z);
				Point a1=a;
				Point b1=b;
				int j=0;
				for(int j=0;j<line_width;j++) {
					if(filled) iFilledRectangle(a1.x,a1.y,b1.x-a1.x,b1.y-a1.y);
					else iRectangle(a1.x,a1.y,b1.x-a1.x,b1.y-a1.y);
					b1 = b1 - Point(1,1);
					a1 = a1 + Point(1,1);
				}
			
				if(filled) iFilledRectangle(a.x,a.y,b.x-a.x,b.y-a.y);
				else iRectangle(a.x,a.y,b.x-a.x,b.y-a.y);
			}
			// be careful
			
			double fs = 9;
			if(font_size != -1) fs=font_size;
			
			iSetColor(text_color.x,text_color.y,text_color.z);
			double gap = (b.x - a.x - strlen(text)*fs)/2;
			if(gap < 0) gap = 0;
			
			double px,py;
			
		
			
			if(text_align & constants.ALIGN_CENTER_X)
			px =  a.x + gap;
			
			if(text_align & constants.ALIGN_CENTER_Y)
			py = (a.y+b.y)/2-fs/2;
			
			if(text_align & constants.ALIGN_LEFT)
			px = a.x;
			
			if(text_align & constants.ALIGN_RIGHT)
			px = b.x - strlen(text)*fs;

			if(text_align & constants.ALIGN_UP)
			py = b.y-15;
			
			if(text_align & constants.ALIGN_DOWN)
			py = a.y+5;
	
			int Px = px, Py = py;
			char temp[2000]="";
			int mod = (b.x - a.x)/fs;
			int at=0,i=0,sz=0,cnt=69;
			while(i < strlen(text) && cnt--) {
				int j = 0;
				char word[100];
				while(text[i + j]!=' ' && i+j<strlen(text)) word[j] = text[i+j], j++;
				word[j++] = ' ';
				word[j] = '\0';
				if(sz + j <= mod) {
					strcat(temp, word);
					i=i+j;
					sz += j;
				} else {
					iiText(px,py,temp,font,font_size,font_width);
					strcpy(temp, "");
					sz = 0;
					py-=20;
				}
			}
			if(cnt == -1) iiText(Px,Py,"Error",font,font_size,font_width);
			else iiText(px,py,temp,font,font_size,font_width);
			/*
			for(int i=0;i<strlen(text);i++) {
				temp[at++] = text[i];
				if(at%mod == 0) {
 					temp[at]='\0';
					iText(px,py,temp,font);
					strcpy(temp, "");
					at=0;
					py-=14;
				}
			}
			temp[at]='\0';
			iText(px,py,temp,font);
			*/
		}
		
	}
	void append_child(Div *d) {
		d->parent = this;
		children.push_back(d);
	}
	
	void on_leave_focus(Div *to) {
	//	if(type & constants.DIV_INPUT) {
			this->line_width -= 5;
	//	}
		leave_focus(this, to);
	}
	void on_do_focus(Div *from) {
	//	if(type & constants.DIV_INPUT) {
			this->line_width += 5;
			
	//	}
		do_focus(from, this);
	}
	int get_id();
	void on_click(Point where, int button,int state);
};
struct Animation {
	Div *from, *to;
	double duration, delta;
	int cnt;
	Animation(Div *a, Div *b,double T,double dT) {
		from=a;
		to=b;
		duration=T;
		delta=dT;
	}
	Animation() {
	}
};

Div *find_div_by_position(Div *node,int x,int y) {
	if(node->abs_a().x>x or x>node->abs_b().x or node->abs_a().y>y or y>node->abs_b().y) return;
	Div *next = NULL;
	for(Div *d: node->children) {
		if(d->abs_a().x<=x and x<=d->abs_b().x and d->abs_a().y<=y and y<=d->abs_b().y) next=d;
	}
	if(next == NULL) return node;
	return find_div_by_position(next,x,y);
}
Div *focus = new Div();

void Div::on_click(Point where, int button,int state) {
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if(this->_id != focus->_id) {
			focus->on_leave_focus(this);
			this->on_do_focus(focus);
			focus=this;
		}
		
		// check this code 
	}
	if(this->type & constants.DIV_BUTTON) {
		if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
			if(click_anim) animate(ANI_BTN_CLICK);
			click(where, this);
		}
		else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
			animate(ANI_BTN_CLICK);
			right_click(where, this);
		}
		else if(button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
			// handle drag
			if(drag.size() > 0) {
				Point start=*drag.begin();
				Point end=*drag.rbegin();
				Div *object = find_div_by_position(this, start.x, start.y);
				object->dragged(object, start, end);
			}
			drag.clear();
		}
	}
	if(type & constants.DIV_INPUT) {
		
	}
}

HANDLE anim_handle = NULL;
DWORD WINAPI animationThread(Animation *a) {
	// position update
	Point da=(a->to->a-a->from->a)/a->delta;
	Point db=(a->to->b-a->from->b)/a->delta;
	double dr=(a->to->R-a->from->R)/a->delta;
	// color update
	Point dc=(a->to->color-a->from->color)/a->delta;
	// update text
	strcpy(a->from->text,a->to->text);
	// now start update each delta time
	for(double t=0;t<=a->duration;t+=a->delta) {
		a->from->a = a->from->a + da;
		a->from->b = a->from->b + db;
		a->from->color = a->from->color + dc;
		a->from->R = a->from->R + dr;
		Sleep(a->delta);
	}
	a->from->filled=a->to->filled;
	//free(a->to);
}
void Div::animate(int type) {
	if(type == ANI_BTN_CLICK) {
		Div *to = new Div();
		*to = *this;
		
		to->filled=0;
		filled=1;
		
		Animation *anim=new Animation(this,to,100,100);
		CreateThread(NULL,0,animationThread, anim,0,NULL);
	}
}
/**************** Global variables ***************/
Div *root = new Div();

// :')
//map<int,Div*> div_from_id;
Div *div_from_id[100000];
/**************** DIV MAIN ***********************/
int Div::get_id() {
	if(div_id == -1) {
		div_id = DIV_ID++;
		div_from_id[div_id] = this;
	}
	return div_id;
}
	
/**************** Functions **********************/

void Draw(Div *parent) {
	if(!parent->hidden) parent->draw();
	for(Div *child: parent->children) Draw(child);
}
void handle_click(Div *node,int x,int y,int button,int state) {
	if(node->abs_a().x>x or x>node->abs_b().x or node->abs_a().y>y or y>node->abs_b().y) return;

	Div *next = NULL;
	for(Div *d: node->children) {
		if(d->abs_a().x<=x and x<=d->abs_b().x and d->abs_a().y<=y and y<=d->abs_b().y) next=d;
	}
	if(next == NULL) {
		node->on_click(Point(x,y),button,state);
		
	}
	else handle_click(next,x,y,button, state);
}

void handle_key(unsigned char key) {
	if(focus->type & constants.DIV_INPUT) {
		char s[1]={key};
		if((int)key == 8 && strlen(focus->text) > 0) focus->text[strlen(focus->text)-1] = '\0';
		else
		strcat(focus->text, s);
	}
	else {
		focus->key_pressed(key);
	}
}

void Unselect(Div *d) {
	d->color=theme->color;
	d->bg_color=theme->bg_color;
}
void Yellow(Div *d) {
	d->color=Point(255,255,0);
	d->bg_color=Point(255,255,0);
}
void White(Div *d) {
	d->color=WHITE;
	d->bg_color=WHITE;
}
void Pink(Div *d) {
	d->color=PINK;
	d->bg_color=PINK;
}
void Blue(Div *d) {
	d->color=Point(0,0,255);
	d->bg_color=Point(0,0,255);
}
void Red(Div *d) {
	d->color=Point(255,0,0);
	d->bg_color=Point(255,0,0);
}
void Green(Div *d) {
	d->color=Point(0,255,0);
	d->bg_color=Point(0,255,0);
}

void left_click(Point w, Div *d) {
	int val = atoi(d->parent->children[1]->text);
	if(val == d->parent->children[1]->data[0]) return;
	val--;
	char Val[10];
	itoa(val, Val, 10);
	strcpy(d->parent->children[1]->text, Val);
	d->parent->click(w,d->parent->children[1]);
}
void right_click(Point w, Div *d) {
	int val = atoi(d->parent->children[1]->text);
	if(val == d->parent->children[1]->data[1]) return;
	val++;
	char Val[10];
	itoa(val, Val, 10);
	strcpy(d->parent->children[1]->text, Val);
	d->parent->click(w,d->parent->children[1]);
}
struct sliderx {
	Div *mid = new Div();
	Div *right = new Div();
	Div *left = new Div();
	Div *slider = new Div();

	sliderx(int l,int r,int def) {
		left->positioning=constants.DIV_RELATIVE;
		left->a=Point(0,0);
		left->b=Point(20,20);
		left->click=left_click;
		strcpy(left->text,"<");
		mid->positioning=constants.DIV_RELATIVE;
		mid->a=Point(20,0);
		mid->b=Point(80,20);
		mid->bg_color=Point(255,255,255);
		mid->text_color=Point(0,0,0);
		mid->data[0]=l;
		mid->data[1]=r;
		
		char Def[10]; itoa(def, Def, 10);
		strcpy(mid->text, Def);
		
		right->positioning=constants.DIV_RELATIVE;
		right->a=Point(100,0);
		right->b=Point(20,20);
		right->click=right_click;
		strcpy(right->text,">");
		slider->append_child(left);
		slider->append_child(mid);
		slider->append_child(right);	
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
		bg->click_anim = 0;
		
        root->append_child(bg);

        baseBar = new Div();
        baseBar->positioning = constants.DIV_RELATIVE;
        baseBar->a = Point((sliderWidth - basebarWidth) / 2 , (sliderHeight - basebarHeight) / 2 + 14 );
        baseBar->b = Point(basebarWidth, basebarHeight );
        baseBar->color = DARK_GREY;
        baseBar->type|=constants.DIV_BUTTON;
        baseBar->click = handle_slider_click;
        baseBar->click_anim = 0;

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
        minValueBox->click_anim = 0;
        itoa(minValue, minValueBox->text, 10);
        minValueBox->filled = 1;

        bg->append_child(minValueBox);

        maxValueBox = new Div();
        maxValueBox->positioning = constants.DIV_RELATIVE;
        maxValueBox->a = Point(baseBar->a.x + baseBar->b.x - 50,basebarHeight);
        maxValueBox->b = Point(50,25);
        maxValueBox->color = DARK_GREY;
        maxValueBox->click_anim = 0;
        itoa(maxValue, maxValueBox->text, 10);
        maxValueBox->filled = 1;

        bg->append_child(maxValueBox);

        nowValueBox = new Div();
        nowValueBox->positioning = constants.DIV_RELATIVE;
        nowValueBox->a = Point(baseBar->a.x + (baseBar->b.x)/2 - 25,basebarHeight);
        nowValueBox->b = Point(50,25);
        nowValueBox->color = LIGHT_GREEN;
        nowValueBox->text_color = BLACK;
        nowValueBox->click_anim = 0;
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
        applyBtn->type|=constants.DIV_BUTTON;
        applyBtn->click_anim = 0;
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
        applyBtn->type|=constants.DIV_BUTTON;
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


#define RESISTANCE 101
#define WIRE 102
#define AMMETER 103
#define DC_SOURCE 104
#define VOLTMETER 105
#define CAPACITOR 106
#define INDUCTOR 107
#define LED 108

#define pb push_back
#define fo(i,n) for(int i=0;i<n;i++)
