/*
 */

#include "includes.h"

static int my_getc(FILE *fp)
{
	int ch = fgetc(fp);
//	putchar(ch);
	return ch;
}

static bool in_string(char ch, const char string[])
{
	int i;
	for (i = 0; i < strlen(string); i++)
	{
		if (ch == string[i])
		{
			return true;
		}
	}

	return false;
}

static char* delete_char_at_both_ends(char *target_str,
		const char useless_char_at_head[], const char useless_char_at_tail[])
{
	// '#'应保留,写到with_args_file, 读with_args_file时,遇到'#'时,忽略此行
	int i;
	char *p = target_str;

	// look for ' ' and '\n' from [0] to [end]
	for (i = 0; i < strlen(target_str); i++)
	{
		if (in_string(target_str[i], useless_char_at_head))
		{
			// continue;
		}
		else
		{
			break;
		}
//		if(isalnum(target_str[i])) break;_
	}
	p = &target_str[i];

	/*************************************************************************************/
	// look for ' ' and '\n' blank from [end] to [0]
	for (i = strlen(target_str) - 1; i > -1; i--)
	{
		if (in_string(target_str[i], useless_char_at_tail))
		{
			target_str[i] = 0;
			continue;
		}
		else
		{
			break;
		}
	}

	return p;
}

//// whole match
//bool word_whole_match_string__c_file(char str[], char *word)
//{
//	char *p = strstr(str, word);
//	if (!p)
//	{
//		return false;
//	}
//
//	static char blank_char[] =
//	{ '_', 0 };
//	int l = strlen(word);
//	int L = strlen(str);
//	/**************** 	p != false, matched ******************/    // whole match
//	// 在左边 或 p[-1]不是数字字母下划线
//	// 在右边 或 p[l]不是数字字母下划线
//	if ((p == str || (!isalnum(p[-1]) && !in_string(p[-1], blank_char)))
//			&& (p + l == str + L
//					|| (!isalnum(p[l]) && !in_string(p[l], blank_char))))
//	{
//		return true;
//	}
//
//	return false;
//}
//
//bool word_whole_match_string__file_name(char str[], char *word)
//{
//	char *p = strstr(str, word);
//	if (!p)
//	{
//		return false;
//	}
//
//	static char blank_char[] =
//	{ '_', '-', '.', 0 };
//	int l = strlen(word);
//	int L = strlen(str);
//	/**************** 	p != false, matched ******************/    // whole match
//	if ((p == str || (!isalnum(p[-1]) && !in_string(p[-1], blank_char)))
//			&& (p + l == str + L
//					|| (!isalnum(p[l]) && !in_string(p[l], blank_char))))
//	{
//		return true;
//	}
//
//	return false;
//}

static int case_macro(FILE *pfs, FILE *pft)
{
	int ch = '#';
	fputc(ch, pft);
	int last_ch = ch;

	while (EOF != (ch = my_getc(pfs)))
	{
		DEBUG;
		fputc(ch, pft);
		if (ch == '\n' && last_ch != '\\') // '\\' + '\n'-----------'\\' + ' ' + '\n' ..............................
		{
			return 1;
		}
		last_ch = ch;
	}

	LOG_ERROR();
	return -1;
}

// maybe // or /**/ or /= or /
static int case_comment(FILE *pfs, FILE *pft)
{
	//                  \
	sdsdsd
	int /**/a/**//**/= /**//**/10/**// /**/4/**/;

	int last_ch = '/';
	int ch = my_getc(pfs);

	if (ch == '/')        // "//"        // read until '\n'
	{
		fputc(last_ch, pft);
		fputc(ch, pft);

		while (EOF != (ch = my_getc(pfs)))
		{
			DEBUG;
			fputc(ch, pft);
			if (ch == '\n' && last_ch != '\\') // '\\' + '\n'-----------'\\' + ' ' + '\n'
			{
				return 1;
			}
			last_ch = ch;
		}
		LOG_ERROR();
		return -1;
	}

	if (ch == '*')			//  "/* */"		// read until "*/"
	{
		fputc(last_ch, pft);
		fputc(ch, pft);

		while (EOF != (ch = my_getc(pfs)))
		{
			DEBUG;
			fputc(ch, pft);
			if (ch == '/' && last_ch == '*')
			{
				return true;
			}
			last_ch = ch;
		}
		LOG_ERROR();
		return -1;
	}

	// /=
	// / 除法符号
	ungetc(ch, pfs);
	return 0;
}

