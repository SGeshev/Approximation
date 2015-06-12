
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include "msw.h"

GraphicWindow cwin;

extern int win_main();

COLORREF mainBk=RGB(150,150,150);
int snapfract=1;
/*-------------------------------------------------------------------------*/

#define b1 111
#define b2 222
#define b3 333
#define ID_BUTTON 1
#define ID_TEXTBOX 2
#define ID_TEXTBOXX 66
#define NUM 1000 
#define ID_COMBO 3
#define listBoxx 5
#define ID_APROX 6
static HWND hwndF;
static HWND xField;
static HWND yField;
static HWND listBox;

void showCoord2(HWND hwnd, LPARAM lParam){
	char mouf[20];
	char spac[]="                    ";
	RECT rect; 
	HDC hdc;
	sprintf(mouf,"%i   %i", LOWORD (lParam), HIWORD (lParam));
    hdc = GetDC (hwnd);		  
	GetClientRect (hwnd, &rect);
		  
	DrawText (hdc, spac, -1, &rect, DT_SINGLELINE | DT_BOTTOM | DT_RIGHT);		  
	DrawText (hdc, mouf, -1, &rect, DT_SINGLELINE | DT_BOTTOM | DT_RIGHT);
          
	ReleaseDC (hwnd, hdc);
}




void showCoord(HWND hwnd, LPARAM lParam){
	char mouf[20];
	char spac[]="                    ";
	RECT rect; 
	HDC hdc;
		  
	int displX = cwin.disp_to_user_x((double)LOWORD (lParam));
	int displY = cwin.disp_to_user_y((double)HIWORD (lParam));

	

	sprintf(mouf,"%i   %i", displX, displY);
    	  
	hdc = GetDC (hwnd);
		  
	GetClientRect (hwnd, &rect);
		  
	DrawText (hdc, spac, -1, &rect, DT_SINGLELINE | DT_BOTTOM | DT_LEFT);		  
	DrawText (hdc, mouf, -1, &rect, DT_SINGLELINE | DT_BOTTOM | DT_LEFT);
          
	ReleaseDC (hwnd, hdc);
}

void drawMarker(HWND hwnd,INT x, INT y){     // kvadrat okolo tochkata
	x= x;
	y=y;   
	HDC 	hdc=GetDC(hwnd);
   	
	Rectangle(hdc,x-2,y-2,x+3,y+3);
	ReleaseDC (hwnd, hdc) ;
}

struct ApproxPoint {
	ApproxPoint(int x, int y) {
		this->x = x;
		this->y = y;
	}
	
	int x;
	int y;
};

vector<ApproxPoint> approxPoints; // struktura sydyrjashta tochkite za aproximirane
vector<int> xPositions;
vector<int> yPositions;
vector<int> xPositionsMultyPositions;
vector<int> xPositionsSquared;

int IntVectorSum(vector<int> vector) {
	int res = 0;
	
	for(std::vector<int>::iterator j=vector.begin(); j!=vector.end(); ++j)
    	res += *j;
    	
    return res;
}

