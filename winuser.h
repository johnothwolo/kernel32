//
//  winuser.h
//  kernel32
//
//  Created by John Othwolo on 4/17/22.
//  Copyright © 2022 John Othwolo. All rights reserved.
//

#ifndef winuser_h
#define winuser_h

// taken from https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-messageboxa

// button type
enum {
    MB_OK                   = 0x00000000L,
    MB_OKCANCEL             = 0x00000001L,
    MB_ABORTRETRYIGNORE     = 0x00000002L,
    MB_YESNOCANCEL          = 0x00000003L,
    MB_YESNO                = 0x00000004L,
    MB_RETRYCANCEL          = 0x00000005L,
    MB_CANCELTRYCONTINUE    = 0x00000006L,
    MB_BUTTON_TYPE          = MB_ABORTRETRYIGNORE | MB_CANCELTRYCONTINUE | MB_OK |
                              MB_OKCANCEL | MB_RETRYCANCEL | MB_YESNO | MB_YESNOCANCEL,
    MB_HELP                 = 0x00004000L
};

enum {
    IDOK            = 1,
    IDCANCEL        = 2,
    IDABORT         = 3,
    IDRETRY         = 4,
    IDIGNORE        = 5,
    IDYES           = 6,
    IDNO            = 7,
    IDTRYAGAIN      = 10,
    IDCONTINUE      = 11,
};


// icon type
enum {
    MB_ICONSTOP                = 0x00000010L,
    MB_ICONERROR               = MB_ICONSTOP,
    MB_ICONHAND                = MB_ICONSTOP,
    MB_ICONQUESTION            = 0x00000020L,
    MB_ICONEXCLAMATION         = 0x00000030L,
    MB_ICONWARNING             = MB_ICONEXCLAMATION,
    MB_ICONINFORMATION         = 0x00000040L,
    MB_ICONASTERISK            = MB_ICONINFORMATION,
    MB_ICON_TYPES              = MB_ICONSTOP | MB_ICONQUESTION | MB_ICONEXCLAMATION | MB_ICONINFORMATION,
};

// default highlighted button
#define MB_DEFBUTTON1              0x00000000L
#define MB_DEFBUTTON2              0x00000100L
#define MB_DEFBUTTON3              0x00000200L
#define MB_DEFBUTTON4              0x00000300L


#endif /* winuser_h */
