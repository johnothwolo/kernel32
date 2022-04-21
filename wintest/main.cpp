//
//  main.cpp
//  winperun
//
//  Created by John Othwolo on 2/14/22.
//  Copyright © 2022 John Othwolo. All rights reserved.
//

#include <iostream>
#include <algorithm>
#include <dlfcn.h>

#include "kernel32.h"
#include "winuser.h"

// test some win32 functions. Not all are implemented yet.

int main(int argc, char** argv)
{
    char buffer[1024];
    unsigned bread, bwrit;
    HANDLE outh, inh;
    
    inh = GetStdHandle(STD_INPUT_HANDLE);
    outh = GetStdHandle(STD_OUTPUT_HANDLE);
    
    WriteConsoleA(outh, "Type anything: ", sizeof("Type anything: "), &bwrit, nullptr);
    
    ReadConsoleA(inh, &buffer[0], sizeof(buffer), &bread, nullptr);
    
    MessageBoxA((void*)stdout, "You typed in:", buffer, MB_OK + MB_ICONEXCLAMATION);
    
    printf("%s", buffer);

    return 0;
}
