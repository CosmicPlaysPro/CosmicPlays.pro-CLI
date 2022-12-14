#pragma once
#include "Options.h"
#include "include/obs.hpp"
#include <vector>
#include <iostream>
#include <windows.h>

#define DESKTOP_AUDIO_1 "DesktopAudioDevice1"
#define DESKTOP_AUDIO_2 "DesktopAudioDevice2"
#define AUX_AUDIO_1 "AuxAudioDevice1"
#define AUX_AUDIO_2 "AuxAudioDevice2"
#define AUX_AUDIO_3 "AuxAudioDevice3"
#define AUX_AUDIO_4 "AuxAudioDevice4"

struct sEnumInfo {
	int iIndex = 0;
	HMONITOR hMonitor = NULL;
};

class StreamMgr
{
	std::vector<OBSSource> scourcesArr;
	OBSEncoder h264Streaming;
	OBSEncoder aacStreaming;
	OBSScene curScene;
	std::vector<OBSSource> Mytransitions;
	OBSOutput streamOutput;
	OBSService service;

	OBSSource fadeTransition;
	OBSSource cutTransition;

	Options opt;

public:
	StreamMgr(Options &_opt);
	~StreamMgr();

	bool Init();
	bool StartStreaming();
	void StopStreaming();
	bool StartRecording();
	void StopRecording();

	bool InitVideo();
	bool InitAudio();

	bool CreateEncoders();
	bool LoadSources();

	void InitDefaultTransitions();
	void LoadTransitions(obs_data_array_t *transitions);
	obs_source_t* FindTransition(const char *name);
	void LoadAudioDevice(const char *name, int channel, obs_data_t *parent);
	void SetTransition(OBSSource transition);
	void CreateDefaultScene();
	void ResetAudioDevice(const char *sourceId, const char *deviceId, const char *deviceDesc, int channel);
	void Save(const char *file);

	static void SourceCreated(void *data, calldata_t *params);
	static void ShutDown();

	HMONITOR GetPrimaryMonitor();
	int GetPrimaryMonitorIndex();
};
