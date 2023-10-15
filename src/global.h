  
#ifndef __GLOBAL_H
#define __GLOBAL_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>

#if __GNUC__ >= 3
# define inline inline __attribute__ ((always_inline))
# define __pure __attribute__ ((pure))
# define __const __attribute__ ((const))
# define __noreturn __attribute__ ((noreturn))
# define __malloc __attribute__ ((malloc))
# define __must_check __attribute__ ((warn_unused_result))
# define __deprecated __attribute__ ((deprecated))
# define __used __attribute__ ((used))
# define __unused __attribute__ ((unused))
# define __packed __attribute__ ((packed))
# define __weak __attribute__ ((weak))
# define likely(x) __builtin_expect (!!(x), 1)
# define unlikely(x) __builtin_expect (!!(x), 0)
#define PACK( __Declaration__ ) __Declaration__ __attribute__((__packed__))
#else
# define inline /* no inline */
# define __pure /* no pure */
# define __const /* no const */
# define __noreturn /* no noreturn */
# define __malloc /* no malloc */
# define __must_check /* no warn_unused_result */
# define __deprecated /* no deprecated */
# define __used /* no used */
# define __unused /* no unused */
# define __packed /* no packed */
# define likely(x) (x)
# define unlikely(x) (x)
#define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop))
#endif

#define SGBOOL  bool

#define	IO_HIGH(p,i)		{p->BSRR=i;}
#define IO_LOW(p,i)		    {p->BRR=i;}
#define IO_TOGGLE(p,i)      {p->ODR ^=i;}

enum
{
    SETBIT0 = 0x0001,   SETBIT1 = 0x0002,	SETBIT2 = 0x0004,	SETBIT3 = 0x0008,
    SETBIT4 = 0x0010,	SETBIT5 = 0x0020,	SETBIT6 = 0x0040,	SETBIT7 = 0x0080,
    SETBIT8 = 0x0100,	SETBIT9 = 0x0200,	SETBIT10 = 0x0400,  SETBIT11 = 0x0800,
    SETBIT12 = 0x1000,  SETBIT13 = 0x2000,	SETBIT14 = 0x4000,  SETBIT15 = 0x8000
};

enum
{
    CLRBIT0 = 0xFFFE,   CLRBIT1 = 0xFFFD,	CLRBIT2 = 0xFFFB,	CLRBIT3 = 0xFFF7,
    CLRBIT4 = 0xFFEF,	CLRBIT5 = 0xFFDF,	CLRBIT6 = 0xFFBF,	CLRBIT7 = 0xFF7F,
    CLRBIT8 = 0xFEFF,	CLRBIT9 = 0xFDFF,	CLRBIT10 = 0xFBFF,  CLRBIT11 = 0xF7FF,
    CLRBIT12 = 0xEFFF,  CLRBIT13 = 0xDFFF,	CLRBIT14 = 0xBFFF,  CLRBIT15 = 0x7FFF
};

enum
{
    CHSBIT0 = 0x0001,   CHSBIT1 = 0x0002,	CHSBIT2 = 0x0004,   CHSBIT3 = 0x0008,
    CHSBIT4 = 0x0010,	CHSBIT5 = 0x0020,	CHSBIT6 = 0x0040,	CHSBIT7 = 0x0080,
    CHSBIT8 = 0x0100,	CHSBIT9 = 0x0200,	CHSBIT10 = 0x0400,  CHSBIT11 = 0x0800,
    CHSBIT12 = 0x1000,  CHSBIT13 = 0x2000,	CHSBIT14 = 0x4000,  CHSBIT15 = 0x8000
};

enum
{
    STEP0 = 0,         STEP1 = 1,         STEP2 = 2,         STEP3 = 3,
    STEP4 = 4,         STEP5 = 5,         STEP6 = 6,         STEP7 = 7,
    STEP8 = 8,         STEP9 = 9,         STEP10 = 10,       STEP11 = 11,
    STEP12 = 12,       STEP13 = 13,       STEP14 = 14,       STEP15 = 15,
    STEP16 = 16,       STEP17 = 17,       STEP18 = 18,       STEP19 = 19,
    STEP20 = 20,       STEP21 = 21,       STEP22 = 22,       STEP23 = 23,
    STEP24 = 24,       STEP25 = 25,       STEP26 = 26,       STEP27 = 27,
    STEP28 = 28,       STEP29 = 29,       STEP30 = 30,       STEP31 = 31,
    STEP32 = 32,       STEP33 = 33,       STEP34 = 34,       STEP35 = 35
};

