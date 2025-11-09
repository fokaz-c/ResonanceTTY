#include "framebuffer.h"
#include <fcntl.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* This is Shit and needs to be fixed */
/* FIXME: Replace this bugprone piece of shit inputs with a struct instead */
FrameBuffer *framebuffer_create(uint16_t width, uint16_t height) {
	if (width == 0 || height == 0) {
		return NULL;
	}

	FrameBuffer *fb = (FrameBuffer *)malloc(sizeof(FrameBuffer));
	fb->data = NULL;
	fb->width = width;
	fb->height = height;
	return fb;
}

void framebuffer_free(FrameBuffer *fb) {
	if (!fb)
		return;

	if (fb->data)
		free(fb->data);

	free(fb);
	fb = NULL;
	// This is dumb the user still needs to take care of the dangling
	// pointer
	// FIXME- Use **fb later to set the dangling pointer to NULL here.
}

bool framebuffer_load(FrameBuffer *fb, const char *path) {
	if (!fb || !path)
		return false;

	size_t size = (size_t)fb->width * fb->height * sizeof(Pixel);
	Pixel *pixelbuffer = (Pixel *)malloc(size);
	if (!pixelbuffer)
		return false;

	int fd = open(path, O_RDONLY);
	if (fd < 0) {
		perror("open");
		free(pixelbuffer);
		return false;
	}

	ssize_t bytes = read(fd, pixelbuffer, size);
	close(fd);

	if (bytes < 0) {
		perror("read");
		free(pixelbuffer);
		return false;
	}

	if ((size_t)bytes < size) {
		fprintf(stderr, "Warning: file smaller than expected "
		                "framebuffer size.\n");
	}

	fb->data = pixelbuffer;
	return true;
}

void framebuffer_clear(FrameBuffer *fb, Pixel color) {
	if (!fb || !fb->data) {
		fprintf(stderr, "Invalid framebuffer or data buffer\n");
		return;
	}

	for (size_t i = 0; i < (size_t)fb->height * fb->width; ++i) {
		Pixel *pixel = &fb->data[i];
		pixel->r = color.r;
		pixel->g = color.g;
		pixel->b = color.b;
	}
}

Pixel framebuffer_get_pixel(const FrameBuffer *fb, uint16_t x, uint16_t y) {
	int index = (y * fb->width) + x;
	if (fb->width * fb->height <= index || index < 0)
		fprintf(stderr, "Invalid Index or Out of Bounds");
	return fb->data[index];
}

void framebuffer_set_pixel(FrameBuffer *fb, uint16_t x, uint16_t y, Pixel p) {
	int index = (y * fb->width) + x;
	if (fb->width * fb->height <= index || index < 0)
		fprintf(stderr, "Invalid Index or Out of Bounds");
	fb->data[index] = p;
}
