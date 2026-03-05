#ifndef DC_UTILS_H_
#define DC_UTILS_H_

#include <stdlib.h>
#include <sys/types.h>

int write_sysfs_file(char *file, char *buffer, ssize_t sz);
int read_sysfs_file(char *file, char *buffer, ssize_t len);
int execute_cmd(char *cmd);

#endif  /* DC_UTILS_H_ */
