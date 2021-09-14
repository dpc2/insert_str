#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdarg.h>

static void read_env(char **valid_path)
{
    static bool first_time = true;
    if (!first_time)
        return;
    first_time = false;

    char *r;
    char *env_and_path[] = {
    "print_src_gallium_drivers_zink", "src/gallium/drivers/zink",
    "print_src_gallium_drivers_llvmpipe", "src/gallium/drivers/llvmpipe",
    "print_src_gallium_frontends", "src/gallium/frontends",
    "print_src_gallium_auxiliary", "src/gallium/auxiliary",
    "print_src_vulkan", "src/vulkan",
    "print_src_mesa_main", "src/mesa/main",
    "print_src_mesa_vbo", "src/mesa/vbo",
    "print_src_mesa_state_tracker", "src/mesa/state_tracker",
    "print_src_glx", "src/glx", };

    for (int i = 0; i < sizeof(env_and_path); i += 2)
    {
        if ((r = getenv(env_and_path[i])) && !strcmp(r, "1"))
            *valid_path++ = env_and_path[i + 1];
    }

//   for (int i = 0; i < items; i++)
//   {
//      printf("ar[%d]: %s\n", i, ar[i]);
//   }

    return;
}

static void my_print(const char *func, int line, const char *path, char *fmt, ...)
{
//   printf("%s %d %s \n", func, line, file);

    static char *valid_path[100] = {};
    read_env(valid_path);

    bool print = false;
    for (int i = 0; i < 100 && valid_path[i]; i++)
    {
        if (strstr(path, valid_path[i]))
        {
            print = true;
            break;
        }
    }

    if (!print)
        return;

    char str[4096] = {0};
    sprintf(str, "--%-40s--%5d--%-30s-- ", func, line, path);

    va_list args;
    va_start(args, fmt);
    vsnprintf(str + strlen(str), 4096 - 1 - strlen(str), fmt, args);
    va_end(args);

    sprintf(str + strlen(str), "\n");
    fputs(str, stdout);
    fflush(stdout);
}
