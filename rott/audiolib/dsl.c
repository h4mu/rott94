#include <stdlib.h>
#include <string.h>

#include "dsl.h"
#include "util.h"

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

extern volatile int MV_MixPage;

static int DSL_ErrorCode = DSL_Ok;

static int mixer_initialized;

static void ( *_CallBackFunc )( void );
static volatile char *_BufferStart;
static int _BufferSize;
static int _NumDivisions;
static int _SampleRate;
static int _remainder;

static MIX_Mixer *sdl_mixer = NULL;
static MIX_Track *stream_track = NULL;
static MIX_Audio *blank_audio = NULL;
static unsigned char *blank_buf;

/*
possible todo ideas: cache sdl/sdl mixer error messages.
*/

char *DSL_ErrorString( int ErrorNumber )
{
	char *ErrorString;
	
	switch (ErrorNumber) {
		case DSL_Warning:
		case DSL_Error:
			ErrorString = DSL_ErrorString(DSL_ErrorCode);
			break;
		
		case DSL_Ok:
			ErrorString = "SDL Driver ok.";
			break;
		
		case DSL_SDLInitFailure:
			ErrorString = "SDL Audio initialization failed.";
			break;
		
		case DSL_MixerActive:
			ErrorString = "SDL Mixer already initialized.";
			break;	
	
		case DSL_MixerInitFailure:
			ErrorString = "SDL Mixer initialization failed.";
			break;
			
		default:
			ErrorString = "Unknown SDL Driver error.";
			break;
	}
	
	return ErrorString;
}

static void DSL_SetErrorCode(int ErrorCode)
{
	DSL_ErrorCode = ErrorCode;
}

int DSL_Init( void )
{
	DSL_SetErrorCode(DSL_Ok);
	
	if (!SDL_InitSubSystem(SDL_INIT_AUDIO)) {
		DSL_SetErrorCode(DSL_SDLInitFailure);
		
		return DSL_Error;
	}
	
	return DSL_Ok;
}

void DSL_Shutdown( void )
{
	DSL_StopPlayback();
}

static void SDLCALL mixer_callback(void *userdata, MIX_Track *track, const SDL_AudioSpec *spec, float *pcm, int samples)
{
	Uint8 *stptr;
	Uint8 *fxptr;
    int copysize;
    int bytes_per_sample = SDL_AUDIO_BYTESIZE(spec->format);
    int total_bytes = samples * bytes_per_sample;
    int samples_per_frame = spec->channels;

    // We need a temporary buffer for the raw 8-bit or 16-bit PCM from ROTT
    static Uint8 *raw_buf = NULL;
    static int raw_buf_len = 0;

    if (raw_buf_len < total_bytes) {
        raw_buf = realloc(raw_buf, total_bytes);
        raw_buf_len = total_bytes;
    }

    int len = total_bytes;
	stptr = raw_buf;
	
	if (_remainder > 0) {
		copysize = min(len, _remainder);
		
		fxptr = (Uint8 *)(&_BufferStart[MV_MixPage * 
			_BufferSize]);
		
		memcpy(stptr, fxptr+(_BufferSize-_remainder), copysize);
		
		len -= copysize;
		_remainder -= copysize;
		
		stptr += copysize;
	}
	
	while (len > 0) {
		/* new buffer */
		
		_CallBackFunc();
		
		fxptr = (Uint8 *)(&_BufferStart[MV_MixPage * 
			_BufferSize]);

		copysize = min(len, _BufferSize);
		
		memcpy(stptr, fxptr, copysize);
		
		len -= copysize;
		
		stptr += copysize;
	}
	
	_remainder = len;

    // Now convert raw_buf to floats in pcm
    if (spec->format == SDL_AUDIO_U8) {
        for (int i = 0; i < samples; i++) {
            pcm[i] = ((float)raw_buf[i] - 128.0f) / 128.0f;
        }
    } else if (spec->format == SDL_AUDIO_S16) {
        Sint16 *s16 = (Sint16 *)raw_buf;
        for (int i = 0; i < samples; i++) {
            pcm[i] = (float)s16[i] / 32768.0f;
        }
    }
}

int   DSL_BeginBufferedPlayback( char *BufferStart,
      int BufferSize, int NumDivisions, unsigned SampleRate,
      int MixMode, void ( *CallBackFunc )( void ) )
{
	SDL_AudioSpec spec;
		
	if (mixer_initialized) {
		DSL_SetErrorCode(DSL_MixerActive);
		
		return DSL_Error;
	}
	
	_CallBackFunc = CallBackFunc;
	_BufferStart = BufferStart;
	_BufferSize = (BufferSize / NumDivisions);
	_NumDivisions = NumDivisions;
	_SampleRate = SampleRate;

	_remainder = 0;
	
	SDL_zero(spec);
    spec.format = (MixMode & SIXTEEN_BIT) ? SDL_AUDIO_S16 : SDL_AUDIO_U8;
	spec.channels = (MixMode & STEREO) ? 2 : 1;
    spec.freq = SampleRate;

    if (!MIX_Init()) {
        DSL_SetErrorCode(DSL_MixerInitFailure);
        return DSL_Error;
    }

    sdl_mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec);
	if (sdl_mixer == NULL) {
		DSL_SetErrorCode(DSL_MixerInitFailure);
		
		return DSL_Error;
	}

    stream_track = MIX_CreateTrack(sdl_mixer);
	MIX_SetTrackCookedCallback(stream_track, mixer_callback, NULL);
	
	/* create a dummy sample just to allocate that channel */
	blank_buf = (Uint8 *)malloc(4096);
	memset(blank_buf, 0, 4096);
	
	blank_audio = MIX_LoadRawAudio(sdl_mixer, blank_buf, 4096, &spec);
		
    SDL_PropertiesID props = SDL_CreateProperties();
    SDL_SetNumberProperty(props, MIX_PROP_PLAY_LOOPS_NUMBER, -1);
    MIX_SetTrackAudio(stream_track, blank_audio);
	MIX_PlayTrack(stream_track, props);
    SDL_DestroyProperties(props);
	
	mixer_initialized = 1;
	
	return DSL_Ok;
}

void DSL_StopPlayback( void )
{
	if (mixer_initialized) {
		MIX_StopTrack(stream_track, 0);
	}
	
	if (blank_audio != NULL) {
		MIX_DestroyAudio(blank_audio);
	}
	
	blank_audio = NULL;
	
	if (blank_buf  != NULL) {
		free(blank_buf);
	}
	
	blank_buf = NULL;
	
	if (mixer_initialized) {
		MIX_DestroyMixer(sdl_mixer);
        MIX_Quit();
	}
	
    sdl_mixer = NULL;
    stream_track = NULL;
	mixer_initialized = 0;
}

unsigned DSL_GetPlaybackRate( void )
{
	return _SampleRate;
}

unsigned long DisableInterrupts( void )
{
	return 0;
}

void RestoreInterrupts( unsigned long flags )
{
}