long __stdcall win_proc(HWND hwnd, UINT message, UINT wParam, LONG lParam){  
   static int paint_flag = 1;   
   PAINTSTRUCT ps; 
   HDC mainwin_hdc;
   
	static int  cxClient, cyClient ;
	HDC hdc;
	HDC hdcx;    
	HDC hdcy;
	static POINT p;
	RECT rect;
             
	  switch (message)  { 
	case WM_SIZE:{ 
		InvalidateRect (hwnd, NULL, TRUE) ;
		cxClient = LOWORD (lParam) ;
		cyClient = HIWORD (lParam) ;
		return 0 ;
		}
	  case WM_PAINT:
		  if(paint_flag){
			paint_flag = false;
			mainwin_hdc = BeginPaint(hwnd, &ps);
            cwin.open(hwnd, mainwin_hdc);
 			win_main();
			EndPaint(hwnd, &ps);
		  }
		  else{
		   cwin.set_sizes();
		   cwin.repaint();
		  }
		  break;
		  case WM_CREATE:{  
           //    hwndF =  CreateWindow(TEXT("STATIC"), TEXT(""),
            //   WS_VISIBLE | WS_CHILD | WS_BORDER | WS_OVERLAPPED,
            //   10, 50, 500, 500,
            //   hwnd, (HMENU) NULL, NULL, NULL);
                
             
                
                
               xField = CreateWindow(TEXT("EDIT"), TEXT("X"),
               WS_VISIBLE |WS_CHILD | WS_BORDER | WS_OVERLAPPED,
               10, 10, 50, 20,
               hwnd, (HMENU) ID_TEXTBOX, NULL, NULL);
               
               
               
               yField = CreateWindow(TEXT("EDIT"), TEXT("Y"),
               WS_VISIBLE |WS_CHILD | WS_BORDER | WS_OVERLAPPED,
               70, 10, 50, 20,
               hwnd, (HMENU) ID_TEXTBOXX, NULL, NULL);
                
                
                
               CreateWindow(TEXT("BUTTON"), TEXT("Pixel"),
               WS_VISIBLE | WS_CHILD | WS_OVERLAPPED,
               160, 10, 80, 20,
               hwnd, (HMENU) ID_BUTTON, NULL, NULL);
               
                CreateWindow(TEXT("BUTTON"), TEXT("Y=X^2"),
               WS_VISIBLE | WS_CHILD | WS_OVERLAPPED,
               300, 10, 60, 20,
               hwnd, (HMENU) b1, NULL, NULL);
               
                CreateWindow(TEXT("BUTTON"), TEXT("Y=X"),
               WS_VISIBLE | WS_CHILD | WS_OVERLAPPED,
               370, 10, 60, 20,
               hwnd, (HMENU) b2, NULL, NULL);
               
                CreateWindow(TEXT("BUTTON"), TEXT("Y=-X"),
               WS_VISIBLE | WS_CHILD | WS_OVERLAPPED,
               440, 10, 60, 20,
               hwnd, (HMENU) b3, NULL, NULL);
               
               
               CreateWindow(TEXT("BUTTON"), TEXT("Aproxxx"),
               WS_VISIBLE | WS_CHILD | WS_OVERLAPPED,
               440, 40, 60, 20,
               hwnd, (HMENU) ID_APROX, NULL, NULL);
               
       
                        
         /*  listBox = CreateWindow(TEXT("LISTBOX"), NULL,
               WS_VISIBLE | WS_CHILD | WS_OVERLAPPED | LBS_NOTIFY | WS_VSCROLL | WS_BORDER | LBS_NOTIFY, 
               
               300, 10, 80, 20, 
               hwnd, (HMENU) listBoxx, NULL, NULL);
               SendMessage(listBox, LB_ADDSTRING, 0, (LPARAM)"Y = X^2");
                SendMessage(listBox, LB_ADDSTRING, 1, (LPARAM)"Y = X");
                SendMessage(listBox, LB_ADDSTRING, 2, (LPARAM)"Y = -X"); */
                
                
             break;}

						 case WM_COMMAND:
	{
	  
		if(LOWORD(wParam) == b1)
		{
		
			hdc = GetDC(hwnd);
			BeginPaint(hwnd, &ps);
			
			for(int i=-cxClient; i<cxClient;i++)
			
			{
				SetMapMode(hdc,MM_LOENGLISH);               //koordinatna sistema
				SetViewportOrgEx(hdc,cxClient/2,cyClient/2,NULL); 
				SetPixel(hdc,i, i*i, 000000000);
			}
			EndPaint(hwnd, &ps);
			
			ReleaseDC(hwnd, hdc); 
			UpdateWindow(hwnd);
		               
		}      
	                       
	                       
		if(LOWORD(wParam) == b2)
		{
			
			hdc = GetDC(hwnd);
			BeginPaint(hwnd, &ps);
			
			for(int i=-cxClient;i<cxClient;i++)
			
			{
				SetMapMode(hdc,MM_LOENGLISH);               //koordinatna sistema
				SetViewportOrgEx(hdc,cxClient/2,cyClient/2,NULL); 
				SetPixel(hdc,i, i, 000000000);
			}
			EndPaint(hwnd, &ps);
			
			ReleaseDC(hwnd, hdc); 
			UpdateWindow(hwnd);
		
		}    
	                       
		if(LOWORD(wParam) == b3)
		{
		
			hdc = GetDC(hwnd);
			BeginPaint(hwnd, &ps);
			
			for(int i=-cxClient;i<cxClient;i++)
			
			{
			SetMapMode(hdc,MM_LOENGLISH);               //koordinatna sistema
				SetViewportOrgEx(hdc,cxClient/2,cyClient/2,NULL); 
				SetPixel(hdc,i, -i, 000000000);
			}
			EndPaint(hwnd, &ps);
			
			ReleaseDC(hwnd, hdc); 
			UpdateWindow(hwnd);
		
		}          
	             
	             
	             
	       
	
	     
	     
	/*	if(LOWORD(wParam) == ID_BUTTON)
		{  
		
			TCHAR a[6];
			int valueX = GetWindowText(xField, a, 6);
			
			TCHAR  b[6];
			int valueY = GetWindowText(yField, b, 6);
			
			
			int xx = atoi(a); 
			int yy = atoi(b);
			
			ApproxPoint appPoint(xx, yy);
			approxPoints.push_back(appPoint);
			
			xPositions.push_back(xx);
			yPositions.push_back(yy);
			xPositionsMultyPositions.push_back(xx * yy);
			xPositionsSquared.push_back(xx * xx);
			
			
			hdc = GetDC(hwnd);
			
			
			SetMapMode(hdc,MM_LOENGLISH);
			SetViewportOrgEx(hdc,cxClient/2,cyClient/2,NULL);      
			
			
			
			SetPixel(hdc, xx, yy, 000000000); //ako e sys SetPixel(hdcx, 100, 120,000000000) primerno (s to4no koordinati) stava  , a taka ne iska   
			
			ReleaseDC(hwnd, hdc); 
			UpdateWindow(hwnd);
		
		
		
		}*/
     
     	if(LOWORD(wParam) == ID_APROX) 
		{
			int sumX = IntVectorSum(xPositions);
			int sumY = IntVectorSum(yPositions);
			int sumXMultY = IntVectorSum(xPositionsMultyPositions);
			int sumXX = IntVectorSum(xPositionsSquared);			
			
			int n = xPositions.size();
			
			double ch = sumXMultY - ((sumX * sumY) / n);
			double zn = sumXX - ((sumX * sumX) / n);
			
			double m = ch / zn;
			double _x = sumX / n;
			double _y = sumY / n;
			double b = _y - (m * _x); 
			
			
			// funkciyata e  y = m * x + b
			hdcy = GetDC(hwnd);
			BeginPaint(hwnd, &ps);
			
			for(int i= -cxClient;i<cxClient;i++)
			
			{
				SetMapMode(hdc, MM_LOENGLISH);               //koordinatna sistema
				SetViewportOrgEx(hdc,cxClient/2,cyClient/2,NULL); 
				SetPixel(hdcy, i, m * i + b, 000000000);
			}
			EndPaint(hwnd, &ps);
			
			ReleaseDC(hwnd, hdcy); 
			UpdateWindow(hwnd);
			
     	}
     
    	break;
	} 

	 case WM_MOUSEMOVE:{ //при движение на мишката
    		  showCoord(hwnd,lParam);showCoord2(hwnd,lParam);
              return 0 ;
              }
	
     
      case WM_LBUTTONDOWN:
          
          {  
        
  
         hdc = GetDC(hwnd);  
     // SetMapMode(hdc,MM_LOENGLISH);               //koordinatna sistema
     // SetViewportOrgEx(hdc,cxClient/2,cyClient/2,NULL);
        
        p.x = LOWORD (lParam) ;
       p.y = HIWORD (lParam) ;
       
          //SetMapMode(hdc, MM_ISOTROPIC);
        //  SetViewportOrgEx(hdc, cxClient/2, cyClient/2, NULL);
          SetPixel(hdc,LOWORD (lParam),HIWORD (lParam),000000000); 
          
           int xx = p.x;
           int yy = p.y;
           
           drawMarker(hwnd,p.x,p.y);
           
             ApproxPoint appPoint(xx, yy);
			approxPoints.push_back(appPoint);
		 
			xPositions.push_back(xx);
			yPositions.push_back(yy);
			xPositionsMultyPositions.push_back(xx * yy);
			xPositionsSquared.push_back(xx * xx);
          ReleaseDC(hwnd, hdc); 
                         break;} 
            
         
      case WM_DESTROY:
		 KillTimer (hwnd, 1) ;  cwin.close(); exit(0); 
   }
   return DefWindowProc(hwnd, message, wParam, lParam);
}
/*-------------------------------------------------------------------------*/
int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
   LPSTR, int nShowCmd){  
   MSG msg; WNDCLASS wndclass;

   if (!hPrevInstance)
   {  
      wndclass.style = CS_HREDRAW | CS_VREDRAW;
      wndclass.lpfnWndProc = win_proc;
      wndclass.cbClsExtra = 0;
      wndclass.cbWndExtra = 0;
      wndclass.hInstance = hInstance;
      wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
      wndclass.hCursor = LoadCursor (NULL, IDC_ARROW);
      wndclass.hbrBackground = CreateSolidBrush(mainBk);
      wndclass.lpszMenuName = NULL;
      wndclass.lpszClassName = "XXX";

      RegisterClass (&wndclass);
   }

   char title[200];
   GetModuleFileName(hInstance, title, sizeof(title));
/**/
   HWND hwnd = CreateWindow("XXX",title,
      WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      GetSystemMetrics(SM_CYFULLSCREEN) * 3 / 4,
      GetSystemMetrics(SM_CYFULLSCREEN) * 3 / 4,
      NULL,
      NULL,
      hInstance,
      0);

   ShowWindow(hwnd, nShowCmd);
   UpdateWindow(hwnd);

   while (GetMessage(&msg, NULL, 0, 0))   {  
      TranslateMessage(&msg);
      DispatchMessage(&msg);
   }
   return msg.wParam;
}
/*-------------------------------------------------------------------------*/
GraphicWindow::GraphicWindow():
   _user_xmin(DEFAULT_XMIN),
   _user_ymin(DEFAULT_YMIN),
   _user_xmax(DEFAULT_XMAX),
   _user_ymax(DEFAULT_YMAX),
   SNAPON(false),FRONT(false),ax_on(false)
{}

