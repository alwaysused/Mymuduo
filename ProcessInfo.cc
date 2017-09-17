#include "ProcessInfo.h"
#include "CurrentThread.h"
#include "FileUtil.h"

#include <algorithm>

#include <assert.h>
#include <dirent.h>
#include <pwd.h>
#include <stdio.h> // snprintf
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/times.h>
namespace muduo{
namespace Detail
{
__thread int t_numOpenedFiles = 0;
int fdDirFilter(const struct dirent * dir)
{
	if (isdigit(dir->d_name[0]))
		++t_numOpenedFiles;
	return 0;
}

int scanDir(const char* dir, (int)(* filter)(const struct dirent *)
{
	struct dirent** namelist = NULL;
	int result = scandir(dir, &namelist, filter ,alphasort);
	assert(namelist == NULL);
	return result;

}
}
}
using namespace muduo;
using namespace muduo::Detail;
int ProcessInfo::openedFiles()
{
	t_numOpenedFiles = 0;
  scanDir("/proc/self/fd", fdDirFilter);
  return t_numOpenedFiles;

}