#ifndef     NULL
#define     NULL            (0)
#endif
#define     TRUE            (1)
#define     FALSE           (0)
#define     ZERO            (0)
#define     ONE         	(1)
#define     TWO         	(2)
#define     THREE       	(3)
#define     FOUR        	(4)
#define     FIVE        	(5)
#define     SIX         	(6)
#define     SEVEN       	(7)
#define     EIGHT       	(8)
#define     NINE        	(9)
#define     TEN         	(10)
#define     HUNDRED     	(100)
#define     THOUSAND    	(1000)
#define     HALFBYTEMAX 	(15)
#define     ONEBYTEMAX  	(255)

#define _SWAP(a, b)     ((a) ^= (b), (b) ^= (a), (a) ^= (b))
#define _ARRAY_SIZE(a)  ((sizeof(a)) / (sizeof(a[0])))
#define BIG_LITTLE_SWAP16(A)        ((((uint16_t)(A) & 0xff00) >> 8)        \
                                    | (((uint16_t)(A) & 0x00ff) << 8))

#define BIG_LITTLE_SWAP32(A)        ((((uint32_t)(A) & 0xff000000) >> 24)   \
                                    | (((uint32_t)(A) & 0x00ff0000) >> 8)   \
                                    | (((uint32_t)(A) & 0x0000ff00) << 8)   \
                                    | (((uint32_t)(A) & 0x000000ff) << 24))

#define _SET_BIT(target, bit) ((target) |= (unsigned long)(((unsigned long)1) << (bit)))
#define _CLR_BIT(target, bit) ((target) &= (unsigned long)(~(((unsigned long)1) << (bit))))
#define _GET_BIT(target, bit) ((target) & (((unsigned long)1) << (bit)))
#define _FLP_BIT(target, bit) ((target) ^= (((unsigned long)1) << (bit)))

#define PRINTF(...)         printf(__VA_ARGS__)

#define ASSERT(val)                                                     \
if (!(val))                                                             \
{                                                                       \
    PRINTF("(%s) has assert failed at %s.\n", #val, __FUNCTION__);      \
    while (1);                                                          \
}

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define __FORCE_INLINE	__attribute__((always_inline)) inline
#define CLI_PRINTF	TRUE

#define KEY_UP              "\x1b\x5b\x41"  /* [up] key: 0x1b 0x5b 0x41 */
#define KEY_DOWN            "\x1b\x5b\x42"  /* [down] key: 0x1b 0x5b 0x42 */
#define KEY_RIGHT           "\x1b\x5b\x43"  /* [right] key: 0x1b 0x5b 0x43 */
#define KEY_LEFT            "\x1b\x5b\x44"  /* [left] key: 0x1b 0x5b 0x44 */
#define KEY_ENTER           '\r'            /* [enter] key */
#define KEY_ENTER_ALT       '\n'            /* [enter] key */
#define KEY_BACKSPACE       '\b'            /* [backspace] key */

#if CLI_PRINTF
    #define PRINTF(...)         printf(__VA_ARGS__)
    #define SPRINTF(...)        sprintf(__VA_ARGS__)

enum {
    E_FONT_BLACK,
    E_FONT_L_RED,
    E_FONT_RED,
    E_FONT_GREEN,
    E_FONT_YELLOW,
    E_FONT_BLUE,
    E_FONT_PURPLE,
    E_FONT_CYAN,
    E_FONT_WHITE,
};

    #define PRINTF_COLOR(c, ...)    do {                            \
                                        switch (c) {                \
                                            case E_FONT_BLACK:      \
                                            TERMINAL_FONT_BLACK();  \
                                            break;                  \
                                            case E_FONT_L_RED:      \
                                            TERMINAL_FONT_L_RED();  \
                                            break;                  \
                                            case E_FONT_RED:        \
                                            TERMINAL_FONT_RED();    \
                                            break;                  \
                                            case E_FONT_GREEN:      \
                                            TERMINAL_FONT_GREEN();  \
                                            break;                  \
                                            case E_FONT_YELLOW:     \
                                            TERMINAL_FONT_YELLOW(); \
                                            break;                  \
                                            case E_FONT_BLUE:       \
                                            TERMINAL_FONT_BLUE();   \
                                            break;                  \
                                            case E_FONT_PURPLE:     \
                                            TERMINAL_FONT_PURPLE(); \
                                            break;                  \
                                            case E_FONT_CYAN:       \
                                            TERMINAL_FONT_CYAN();   \
                                            break;                  \
                                            case E_FONT_WHITE:      \
                                            TERMINAL_FONT_WHITE();  \
                                            break;                  \
                                        }                           \
                                        printf(__VA_ARGS__);        \
                                        TERMINAL_FONT_GREEN();      \
                                    } while(0)