void GraphicWindow::open(HWND hwnd, HDC mainwin_hdc)
{HINSTANCE hInstance = 
     (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);
 child_hwnd = CreateWindow(
	 "static",	 NULL,
	 WS_CHILD | WS_VISIBLE,
      0,  0,  /* UL corner*/
      0,  0, /* sizes */
	  hwnd, (HMENU)84,  /* child ID */
	  hInstance, 0);

 edit_hwnd  = CreateWindow(
	 "edit",  NULL,
      WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL, /* style */
      0, 0, 0, 40, 
      hwnd, (HMENU)85,  /* child ID */
      hInstance, 0);
	own_hwnd=hwnd;  RECT rect;
   GetClientRect(hwnd, &rect);
   _disp_xmax = rect.right - 1;
   _disp_ymax = rect.bottom - 1;

   _hdc = mainwin_hdc;
   HBRUSH brush = CreateSolidBrush(mainBk);
   SelectObject(_hdc,brush);
   SelectObject(_bitmap_hdc,brush);
   DeleteObject(brush);
   SelectObject(_hdc, GetStockObject(BLACK_PEN));
   SelectObject(_bitmap_hdc, GetStockObject(BLACK_PEN));
   SelectObject(_hdc, GetStockObject(SYSTEM_FONT));
   SelectObject(_bitmap_hdc, GetStockObject(SYSTEM_FONT));
   _bitmap_xmax = GetSystemMetrics(SM_CXFULLSCREEN);
   _bitmap_ymax = GetSystemMetrics(SM_CYFULLSCREEN);
   HDC hdc = GetDC(hwnd);
   _bitmap_hdc = CreateCompatibleDC(hdc);
   _hbm = CreateCompatibleBitmap(hdc, _bitmap_xmax, _bitmap_ymax);
   ReleaseDC(hwnd, hdc);
   if (!_hbm)
   {  
      MessageBox(hwnd, (LPSTR)"Memory problems in Bitmap Creation",
         (LPSTR)"Error", MB_ICONHAND);
         SendMessage(hwnd, WM_DESTROY, 0, 0L);
         return;
   }
   _hbm_saved = (HBITMAP)SelectObject(_bitmap_hdc, _hbm);
   clear();
}
void GraphicWindow::set_sizes(){
	if(!ax_on){
	RECT rect;  GetClientRect(own_hwnd, &rect);
	_disp_xmax = rect.right - 1; _disp_ymax = rect.bottom - 1;
	}
}
void GraphicWindow::axes(){
	*this<<Line(Point(_user_xmin,0),Point(_user_xmax,0));
	*this<<Line(Point(0,_user_ymin),Point(0,_user_ymax));
	*this<<Text(Point(_user_xmax-1,1.5),"X");
	*this<<Text(Point(-1.5,_user_ymin-2),"Y");
	for(double x=_user_xmin;x<_user_xmax;x+=1)*this<<Line(Point(x,0),Point(x,0.15));
	for(double y=_user_ymin;y>_user_ymax ;y-=1)*this<<Line(Point(0,y),Point(-0.15,y));
	ax_on=true;
}
void GraphicWindow::clear(){  
   COLORREF color = mainBk;
   HBRUSH brush = CreateSolidBrush(color);
   HBRUSH saved_brush = (HBRUSH)SelectObject(_hdc, brush);
   PatBlt(_hdc, 0, 0, _disp_xmax, _disp_ymax, PATCOPY);
   SelectObject(_hdc, saved_brush);
   saved_brush = (HBRUSH)SelectObject(_bitmap_hdc, brush);
   PatBlt(_bitmap_hdc, 0, 0, _bitmap_xmax, _bitmap_ymax, PATCOPY);
   SelectObject(_bitmap_hdc, saved_brush);
   DeleteObject(brush);
}

