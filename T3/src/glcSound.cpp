#include "glcSound.h"
#include <iostream>


glcSound::glcSound()
{
    alutInit(NULL, 0);
    alGetError();
}

glcSound::~glcSound()

{
    this->KillALData();
}

void glcSound::SetNumberOfSounds(int sources)
{
    this->numberOfSources = sources;

    // Allocate buffers and sources based on the required number of sources
    this->Buffers = new ALuint(sources);
    this->Sources = new ALuint(sources);
    alGenSources(sources, Sources);
}

void glcSound::AddSound(int id, string file)
{
    // Create buffer from file
    Buffers[id] = alutCreateBufferFromFile (file.c_str());

    if(alGetError() != AL_NO_ERROR) this->PrintError("glcSound::AddSound", "Error creating buffers.");

    // Create source from buffer
    ALfloat SourcePos[] = { 0.0, 0.0, 0.0 };
    ALfloat SourceVel[] = { 0.0, 0.0, 0.0 };
    ALfloat ListenerPos[] = { 0.0, 0.0, 0.0 };  // Position of the Listener.
    ALfloat ListenerVel[] = { 0.0, 0.0, 0.0 };  // Velocity of the Listener.
    // Orientation of the Listener. (first 3 elements are "at", second 3 are "up")
    // Also note that these should be units of '1'.
    ALfloat ListenerOri[] = { 0.0, 0.0, 1.0,  0.0, 1.0, 0.0 };

    alListenerfv(AL_POSITION,    ListenerPos);
    alListenerfv(AL_VELOCITY,    ListenerVel);
    alListenerfv(AL_ORIENTATION, ListenerOri);

    alSourcei (Sources[id], AL_BUFFER,   Buffers[id] );
    alSourcef (Sources[id], AL_PITCH,    1.0      );
    alSourcef (Sources[id], AL_GAIN,     1.0      );
    alSourcefv(Sources[id], AL_POSITION, SourcePos);
    alSourcefv(Sources[id], AL_VELOCITY, SourceVel);


    if(alGetError() != AL_NO_ERROR) this->PrintError("glcSound::AddSound", "Error creating Sources.");
}

void glcSound::PrintError(string func, string error)
{
    cout << "In " << func << " - " << "error";
    exit(1);
}

void glcSound::PlaySound(int id)
{
    alSourcePlay(this->Sources[id]);
}

void glcSound::KillALData()
{
    alDeleteSources(this->numberOfSources, Sources);

    ALCcontext *context = alcGetCurrentContext();
    alcDestroyContext(context);
    alutExit();
}
