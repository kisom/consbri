/*
 * Copyright (c) 2013 Kyle Isom <kyle@tyrfingr.is>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA
 * OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 * ---------------------------------------------------------------------
 */

#include <sys/types.h>
#include <linux/limits.h>
#include <dirent.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SYSFS_BRIGHTNESS_CLASS  "/sys/class/backlight"
#define MAX_BRIGHTNESS_SIZE     10


static char     *get_backlight_class(void);
static double    get_max_brightness(char *);
static int       write_brightness(char *, double);
static double    get_brightness(char *);


/*
 * consbri is a console utility for setting the brightness on Linux
 * systems.
 */
int main(int argc, char *argv[])
{
        char    *backlight = NULL;              /* store backlight dir */
        double   pct = 0.0;
        double   bright = 0.0;

        backlight = get_backlight_class();
        bright = get_max_brightness(backlight);
        if (1 == argc) {
                double   cur = 0.0;
                cur = get_brightness(backlight);
                pct = (cur / bright) * 100.0;
                printf("backlight is at %0.0f%% brightness.\n", pct);
        } else {
                pct = strtod(argv[1], NULL) / 100.0;
                if (pct < 0 || pct > 100) {
                        fprintf(stderr, "[!] invalid brightness setting.\n");
                        return EXIT_FAILURE;
                }
                if (write_brightness(backlight, (pct * bright)))
                        fprintf(stderr, "failed.\n");
        }

        free(backlight);
        return EXIT_SUCCESS;
}


/*
 * get_backlight_class is used to determine the backlight class
 * directory in the sysfs filesystem. The caller is responsible
 * for freeing the returned character array.
 */
char *get_backlight_class()
{
        DIR             *dirp = NULL;
        struct dirent   *entry = NULL;
        char            *backlight = NULL;


        dirp = opendir(SYSFS_BRIGHTNESS_CLASS);
        if (NULL == dirp)
                goto backlight_cleanup;

        backlight = (char *)malloc(sizeof(*backlight) * PATH_MAX + 1);
        if (NULL == backlight)
                goto backlight_cleanup;
        memset(backlight, 0x0, PATH_MAX);

        while (NULL != (entry = readdir(dirp))) {
                if (0 == strncmp(entry->d_name, "..", PATH_MAX))
                        continue;
                else if (0 == strncmp(entry->d_name, ".", PATH_MAX))
                        continue;
                else
                        break;
        }
        if (NULL != entry)
                snprintf(backlight, PATH_MAX, "%s/%s",
                         SYSFS_BRIGHTNESS_CLASS, entry->d_name);
backlight_cleanup:
        if (NULL != dirp && 0 != closedir(dirp))
                warn("failed to close directory");
        if (backlight == NULL || strlen(backlight) == 0) {
                free(backlight);
                backlight = NULL;
        }
        return backlight;
}


/*
 * get_max_brightness returns the maximum brightness for the given
 * LCD backlight.
 */
double get_max_brightness(char *backlight)
{
        double   max_bright = 0.0;
        char     buf[MAX_BRIGHTNESS_SIZE + 1];
        char     fname[PATH_MAX + 1];
        FILE    *max_bright_file = NULL;

        memset(buf, 0x0, MAX_BRIGHTNESS_SIZE + 1);
        memset(fname, 0x0, PATH_MAX + 1);
        snprintf(fname, PATH_MAX, "%s/max_brightness", backlight);

        max_bright_file = fopen(fname, "r");
        if (NULL == max_bright_file || 0 != ferror(max_bright_file))
                return max_bright;

        if (0 == fread(buf, sizeof(buf[0]), MAX_BRIGHTNESS_SIZE,
                       max_bright_file))
                warn("failed to read max brightness");
        else
                max_bright = strtod(buf, NULL);

        if (0 != fclose(max_bright_file))
                warn("failed to close max_brightness file");
        return max_bright;
}


/*
 * write brightness writes the desired brightness to the
 * sysfs backlight file.
 */
int write_brightness(char *backlight, double brightness)
{
        char     buf[MAX_BRIGHTNESS_SIZE + 1];
        char     fname[PATH_MAX + 1];
        int      retval = 0;
        FILE    *brightness_file;

        memset(buf, 0x0, MAX_BRIGHTNESS_SIZE + 1);
        memset(fname, 0x0, PATH_MAX + 1);

        snprintf(fname, PATH_MAX, "%s/brightness", backlight);
        brightness_file = fopen(fname, "w");
        if (NULL == brightness_file || 0 != ferror(brightness_file)) {
                warn("failed to write brightness");
                return -1;
        }

        snprintf(buf, MAX_BRIGHTNESS_SIZE, "%u", (unsigned)brightness);
        if (strlen(buf) != fwrite(buf, sizeof(buf[0]),
                                  strlen(buf), brightness_file)) {
                warn("failed to write brightness");
                retval = -1;
        }

        if (0 != fclose(brightness_file)) {
                warn("failed to close brightness file");
                retval = -1;
        }
        return retval;
}


/*
 * get_brightness returns the current brightness level
 */
double get_brightness(char *backlight)
{
        double   max_bright = 0.0;
        char     buf[MAX_BRIGHTNESS_SIZE + 1];
        char     fname[PATH_MAX + 1];
        FILE    *max_bright_file = NULL;

        memset(buf, 0x0, MAX_BRIGHTNESS_SIZE + 1);
        memset(fname, 0x0, PATH_MAX + 1);
        snprintf(fname, PATH_MAX, "%s/brightness", backlight);

        max_bright_file = fopen(fname, "r");
        if (NULL == max_bright_file || 0 != ferror(max_bright_file))
                return max_bright;

        if (0 == fread(buf, sizeof(buf[0]), MAX_BRIGHTNESS_SIZE,
                       max_bright_file))
                warn("failed to read max brightness");
        else
                max_bright = strtod(buf, NULL);

        if (0 != fclose(max_bright_file))
                warn("failed to close max_brightness file");
        return max_bright;
}


