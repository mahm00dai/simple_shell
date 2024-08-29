#include "main.h"
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <sys/ioctl.h>
#include <stdio.h>

/**
 * print_file_info - Prints file information in long format
 * @path: Path to the file
 * @entry: Directory entry
 *
 * Description: Prints detailed information about a file similar to `ls -l`.
 */
void print_file_info(const char *path, struct dirent *entry)
{
	struct stat file_stat;
	char full_path[1024];
	struct passwd *pw;
	struct group *gr;
	char mode[] = "----------";
	char time_str[20];

	snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

	if (stat(full_path, &file_stat) == 0)
	{
		mode[0] = S_ISDIR(file_stat.st_mode) ? 'd' : '-';
		mode[1] = (file_stat.st_mode & S_IRUSR) ? 'r' : '-';
		mode[2] = (file_stat.st_mode & S_IWUSR) ? 'w' : '-';
		mode[3] = (file_stat.st_mode & S_IXUSR) ? 'x' : '-';
		mode[4] = (file_stat.st_mode & S_IRGRP) ? 'r' : '-';
		mode[5] = (file_stat.st_mode & S_IWGRP) ? 'w' : '-';
		mode[6] = (file_stat.st_mode & S_IXGRP) ? 'x' : '-';
		mode[7] = (file_stat.st_mode & S_IROTH) ? 'r' : '-';
		mode[8] = (file_stat.st_mode & S_IWOTH) ? 'w' : '-';
		mode[9] = (file_stat.st_mode & S_IXOTH) ? 'x' : '-';

		pw = getpwuid(file_stat.st_uid);
		gr = getgrgid(file_stat.st_gid);
		strftime(time_str, sizeof(time_str), "%b %d %H:%M", localtime(&file_stat.st_mtime));

		dprintf(STDOUT_FILENO, "%s %lu %s %s %ld %s %s \n",
			mode, (unsigned long)file_stat.st_nlink,
			pw->pw_name, gr->gr_name, (long)file_stat.st_size,
			time_str, entry->d_name);
	}
}

/**
 * ls_cmd - Handles the `ls` command to list files in a directory
 * @argv: Array of arguments, e.g., `ls -l -a [directory]`
 *
 * Description: Lists files with options like `-l` and `-a`.
 */
void ls_cmd(char **argv)
{
	DIR *dir;
	struct dirent *entry;
	char *path = ".";
	int long_format = 0;
	int show_hidden = 0;
	int i = 1;
	struct winsize w; /* Terminal width */
	char full_path[1024]; /* Moved declaration of full_path */

	/* Parse options */
	while (argv[i] && argv[i][0] == '-')
	{
		if (strchr(argv[i], 'l'))
			long_format = 1;
		if (strchr(argv[i], 'a'))
			show_hidden = 1;
		i++;
	}

	if (argv[i])
		path = argv[i]; /* Last argument is the path */

	dir = opendir(path);
	if (dir == NULL)
	{
		handle_error();
		return;
	}

	/* Get terminal width */
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	/* Print total blocks if in long format */
	if (long_format)
	{
		struct stat st;
		unsigned long total_blocks = 0;
		while ((entry = readdir(dir)) != NULL)
		{
			if (!show_hidden && entry->d_name[0] == '.')
				continue; /* Skip hidden files if not `-a` */
			snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
			if (stat(full_path, &st) == 0)
			{
				total_blocks += st.st_blocks;
			}
		}
		dprintf(STDOUT_FILENO, "total %lu\n", total_blocks / 2);
		rewinddir(dir); /* Reset directory pointer */
	}

	/* Read and print each file in the directory */
	while ((entry = readdir(dir)) != NULL)
	{
		if (!show_hidden && entry->d_name[0] == '.')
			continue; /* Skip hidden files if not `-a` */

		if (long_format)
			print_file_info(path, entry);
		else
		{
			write(STDOUT_FILENO, entry->d_name, strlen(entry->d_name));
			write(STDOUT_FILENO, " ", 1); /* Print space between filenames */
		}
	}
	if (!long_format)
		write(STDOUT_FILENO, "\n", 1); /* Newline at the end of the output */

	closedir(dir);
}