static int case_string(FILE *pfs, FILE *pft)
{
	int ch = '"';
	fputc(ch, pft);
	int last_ch = ch;

	while (EOF != (ch = my_getc(pfs)))
	{
		fputc(ch, pft);
		if (ch == '"' && last_ch != '\\')
		{
			return 1;
		}
		last_ch = ch;

		" 'd' '\t'   \\   \'  '''''  '    \"  ";
		'\t';
		'\'';
		'"';
		'\"';
	}

	LOG_ERROR();
	return -1;
}

static int case_char(FILE *pfs, FILE *pft)
{
	int ch = '\'';
	fputc(ch, pft);
	int last_ch = ch;

	while (EOF != (ch = my_getc(pfs)))
	{
		fputc(ch, pft);
		if (ch == '\'' && last_ch != '\\')
		{
			return 1;
		}
		last_ch = ch;
	}

	LOG_ERROR();
	return -1;
}

// return 1: is macro/comment/string/char and no error
// return 0: is not macro/comment/string/char and no error
// return -1： error occured
static int check_macro_and_comment_and_string_and_char(int ch, FILE *pfs, FILE *pft)
{
	switch (ch)
	{
	case '#':
		return case_macro(pfs, pft);		// [#]

	case '/':
		return case_comment(pfs, pft);		// [/] 如果不是注释， 不做任何处理，也不ungetc()

	case '"':
		return case_string(pfs, pft);        // ["]

	case '\'':
		return case_char(pfs, pft);        // [']

	default:
		return 0;
	}
}

static int read_until_matched_bracket(FILE *pfs, FILE *pft, int left_bracket,
		int left_bracket_base_num, int right_bracket)
{
	int ch;
	int t = left_bracket_base_num;

	while (EOF != (ch = my_getc(pfs)))
	{
		int r = check_macro_and_comment_and_string_and_char(ch, pfs, pft);
		if (r > 0)
			continue;
		else if (r < 0)
			return r;

		fputc(ch, pft);

		if (ch == left_bracket)
			t++;
		if (ch == right_bracket)
			t--;
		if (0 == t)
			return 1;
	}

	// can not find matched_bracket
	LOG_ERROR();
	return -1;
}

static int insert_string(FILE *pfs, FILE *pft, char *s2)
{
	if (strlen(s2) != fwrite(s2, 1, strlen(s2), pft))
	{
		LOG("ERR");
		exit(1);
	}
	return read_until_matched_bracket(pfs, pft, '{', 1, '}');
}

static int insert_str_to_one_file(char *src_file, char *macro_str, char *str)
{
	DEBUG;
	if(!src_file || !macro_str || !str)
	{
		LOG("err");
		exit(1);
	}

	FILE *pf_source = fopen(src_file, "r");
	if (!pf_source)
	{
		perror("error: \n");
		LOG("err, open file: %s", src_file);
		exit(1);
	}

	char temp_file[STRLEN];
	sprintf(temp_file, "%s_temp", src_file);

	FILE *pf_target = fopen(temp_file, "w");
	if (!pf_target)
	{
		perror("error: \n");
		LOG("err");
		exit(1);
	}

	if (strlen(macro_str) != fwrite(macro_str, 1, strlen(macro_str), pf_target))
	{
		LOG("ERR");
		exit(1);
	}

	/****************************************************************************************/
	int t = 0;
	static int T = 0;
	const char blank_char[] = { ' ', '\t', '\n', ')', 0 };
	int ch;
	enum
	{
		STEP_0,        // is looking for '#'
		STEP_1,        // is looking for ')'
		STEP_2,        // is looking for '{'
		STEP_3			// is looking for ';'
	};
	int step = STEP_1;
	bool do_what_you_want = false;
	bool error_occured = false;

	while (EOF != (ch = my_getc(pf_source)))
	{
		int r = check_macro_and_comment_and_string_and_char(ch, pf_source, pf_target);
		if (r > 0)
		{
			continue;
		}
		else if (r < 0)
		{
			error_occured = true;
			break;
		}

		switch (step)
		{
		case STEP_1:
			if (ch == ')')
			{
				step = STEP_2;
				DEBUG;
			}
			break;

		case STEP_2:
			switch (ch)
			{
			case ' ':
			case '\t':
			case '\n':
			case ')':
				// do nothing
				break;

			case ';':
			case '}':
				step = STEP_1;
				DEBUG;
				break;

			case '{':        // now is probably the start of a function
				do_what_you_want = true;
				step = STEP_1;
				DEBUG;
				t++;
				T++;
				break;
				// ??? if(*)...    while(*)...	小括号后面有实体字符，-> look for ‘；’
			default:
				step = STEP_1;
				break;
			}
			break;
		default:
			LOG("ERR");
			exit(1);
		}

		fputc(ch, pf_target);
		if (do_what_you_want)
		{
			DEBUG;
			if(insert_string(pf_source, pf_target, str) < 0)
			{
				error_occured = true;
				break;
			}
			do_what_you_want = false;
		}
	}

	fflush(pf_target);
	fclose(pf_source);
	fclose(pf_target);
	char cmd[STRLEN * 2];

	if (error_occured)
	{
		LOG("error occured in %s \n", src_file);
		sprintf(cmd, "rm %s", temp_file);
		system(cmd);
		sprintf(cmd, "echo %s >> errlog", src_file);
		system(cmd);
		sleep(1);
	}
	else
	{
		LOG("t: %d, T: %d", t, T);
		if (0 == t)
		{
			sprintf(cmd, "rm %s", temp_file);
			system(cmd);
			return 0;
		}

		sprintf(cmd, "mv %s %s", temp_file, src_file);
		system(cmd);
	}

	return 0;
}

