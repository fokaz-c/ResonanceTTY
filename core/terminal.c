#include "terminal.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

static struct termios orig_termios;
static bool termios_saved = false;

bool terminal_init(TerminalConfig *config) {
	if (!config)
		return false;

	if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) {
		return false;
	}
	termios_saved = true;

	if (config->flags & TERMINAL_RAW_MODE) {
		struct termios raw = orig_termios;

		raw.c_lflag &= ~(ICANON | ECHO);
		raw.c_oflag &= ~(OPOST);
		raw.c_cc[VMIN] = 0;
		raw.c_cc[VTIME] = 0;

		if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
			return false;
		}
	}

	if (config->flags & TERMINAL_HIDE_CURSOR) {
		printf("\033[?25l");
		fflush(stdout);
	}

	if (config->flags & TERMINAL_CLEAR_SCREEN) {
		printf("\033[2J\033[H");
		fflush(stdout);
	}

	if (config->flags & TERMINAL_ALT_BUFFER) {
		printf("\033[?1049h");
		fflush(stdout);
	}

	config->isActive = true;
	return true;
}

void terminal_exit(TerminalConfig *config) {
	if (!config || !config->isActive)
		return;

	if (config->flags & TERMINAL_ALT_BUFFER) {
		printf("\033[?1049l");
		fflush(stdout);
	}

	if (config->flags & TERMINAL_HIDE_CURSOR) {
		printf("\033[?25h");
		fflush(stdout);
	}

	if (termios_saved) {
		tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
	}

	config->isActive = false;
}

void terminal_clear(void) {
	printf("\033[2J\033[H");
	fflush(stdout);
}

void terminal_flush(void) {
	fflush(stdout);
}

TerminalSize terminal_get_size(void) {
	struct winsize ws;
	TerminalSize size = {80, 24};

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) != -1) {
		size.width = ws.ws_col;
		size.height = ws.ws_row;
	}

	return size;
}

void terminal_cursor_move(uint16_t x, uint16_t y) {
	printf("\033[%d;%dH", y + 1, x + 1);
}

void terminal_cursor_hide(void) {
	printf("\033[?25l");
	fflush(stdout);
}

void terminal_cursor_show(void) {
	printf("\033[?25h");
	fflush(stdout);
}

TerminalBuffer *terminal_buffer_create(uint16_t width, uint16_t height) {
	TerminalBuffer *buf = malloc(sizeof(TerminalBuffer));
	if (!buf)
		return NULL;

	buf->width = width;
	buf->height = height;

	size_t size = (size_t)width * (size_t)height;
	buf->buffer = malloc(size);
	if (!buf->buffer) {
		free(buf);
		return NULL;
	}

	memset(buf->buffer, ' ', size);
	return buf;
}

void terminal_buffer_free(TerminalBuffer *buf) {
	if (!buf)
		return;
	free(buf->buffer);
	free(buf);
}

void terminal_buffer_clear(TerminalBuffer *buf) {
	if (!buf)
		return;
	memset(buf->buffer, ' ', (size_t)buf->width * buf->height);
}

void terminal_buffer_set(TerminalBuffer *buf, uint16_t x, uint16_t y, char c) {
	if (!buf || x >= buf->width || y >= buf->height)
		return;
	buf->buffer[(y * buf->width) + x] = c;
}

void terminal_buffer_present(TerminalBuffer *buf) {
	if (!buf)
		return;

	printf("\033[H");
	for (uint16_t y = 0; y < buf->height; y++) {
		printf("\033[%d;1H", y + 1);
		for (uint16_t x = 0; x < buf->width; x++) {
			putchar(buf->buffer[(y * buf->width) + x]);
		}
	}
	fflush(stdout);
}

void terminal_set_fg_color_256(uint8_t color) {
	printf("\033[38;5;%dm", color);
}

void terminal_set_bg_color_256(uint8_t color) {
	printf("\033[48;5;%dm", color);
}

void terminal_reset_colors(void) {
	printf("\033[0m");
	fflush(stdout);
}
