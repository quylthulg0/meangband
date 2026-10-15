/**
 * \file z-debug.c
 * \brief Debug utilities
 *
 * Copyright (c) 2026 quylthulg
 *
 * This work is free software; you can redistribute it and/or modify it
 * under the terms of either:
 *
 * a) the GNU General Public License as published by the Free Software
 *    Foundation, version 2, or
 *
 * b) the "Angband licence":
 *    This software may be copied and distributed for educational, research,
 *    and not for profit purposes provided that this copyright and statement
 *    are included in all such copies.  Other copyrights may also apply.
 */

#include "h-basic.h"
#include "z-debug.h"

#include "z-file.h"
#include "z-util.h"

#define MAX_OPENED_FILES 16
#define MAX_PATH_LEN 256
#define MAX_FILENAME_LEN 16


static size_t real_filename(char *buf, const char *name, size_t bufsize)
{
    const char exten[] = ".log";
    size_t result;

    result = my_strcpy(buf, name, bufsize - sizeof exten);
    result = my_strcat(buf, exten, bufsize);

    return result;
}

static void dbg_log_put(const char *path, const char *message, bool exists)
{
    file_mode mode = exists ? MODE_APPEND : MODE_WRITE;
    ang_file *f;

    f = file_open(path, mode, FTYPE_TEXT);

    assert(f);

    file_putf(f, "%s\n", message);

    file_close(f);
}

void dbg_log_prep(const char *filepath, const char *filename, const char *message)
{
    static char opened_files[MAX_OPENED_FILES][MAX_FILENAME_LEN] = { "" };
    char path_name[MAX_PATH_LEN] = "", real_fname[MAX_FILENAME_LEN] = "";
    size_t i;
    bool exists = false;

    assert(filepath && filename);

    real_filename(real_fname, filename, sizeof real_fname);

    // see if we've opened this file already this session
    for (i = 0; i < MAX_OPENED_FILES; ++i) {
        if (!(opened_files[i][0])) {
            my_strcpy(opened_files[i], real_fname, sizeof opened_files[i]);
            break;
        }
        else if (streq(opened_files[i], real_fname)) {
            exists = true;
            break;
        }
    }

    assert(i < MAX_OPENED_FILES);

    path_build(path_name, sizeof path_name, filepath, real_fname);

    dbg_log_put(path_name, message, exists);
}

 