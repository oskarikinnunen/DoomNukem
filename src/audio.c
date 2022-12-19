/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   audio.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 15:14:01 by raho              #+#    #+#             */
/*   Updated: 2022/12/12 18:16:46 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"
#include "doomnukem.h"

void	play_music(t_audio audio)
{
	if (audio.device == NULL)
		return;
	if (SDL_GetQueuedAudioSize(audio.device) == 0)
	{
		if (SDL_QueueAudio(audio.device, audio.wav_start, audio.wav_length) < 0)
			error_log(EC_SDL_QUEUEAUDIO);
		SDL_PauseAudioDevice(audio.device, 0);
	}
}

void	pause_unused_audio(t_sdlcontext sdl)
{
	int	index;

	index = 0;
	while (index < sdl.audiocount)
	{
		if (SDL_GetQueuedAudioSize(sdl.audio[index].device) == 0)
			SDL_PauseAudioDevice(sdl.audio[index].device, 1);
		index++;
	}
}

void	play_audio(t_audio audio)
{
	if (audio.device == NULL)
		return;
	if (SDL_QueueAudio(audio.device, audio.wav_start, audio.wav_length) < 0)
		error_log(EC_SDL_QUEUEAUDIO);
	SDL_PauseAudioDevice(audio.device, 0);
}

void	force_play_audio(t_audio audio)
{
	if (audio.device == NULL)
		return;
	if (SDL_GetQueuedAudioSize(audio.device) > 0)
		SDL_ClearQueuedAudio(audio.device);
	if (SDL_QueueAudio(audio.device, audio.wav_start, audio.wav_length) < 0)
		error_log(EC_SDL_QUEUEAUDIO);
	SDL_PauseAudioDevice(audio.device, 0);
}

void	pause_audio(t_audio audio)
{
	if (audio.device == NULL)
		return;
	SDL_PauseAudioDevice(audio.device, 1);
}

void	open_audiodevice(t_audio *audio)
{
	audio->device = SDL_OpenAudioDevice(NULL, 0, &audio->wav_spec, NULL, SDL_AUDIO_ALLOW_ANY_CHANGE);
	/*if (audio->device == 0)
		error_log(EC_SDL_OPENAUDIODEVICE);*/
}

void	load_wav(t_audio *audio, char *file)
{
	if (audio->device == NULL)
		return;
	ft_strcpy(audio->name, file);
	if (SDL_LoadWAV(file, &audio->wav_spec, &audio->wav_start, &audio->wav_length) == NULL)
		error_log(EC_SDL_LOADWAV);
}

void	close_audio(t_sdlcontext *sdl)
{
	int	index;

	index = 0;
	if (sdl->audio)
	while (index < sdl->audiocount)
	{
		SDL_FreeWAV(sdl->audio[index].wav_start);
		SDL_CloseAudioDevice(sdl->audio[index].device);
		index++;
	}
	free(sdl->audio);
}

void	load_audio(t_sdlcontext *sdl)
{
	int	index;

	printf("number of audio devices: %d\n", SDL_GetNumAudioDevices(0));
	printf("default audio device: %s\n", SDL_GetAudioDeviceName(0, 0));

	sdl->audiocount = 3;
	index = 0;
	
	sdl->audio = (t_audio *)malloc(sizeof(t_audio) * sdl->audiocount);
	if (!sdl->audio)
		error_log(EC_MALLOC);
	while (index < sdl->audiocount)
	{
		ft_bzero(&sdl->audio[index], sizeof(sdl->audio[index]));
		index++;
	}
	load_wav(&sdl->audio[0], "assets/audio/migfus20_rock-music.wav");
	load_wav(&sdl->audio[1], "assets/audio/gattoangus_9mm-pistol.wav");
	load_wav(&sdl->audio[2], "assets/audio/godowan_bubble1.wav");
	index = 0;
	while (index < sdl->audiocount)
	{
		open_audiodevice(&sdl->audio[index]);
		index++;
	}
}

// void	play_sound(void)
// {
// 	//wavSpec.callback = MyAudioCallBack;
// 	//wavSpec.userdata = &audio;
// }

// struct AudioData
// {
// 	Uint8   *pos;
// 	Uint32  length;
// };

// void	MyAudioCallBack(void *userdata, Uint8 *stream, int streamLength)
// {
// 	struct AudioData *audio = (struct AudioData*)userdata;

// 	if (audio->length == 0)
// 		return ;

// 	Uint32  length = (Uint32)streamLength;
// 	length = (length > audio->length ? audio->length : length);

// 	SDL_memcpy(stream, audio->pos, length);

// 	audio->pos += length;
// 	audio->length -= length;
// }

// void	fill_audio(void *udata, Uint8 *stream, int len, Uint32 audio_len, Uint8 *audio_pos)
// {
// 	/* Only play if we have data left */
// 	if (audio_len == 0)
// 		return ;
	
// 	/* Mix as much data as possible */
// 	if (len > audio_len)
// 		len = audio_len;
// 	else
// 		len = len;
// 	SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);
// 	audio_pos += len;
// 	audio_len -= len;
// }


// void	test_sound(t_sdlcontext *sdl)
// {
// 	SDL_AudioSpec	wanted;
// 	SDL_AudioSpec	*test;
// 	extern void	fill_audio(void *udata, Uint8 *strem, int len, Uint32 audio_len, Uint8 *audio_pos);

// 	SDL_memset(&wanted, 0, sizeof(wanted)); /* or SDL_zero(wanted) */
// 	wanted.freq = 22050;
// 	wanted.format = AUDIO_S16;
// 	wanted.channels = 2;	// 1 = mono, 2 = stereo 
// 	wanted.samples = 1024;	// good low-latency value for callback
// 	wanted.callback = fill_audio;
	
// 	//With SDL >= 2.0.4 you can choose to avoid callbacks and use SDL_QueueAudio() instead, if you like. Just open your audio device with a NULL callback.
// 	//want.callback = NULL;
// 	//SDL_AudioDeviceID dev = SDL_OpenAudioDevice(NULL, 0, &want, &have, SDL_AUDIO_ALLOW_FORMAT_CHANGE);
// 	//if (dev == 0)
// 	//	error_log(EC_SDL_OPENAUDIODEVICE);

// 	if (SDL_OpenAudio(&wanted, NULL) < 0)
// 		error_log(EC_SDL_OPENAUDIODEVICE);



// 	static Uint8 *audio_chunk;
// 	static Uint32 audio_len;
// 	static Uint8 *audio_pos;

// 	/* The audio function callback takes the following parameters:
//        stream:  A pointer to the audio buffer to be filled
//        len:     The length (in bytes) of the audio buffer
//     */

// 	/* Load the audio data ... */

// 	test = SDL_LoadWAV("assets/audio/cloud-10_gunshot.wav", &wanted, audio_chunk, audio_len);

// 	audio_pos = audio_chunk;

// 	/* Let the callback function play the audio chunk */
//     //SDL_PauseAudio(0);

//     /* Do some processing */

//     ;;;;;

//     /* Wait for sound to complete */
//     while ( audio_len > 0 ) {
//         SDL_Delay(100);         /* Sleep 1/10 second */
//     }
//     SDL_CloseAudio();

// }
