#ifndef _GLOBAL_DEFINES_
#define _GLOBAL_DEFINES_

typedef unsigned short ushort;
typedef unsigned int uint;

#ifndef NULL
#define NULL  0
#endif 

#ifndef RELEASE
#define RELEASE( x )\
    {\
       if( x != nullptr )\
       {\
         delete x;\
	     x = nullptr;\
       }\
    }
#endif 

#ifndef RELEASE_ARRAY
#define RELEASE_ARRAY( x )\
	{\
       if( x != nullptr )\
       {\
           delete[] x;\
	       x = nullptr;\
		 }\
	 }
#endif 

#ifndef DEGTORAD
#define DEGTORAD 0.0174532925199432957f
#endif 

#ifndef RADTODEG
#define RADTODEG 57.295779513082320876f
#endif 

#ifndef CIRCLEDEGREES
#define CIRCLEDEGREES 360.f
#endif 

#ifndef CIRCLEPERFECTION
#define CIRCLEPERFECTION 20
#endif 

// DEBUG ------------------------------------------------

#ifndef DEBUG_COLOR_LINES
#define DEBUG_COLOR_LINES 0.7f, 1.0f, 1.0f// AZURE 
#endif  

//COLORS

// AZURE  0.7f, 1.0f, 1.0f //BLUE
// ORANGE  1.3f, 0.5f, 0.3f
// BROWN 0.6f, 0.3f, 0.2f
// YELLOW 1.0f, 0.85f, 0.0f
// CYAN 0.0f, 0.8f, 0.9f

#endif // !_GLOBAL_DEFINES_
