
#pragma once
#include <string>
#include <windows.h>

using namespace std;
//razmeri na prozoreca v potrebitelski edinici
extern  double DEFAULT_XMIN;
extern  double DEFAULT_YMIN;
extern  double DEFAULT_XMAX;
extern  double DEFAULT_YMAX;

//snap modes
enum modes{RST=1,SNAP,CLR,FRN};


class Point{
public:
	Point operator *(double k)const;
	Point operator -(const Point &)const;
	Point operator +(const Point &)const;
	Point operator !()const;
	double operator *(const Point &b)const;
	double mod()const;
	Point();
	Point(double x1, double y1);
	double get_x() const;
	double get_y() const;
	void move(double dx, double dy);
private:
	double x;
	double y;
};
//line
class Line{
public:
   Line();
   Line(Point p1, Point p2);
   Point get_start() const;
   Point get_end() const;
   void move(double dx, double dy);
private:
   Point from;
   Point to;
};
//text
class Text{
public:
   Text();
   Text(Point s, double x);
   Text(Point s, const string& m);
   Point get_start() const;
   string get_text() const;
   void move(double dx, double dy);
private:
   Point start;
   string text;
};
//graphic console
class GraphicWindow {
public:
   GraphicWindow();
  
   void coord(double xmin, double ymin, double xmax, double ymax);
   void axes();
   void set_sizes();
   void clear();
   
 
   GraphicWindow& operator<<(Point p);
   GraphicWindow& operator<<(Line s); 
   GraphicWindow& operator<<(Text t); 
   
  
   GraphicWindow& operator<<(COLORREF t);//smqna cvqt
   GraphicWindow& operator<<(modes r);   //smqna rejim
   
  
   GraphicWindow& operator>>(string & s);       
   string get_string(const string& out_string); 
   int get_int(const string& out_string);		
   double get_double(const string& out_string); 
   Point get_mouse(const string& out_string);   
   Point get_point(const Point &strt); 
   
   //snap rejim 
   void setSnap(int snap);

   //open/close/refresh
   void open(HWND hwnd, HDC mainwin_hdc);
   void close();   void repaint();

   //smqtane na potrebitelskite koordinati ot pikselni
   double disp_to_user_x(int x) const;
   double disp_to_user_y(int y) const;

   //razmeri
   int _disp_xmax; 
   int _disp_ymax;

//private:
   int user_to_disp_x(double x) const;
   int user_to_disp_y(double y) const;
   void statusline_prompt(string s);
   void point(double x, double y);
   void line(double xfrom, double yfrom, double xto, double yto);
   void text(string t, double x, double y);

   double _user_xmin; // prozorec
   double _user_xmax;   double _user_ymin;   double _user_ymax;
   
   bool SNAPON,FRONT, ax_on;
  
   HWND own_hwnd;   HWND child_hwnd;   HWND edit_hwnd;

   //chetka moliv
   HBRUSH saved_brush;   HPEN saved_pen;

   //chetka za chartane
   HDC _hdc; 
   int _mlength; 
   int _elength; 

  
   HDC _bitmap_hdc; 
   HBITMAP _hbm, _hbm_saved;  int _bitmap_xmax;  int _bitmap_ymax; 
};

extern GraphicWindow cwin;
