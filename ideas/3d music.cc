#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() {
  ALCdevice *device;
  ALCcontext *context;
  device = alcOpenDevice(NULL);
  context = alcCreateContext(device, NULL);
  alcMakeContextCurrent(context);

  // Set up the source and listener
  ALfloat listenerPos[] = { 0.0, 0.0, 0.0 };
  ALfloat listenerVel[] = { 0.0, 0.0, 0.0 };
  ALfloat listenerOri[] = { 0.0, 0.0, -1.0,  0.0, 1.0, 0.0 };
  alListenerfv(AL_POSITION, listenerPos);
  alListenerfv(AL_VELOCITY, listenerVel);
  alListenerfv(AL_ORIENTATION, listenerOri);

  // Load and play the sound
  ALuint buffer;
  ALuint source;
  alGenBuffers(1, &buffer);
  alGenSources(1, &source);
  alutLoadWAVFile((ALbyte*)"song.wav", &buffer);
  alSourcei(source, AL_BUFFER, buffer);
  alSourcePlay(source);

  // Set the 3D position of the source
  ALfloat sourcePos[] = { 0.0, 0.0, 1.0 };
  alSourcefv(source, AL_POSITION, sourcePos);

  // Wait for the sound to finish playing
  ALint state;
  do {
    alGetSourcei(source, AL_SOURCE_STATE, &state);
  } while (state == AL_PLAYING);

  // Clean up
  alDeleteSources(1, &source);
  alDeleteBuffers(1, &buffer);
  alcMakeContextCurrent(NULL);
  alcDestroyContext(context);
  alcCloseDevice(device);

  return 0;
}
