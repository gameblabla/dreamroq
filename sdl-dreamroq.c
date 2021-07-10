/*
 * Dreamroq by Mike Melanson
 *
 * This is a simple, sample program that helps test the Dreamroq library.
 */

#include <stdio.h>
#include <SDL/SDL.h>
#include <alsa/asoundlib.h>
static snd_pcm_t *handle;

#include "dreamroqlib.h"

extern int32_t framerate;
SDL_Surface* screen, *backbuffer[2];

uint16_t* frame_contain[2];

#define SOUND_FREQUENCY 22050
#define SOUND_SAMPLES_SIZE 1024

#ifndef SDL_TRIPLEBUF
#define SDL_TRIPLEBUF SDL_DOUBLEBUF
#endif

int32_t quit_cb()
{

	SDL_Event event;
	Uint8 *keystate = SDL_GetKeyState(NULL);
    SDL_PollEvent(&event);
    if (keystate[SDLK_ESCAPE]) return 1;
    /* Big, fat no-op for command line tool */
    return 0;
}

int32_t render_cb(uint16_t buf[], int32_t width, int32_t height, int32_t stride,
    int32_t texture_height, int32_t colorspace)
{
    SDL_Flip(backbuffer[1]);
    return ROQ_SUCCESS;
}

#ifdef AUDIO

int32_t audio_cb(uint8_t *snd, int32_t samples, int32_t channels)
{
	uint32_t ret, len;
	len = samples / 4;
	ret = snd_pcm_writei(handle, snd, len);
	while(ret != len) 
	{
		if (ret < 0) 
		{
			snd_pcm_prepare( handle );
		}
		else 
		{
			len -= ret;
		}
		ret = snd_pcm_writei(handle, snd, len);
	}

    return ROQ_SUCCESS;
}
#endif

int finish_cb()
{
	#ifdef AUDIO
	if (handle)
	{
		snd_pcm_drain(handle);
		snd_pcm_close(handle);
	}
    #endif
    return ROQ_SUCCESS;
}

int main(int argc, char *argv[])
{
    int32_t status;
    roq_callbacks_t cbs;

    if (argc < 2)
    {
        printf("USAGE: test-dreamroq <file.roq>\n");
        return 1;
    }
    
    #ifdef AUDIO
	snd_pcm_hw_params_t *params;
	uint32_t val;
	uint32_t ret;
	int32_t dir = -1;
	snd_pcm_uframes_t frames;
	
	/* Open PCM device for playback. */
	int32_t rc = snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0);

	if (rc < 0)
		rc = snd_pcm_open(&handle, "plughw:0,0,0", SND_PCM_STREAM_PLAYBACK, 0);

	if (rc < 0)
		rc = snd_pcm_open(&handle, "plughw:0,0", SND_PCM_STREAM_PLAYBACK, 0);
		
	if (rc < 0)
		rc = snd_pcm_open(&handle, "plughw:1,0,0", SND_PCM_STREAM_PLAYBACK, 0);

	if (rc < 0)
		rc = snd_pcm_open(&handle, "plughw:1,0", SND_PCM_STREAM_PLAYBACK, 0);

	
	/* Allocate a hardware parameters object. */
	snd_pcm_hw_params_alloca(&params);

	/* Fill it in with default values. */
	rc = snd_pcm_hw_params_any(handle, params);

	/* Set the desired hardware parameters. */

	/* Interleaved mode */
	rc = snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);

	/* Signed 16-bit little-endian format */
	rc = snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);

	/* Two channels (stereo) */
	rc = snd_pcm_hw_params_set_channels(handle, params, 2);
	
	val = SOUND_FREQUENCY;
	rc=snd_pcm_hw_params_set_rate_near(handle, params, &val, &dir);

	/* Set period size to settings.aica.BufferSize frames. */
	frames = SOUND_SAMPLES_SIZE;
	rc = snd_pcm_hw_params_set_period_size_near(handle, params, &frames, &dir);

	frames *= 4;
	rc = snd_pcm_hw_params_set_buffer_size_near(handle, params, &frames);

	/* Write the parameters to the driver */
	rc = snd_pcm_hw_params(handle, params);

    #endif
    
	SDL_Init( SDL_INIT_VIDEO );
	SDL_ShowCursor(0);
	backbuffer[1] = SDL_SetVideoMode(320, 240, 16, SDL_HWSURFACE);
	backbuffer[0] = SDL_CreateRGBSurface(SDL_HWSURFACE, 320, 240, 16, 0,0,0,0);
	frame_contain[0] = backbuffer[0]->pixels;
	frame_contain[1] = backbuffer[1]->pixels;

    cbs.render_cb = render_cb;
    #ifdef AUDIO
    cbs.audio_cb = audio_cb;
    #endif
    cbs.quit_cb = quit_cb;
    cbs.finish_cb = finish_cb;

    status = dreamroq_play(argv[1], ROQ_RGB565, 0, &cbs);
    if (backbuffer[1]) SDL_FreeSurface(backbuffer[1]);
    if (backbuffer[0]) SDL_FreeSurface(backbuffer[0]);
	SDL_Quit();

    return status;
}

