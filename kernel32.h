//
//  kernel32.c
//  kernel32
//
//  Created by John Othwolo on 1/12/22.
//  Copyright © 2022 John Othwolo. All rights reserved.
//

#include <sys/cdefs.h>
#include <stdio.h>
#include <stdint.h>

# define ALIAS(name, aliasname) \
        __asm__(".globl _" #aliasname); \
        __asm__(".set _" #aliasname ", _" #name); \
        extern __typeof(name) aliasname

#define __nullterminated

#define STD_INPUT_HANDLE ((DWORD)-10)    // The standard input device. Initially, this is the console input buffer, CONIN$.
#define STD_OUTPUT_HANDLE ((DWORD)-11)   // The standard output device. Initially, this is the active console screen buffer, CONOUT$.
#define STD_ERROR_HANDLE ((DWORD)-12)

typedef __nullterminated const char *LPCSTR;

typedef bool    BOOL;
typedef char    CHAR;
typedef short   SHORT;
typedef int     INT;
typedef long    LONG;
typedef long long LONGLONG;
typedef wchar_t WCHAR;


typedef unsigned char       UCHAR;
typedef unsigned short      WORD;
typedef unsigned int        UINT;
typedef uint32_t            DWORD;
typedef unsigned long long  QWORD;
typedef wchar_t WCHAR;

typedef UCHAR BYTE;
typedef DWORD* LPDWORD;
typedef LONG* LPLONG;
typedef QWORD ULONGLONG;

typedef void        *LPVOID;
typedef const void  *LPCVOID;
typedef WORD        *LPWORD;
typedef CHAR        *LPSTR;
typedef WCHAR       *LPWSTR;
typedef const WCHAR *LPCWSTR;




#ifdef UNICODE
 typedef LPWSTR LPTSTR;
#else
 typedef LPSTR LPTSTR;
#endif


typedef DWORD ULONG_PTR, SIZE_T, *PSIZE_T;

typedef void*  PVOID;
typedef PVOID  HANDLE;
typedef HANDLE HGDIOBJ;
typedef HANDLE HGLOBAL;
typedef HANDLE HLOCAL;
typedef HANDLE GLOBALHANDLE;
typedef HANDLE LOCALHANDLE;
typedef HANDLE HWND;

/* The 'overlapped' data structure used by async I/O functions.
 */
typedef struct _OVERLAPPED {
#ifdef BIG_ENDIAN
        ULONG_PTR InternalHigh;
        ULONG_PTR Internal;
#else
        ULONG_PTR Internal;
        ULONG_PTR InternalHigh;
#endif
        union {
            struct {
#ifdef BIG_ENDIAN
                DWORD OffsetHigh;
                DWORD Offset;
#else
                DWORD Offset;
                DWORD OffsetHigh;
#endif
            } DUMMYSTRUCTNAME;
            PVOID Pointer;
        } DUMMYUNIONNAME;
        HANDLE hEvent;
} OVERLAPPED, *LPOVERLAPPED;

typedef union _LARGE_INTEGER {
  struct {
    DWORD LowPart;
    LONG  HighPart;
  } DUMMYSTRUCTNAME;
  struct {
    DWORD LowPart;
    LONG  HighPart;
  } u;
  LONGLONG QuadPart;
} LARGE_INTEGER;

typedef struct _SYSTEMTIME {
  WORD wYear;
  WORD wMonth;
  WORD wDayOfWeek;
  WORD wDay;
  WORD wHour;
  WORD wMinute;
  WORD wSecond;
  WORD wMilliseconds;
} SYSTEMTIME, *PSYSTEMTIME, *LPSYSTEMTIME;

typedef struct _FILETIME {
  DWORD dwLowDateTime;
  DWORD dwHighDateTime;
} FILETIME, *PFILETIME, *LPFILETIME;

typedef struct _CONSOLE_CURSOR_INFO {
    DWORD dwSize;   /* Between 1 & 100 for percentage of cell filled */
    BOOL bVisible; /* Visibility of cursor */
} CONSOLE_CURSOR_INFO, *LPCONSOLE_CURSOR_INFO;

typedef struct tagCOORD
{
    SHORT X;
    SHORT Y;
} COORD, *LPCOORD;

typedef struct tagSMALL_RECT
{
    SHORT Left;
    SHORT Top;
    SHORT Right;
    SHORT Bottom;
} SMALL_RECT, *LPSMALL_RECT;

typedef struct tagCONSOLE_SCREEN_BUFFER_INFO
{
    COORD       dwSize;
    COORD       dwCursorPosition;
    WORD        wAttributes;
    SMALL_RECT  srWindow;
    COORD       dwMaximumWindowSize;
} CONSOLE_SCREEN_BUFFER_INFO,*LPCONSOLE_SCREEN_BUFFER_INFO;

typedef struct tagMOUSE_EVENT_RECORD
{
    COORD    dwMousePosition;
    DWORD    dwButtonState;
    DWORD    dwControlKeyState;
    DWORD    dwEventFlags;
} MOUSE_EVENT_RECORD,*LPMOUSE_EVENT_RECORD;

/* MOUSE_EVENT_RECORD.dwButtonState */
#define FROM_LEFT_1ST_BUTTON_PRESSED    0x0001
#define RIGHTMOST_BUTTON_PRESSED        0x0002
#define FROM_LEFT_2ND_BUTTON_PRESSED    0x0004
#define FROM_LEFT_3RD_BUTTON_PRESSED    0x0008
#define FROM_LEFT_4TH_BUTTON_PRESSED    0x0010

/* MOUSE_EVENT_RECORD.dwEventFlags */
#define MOUSE_MOVED                     0x0001
#define DOUBLE_CLICK                    0x0002
#define MOUSE_WHEELED                   0x0004
#define MOUSE_HWHEELED                  0x0008


typedef struct tagWINDOW_BUFFER_SIZE_RECORD
{
    COORD    dwSize;
} WINDOW_BUFFER_SIZE_RECORD,*LPWINDOW_BUFFER_SIZE_RECORD;

typedef struct tagMENU_EVENT_RECORD
{
    UINT    dwCommandId; /* perhaps UINT16 ??? */
} MENU_EVENT_RECORD,*LPMENU_EVENT_RECORD;

typedef struct tagFOCUS_EVENT_RECORD
{
    BOOL      bSetFocus; /* perhaps BOOL16 ??? */
} FOCUS_EVENT_RECORD,*LPFOCUS_EVENT_RECORD;


typedef struct tagKEY_EVENT_RECORD
{
    BOOL    bKeyDown;        /* 04 */
    WORD    wRepeatCount;        /* 08 */
    WORD    wVirtualKeyCode;    /* 0A */
    WORD    wVirtualScanCode;    /* 0C */
    union                /* 0E */
    {
    WCHAR UnicodeChar;        /* 0E */
    CHAR AsciiChar;            /* 0E */
    } uChar;
    DWORD    dwControlKeyState;    /* 10 */
} KEY_EVENT_RECORD,*LPKEY_EVENT_RECORD;

typedef struct tagINPUT_RECORD
{
    WORD        EventType;        /* 00 */
    union
    {
    KEY_EVENT_RECORD KeyEvent;
    MOUSE_EVENT_RECORD MouseEvent;
    WINDOW_BUFFER_SIZE_RECORD WindowBufferSizeEvent;
    MENU_EVENT_RECORD MenuEvent;
    FOCUS_EVENT_RECORD FocusEvent;
    } Event;
} INPUT_RECORD,*PINPUT_RECORD;

// ----------------------------------------------------------------------------

extern "C" {

void CloseHandle(        // close file handle
        HANDLE hObject
);

HANDLE CreateFileA(        // create new file
        LPCSTR lpFilename,        // ptr to filename
        DWORD dwDesiredAccess,        // access mode
        DWORD dwShareMode,        // share mode
        DWORD lpSecurityAttributes,          // pointer to security attributes
        DWORD dwCreationDisposition,        // file creation options
        DWORD dwFlagsAndAttributes,        // file attributes
        DWORD hTemplateFile        // handle to template file
);

void ExitProcess(                      // exit program
        DWORD dwExitCode        // return code
);

BOOL FileTimeToSystemTime(              // convert FILETIME to SYSTEMTIME
        const FILETIME *lpFileTime,
        LPSYSTEMTIME   lpSystemTime
);

BOOL FileTimeToDosDateTime(             // converts a Win32 file time to MS-DOS date/time
        const FILETIME *lpFileTime,
        LPWORD         lpFatDate,
        LPWORD         lpFatTime
);

DWORD FormatMessage(                     // format a message
        DWORD   dwFlags,        // formatting options
        LPCVOID lpSource,       // location of message definition
        DWORD   dwMessageId,    // message identifier
        DWORD   dwLanguageId,   // language identifier
        LPTSTR  lpBuffer,       // pointer to buffer receiving string
        DWORD   nSize,          // buffer size
        va_list *Arguments      // pointer to list of);
);

BOOL FlushConsoleInputBuffer(    // flush the input buffer
  const HANDLE hConsoleInput            // standard input handle
);

LPSTR GetCommandLineA();        // returns pointer to command-line string

UINT GetConsoleCP(void);        // returns code page ID in rax

BOOL GetConsoleCursorInfo(
        HANDLE hConsoleOutput, // console output handle
        LPCONSOLE_CURSOR_INFO lpConsoleCursorInfo  // cursor information
);

BOOL GetConsoleMode(
        HANDLE hConsoleHandle,   // input or output handle
        LPDWORD lpMode           // pointer to dword containing flags
);

BOOL GetConsoleScreenBufferInfo(
    HANDLE hConsoleOutput,        // handle to screen buffer
    LPCONSOLE_SCREEN_BUFFER_INFO lpConsoleScreenBufferInfo
);

BOOL GetFileTime(        // get date/time stamp of a file
    HANDLE hFile,
    LPFILETIME pCreationTime,
    LPFILETIME pLastAccessTime,
    LPFILETIME pLastWriteTime
);

SHORT GetKeyState(        // get state of keyboard keys (see Ch11\keybd.asm)
    INT nVirtKey
);


DWORD GetLastError();        // get most recent error return value

void GetLocalTime(        // system time, adjusted for local time zone
    LPSYSTEMTIME lpSystemTime        // ptr to SYSTEMTIME object
);


BOOL GetNumberOfConsoleInputEvents(    // get number of unread records
    HANDLE hConsoleInput,        // input handle
    LPDWORD lpCount        // pointer to counter
);

HANDLE GetStdHandle(                   // get standard handle
    DWORD nStdHandle          // type of console handle
);

void GetSystemTime(        // returns the system time
    LPSYSTEMTIME lpSystemTime        // ptr to SYSTEMTIME object
);

// get elapsed milliseconds since computer was turned on
DWORD     GetTickCount(void);
ULONGLONG GetTickCount64(void);

HANDLE GetProcessHeap(void); // Get the current process heap handle

LPVOID HeapAlloc(
    HANDLE hHeap,        // handle to private heap block
    DWORD dwFlags,        // heap allocation control flags
    SIZE_T dwBytes         // number of bytes to allocate
);

HANDLE HeapCreate(
    DWORD flOptions,        // heap allocation options
    SIZE_T dwInitialSize,        // initial heap size, in bytes
    SIZE_T dwMaximumSize        // maximum heap size, in bytes
);

BOOL HeapDestroy(
    HANDLE hHeap        // heap handle
);

BOOL HeapFree(
    HANDLE hHeap,        // handle to heap with memory block
    DWORD dwFlags,        // heap free options
    LPVOID lpMem        // pointer to block to be freed
);

SIZE_T HeapSize(
    HANDLE hHeap,        // handle to heap
    DWORD dwFlags,        // heap size options
    LPCVOID pMem        // pointer to memory block
);

HLOCAL LocalFree(        // free up local memory, using its handle
    HLOCAL hMem
);

INT MessageBoxA(        // displays a pop-up message box
    HWND hWnd,        // handle to window (can be null)
    LPCSTR lpText,        // string for inside of dialog box
    LPCSTR lpCaption,        // string for dialog box title
    UINT uType        // contents and behavior of the dialog
);

BOOL PeekConsoleInput(
    HANDLE hConsoleInput,        // input handle
    PINPUT_RECORD lpBuffer,        // pointer to buffer
    DWORD nLength,            // number of chars to read
    LPDWORD lpNumberOfEventsRead       // ptr to number of events read
);

//void printf(        // Std C library function
//       LPCSTR format, ...        // (in libc.lib)
//);

BOOL ReadConsoleA(
    HANDLE hConsoleInput,              // input handle
    LPVOID lpBuffer,                    // pointer to buffer
    DWORD nNumberOfBytesToRead,        // number of chars to read
    LPDWORD lpNumberOfCharsRead,      // ptr to number of bytes read
    LPVOID lpReserved                   // (not used)
);

BOOL ReadConsoleInputA(
    HANDLE hConsoleInput,        // input handle
    PINPUT_RECORD lpBuffer,        // ptr to input record
    DWORD nLength,        // request number of recs
    LPDWORD lpNumberOfEventsRead    // ptr to number of bytes read
);
                 
BOOL ReadFile(        // read buffer from input file
    HANDLE hFile,        // input handle
    LPVOID lpBuffer,        // ptr to buffer
    DWORD nNumberOfBytesToRead,        // num bytes to read
    LPDWORD lpNumberOfBytesRead,    // bytes actually read
    LPOVERLAPPED lpOverlapped        // ptr to asynchronous info
);

//scanf PROTO C,        // Std C library function
//    format:PTR BYTE, args:VARARG        // (in libc.lib)

BOOL SetConsoleCursorInfo(
    HANDLE hConsoleOutput,         // output handle
    LPCONSOLE_CURSOR_INFO lpConsoleCursorInfo    // cursor information
);

BOOL SetConsoleCursorPosition(
    HANDLE hConsoleOutput,        // output handle
    COORD dwCursorPosition        // screen X,Y coordinates
);

BOOL SetConsoleMode(
    HANDLE hConsoleHandle,        // output handle
    DWORD dwMode        // console mode flags
);

BOOL SetConsoleScreenBufferSize(
    HANDLE hConsoleOutput,        // handle to screen buffer
    COORD dwSize        // new screen buffer size
);

BOOL SetConsoleTextAttribute(
    HANDLE hConsoleOutput,        // output handle
    WORD wAttributes        // color attribute (fixed 6/20/05)
);

BOOL SetConsoleTitle(        // set console window title
    LPCSTR lpConsoleTitle        // points to string
);

BOOL SetConsoleWindowInfo(        // set position of console window
    HANDLE hConsoleOutput,        // screen buffer handle
    BOOL bAbsolute,        // coordinate type
    LPSMALL_RECT lpConsoleWindow    // ptr to window rectangle
);

DWORD SetFilePointer(        // set the file pointer position
    HANDLE hFile,        // file handle
    LONG lDistanceToMove,        // bytes to move pointer
    LPLONG lpDistanceToMoveHigh,    // ptr to bytes to move, high
    DWORD dwMoveMethod        // starting point
);

BOOL SetLocalTime(        // set the system's local date and time
    const SYSTEMTIME* lpSystemTime        // ptr to SYSTEMTIME structure
);

void Sleep(           // sleeep for n milliseconds
    DWORD dwMilliseconds
);

BOOL SystemTimeToFileTime(        // Convert a SYSTEMTIME structure
    const SYSTEMTIME* lpSystemTime,        // to a FILETIME structure
    LPFILETIME lpFileTime
);

BOOL WriteConsoleA(        // write a buffer to the console
    HANDLE hConsoleOutput,        // output handle
    const void* lpBuffer,        // pointer to buffer
    DWORD nNumberOfCharsToWrite,        // size of buffer
    LPDWORD lpNumberOfCharsWritten,        // ptr to number of bytes written
    void* lpReserved        // (not used)
);
             
BOOL WriteConsoleOutputCharacter(
    HANDLE hConsoleOutput,        // console output handle
    LPCSTR lpCharacter,        // pointer to buffer
    DWORD nLength,        // size of buffer
    COORD dwWriteCoord,        // first cell coordinates
    LPDWORD lpNumberOfCharsWritten    // output count
);
                            
BOOL WriteConsoleOutputAttribute(
      HANDLE  hConsoleOutput,              // output handle
const WORD    *lpAttribute,                // write attributes
      DWORD   nLength,                 // number of cells
      COORD   dwWriteCoord,           // first cell coordinates
      LPDWORD lpNumberOfAttrsWritten      // number of cells written
);

BOOL WriteFile(                        // write buffer to output file
    HANDLE       hFile,                    // output handle
    LPCVOID      lpBuffer,                 // pointer to buffer
    DWORD        nNumberOfBytesToWrite,    // size of buffer
    LPDWORD      lpNumberOfBytesWritten,    // num bytes written
    LPOVERLAPPED lpOverlapped              // ptr to asynchronous info
);

//int wsprintf(  // output buffer
//        LPSTR  lpOut,   // format-control string
//        LPCSTR lpFmt,     // optional arguments
//        ...
//);
}
