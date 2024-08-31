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
		strftime(time_str, sizeof(time_str), "%b %d %H:%M",
				localtime(&file_stat.st_mtime));

		dprintf(STDOUT_FILENO, "%s %lu %s %s %ld %s %s\n",
				mode, (unsigned long)file_stat.st_nlink,
				pw ? pw->pw_name : "unknown",
				gr ? gr->gr_name : "unknown",
				(long)file_stat.st_size,
				time_str, entry->d_name);
	}
}

/**
 * print_total_blocks - Prints the total number of blocks
 * @total_blocks: Total number of blocks
 */
void print_total_blocks(unsigned long total_blocks)
{
	dprintf(STDOUT_FILENO, "total %lu\n", total_blocks / 2);
}

/**
 * print_file_name - Prints a file name
 * @name: Name of the file
 */
void print_file_name(const char *name)
{
	write(STDOUT_FILENO, name, strlen(name));
	write(STDOUT_FILENO, "\n", 1);
}

/**
 * process_entries - Processes directory entries
 * @dir: Directory stream
 * @path: Path to the directory
 * @long_format: Whether to use long format
 * @show_hidden: Whether to show hidden files
 */
void process_entries(DIR *dir, const char *path,
		int long_format, int show_hidden)
{
	struct dirent *entry;
	struct stat st;
	char full_path[1024];
	unsigned long total_blocks = 0;

	if (long_format)
	{
		while ((entry = readdir(dir)) != NULL)
		{
			if (!show_hidden && entry->d_name[0] == '.')
				continue;
			snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
			if (stat(full_path, &st) == 0)
				total_blocks += st.st_blocks;
		}
		print_total_blocks(total_blocks);
		rewinddir(dir);
	}

	while ((entry = readdir(dir)) != NULL)
	{
		if (!show_hidden && entry->d_name[0] == '.')
			continue;
		if (long_format)
			print_file_info(path, entry);
		else
			print_file_name(entry->d_name);
	}
}

/**
 * ls_cmd - Handles the `ls` command to list files in a directory
 * @argv: Array of arguments, e.g., `ls -l -a [directory]`
 */
void ls_cmd(char **argv)
{
	DIR *dir;
	char *path = ".";
	int long_format = 0;
	int show_hidden = 0;
	int i = 1;
	struct winsize w;

	while (argv[i] && argv[i][0] == '-')
	{
		if (strchr(argv[i], 'l'))
			long_format = 1;
		if (strchr(argv[i], 'a'))
			show_hidden = 1;
		i++;
	}
	if (argv[i])
		path = argv[i];

	dir = opendir(path);
	if (dir == NULL)
	{
		handle_error();
		return;
	}

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	process_entries(dir, path, long_format, show_hidden);
	closedir(dir);
}

