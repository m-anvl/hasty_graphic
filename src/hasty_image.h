#ifndef HASTY_IMAGE_H_SENTRY
#define HASTY_IMAGE_H_SENTRY

#include "hasty_graphic.h"

#ifdef __cplusplus
extern "C" {
#endif

int load_image(uint32_t* dest, int w, int h, const char* image_path);

#ifdef __cplusplus
}
#endif

#endif // !HASTY_IMAGE_H_SENTRY
