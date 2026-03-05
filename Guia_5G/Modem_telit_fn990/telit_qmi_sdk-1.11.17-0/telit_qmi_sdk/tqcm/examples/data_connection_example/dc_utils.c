#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <telit_sdk.h>

#include "dc_utils.h"

int write_sysfs_file(char *file, char *buffer, ssize_t sz)
{
    int ret = 0;
    int fd;

    fd = open(file, O_WRONLY);
    if (fd < 0) {
        RLOGE("%s: unable to open %s, %s", __FUNCTION__,
                file, strerror(errno));
        ret = -1;
        goto end;
    }

    sz = pwrite(fd, buffer, sz, 0);
    if (sz < 0) {
        RLOGE("%s: unable to write to %s, %s", __FUNCTION__,
                file, strerror(errno));
        ret = -1;
        goto end;
    }

end:
    return ret;
}

int read_sysfs_file(char *file, char *buffer, ssize_t len)
{
    int ret = 0;
    int fd;
    ssize_t sz;

    fd = open(file, O_RDONLY);
    if (fd < 0) {
        RLOGE("%s: unable to open %s, %s", __FUNCTION__,
                file, strerror(errno));
        ret = -1;
        goto end;
    }

    sz = pread(fd, buffer, len - 1, 0);
    if (sz < 0) {
        RLOGE("%s: unable to read from %s, %s", __FUNCTION__,
                file, strerror(errno));
        ret = -1;
        goto end;
    }

    buffer[sz] = '\0';

end:
    return ret;
}

int execute_cmd(char *cmd)
{
    int ret = 0;

    RLOGD("%s", cmd);
    if ((ret = system(cmd)) == -1) {
        RLOGE("%s: unable to run %s", cmd);
        goto end;
    }
    if (WEXITSTATUS(ret)) {
        RLOGE("%s: cmd %s exited with %d", __FUNCTION__, cmd, WEXITSTATUS(ret));
        goto end;
    }

end:
    return ret;
}
