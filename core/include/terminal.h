#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdbool.h>
#include <stdint.h>

/* Terminal Option Flags (bitmask) */
#define TERMINAL_RAW_MODE     (1ULL << 0)
#define TERMINAL_HIDE_CURSOR  (1ULL << 1)
#define TERMINAL_CLEAR_SCREEN (1ULL << 2)
#define TERMINAL_ALT_BUFFER   (1ULL << 3)

/* Terminal configuration structure */
typedef struct TerminalConfig {
	uint64_t flags;
	bool isActive;
} TerminalConfig;

/* Terminal dimensions */
typedef struct {
	uint16_t width;
	uint16_t height;
} TerminalSize;

/* Terminal buffer for double buffering */
typedef struct {
	char *buffer;
	uint16_t width;
	uint16_t height;
} TerminalBuffer;

/* Core terminal control */
bool terminal_init(TerminalConfig *config);
void terminal_exit(TerminalConfig *config);
void terminal_clear(void);
void terminal_flush(void);

/* Terminal information */
TerminalSize terminal_get_size(void);

/* Cursor control */
void terminal_cursor_move(uint16_t x, uint16_t y);
void terminal_cursor_hide(void);
void terminal_cursor_show(void);

/* Buffer management (double buffering) */
TerminalBuffer *terminal_buffer_create(uint16_t width, uint16_t height);
void terminal_buffer_free(TerminalBuffer *buf);
void terminal_buffer_clear(TerminalBuffer *buf);
void terminal_buffer_set(TerminalBuffer *buf, uint16_t x, uint16_t y, char c);
void terminal_buffer_present(TerminalBuffer *buf);

/* ANSI color control */
void terminal_set_fg_color_256(uint8_t color);
void terminal_set_bg_color_256(uint8_t color);
void terminal_reset_colors(void);

#endif /* TERMINAL_H */
