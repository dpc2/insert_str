
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#include <time.h>
#include <errno.h>


#if 0
#define DEBUG  							do{printf("--%-40s--%5d--%-10s--\n", __FUNCTION__, __LINE__, __FILE__); }while(0)
#else
#define DEBUG
#endif

#define LOG(fmt, args...)			do{printf("--%-40s--%5d--%-10s--" fmt "\n", __FUNCTION__, __LINE__, __FILE__, ##args); }while(0)
#define LOG_ERROR(fmt, args...)		do{LOG(fmt, ##args); perror("[err]"); fflush(stdout); fflush(stderr); }while(0)

#define MAX_LINE_OF_FILE 			10000
#define STRLEN 						2048
#define LINE_LEN 					20000

typedef struct
{
	char lines[MAX_LINE_OF_FILE][STRLEN];		// file context
	int total;

	int total_comment;
	int matched_times[MAX_LINE_OF_FILE];
} file_t;




