//
//  kernel32.c
//  kernel32
//
//  Created by John Othwolo on 1/12/22.
//  Copyright © 2022 John Othwolo. All rights reserved.
//

#include <sys/cdefs.h>
#include <sys/ioctl.h>
#include <sys/sysctl.h>
#include <termios.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <stdint.h>
#include <stdarg.h>
#include <string>
#include <time.h>
#include <unordered_map>
#include <mach-o/loader.h>
#include <sched.h>

#include <CoreFoundation/CoreFoundation.h>

#include "kernel32.h"
#include "cmdline.h"
#include "wincon.h"
#include "winuser.h"


// stolen from apple sys/buf.h
/* Macros to clear/set/test flags. */
#define    SET(t, f)    (t) |= (f)
#define    CLR(t, f)    (t) &= ~(f)
#define    ISSET(t, f)    ((t) & (f))

/*
 * number of seconds from 1 Jan. 1601 00:00 to 1 Jan 1970 00:00 UTC
 */
#define MSFT_EPOCH_DIFF 11644473600LL

//extern int proc_pidinfo(int pid, int flavor,
//                        uint64_t arg, user_addr_t buffer,
//                        uint32_t  buffersize);
extern "C" { // this forces compilation as C symbols instead of mangled cpp symbols.

//__asm__(".globl ___mh_execute_header");
//__asm__(".set ___mh_execute_header, __mh_execute_header");
// this can't be here but i have no choice if I'm converting
// macho to elf with objconv for Irvine64
//struct mach_header_64 __mh_execute_header;
static int g_consoleModes[3] = {
    ENABLE_PROCESSED_INPUT | ENABLE_LINE_INPUT |
    ENABLE_ECHO_INPUT | ENABLE_MOUSE_INPUT | ENABLE_INSERT_MODE |
    ENABLE_EXTENDED_FLAGS
};
static std::vector<INPUT_RECORD> g_consoleRecords[3] {};

__attribute__((constructor))
void __initLibrary(void){
    
}

__attribute__((destructor))
void __deinitLibrary(void){
    
}

//ALIAS(_mh_execute_header, _mh_execute_header);

void CloseHandle(        // close file handle
        HANDLE hObject
){
    fclose((FILE*)hObject);
}

HANDLE CreateFileA(        // create new file
        LPCSTR lpFilename,        // ptr to filename
        DWORD dwDesiredAccess,        // access mode
        DWORD dwShareMode,        // share mode
        DWORD lpSecurityAttributes,          // pointer to security attributes
        DWORD dwCreationDisposition,        // file creation options
        DWORD dwFlagsAndAttributes,        // file attributes
        DWORD hTemplateFile        // handle to template file
){
    return 0;
}
//ALIAS(CreateFile, CreateFileA);

__attribute__((noreturn))
void ExitProcess(                      // exit program
        DWORD dwExitCode        // return code
){
    exit((int)dwExitCode);
}

BOOL winTimeToSecondsSince1970(const LARGE_INTEGER *Time, LPDWORD Seconds)
{
    int nanosecsPerSecs = 1/0.0000001; // what they call ticks per sec
    auto SECS_1601_TO_1970 = 11644473600LL;
    ULONGLONG tmp = Time->QuadPart / nanosecsPerSecs - SECS_1601_TO_1970;
    if (tmp > 0xffffffff) return false;
    *Seconds = (DWORD)tmp;
    return true;
}

BOOL FileTimeToSystemTime(              // convert FILETIME to SYSTEMTIME
        const FILETIME *lpFileTime,
        LPSYSTEMTIME   lpSystemTime
){
    unsigned long unixtime;
    struct tm *tm = nullptr;
    
    if(!winTimeToSecondsSince1970((const LARGE_INTEGER*)lpFileTime, (LPDWORD)&unixtime))
        return false;
    
    tm = gmtime((const time_t*)&unixtime);
    
    if(tm == nullptr)
        return false;
    
    lpSystemTime->wYear = tm->tm_year;
    lpSystemTime->wMonth = tm->tm_mon;
    lpSystemTime->wDay = tm->tm_mday;
    lpSystemTime->wHour = tm->tm_hour;
    lpSystemTime->wMinute = tm->tm_min;
    lpSystemTime->wSecond = tm->tm_sec;
    lpSystemTime->wMilliseconds = tm->tm_sec;
    lpSystemTime->wDayOfWeek = tm->tm_wday;
    
    return true;
}

BOOL FileTimeToDosDateTime(             // converts a Win32 file time to MS-DOS date/time
        const FILETIME *lpFileTime,
        LPWORD         lpFatDate,
        LPWORD         lpFatTime
){
    unsigned long unixtime;
    struct tm *tm = nullptr;
    
    if(!winTimeToSecondsSince1970((const LARGE_INTEGER*)lpFileTime, (LPDWORD)&unixtime))
        return false;
    
    tm = gmtime((const time_t*)&unixtime);
    
    if(tm == nullptr)
        return false;
    
    *lpFatTime = (tm->tm_hour << 11) + (tm->tm_min << 5) + (tm->tm_sec / 2);
    *lpFatDate = ((tm->tm_year - 80) << 9) + ((tm->tm_mon + 1) << 5) + tm->tm_mday;
    
    return true;
}

DWORD FormatMessage(                     // format a message
        DWORD   dwFlags,        // formatting options
        LPCVOID lpSource,       // location of message definition
        DWORD   dwMessageId,    // message identifier
        DWORD   dwLanguageId,   // language identifier
        LPTSTR  lpBuffer,       // pointer to buffer receiving string
        DWORD   nSize,          // buffer size
        va_list *Arguments      // pointer to list of);
){
    return 0;
}
ALIAS(FormatMessage, FormatMessageA);

BOOL FlushConsoleInputBuffer(    // flush the input buffer
  const HANDLE hConsoleInput            // standard input handle
){
    fflush((FILE*)hConsoleInput);
    return true;
}

LPSTR GetCommandLineA(void)        // returns pointer to command-line string
{
#ifdef __APPLE__
    std::vector<std::string> args;
    if (!getcmdline(args))
        return nullptr;
    
    auto cmdline =  std::unique_ptr<std::string>(new std::string());
    
    for (auto arg : args) {
        *cmdline += arg + " ";
    }
    
    return (char*) cmdline->c_str();
#endif
}
//ALIAS(GetCommandLine, GetCommandLineA);

UINT GetConsoleCP(void)        // returns code page ID in rax
{
    return 65001; // just return CP_UTF8.
}

BOOL GetConsoleCursorInfo(
        HANDLE hConsoleOutput, // console output handle
        LPCONSOLE_CURSOR_INFO lpConsoleCursorInfo  // cursor information
){
    if (lpConsoleCursorInfo != nullptr){
        lpConsoleCursorInfo->bVisible = 1;
        lpConsoleCursorInfo->dwSize = 100;
        return true;
    }
    return false;
}

BOOL GetConsoleMode(
        HANDLE hConsoleHandle,   // input or output handle
        LPDWORD lpMode           // pointer to dword containing flags
){
    int fd = fileno((FILE*)hConsoleHandle);
    if(fd < 3) return false;
    if (lpMode != nullptr){
        if (hConsoleHandle != nullptr){
            *lpMode = g_consoleModes[fd];
            return true;
        }
        *lpMode = 0;
    }
    return false;
}

int get_cursor_pos(int *y, int *x) {

 char buf[30]={0};
 int ret, i, pow;
 char ch;

*y = 0; *x = 0;

 struct termios term, restore;

 tcgetattr(0, &term);
 tcgetattr(0, &restore);
 term.c_lflag &= ~(ICANON|ECHO);
 tcsetattr(0, TCSANOW, &term);

 write(1, "\033[6n", 4);

 for( i = 0, ch = 0; ch != 'R'; i++ )
 {
    ret = (int) read(0, &ch, 1);
    if ( !ret ) {
       tcsetattr(0, TCSANOW, &restore);
//       fprintf(stderr, "getpos: error reading response!\n");
       return 1;
    }
    buf[i] = ch;
//    printf("buf[%d]: \t%c \t%d\n", i, ch, ch);
 }

 if (i < 2) {
    tcsetattr(0, TCSANOW, &restore);
    printf("i < 2\n");
    return(1);
 }

 for( i -= 2, pow = 1; buf[i] != ';'; i--, pow *= 10)
     *x = *x + ( buf[i] - '0' ) * pow;

 for( i-- , pow = 1; buf[i] != '['; i--, pow *= 10)
     *y = *y + ( buf[i] - '0' ) * pow;

 tcsetattr(0, TCSANOW, &restore);
 return 0;
}

BOOL GetConsoleScreenBufferInfo(
    HANDLE hConsoleOutput,        // handle to screen buffer
    LPCONSOLE_SCREEN_BUFFER_INFO lpConsoleScreenBufferInfo
){
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    lpConsoleScreenBufferInfo->dwSize.X = w.ws_col;
    lpConsoleScreenBufferInfo->dwSize.Y = w.ws_row;
    lpConsoleScreenBufferInfo->wAttributes = 0;
    return true;
}

BOOL GetFileTime(        // get date/time stamp of a file
    HANDLE hFile,
    LPFILETIME pCreationTime,
    LPFILETIME pLastAccessTime,
    LPFILETIME pLastWriteTime
);

SHORT GetKeyState(        // get state of keyboard keys (see Ch11\keybd.asm)
    INT nVirtKey
);


DWORD GetLastError()        // get most recent error return value
{
    return 0;
}

void GetLocalTime(        // system time, adjusted for local time zone
    LPSYSTEMTIME lpSystemTime        // ptr to SYSTEMTIME object
){
    return;
}


BOOL GetNumberOfConsoleInputEvents(    // get number of unread records
    HANDLE hConsoleInput,        // input handle
    LPDWORD lpCount        // pointer to counter
);

HANDLE GetStdHandle(                   // get standard handle
    DWORD nStdHandle          // type of console handle
){
    if (nStdHandle == STD_INPUT_HANDLE)
        return stdin;
    else if (nStdHandle == STD_OUTPUT_HANDLE)
        return stdout;
    else if (nStdHandle == STD_ERROR_HANDLE)
        return stderr;
    else return (HANDLE)-1;
}

void GetSystemTime(        // returns the system time
    LPSYSTEMTIME lpSystemTime        // ptr to SYSTEMTIME object
){
    time_t timet;
    time(&timet);
    struct tm *tm = gmtime(&timet);
    
    if (lpSystemTime != nullptr) {
        lpSystemTime->wYear = tm->tm_year;
        lpSystemTime->wMonth = tm->tm_mon;
        lpSystemTime->wDayOfWeek = tm->tm_wday;
        lpSystemTime->wDay = tm->tm_mday;
        lpSystemTime->wHour = tm->tm_hour;
        lpSystemTime->wMinute = tm->tm_min;
        lpSystemTime->wSecond = tm->tm_sec;
        lpSystemTime->wMilliseconds = tm->tm_sec / 1000;
    }
}

// get elapsed milliseconds since computer was turned on

ULONGLONG GetTickCount64(void)
{
    struct timeval boottime;
    size_t len = sizeof(boottime);
    int mib[2] = { CTL_KERN, KERN_BOOTTIME };
    if( sysctl(mib, 2, &boottime, &len, NULL, 0) < 0 )
    {
        return -1.0;
    }
    time_t bsec = boottime.tv_sec / 1000, csec = time(NULL) / 1000;

    return csec-bsec;
}

DWORD GetTickCount(void)
{
    return (unsigned)GetTickCount64();
}

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
){
    return 0;
}

