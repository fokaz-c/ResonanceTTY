#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stdbool.h>
#include <stdint.h>

/* RGB pixel */
// FIXME: Replace this with a union or something, so that this is interchangable
// with greyscale
typedef struct {
	uint8_t r, g, b;
} Pixel;

/* Framebuffer container */
typedef struct {
	Pixel *data;
	uint16_t width;
	uint16_t height;
} FrameBuffer;

/* Framebuffer management */
FrameBuffer *framebuffer_create(uint16_t width, uint16_t height);
void framebuffer_free(FrameBuffer *fb);
bool framebuffer_load(FrameBuffer *fb, const char *path);
void framebuffer_clear(FrameBuffer *fb, Pixel color);

/* Pixel access */
Pixel framebuffer_get_pixel(const FrameBuffer *fb, uint16_t x, uint16_t y);
void framebuffer_set_pixel(FrameBuffer *fb, uint16_t x, uint16_t y, Pixel p);

#endif /* FRAMEBUFFER_H */