#else
    #define PRINTF(...)         ;
    #define SPRINTF(...)        ;
    #define PRINTF_COLOR(c, ...);
#endif /* CLI_PRINTF */


#define ASSERT(val)                                                     \
if (!(val))                                                             \
{                                                                       \
    PRINTF("(%s) has assert failed at %s.\n", #val, __FUNCTION__);      \
    while (1);                                                          \
}

enum {
	LOGE_LV_INFO = 0,
	LOGE_LV_DEBUG,
	LOGE_LV_WARN,
	LOGE_LV_ERROR,
};

#define LOG_LEVEL	(LOGE_LV_WARN)

#define DIE(fmt)        do {                                            \
                            TERMINAL_FONT_RED();                        \
                            TERMINAL_HIGH_LIGHT();                      \
                            PRINTF("### DIE ### %s(%d): "fmt"\r\n",  __FUNCTION__, __LINE__);                \
                        } while(1) /* infinite loop */

#if (LOG_LEVEL >= LOGE_LV_ERROR)
#define ERR(fmt)        do {                                            \
                            TERMINAL_FONT_RED();                        \
                            PRINTF("### ERROR ###: "fmt"\r\n");         \
                            TERMINAL_FONT_GREEN();                      \
                        } while(0)
#define ERRA(fmt, ...)  do {                                            \
                            TERMINAL_FONT_RED();                        \
                            PRINTF("### ERROR ### %s(%d): "fmt"\r\n", __FUNCTION__, __LINE__, __VA_ARGS__);   \
                            TERMINAL_FONT_GREEN();                      \
                        }while(0)
#else
#define ERR(fmt)
#define ERRA(fmt, ...)
#endif

#if (LOG_LEVEL >= LOGE_LV_WARN)
#define WRN(fmt)        do {                                            \
                            TERMINAL_FONT_YELLOW();                     \
                            PRINTF("[WARN] %s(%d): "fmt"\r\n", __FUNCTION__, __LINE__); \
                            TERMINAL_FONT_GREEN();                      \
                        } while(0)
#define WRNA(fmt, ...)  do {                                            \
                            TERMINAL_FONT_YELLOW();                     \
                            PRINTF("[WARN] %s(%d): "fmt"\r\n", __FUNCTION__, __LINE__, __VA_ARGS__);   \
                            TERMINAL_FONT_GREEN();                      \
                        } while(0)
#else
#define WRN(fmt)
#define WRNA(fmt, ...)
#endif

#if (LOG_LEVEL >= LOGE_LV_INFO)
#define INFO(fmt)        do {                                            \
                            TERMINAL_FONT_CYAN();                       \
                            PRINTF("[INFO] %s(%d): "fmt"\r\n", __FUNCTION__, __LINE__); \
                            TERMINAL_FONT_GREEN();                      \
                        } while(0)
#define INFA(fmt, ...)  do {                                            \
                            TERMINAL_FONT_CYAN();                       \
                            PRINTF("[INFO] %s(%d): "fmt"\r\n", __FUNCTION__, __LINE__, __VA_ARGS__);   \
                            TERMINAL_FONT_GREEN();                      \
                        } while(0)
#else
#define INFO(fmt)
#define INFA(fmt, ...)
#endif

#if (LOG_LEVEL >= LOGE_LV_DEBUG)
#define DBG(fmt)        do {                                            \
                            TERMINAL_FONT_WHITE();                     \
                            PRINTF("[DEBUG] %s(%d): "fmt"\r\n", __FUNCTION__, __LINE__);                \
                            TERMINAL_FONT_GREEN();                      \
                        } while(0)
#define DBGA(fmt, ...)  do {                                            \
                            TERMINAL_FONT_WHITE();                     \
                            PRINTF("[DEBUG] %s(%d): "fmt"\r\n", __FUNCTION__, __LINE__, __VA_ARGS__);   \
                            TERMINAL_FONT_GREEN();                      \
                        } while(0)
#else
#define DBG(fmt)
#define DBGA(fmt, ...)
#endif