// 0x00004000L (MB_HELP) is just an option not return value. We'll ignore it

static const int mboxReturns[7][3] = {
    {IDOK},                             // 0x00000000L (MB_OK)
    {IDOK, IDCANCEL},                   // 0x00000001L (MB_OKCANCEL)
    {IDABORT, IDRETRY, IDIGNORE},       // 0x00000002L (MB_ABORTRETRYIGNORE)
    {IDYES, IDNO, IDCANCEL},            // 0x00000003L (MB_YESNOCANCEL)
    {IDYES, IDNO},                      // 0x00000004L (MB_YESNO)
    {IDRETRY, IDCANCEL},                // 0x00000005L (MB_RETRYCANCEL)
    {IDCANCEL, IDTRYAGAIN, IDCONTINUE}, // 0x00000006L (MB_CANCELTRYCONTINUE)
};

static const CFStringRef mboxButtons[][3] = {
    { 0 },                                                     // 0x00000000L (MB_OK)
    { 0, CFSTR("Cancel"), 0 },                                 // 0x00000001L (MB_OKCANCEL)
    { CFSTR("Abort"), CFSTR("Retry"), CFSTR("Ignore") },       // 0x00000002L (MB_ABORTRETRYIGNORE)
    { CFSTR("Yes"), CFSTR("No"), CFSTR("Cancel") },            // 0x00000003L (MB_YESNOCANCEL)
    { CFSTR("Yes"), CFSTR("No"), 0 },                          // 0x00000004L (MB_YESNO)
    { CFSTR("Retry"), CFSTR("Cancel"), 0 },                    // 0x00000005L (MB_RETRYCANCEL)
    { CFSTR("Cancel"), CFSTR("Try Again"), CFSTR("Continue") } // 0x00000006L (MB_CANCELTRYCONTINUE)
};

