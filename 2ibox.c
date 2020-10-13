                     /*┌────────────────────────────────────┐*/
                     /*│ IBox - Draw box around the strings │*/
#include <stdio.h>   /*├────────────────────────────────────┤*/
#include <string.h>  /*│ Author   : arfanamd <@arfanamd>    │*/
#include <unistd.h>  /*│ Timestamp: 7 September 2020        │*/
                     /*└────────────────────────────────────┘*/

typedef enum { NO_ARG = 1, U_ARG } Mode;

static int wrap_fd(int fd) {
	char buffer[64];
	char dipper[68];
	ssize_t _BUF_READ_;

	/* Read string from standard input and store it to buffer.
	 * WARNING: See manual page for read function to see any 
	 *          usefull information about buffer-overflow.
	 *     XXX: Any over-inputted string will be wrap to the   
	 *          next buffer. Less worry matters :-)
	 *********************************************************/
	while ((_BUF_READ_ = read(fd, buffer, sizeof(buffer))) > 0) {
		ssize_t buf_totalwritten = 0;

		while (buf_totalwritten < _BUF_READ_) {
			dprintf(STDOUT_FILENO, "%s", "┌");
			for (int i = 0; i < (_BUF_READ_ + NO_ARG); i++) {
				dprintf(STDOUT_FILENO, "%s", "─");
			}
			dprintf(STDOUT_FILENO, "%s\n", "┐");

			/* We cat the inputted string to the dipper, so we have
			 * full control of it without effect the buffer.
			 * In this case we change any new line character '\n'
			 * inside the dipper to null terminated character '\0'
			 *****************************************************/
			strncat(dipper, buffer, _BUF_READ_);
			dipper[_BUF_READ_ - 1] = '\0';

			ssize_t _BUF_WRITE_ = dprintf(STDOUT_FILENO, "│ %s │\n", dipper);
			if (_BUF_WRITE_ < 1) return -1;

			buf_totalwritten += _BUF_WRITE_;
			dipper[0] = '\0';

			dprintf(STDOUT_FILENO, "%s", "└");
			for (int i = 0; i < (_BUF_READ_ + NO_ARG); i++) {
				dprintf(STDOUT_FILENO, "%s", "─");
			}
			dprintf(STDOUT_FILENO, "%s\n", "┘");
		}
	}
	return _BUF_READ_ == 0 ? 0 : -1;
}

static int wrap_arg(const char *u_arg) {
	int _ARG_READ_ = strlen(u_arg);
	if (_ARG_READ_ == 0) return -1;

	fprintf(stdout, "%s", "┌");
	for (int i = 0; i < (_ARG_READ_ + U_ARG); i++) {
		fprintf(stdout, "%s", "─");
	}
	fprintf(stdout, "%s\n", "┐");

	fprintf(stdout, "│ %s │\n", u_arg);

	fprintf(stdout, "%s", "└");
	for (int i = 0; i < (_ARG_READ_ + U_ARG); i++) {
		fprintf(stdout, "%s", "─");
	}
	fprintf(stdout, "%s\n", "┘");

	return 0;
}

int main(int argc, const char *argv[]) {
	if (argc == NO_ARG) {
		if (wrap_fd(STDIN_FILENO) != 0) return -1; }
	else if (argc == U_ARG) {
		if (wrap_arg(argv[1]) != 0) return -1; }
	else {
		dprintf(STDOUT_FILENO, "[%s] %s %s %s\n", strerror(22),
				"Usage:", argv[0], "\"<arguments>\"");
		return -1;
	}
	return 0;
}
