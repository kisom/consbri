/*
 * Copyright (c) 2011, 2012 Kyle Isom <kyle@tyrfingr.is>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <sys/types.h>
#include <dirent.h>
#include <err.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SYSFS_BRIGHTNESS_CLASS  "/sys/class/backlight"


/*
 * sure would be nice if there was some fucking documentation indicating
 * what type to use, so I'm going to guess and fuck you if your system
 * has too high of a max_brightness.
 */
static  unsigned int get_max_brightness(void);


/*
 * consbri sets the backlight brightness in Linux, using
 * the sysfs subsystem.
 */
int
main(int argc, char *argv[])
{
        int     max_bright = 0;

        max_bright = get_max_brightness();

        return EXIT_SUCCESS;
}


/*
 * load the maximum brightness from the sysfs subsystem
 */
unsigned int
get_max_brightness()
{
        DIR             *dirp;
        struct dirent   *restrict entry;

        dirp = opendir(SYSFS_BRIGHTNESS_CLASS);
        if (NULL == dirp)
                return 0;

        if (!closedir(dirp))
                warn("error closing directory pointer");
        while (NULL != (entry = readdir(dirp))) {
                if (0 == strncmp(entry->d_name, "..", PATH_MAX))
                        continue;
                else if (0 == strncmp(entry->d_name, ".", PATH_MAX))
                        continue;
                else
                        printf("dirname: %s\n", entry->d_name);
        }
        return 0;
}

