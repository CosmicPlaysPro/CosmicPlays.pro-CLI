#pragma once
#include <string>
using namespace std;

class Options {
public:
	int fps = 30;
	int bitrate = 2500;
	int audioBitrate = 160;
	int screenX = 1920;
	int screenY = 1080;
	int outputX = 1920;
	int outputY = 1080;
	int mode = 1;
	int captureMethod = 1;
	char* muxerFlags;
	char* filePath;
	string output = "fullRes";
	string path = "default";
	string fileName = "default";
};