int insert_str(char *all_source_file, char *macro_str, char *insert_str)
{
	DEBUG;
	FILE *pf = fopen(all_source_file, "r");
	if (!pf)
	{
		LOG("err");
		exit(1);
	}

	char str[STRLEN];
	char *p;
	int sources_file_line = 0;
	const char useless_char[] = { ' ', '\n', '\t', 0x0a, 0 };        // 要把文件名字符串最后的'\n'去掉，否则打开文件失败

	while (fgets(str, STRLEN, pf))
	{
		printf("********************************************************************************\n");
		sources_file_line++;

		// 要把文件名字符串最后的'\n'去掉，否则打开文件失败
		p = delete_char_at_both_ends(str, useless_char, useless_char);
		if (!strlen(p))
		{
			LOG("found a empty src_file line: %d, %s", sources_file_line, str);
			continue;
		}

		if (strchr(p, '#'))
		{
			LOG("found '#' src_file line: %d, %s", sources_file_line, str);
			continue;
		}

		LOG("now search file %d: %s", sources_file_line, p);
//		getchar();
		insert_str_to_one_file(p, macro_str, insert_str);
	}

	fclose(pf);
	return 0;
}

//int save_result_to_file(file_t *p)
//{
//	printf("\n\n************************************ save_result_to_file *************************************************\n");
//	FILE *fpr = fopen("./result.txt", "w");
//	if (!fpr)
//	{
//		LOG("err");
//		exit(1);
//	}
//
//	int mesa_func_keys_matched = 0;
//	int mesa_func_in_files_matched = 0;
//	int i;
//	char s[STRLEN * 2];
//	for (i = 0; i < p->total; i++)
//	{
//		mesa_func_keys_matched += (bool) (p->matched_times[i]);
//		mesa_func_in_files_matched += p->matched_times[i];
//
//		sprintf(s, "*** %5d   %-40s   %5d \n", i + 1, p->lines[i], p->matched_times[i]);
//		fputs(s, stdout);
//		if (strlen(s) != fwrite(s, 1, strlen(s), fpr))
//		{
//			LOG("err");
//			exit(1);
//		}
//	}
//	printf("目标函数共 %d 个 \n"
//			"注释了 %d 个 \n"
//			"其中有 %d 个目标函数找到了函数定义 \n"
//			"共在.c文件中更新了 %d 次 \n", p->total, p->total_comment,
//			mesa_func_keys_matched, mesa_func_in_files_matched);
//
//	fclose(fpr);
//	return 0;
//}

int main(int argc, char ** argv)
{
	if(argc < 2 )
	{
		printf("\n***** useage: insert_str file \n");
		return 0;
	}

	char * file = argv[1];
	char *str1 = "#include <my_print.h>\n";
	char *str2 = "my_print(__FUNCTION__, __LINE__, __FILE__, \"\");\n";

	insert_str(file, str1, str2);
	return 0;
}



