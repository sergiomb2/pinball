/***************************************************************************
                          SoundUtil.cpp  -  description
                             -------------------
    begin                : Thu Jan 11 2001
    copyright            : (C) 2001 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#include "Private.h"
#include "SoundUtil.h"
#include "EMath.h"
#include "Config.h"
#include <iostream>

SoundUtil* SoundUtil::p_SoundUtil = NULL;

SoundUtil::SoundUtil() {
  m_bInited = false;
  m_iLoopingMusic = -1;
}

SoundUtil::~SoundUtil() {
  p_SoundUtil = NULL;
}

SoundUtil* SoundUtil::getInstance() {
  if (p_SoundUtil == NULL) {
    p_SoundUtil = new SoundUtil();
  }
  return p_SoundUtil;
}

int SoundUtil::initSound()
{
  int audio_rate      = EM_AUDIO_FREQ;
  Uint16 audio_format = EM_AUDIO_FORMAT;
  int audio_channels  = EM_AUDIO_CHANNELS;
  
  if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
  {
    cerr << "Couldn't init SDL aduio: " << SDL_GetError() << endl;
    m_bInited = false;

    return -1;
  }
  
  if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, 1024) < 0)
  {
    cerr << "Couldn't open audio mixer: " << SDL_GetError() << endl;
    m_bInited = false;

    return -1;
  }
  
  Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);
  cerr << "Opened audio at " << audio_rate << " Hz " 
       << (audio_format & 0xFF) << " bit "
       << (audio_channels > 1 ? "stereo" : "mono") << endl << endl;
  
  m_bInited = true;

  return 0;
}

void SoundUtil::stopSound() {
  cerr << "Stopping sound...";
  if (!m_bInited) return;
  // clean samples and music otherwise we will get segfaults cause stoping sounds
  // de-allocates waves and midis, or does it de-allocate
  // 	vector<EmSample*>::iterator sampleiter = m_vEmSample.begin();
  // 	vector<EmSample*>::iterator sampleend = m_vEmSample.end();
  // 	for (; sampleiter != sampleend; ++sampleiter) {
  // 		(*sampleiter) = NULL;
  // 	}
  // 	vector<EmMusic*>::iterator musiciter = m_vEmMusic.begin();
  // 	vector<EmMusic*>::iterator musicend = m_vEmMusic.end();
  // 	for (; musiciter != musicend; ++musiciter) {
  // 		(*musiciter) = NULL;
  // 	}

  Mix_CloseAudio();
  SDL_QuitSubSystem(SDL_INIT_AUDIO);

  cerr << "ok." << endl;
  m_bInited = false;
  m_iLoopingMusic = -1;
}

void SoundUtil::applyConfigVolume()
{
  // If no volume for sound or music then stop sound module
  if (Config::getInstance()->getSound() == 0 &&
      Config::getInstance()->getMusic() == 0)
  {
    if (m_bInited)
      this->stopSound();
  }
  // We have some sound or music volume, start sound module
  else
  {
    if (!m_bInited)
    {
      if (this->initSound() == 0)
      {
	if (Config::getInstance()->getMusic() == 0)
	{
	  Mix_HaltMusic();
	  m_iLoopingMusic = -1;
	}
	Mix_Volume(-1, Config::getInstance()->getSound()*8);
	Mix_VolumeMusic(Config::getInstance()->getMusic()*8);
      }
      else
      {
	Config::getInstance()->setSound(0);
	Config::getInstance()->setMusic(0);

	cerr << "******************************************" << endl;
	cerr << "Error opening audio device, check that"     << endl;
	cerr << "no other application is occupying audio"    << endl;
	cerr << "resources. Try to kill artsd and/or esd."   << endl;
	cerr << "******************************************" << endl;
      }
    }
  }
}

int SoundUtil::loadSample(const char* filename)
{
  // NOTE! Load samples even if the volume is off, if we change the volume
  //  settings we don't have to reload the table

  // TODO: If volume off then the samples are initialized to NULL...
  //       The samples must be re-loaded anyway...!
  //       Find a way to solve this...

  // look if the sound is already loaded
  if (m_hEmSample.find(string(filename)) != m_hEmSample.end())
  {
    EM_COUT("SoundUtil::loadSample Found sample " << filename
	    << " in cache", 0);
    map<string, int>::iterator element = m_hEmSample.find(string(filename));

    return (*element).second;
  }
  
  Mix_Chunk * wave = Mix_LoadWAV(filename);
  // Error message if we can't load wave file
  if (wave == NULL && m_bInited)
    cerr << "ERROR Mix_LoadWAV: " << Mix_GetError() << endl;
  
  m_vEmSample.push_back(wave);
  int sound = m_vEmSample.size()-1;
  m_hEmSample.insert(pair<string, int>(string(filename), sound));
  m_hSoundName.insert(pair<int, string>(sound, string(filename)));
  
  if (wave == NULL && m_bInited)
  {
    cerr << "SoundUtil::loadSample Unable to load " << filename
	 << " inserting a NULL wave" << endl;
  }

  return sound;
}

const char * SoundUtil::getSoundName(int sound) {
  map<int, string>::iterator element = m_hSoundName.find(sound);
  if (element != m_hSoundName.end()) {
    return (*element).second.c_str();
  }
  cerr << "SoundUtil::getSoundName could not find sound name" << endl;
  return NULL;
}

const char * SoundUtil::getMusicName(int music) {
  map<int, string>::iterator element = m_hMusicName.find(music);
  if (element != m_hMusicName.end()) {
    return (*element).second.c_str();
  }
	cerr << "SoundUtil::getMusicName could not find music name" << endl;
	return NULL;
}

int SoundUtil::loadMusic(const char * filename) {
  //if (!m_bInited) return -1;
  // look if the sound is already loaded
  if (m_hEmMusic.find(string(filename)) != m_hEmMusic.end()) {
    EM_COUT("Found music " << filename << " in cache", 0);
    map<string, int>::iterator element = m_hEmMusic.find(string(filename));
    return (*element).second;
  }
  
  Mix_Music * music = Mix_LoadMUS(filename);
  
  m_vEmMusic.push_back(music);
  int sound = m_vEmMusic.size()-1;
  m_hEmMusic.insert(pair<string, int>(string(filename), sound));
  m_hMusicName.insert(pair<int, string>(sound, string(filename)));
  
  if (music == NULL) {
    cerr << "SoundUtil::loadMusic Unable to load " << filename <<  " inserting a NULL stream" << endl;
  }
  return sound;
}

void SoundUtil::playSample(int sound, bool loop) {
  if (!m_bInited) return;
  if (Config::getInstance()->getSound() == 0) return;
  if (sound < 0 || sound >= (signed)m_vEmSample.size()) return;
  EM_COUT("SoundUtil::playSample " << sound, 1);
  if (m_vEmSample[sound] != NULL) {
    Mix_PlayChannel(-1, m_vEmSample[sound], (loop ? -1 : 0));
  }
}

void SoundUtil::playMusic(int music, bool loop) {
  if (!m_bInited) return;
  if (Config::getInstance()->getMusic() == 0) return;
  if (music < 0 || music >= (signed)m_vEmMusic.size()) return;
  if (loop && m_iLoopingMusic == music) return;
  m_iLoopingMusic = music;
  EM_COUT("SoundUtil::playMusic " << music, 1);
  Mix_HaltMusic();
  if (m_vEmMusic[music] != NULL) {
    Mix_PlayMusic(m_vEmMusic[music], (loop ? -1 : 0));
  }
}

void SoundUtil::stopMusic() {
  if (!m_bInited) return;
  Mix_HaltMusic();
  m_iLoopingMusic = -1;
}

void SoundUtil::pauseMusic() {
  if (!m_bInited) return;
  Mix_PauseMusic();
}

void SoundUtil::resumeMusic() {
  if (!m_bInited) return;
  Mix_ResumeMusic();
}
