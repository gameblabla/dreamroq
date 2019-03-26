/*
 * Dreamroq by Mike Melanson
 *
 * This is a simple, sample program that helps test the Dreamroq library.
 */
#include <stdio.h>
#include <stdint.h>
#include <libndls.h>
#include "n2DLib.h"
#include "dreamroqlib.h"
#include "ticks.h"

extern uint32_t framerate;

int32_t quit_cb()
{
	if (isKeyPressed(KEY_NSPIRE_ESC)) return 1;
    return 0;
}

int render_cb(uint16_t buf[], int32_t width, int32_t height, int32_t stride,
    int32_t texture_height, int32_t colorspace)
{
	
	uint16_t *buf_rgb565 = (uint16_t*)buf;
	uint32_t start;
	start = TI_GetTicks();
	if((1000/framerate) > TI_GetTicks()-start) TI_Delay((1000/framerate)-(TI_GetTicks()-start));
	
	lcd_blit(buf_rgb565, SCR_320x240_565);

    return ROQ_SUCCESS;
}

#ifdef AUDIO
#define AUDIO_FILENAME "./roq-audio.wav.tns"
static char wav_header[] = {
    'R', 'I', 'F', 'F',  /* RIFF header */
      0,   0,   0,   0,  /* file size will be filled in later */
    'W', 'A', 'V', 'E',  /* more header stuff */
    'f', 'm', 't', 0x20,
    0x10,  0,   0,   0,  /* length of format chunk */
      1,   0,            /* format = 1 (PCM) */
      0,   0,            /* channel count will be filled in later */
    0x22, 0x56, 0,   0,  /* frequency is always 0x5622 = 22050 Hz */
      0,   0,   0,   0,  /* byte rate will be filled in later */
      1,   0, 0x10,  0,  /* data alignment and bits per sample */
    'd', 'a', 't', 'a',  /* start of data chunk */
      0,   0,   0,   0   /* data block size will be filled in later */
};
#define WAV_HEADER_SIZE 44
#define SAMPLE_RATE 22050
static FILE *wav_output;
static int data_size = 0;
static int audio_output_initialized = 0;


int audio_cb(unsigned char *buf_rgb565, int samples, int channels)
{
    int byte_rate;

    if (!audio_output_initialized)
    {
        wav_output = fopen(AUDIO_FILENAME, "wb");
        if (!wav_output)
            return ROQ_CLIENT_PROBLEM;

        if (channels != 1 && channels != 2)
            return ROQ_CLIENT_PROBLEM;
        wav_header[22] = channels;
        byte_rate = SAMPLE_RATE * 2 * channels;
        wav_header[0x1C] = (byte_rate >>  0) & 0xFF;
        wav_header[0x1D] = (byte_rate >>  8) & 0xFF;
        wav_header[0x1E] = (byte_rate >> 16) & 0xFF;
        wav_header[0x1F] = (byte_rate >> 24) & 0xFF;

        if (fwrite(wav_header, WAV_HEADER_SIZE, 1, wav_output) != 1)
        {
            fclose(wav_output);
            return ROQ_CLIENT_PROBLEM;
        }

        audio_output_initialized = 1;
    }

    if (fwrite(buf_rgb565, samples, 1, wav_output) != 1)
    {
        fclose(wav_output);
        return ROQ_CLIENT_PROBLEM;
    }
    data_size += samples;

    return ROQ_SUCCESS;
}
#endif

int32_t finish_cb()
{
	#ifdef AUDIO
	if (audio_output_initialized)
    {
        printf("Wrote %d (0x%X) bytes to %s\n", data_size, data_size,
            AUDIO_FILENAME);
        fseek(wav_output, 0, SEEK_SET);
        wav_header[40] = (data_size >>  0) & 0xFF;
        wav_header[41] = (data_size >>  8) & 0xFF;
        wav_header[42] = (data_size >> 16) & 0xFF;
        wav_header[43] = (data_size >> 24) & 0xFF;
        data_size += WAV_HEADER_SIZE - 8;
        wav_header[4] = (data_size >>  0) & 0xFF;
        wav_header[5] = (data_size >>  8) & 0xFF;
        wav_header[6] = (data_size >> 16) & 0xFF;
        wav_header[7] = (data_size >> 24) & 0xFF;
        if (fwrite(wav_header, WAV_HEADER_SIZE, 1, wav_output) != 1)
        {
            fclose(wav_output);
            return ROQ_CLIENT_PROBLEM;
        }
    }
    #endif
    return ROQ_SUCCESS;
}

int dreamroq(char* filename)
{
    int32_t status;
    roq_callbacks_t cbs;
    
	TI_StartTicks();
	initBuffering();

    cbs.render_cb = render_cb;
    #ifdef AUDIO
    cbs.audio_cb = audio_cb;
    #endif
    cbs.quit_cb = quit_cb;
    cbs.finish_cb = finish_cb;

    status = dreamroq_play(filename, ROQ_RGB565, 0, &cbs);
    deinitBuffering();

	return 0;
}