// from stack overflow
#define VAL_1X(x)     (x)
#define VAL_2X(x)     VAL_1X((x)),  VAL_1X((x))
#define VAL_4X(x)     VAL_2X((x)),  VAL_2X((x))
#define VAL_8X(x)     VAL_4X((x)),  VAL_4X((x))
#define VAL_16X(x)    VAL_8X((x)),  VAL_8X((x))
#define VAL_32X(x)    VAL_16X((x)), VAL_16X((x))
#define VAL_64X(x)    VAL_32X((x)), VAL_32X((x))

// this is a sort of hash table.
// using union and struct together allows us to set first 0x10 elements to same val, next 0x10 vals to different val.
// so we can statically access mboxIconTypes.Type[0x10] or mboxIconTypes.Type[0x20].
// we add +1 to the list because array start from 0 not 1.
union MboxIconTypes {
    struct {
        int stop[17];
        int question[16];
        int exclamation[16];
        int information[16];
    } a;
    int Type[];
} mboxIconTypes = {
    .a = {
        { VAL_16X(kCFUserNotificationStopAlertLevel) }, // set position 0x10 to StopAlertLevel
        { VAL_16X(kCFUserNotificationPlainAlertLevel) }, // we have no equivalent for this one
        { VAL_16X(kCFUserNotificationCautionAlertLevel) },
        { VAL_16X(kCFUserNotificationNoteAlertLevel) }
    }
};

