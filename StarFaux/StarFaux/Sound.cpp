/*
#include "Sound.h"

Sound::Sound(int numSounds, const char* baseDir) {
	if (baseDir[strlen(baseDir)] != '/') {
		m_baseDir = (char*)malloc(sizeof(char) * strlen(baseDir) + 2 * sizeof(char));
		strcpy(m_baseDir, baseDir);
		strcat(m_baseDir, "/");
	} else {
		m_baseDir = (char*)malloc(sizeof(char) * strlen(baseDir) + sizeof(char));
		strcpy(m_baseDir, baseDir);
	}

	m_currBuffer = 0;
	m_currSource = 0;
	m_numBuffers = numSounds;
	m_soundBuffers = (ALuint*)malloc(sizeof(ALuint) * m_numBuffers);
	m_sourceBuffers = (ALuint*)malloc(sizeof(ALuint) * MAXSOURCES);

	alutInit(0, NULL);
	alGetError();

	alGenBuffers(m_numBuffers, m_soundBuffers);
	alGenSources(MAXSOURCES, m_sourceBuffers);

	ALfloat listenerPos[3] = {0.0, 0.0, 0.0};
	ALfloat listenerVel[3] = {0.0, 0.0, 0.0};
	ALfloat listenerOri[3] = {0.0, 0.0, 0.0};
	alListenerfv(AL_POSITION,listenerPos);
	alListenerfv(AL_VELOCITY,listenerVel);
	alListenerfv(AL_ORIENTATION,listenerOri);
}

void Sound::loadSound(const char* filename) {
	char* fn = (char*)malloc(sizeof(char) * strlen(filename) + sizeof(char) * strlen(m_baseDir) + sizeof(char));
	strcpy(fn, m_baseDir);
	strcat(fn, filename);

	ALenum     format;
	ALsizei    size;
	ALsizei    freq;
	ALboolean  loop;
	ALvoid*    data;
	alutLoadWAVFile(fn, &format, &data, &size, &freq, &loop);
	alBufferData(m_soundBuffers[m_currBuffer],format,data,size,freq);
	alutUnloadWAV(format,data,size,freq);

	m_soundSrc.at(fn) = m_currBuffer;

	m_currBuffer++;
}

void Sound::playSound(const char* filename) {
	char* fn = (char*)malloc(sizeof(char) * strlen(filename) + sizeof(char) * strlen(m_baseDir) + sizeof(char));
	strcpy(fn, m_baseDir);
	strcat(fn, filename);

	ALuint id = m_soundSrc.at(fn);

	ALfloat sourcePos[3] = {0.0, 0.0, 0.0};
	ALfloat sourceVel[3] = {0.0, 0.0, 0.0};
	ALfloat sourceOri[3] = {0.0, 0.0, 0.0};
	alSourcefv (m_sourceBuffers[m_currSource], AL_POSITION, sourcePos);
	alSourcefv (m_sourceBuffers[m_currSource], AL_VELOCITY, sourceVel);
	alSourcefv (m_sourceBuffers[m_currSource], AL_DIRECTION, sourceOri);

	alSourcei(m_sourceBuffers[m_currSource], AL_BUFFER, m_soundBuffers[id]);

	alSourcePlay(m_sourceBuffers[m_currSource]);
}
*/