void GraphicWindow::coord(double xmin, double ymin,
   double xmax, double ymax){  
   _user_xmin = xmin;   _user_xmax = xmax;
   _user_ymin = ymin;   _user_ymax = ymax;
}

int GraphicWindow::user_to_disp_x(double x) const{  
   return (int) ((x - _user_xmin) * _disp_xmax / (_user_xmax - _user_xmin));
}

int GraphicWindow::user_to_disp_y(double y) const{  
   return (int) ((y - _user_ymin) * _disp_ymax / (_user_ymax - _user_ymin));
}

double GraphicWindow::disp_to_user_x(int x) const{  
   return (double)x * (_user_xmax - _user_xmin) / _disp_xmax + _user_xmin;
}

double GraphicWindow::disp_to_user_y(int y) const
{  
   return (double)y * (_user_ymax - _user_ymin) / _disp_ymax + _user_ymin;
}

void GraphicWindow::point(double x, double y){  
   const int POINT_RADIUS = 3;
   int disp_x = user_to_disp_x(x);
   int disp_y = user_to_disp_y(y);
   Ellipse(_hdc, disp_x - POINT_RADIUS, disp_y - POINT_RADIUS,
      disp_x + POINT_RADIUS, disp_y + POINT_RADIUS);
   Ellipse(_bitmap_hdc, disp_x - POINT_RADIUS, disp_y - POINT_RADIUS,
      disp_x + POINT_RADIUS, disp_y + POINT_RADIUS);
}

