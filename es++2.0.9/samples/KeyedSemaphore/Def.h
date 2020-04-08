// Def.h

#pragma once

#define SHM_KEY_FILENAME "/dev/null"
#define SHM_KEY_ID       1

#define SEM_KEY_FILENAME "/dev/null"
#define SEM_KEY_ID       128

typedef struct {
    int  value;
    char name[128];
} Table;


