//
//  wincon.h
//  kernel32
//
//  Created by John Othwolo on 4/11/22.
//  Copyright © 2022 John Othwolo. All rights reserved.
//

#ifndef wincon_h
#define wincon_h

/* Console Mode flags */
#define ENABLE_PROCESSED_INPUT 0x0001
#define ENABLE_LINE_INPUT      0x0002
#define ENABLE_ECHO_INPUT      0x0004
#define ENABLE_WINDOW_INPUT    0x0008
#define ENABLE_MOUSE_INPUT     0x0010
#define ENABLE_INSERT_MODE     0x0020
#define ENABLE_QUICK_EDIT_MODE 0x0040
#define ENABLE_EXTENDED_FLAGS  0x0080
#define ENABLE_AUTO_POSITION   0x0100

#define ENABLE_PROCESSED_OUTPUT   0x01
#define ENABLE_WRAP_AT_EOL_OUTPUT 0x02

#endif /* wincon_h */