void GraphicWindow::line(double xfrom, double yfrom, 
						 double xto,   double yto){
   MoveToEx(_hdc, user_to_disp_x(xfrom), user_to_disp_y(yfrom), 0);
   LineTo(_hdc,user_to_disp_x(xto), user_to_disp_y(yto));
   if(!FRONT){
   MoveToEx(_bitmap_hdc, user_to_disp_x(xfrom), user_to_disp_y(yfrom), 0);
   LineTo(_bitmap_hdc,user_to_disp_x(xto), user_to_disp_y(yto));}
}

void GraphicWindow::text(string s, double x, double y){  
   const char* t = s.c_str();
   SetBkMode(_hdc, TRANSPARENT);
   TextOut(_hdc, user_to_disp_x(x), user_to_disp_y(y), t, lstrlen(t));
   SetBkMode(_bitmap_hdc, TRANSPARENT);
   TextOut(_bitmap_hdc, user_to_disp_x(x), user_to_disp_y(y), t, lstrlen(t));
}

void GraphicWindow::statusline_prompt(string s){  
   const int M_WIDTH = 40;
   const char* t = s.c_str();
   HDC child_hdc;

   child_hdc = GetDC(child_hwnd);
   SIZE sz;
   GetTextExtentPoint32(child_hdc, t, strlen(t), &sz);
   _mlength = sz.cx;
   GetTextExtentPoint32(child_hdc, " ", 1, &sz);
   _mlength += sz.cx;
   ReleaseDC(child_hwnd, child_hdc);
   MoveWindow (child_hwnd, 0,0, _mlength, M_WIDTH, true);
   SendMessage (child_hwnd, WM_SETTEXT,0, (LPARAM) (LPCSTR) t);

   //move edit window from end of child window to edge of main window
   MoveWindow (edit_hwnd, cwin._mlength+10, 0, cwin._disp_xmax, M_WIDTH, true);
}