INT MessageBoxA(        // displays a pop-up message box
    HWND hWnd,        // handle to window (can be null)
    LPCSTR lpText,        // string for inside of dialog box
    LPCSTR lpCaption,        // string for dialog box title
    UINT uType        // contents and behavior of the dialog
){
    //convert the strings from char* to CFStringRef
    CFStringRef cfText = CFStringCreateWithCString(NULL, lpText, kCFStringEncodingUTF8);
    CFStringRef cfCaption = CFStringCreateWithCString(NULL, lpCaption, kCFStringEncodingUTF8);
    int buttonType = uType & MB_BUTTON_TYPE;
    CFOptionFlags result;  //result code from the message box
    unsigned iconType = uType & MB_ICON_TYPES;

//    printf("cf icon type = %d\n", mboxIconTypes.Type[iconType]);
    
    //launch the message box
    CFUserNotificationDisplayAlert(0x0000000, // no timeout
                                   mboxIconTypes.Type[iconType], // icon type
                                   NULL, // icon url
                                   NULL, // not used
                                   NULL, // string localization
                                   cfText, // lpText, or header
                                   cfCaption, // caption text, or body
                                   mboxButtons[buttonType][0], // default "ok" text in button
                                   mboxButtons[buttonType][1], // alternate button title
                                   mboxButtons[buttonType][2], // other button title, null--> no other button
                                   &result // response flag
                                   );
    
    //Clean up the strings
    CFRelease(cfText);
    CFRelease(cfCaption);

   return (int)mboxReturns[buttonType][result];
}

BOOL PeekConsoleInputA(
    HANDLE hConsoleInput,        // input handle
    PINPUT_RECORD lpBuffer,        // pointer to buffer
    DWORD nLength,            // number of chars to read
    LPDWORD lpNumberOfEventsRead       // ptr to number of events read
){
    if (nLength == 0){
        
    }
    return true;
    
//    let eventTap:CFMachPort = CGEvent.tapCreate(tap: .cghidEventTap, place: .headInsertEventTap, options: .defaultTap, eventsOfInterest: 1 << CGEventType.flagsChanged.rawValue, callback: myCGEventCallback, userInfo: nil)!
//
//           let runLoopSource:CFRunLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);
//
//           CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, CFRunLoopMode.commonModes);
//           CGEvent.tapEnable(tap: eventTap, enable: true);
//
//           CFRunLoopRun();
}
//ALIAS(PeekConsoleInput, PeekConsoleInputA);

//void printf(        // Std C library function
//       LPCSTR format, ...        // (in libc.lib)
//);

BOOL ReadConsoleA(
    HANDLE hConsoleInput,              // input handle
    LPVOID lpBuffer,                    // pointer to buffer
    DWORD nNumberOfBytesToRead,        // number of chars to read
    LPDWORD lpNumberOfCharsRead,      // ptr to number of bytes read
    LPVOID lpReserved                   // (not used)
){
    if (hConsoleInput != nullptr && lpBuffer != nullptr) {
        if(nNumberOfBytesToRead == 0)
            return true;
        if (fgets((char*)lpBuffer, nNumberOfBytesToRead, (FILE*)hConsoleInput) != nullptr){
            if (lpNumberOfCharsRead != nullptr)
                *lpNumberOfCharsRead = (unsigned)strlen((char*)lpBuffer);
            return true;
        }
    }
    return false;
}
//ALIAS(ReadConsole, ReadConsoleA);