#define NL1()           do { PRINTF("\r\n"); } while(0)
#define NL2()           do { PRINTF("\r\n\r\n"); } while(0)
#define NL3()           do { PRINTF("\r\n\r\n\r\n"); } while(0)

/* debug------------------------------------------------------------------END */




/* terminal display-----------------------------------------------------BEGIN */

/*
    @links: http://blog.csdn.net/yangguihao/article/details/47734349
            http://blog.csdn.net/kevinshq/article/details/8179252


    @terminal setting commands:
        \033[0m     reset all
        \033[1m     set high brightness
        \03[4m      underline
        \033[5m     flash
        \033[7m     reverse display
        \033[8m     blanking
        \033[30m    --  \033[37m  set font color
        \033[40m    --  \033[47m  set background color
        \033[nA     cursor up up n lines
        \033[nB     cursor move up n lines
        \033[nC     cursor move right n lines
        \033[nD     cursor left up n lines
        \033[y;xH   set cursor position
        \033[2J     clear all display
        \033[K      clear line
        \033[s      save cursor position
        \033[u      restore cursor position
        \033[?25l   cursor invisible
        \33[?25h    cursor visible


    @background color: 40--49           @font color: 30--39
        40: BLACK                           30: black
        41: RED                             31: red
        42: GREEN                           32: green
        43: YELLOW                          33: yellow
        44: BLUE                            34: blue
        45: PURPLE                          35: purple
        46: CYAN                            36: deep green
        47: WHITE                           37: white
*/

/* font color */
#define TERMINAL_FONT_BLACK()       PRINTF("\033[1;30m")
#define TERMINAL_FONT_L_RED()       PRINTF("\033[0;31m")    /* light red */
#define TERMINAL_FONT_RED()         PRINTF("\033[1;31m")    /* red */
#define TERMINAL_FONT_GREEN()       PRINTF("\033[1;32m")
#define TERMINAL_FONT_YELLOW()      PRINTF("\033[1;33m")
#define TERMINAL_FONT_BLUE()        PRINTF("\033[1;34m")
#define TERMINAL_FONT_PURPLE()      PRINTF("\033[1;35m")
#define TERMINAL_FONT_CYAN()        PRINTF("\033[1;36m")
#define TERMINAL_FONT_WHITE()       PRINTF("\033[1;37m")

/* background color */
#define TERMINAL_BACK_BLACK()       PRINTF("\033[1;40m")
#define TERMINAL_BACK_L_RED()       PRINTF("\033[0;41m")    /* light red */
#define TERMINAL_BACK_RED()         PRINTF("\033[1;41m")    /* red */
#define TERMINAL_BACK_GREEN()       PRINTF("\033[1;42m")
#define TERMINAL_BACK_YELLOW()      PRINTF("\033[1;43m")
#define TERMINAL_BACK_BLUE()        PRINTF("\033[1;44m")
#define TERMINAL_BACK_PURPLE()      PRINTF("\033[1;45m")
#define TERMINAL_BACK_CYAN()        PRINTF("\033[1;46m")
#define TERMINAL_BACK_WHITE()       PRINTF("\033[1;47m")

/* terminal clear end */
#define TERMINAL_CLEAR_END()        PRINTF("\033[K")

/* terminal clear all */
#define TERMINAL_DISPLAY_CLEAR()    PRINTF("\033[2J")

/* cursor move up */
#define TERMINAL_MOVE_UP(x)         PRINTF("\033[%dA", (x))

/* cursor move down */
#define TERMINAL_MOVE_DOWN(x)       PRINTF("\033[%dB", (x))

/* cursor move left */
#define TERMINAL_MOVE_LEFT(y)       PRINTF("\033[%dD", (y))

/* cursor move right */
#define TERMINAL_MOVE_RIGHT(y)      PRINTF("\033[%dC",(y))

/* cursor move to */
#define TERMINAL_MOVE_TO(x, y)      PRINTF("\033[%d;%dH", (x), (y))

/* cursor reset */
#define TERMINAL_RESET_CURSOR()     PRINTF("\033[H")

/* cursor invisible */
#define TERMINAL_HIDE_CURSOR()      PRINTF("\033[?25l")

/* cursor visible */
#define TERMINAL_SHOW_CURSOR()      PRINTF("\033[?25h")

/* reverse display */
#define TERMINAL_HIGH_LIGHT()       PRINTF("\033[7m")
#define TERMINAL_UN_HIGH_LIGHT()    PRINTF("\033[27m")

/* terminal display-------------------------------------------------------END */

#endif /* __GLOBAL_H */