GraphicWindow& GraphicWindow::operator<<(modes m){
 if(m==RST){
		SelectObject(_bitmap_hdc,saved_brush);
		SelectObject(_bitmap_hdc,saved_pen);
		SelectObject(_hdc,saved_brush);
		SelectObject(_hdc,saved_pen);
	}
 SNAPON=m==SNAP?!SNAPON:SNAPON;
 FRONT=m==FRN?!FRONT:FRONT;
 if(m==CLR){
   LOGBRUSH lb={BS_SOLID,mainBk,0};
   HPEN pen = ExtCreatePen(PS_SOLID,1,&lb,0,NULL);	
   HBRUSH brush = CreateSolidBrush(mainBk);
   saved_brush = (HBRUSH)SelectObject(_hdc, brush);
   saved_pen = (HPEN)SelectObject(_hdc, pen);
   SelectObject(_bitmap_hdc, brush);
   SelectObject(_bitmap_hdc, pen);
 }
	return *this;
}
GraphicWindow& GraphicWindow::operator<<(Point p){  
   point(p.get_x(), p.get_y());
   return *this;
}

GraphicWindow& GraphicWindow::operator<<(Line s){  
   line(s.get_start().get_x(), s.get_start().get_y(), s.get_end().get_x(), s.get_end().get_y());
   return *this;
}

GraphicWindow& GraphicWindow::operator<<(Text t){  
   text(t.get_text(), t.get_start().get_x(), t.get_start().get_y());
   return *this;
}

GraphicWindow& GraphicWindow::operator<<(COLORREF t){ 
   LOGBRUSH lb={BS_SOLID,t,0};
   HPEN pen = ExtCreatePen(PS_SOLID,1,&lb,0,NULL);	
   saved_pen = (HPEN)SelectObject(_hdc, pen);
   SelectObject(_bitmap_hdc, pen);
   HBRUSH brush = CreateSolidBrush(mainBk);
   saved_brush = (HBRUSH)SelectObject(_hdc, brush);
   SelectObject(_bitmap_hdc, brush);
   return *this;
}

string GraphicWindow::get_string(const string& out_string=""){  
   MSG msg;
   const int BUFFSIZE = 80;
   char buffer[BUFFSIZE];
   memset(buffer, 0, sizeof(buffer));
   SendMessage (edit_hwnd, WM_SETTEXT,0, (LPARAM) (LPCSTR) buffer);
   string temp;
   statusline_prompt(out_string);   // output prompt
   SetFocus(edit_hwnd);
   HWND phwnd = GetParent(child_hwnd);
   while (GetMessage(&msg, (HWND) NULL, 0, 0))   {  
      TranslateMessage(&msg);
      DispatchMessage(&msg);
      switch(msg.message) {  
         case WM_KEYUP:
            if ((msg.wParam == VK_RETURN) || (msg.wParam == VK_ESCAPE))
            {  
               SendMessage (edit_hwnd, WM_GETTEXT,sizeof(buffer), (LPARAM) (LPCSTR) buffer);
               int buflen = strlen(buffer);
               if (buflen >= 2 && buffer[buflen - 2] == '\r') buffer[buflen - 2] = 0;
               temp = buffer;
               MoveWindow (child_hwnd, 0, 0, 0, 0, true);
               MoveWindow (edit_hwnd, 0, 0, 0, 0, true);
               SetFocus(phwnd);
               return temp;
            }
            break;
      }
   }
   return temp; 
}
GraphicWindow& GraphicWindow::operator>>(string & s){
	s=GraphicWindow::get_string();
	return *this;
}

Point GraphicWindow::get_mouse(const string &outstr){  
   MSG msg;
   Point p;// = new point;
   int mouse_x, mouse_y; double x,y;
   statusline_prompt(outstr);
   char buf[100];
   while (GetMessage(&msg, (HWND) NULL, 0, 0))   {  
      TranslateMessage(&msg);
      DispatchMessage(&msg);
      switch(msg.message) {  
         case WM_LBUTTONDOWN:
            MoveWindow (child_hwnd, 0, 0, 0, 0, true);
            MoveWindow (edit_hwnd, 0, 0, 0, 0, true);
            return p;
         case WM_MOUSEMOVE:
            mouse_x = LOWORD(msg.lParam);
            mouse_y = HIWORD(msg.lParam);
			x=disp_to_user_x(mouse_x);y=disp_to_user_y(mouse_y);
			if(SNAPON){
				x=1./snapfract*(int)(x*snapfract);
				y=1./snapfract*(int)(y*snapfract);
			}
            p = Point(x,y);
			sprintf(buf, "(%f,%f)",x,y); 
            SendMessage(edit_hwnd, WM_SETTEXT, 0, (LPARAM)buf);
            break;
      }
   }
   return p;
}

