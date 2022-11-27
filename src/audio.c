#include "doomnukem.h"

struct AudioData
{
    Uint8   *pos;
    Uint32  length;
};

void    MyAudioCallBack(void *userdata, Uint8 *stream, int streamLength)
{
    struct AudioData *audio = (struct AudioData*)userdata;

    if (audio->length == 0)
        return ;

    Uint32  length = (Uint32)streamLength;
    length = (length > audio->length ? audio->length : length);

    SDL_memcpy(stream, audio->pos, length);

    audio->pos += length;
    audio->length -= length;
}

void    play_sound(void)
{
    SDL_AudioSpec   wavSpec;
    Uint8           *wavStart;
    Uint32          wavLength;

    if (SDL_LoadWAV("assets/audio/godowan_bubble1.wav", &wavSpec, &wavStart, &wavLength) == NULL)
    {
        printf("couldn't load an audio file\n");
        return ;
    }

    struct AudioData audio;
    audio.pos = wavStart;
    audio.length = wavLength;

    wavSpec.callback = MyAudioCallBack;
    wavSpec.userdata = &audio;

    int devices = SDL_GetNumAudioDevices(0);

    printf("number of audiodevices: %d\n", devices);

    printf("audio device name: %s\n", SDL_GetAudioDeviceName(0, 0));

    SDL_AudioDeviceID device = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, SDL_AUDIO_ALLOW_ANY_CHANGE);
    //SDL_AudioDeviceID device = SDL_OpenAudio(&wavSpec, NULL);
    printf("device: %i\n", device);
    if (device == 0)
    {
        printf("%s\n", SDL_GetError());
        return ;
    }

    SDL_PauseAudioDevice(device, 0);

    /* while (audio.length > 0)
    {
        SDL_Delay(100);
    } */

    SDL_CloseAudioDevice(device);
    SDL_FreeWAV(wavStart);
}
