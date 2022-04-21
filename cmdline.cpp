//
//  cmdline.cpp
//  kernel32
//
//  Created by John Othwolo on 1/12/22.
//  Copyright © 2022 John Othwolo. All rights reserved.
//

#include "cmdline.h"

#include <iostream>
#include <unistd.h>
#include <sys/sysctl.h>

static unsigned int GetProcMaxArgs() {
    int mib[3], argmax;
    size_t size;
    /* Get the maximum process arguments size. */
    mib[0] = CTL_KERN;
    mib[1] = KERN_ARGMAX;

    size = sizeof(argmax);
    
    if (sysctl(mib, 2, &argmax, &size, NULL, 0) == -1) {
        return -1;
    } else {
        return argmax;
    }
}


static
std::unique_ptr<struct kinfo_proc> getKprocInfo(){
    struct kinfo_proc *info = nullptr;
    size_t bufSize = 0;
    int mib[4] = {CTL_KERN, KERN_PROC, KERN_PROC_PID, getpid()};

    if (sysctl(&mib[0], 4, nullptr, &bufSize, NULL, 0) < 0) {
        perror("Failure calling sysctl");
        return 0;
    }

    info = (struct kinfo_proc*) malloc(bufSize);

    if (sysctl(&mib[0], 4, info, &bufSize, NULL, 0) < 0) {
        perror("Failure calling sysctl");
        return nullptr;
    }
    
    return std::unique_ptr<struct kinfo_proc>(info);
}


std::vector<struct kinfo_proc> getKprocGroupInfo(){
//    std::shared_ptr<struct kinfo_proc> infoBuf = nullptr;
    size_t bufSize = 0;
    int mib[4] = {CTL_KERN, KERN_PROC, KERN_PROC_PGRP, getpgid(0)};
    std::vector<struct kinfo_proc> results;
    int nentries;
    
    if (sysctl(&mib[0], 4, nullptr, &bufSize, NULL, 0) < 0) {
        perror("Failure calling sysctl");
        return results;
    }
    
    nentries = (int)(bufSize / sizeof(struct kinfo_proc));
//    infoBuf = std::shared_ptr<struct kinfo_proc>((struct kinfo_proc*)malloc(bufSize), free);

    struct kinfo_proc infoBuf[nentries];
    
    if (sysctl(&mib[0], 4, infoBuf, &bufSize, NULL, 0) < 0) {
        perror("Failure calling sysctl");
        return results;
    }
    
    for (auto &proc : infoBuf) {
        results.push_back(proc);
    }
    
//    struct kinfo_proc *kp = nullptr;
//    for (int i = 0; i < nentries; i++){
//        kp = infoBuf;
//        std::cout << kp->kp_proc.p_comm << std::endl;
//        results.push_back(std::make_shared<struct kinfo_proc>(*kp));
//    }
//    free(infoBuf); // smart pointer will free?
    
    return results;
}

bool getcmdline(std::vector<std::string> &argv){
    int mib[4], nargs;
    char *sp, *cp;
    auto maxProcArgs = GetProcMaxArgs();
    char procargs[maxProcArgs];
    auto procinfo = getKprocInfo();
    
    mib[0] = CTL_KERN;
    mib[1] = KERN_PROCARGS2;
    mib[2] = getpid();
    
    if (sysctl(mib, 3, procargs, (size_t*)&maxProcArgs, NULL, 0) == -1)
        return false;
    
    memcpy(&nargs, procargs, sizeof(nargs));
    cp = procargs + sizeof(nargs);

    /* Skip the saved exec_path. */
    for (; cp < &procargs[maxProcArgs]; cp++) {
        if (*cp == '\0') {
            /* End of exec_path reached. */
            break;
        }
    }
    if (cp == &procargs[maxProcArgs]) {
        return false;
    }
    
    /* Skip trailing '\0' characters. */
    for (; cp < &procargs[maxProcArgs]; cp++) {
        if (*cp != '\0') {
            /* Beginning of first argument reached. */
            break;
        }
    }
    if (cp == &procargs[maxProcArgs]) {
        return false;
    }
    
    /* Save where the argv[0] string starts. */
    sp = cp;

    /*
     * Iterate through the '\0'-terminated strings and insert into vector
     */
    char *argstartp = cp, *argendp = cp; // p for pointer
    
    // loop number of 'narg' times and push strings into vector.
    for (int i = 0; i < nargs; i++) {
        if (*argendp != '\0')
            argv.push_back(std::string(argstartp));
        
        // search for null term. (the end of the arg).
        while (*argendp != '\0' && argendp < &procargs[maxProcArgs])
            argendp++;
        
        // skip null terminator(s) and move onto next arg
        while (*argendp == '\0' && argendp < &procargs[maxProcArgs])
            argendp++;
        
        argstartp = argendp; // set new pointer position for next arg.
    }
    
    return true;
}
