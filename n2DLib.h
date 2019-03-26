#ifndef INCLUDE_GRAFX
#define INCLUDE_GRAFX

#include <os.h>
#include <stdarg.h>

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

typedef int32_t Fixed;
typedef struct
{
	int32_t x;
	int32_t y;
	int32_t h;
	int32_t w;
} Rect;

#ifdef __cplusplus
extern "C" {
#endif

extern void initBuffering();
extern void updateScreen();
extern void deinitBuffering();
extern void clearBufferB();
extern void drawString(int32_t *x, int32_t *y, int32_t _x, const int8_t *str, uint16_t fc, uint16_t olc);
extern void fillRect(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t c);
extern void drawChar(int32_t *x, int32_t *y, int32_t margin, int8_t ch, uint16_t fc, uint16_t olc);

#define BUFF_BYTES_SIZE (320*240*2)
extern uint16_t BUFF_BASE_ADDRESS[BUFF_BYTES_SIZE];

#ifdef __cplusplus
}
#endif

#endif