BOOL ReadConsoleInputA(
    HANDLE hConsoleInput,        // input handle
    PINPUT_RECORD lpBuffer,        // ptr to input record
    DWORD nLength,        // request number of recs
    LPDWORD lpNumberOfEventsRead    // ptr to number of bytes read
){
    return true;
}
//ALIAS(ReadConsoleInput, ReadConsoleInputA);
                 
BOOL ReadFile(        // read buffer from input file
    HANDLE hFile,        // input handle
    LPVOID lpBuffer,        // ptr to buffer
    DWORD nNumberOfBytesToRead,        // num bytes to read
    LPDWORD lpNumberOfBytesRead,    // bytes actually read
    LPOVERLAPPED lpOverlapped        // ptr to asynchronous info
){
    size_t bread = 0;
    if (hFile != nullptr && lpBuffer != nullptr) {
        if(nNumberOfBytesToRead == 0)
            return true;
        bread = fread((void*)lpBuffer, nNumberOfBytesToRead, 1, (FILE*)hFile);
        if (bread >= 0){
            if (lpNumberOfBytesRead != nullptr)
                *lpNumberOfBytesRead = (unsigned)bread;
            return true;
        }
    }
    return false;
}

//scanf PROTO C,        // Std C library function
//    format:PTR BYTE, args:VARARG        // (in libc.lib)

BOOL SetConsoleCursorInfo(
    HANDLE hConsoleOutput,         // output handle
    LPCONSOLE_CURSOR_INFO lpConsoleCursorInfo    // cursor information
);

BOOL SetConsoleCursorPosition(
    HANDLE hConsoleOutput,        // output handle
    COORD dwCursorPosition        // screen X,Y coordinates
){
    return true;
}

BOOL SetConsoleMode(
    HANDLE hConsoleHandle,        // output handle
    DWORD dwMode        // console mode flags
){
    int fd = fileno((FILE*)hConsoleHandle);
    if(fd < 3) return false;
    if (hConsoleHandle != nullptr){
        g_consoleModes[fd] = dwMode;
        return true;
    }
     return false;
}

BOOL SetConsoleScreenBufferSize(
    HANDLE hConsoleOutput,        // handle to screen buffer
    COORD dwSize        // new screen buffer size
);

BOOL SetConsoleTextAttribute(
    HANDLE hConsoleOutput,        // output handle
    WORD wAttributes        // color attribute (fixed 6/20/05)
){
    return true;
}

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
){
    struct timespec ts = { 0, dwMilliseconds*1000000};
    if (dwMilliseconds == 0) {
        sched_yield();
    } else if (dwMilliseconds < 0){
        sleep(123456789);
    }
    nanosleep(&ts, nullptr);
}

BOOL SystemTimeToFileTime(        // Convert a SYSTEMTIME structure
    const SYSTEMTIME* lpSystemTime,        // to a FILETIME structure
    LPFILETIME lpFileTime
){
    return true;
}

BOOL WriteConsoleA(        // write a buffer to the console
    HANDLE hConsoleOutput,        // output handle
    const void* lpBuffer,        // pointer to buffer
    DWORD nNumberOfCharsToWrite,        // size of buffer
    LPDWORD lpNumberOfCharsWritten,        // ptr to number of bytes written
    void* lpReserved        // (not used)
){
    int cwritten = 0;
    if (hConsoleOutput != nullptr && lpBuffer != nullptr) {
        if(nNumberOfCharsToWrite == 0)
            return true;
        cwritten = fputs((const char*)lpBuffer, (FILE*)hConsoleOutput);
        if (cwritten >= 0){
            if (lpNumberOfCharsWritten != nullptr)
                *lpNumberOfCharsWritten = cwritten;
            return true;
        }
    }
    return false;
}
//ALIAS(WriteConsole, WriteConsoleA);
             
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
){
    size_t bwritten = 0;
    
    if(lpBuffer != nullptr && hFile != nullptr){
        bwritten = fwrite(lpBuffer, nNumberOfBytesToWrite, 1, (FILE*)hFile);
        if (bwritten >= 0){
            if (lpNumberOfBytesWritten != nullptr)
                *lpNumberOfBytesWritten = (unsigned)bwritten;
            return true;
        }
        // negative return value goes to false;
    }
    
    return false;
}


} // extern "C"