Point GraphicWindow::get_point(const Point &strt){  
   MSG msg;
   Point op=strt,p;
   int mouse_x, mouse_y; double x,y;
   char buf[100];RECT lpR;
   sprintf(buf, "(%f,%f)",op.get_x(),op.get_y()); 
   string prompt(buf);prompt="Line from "+prompt+" to:";
   statusline_prompt(prompt);
   while (GetMessage(&msg, (HWND) NULL,0,0)) {  
      TranslateMessage(&msg);
      DispatchMessage(&msg);
      switch(msg.message){  
         case WM_LBUTTONDOWN:
            MoveWindow (child_hwnd, 0, 0, 0, 0, true);
            MoveWindow (edit_hwnd, 0, 0, 0, 0, true);
			GetClientRect(own_hwnd,&lpR);
            InvalidateRect(own_hwnd,&lpR,false);
			return p;
         case WM_MOUSEMOVE:
            mouse_x = LOWORD(msg.lParam);
            mouse_y = HIWORD(msg.lParam);
			x=disp_to_user_x(mouse_x);y=disp_to_user_y(mouse_y);
			if(SNAPON){
				x=1./snapfract*(int)(x*snapfract);
				y=1./snapfract*(int)(y*snapfract);
			}
            p = Point(x,y);
			sprintf(buf, "(%f,%f)",x,y); 
            SendMessage(edit_hwnd, WM_SETTEXT, 0, (LPARAM)buf);
			cwin<<FRN<<CLR<<Line(strt,op);
			cwin<<RST<<Line(strt,p)<<FRN;op=p;
            break;
      }
   }
   return p;
}
void GraphicWindow::close(){  
   SelectObject(_bitmap_hdc, _hbm_saved);
   DeleteDC(_bitmap_hdc);
   DeleteObject(_hbm);
}

void GraphicWindow::repaint(){
	PAINTSTRUCT ps;
	BeginPaint(own_hwnd, &ps);
	  BitBlt(ps.hdc, ps.rcPaint.left, ps.rcPaint.top,
      ps.rcPaint.right, ps.rcPaint.bottom, _bitmap_hdc,
      ps.rcPaint.left, ps.rcPaint.top, SRCCOPY);
	EndPaint(own_hwnd, &ps);
}

int GraphicWindow::get_int(const string& out_string){  
   return atoi(get_string(out_string).c_str()); 
}
void GraphicWindow::setSnap(int snap){
	snapfract=snap;SNAPON=true;
}
double GraphicWindow::get_double(const string& out_string){  
   return atof(get_string(out_string).c_str()); 
}
Point::Point():x(0.0),y(0.0){}
Point::Point(double x, double y):x(x),y(y){}

double Point::get_x() const { return x;}
double Point::get_y() const { return y;}

void Point::move(double dx, double dy){  
   x += dx;   y += dy;
}
double Point::mod()const { return sqrt(*this * *this); }
double Point::operator *(const Point &b)const{ return x*b.x+y*b.y;}
Point Point::operator !()const{
  double m=mod();return Point(x/m,y/m);
}
Point Point::operator -(const Point &b)const{return Point(x-b.x,y-b.y);}
Point Point::operator +(const Point &b)const{return Point(x+b.x,y+b.y);}
Point Point::operator *(double k)const{return Point(x*k,y*k);}

Line::Line(){}
Line::Line(Point from, Point to):from(from),to(to){}

Point Line::get_start() const {return from;}
Point Line::get_end() const {return to;}

void Line::move(double dx, double dy){  
   from.move(dx, dy);  to.move(dx, dy);
}

Text::Text(){}
Text::Text(Point s, const string& m){  
   start = s;   text = m;
}
Text::Text(Point s, double x){  
   start = s;   char buf[20];
   sprintf(buf, "%g", x);   text = buf;
}

Point Text::get_start() const {return start;}
string Text::get_text() const {return text; }
void Text::move(double dx, double dy){ start.move(dx, dy);}
