#pragma once

char filePath[256], ffmuxerFlags[512];

void PrintHeader() {
	cout << "------------------------" << endl;
	cout << "--   CosmicPlaysCLI   --" << endl;
	cout << "------------------------" << endl;
	cout << "- Website: https://CosmicPlays.pro" << endl;
	cout << "- Partner: https://CosmicBoost.pro" << endl;
	cout << "- Powered by: OBS Studio (https://github.com/obsproject/obs-studio)" << endl;
	cout << endl;
}

void PrintHelp() {
	cout << endl;
	cout << "Usage: CosmicPlaysCLI.exe <parameters>" << endl << endl;
	cout << "Available parameters:" << endl;
	cout << "--fps      <30/60> Change FPS of recorded video (default: 30)" << endl;
	cout << "--output   <1280x768/1920x1080/fullRes> Change FPS of recorded video (default: fullRes)" << endl;
	cout << "--bitrate  <2500/9000/24000> Change video Bitrate (default: 2500)" << endl;
	cout << "--path     <D:\\Recordings> Change recording location (default: C:\\Users\\<user>\\Videos)" << endl;
	cout << "--fileName <default/filename[.mp4/.mkv]> Change recording file name (default: rec-<datetime>.mp4)" << endl;
	cout << "--mode <1/2> Change recording mode: 1 - ffmuxer, 2 - ffmpeg (default: 1)" << endl;
	cout << "--muxerFlags <flags> Change ffmuxer flags (default: movflags=frag_keyframe min_frag_duration=4000000)" << endl;
	cout << endl;
}

void HideConsole() {
	try {
		::ShowWindow(::GetConsoleWindow(), SW_HIDE);
	}
	catch (...) {
		cout << "[Error] Cannot hide the console" << endl;
	}
}

void ShowConsole() {
	try {
		::ShowWindow(::GetConsoleWindow(), SW_SHOW);
	}
	catch (...) {
		cout << "[Error] Cannot show the console" << endl;
	}
}

int ParseInt(string s) {
	int num;
	try {
		num = stoi(s);
	}
	catch(...) {
		num = -1;
	}
	return num;
}

void StringToWString(std::wstring& ws, const std::string& s) {
	std::wstring wsTmp(s.begin(), s.end());
	ws = wsTmp;
}

bool createDirectoryRecursively(const std::wstring& directory) {
	static const std::wstring separators(L"\\/");
	DWORD fileAttributes = ::GetFileAttributesW(directory.c_str());
	if (fileAttributes == INVALID_FILE_ATTRIBUTES) {
		std::size_t slashIndex = directory.find_last_of(separators);
		if (slashIndex != std::wstring::npos) {
			createDirectoryRecursively(directory.substr(0, slashIndex));
		}
		BOOL result = ::CreateDirectoryW(directory.c_str(), nullptr);
		if (result == FALSE) {
			return false;
		}
	}
	else {
		return true;
	}
	return false;
}

char* GetFilePath(string path, string fileName) {
	if (fileName.empty() || fileName == "default") {
		time_t     now = time(0);
		struct tm  tstruct;
		char       buf[80];
		localtime_s(&tstruct, &now);
		strftime(buf, sizeof(buf), "%Y-%m-%d-%H-%I-%S", &tstruct);
		snprintf(filePath, sizeof(filePath), "%s\\rec-%s.mp4", path.c_str(), buf);
	}
	else {
		snprintf(filePath, sizeof(filePath), "%s\\%s", path.c_str(), fileName.c_str());
	}
	return filePath;
}

char* GetFFMuxerFlags(string muxerFlags) {
	if (muxerFlags.empty() || muxerFlags == "default") {
		snprintf(ffmuxerFlags, sizeof(ffmuxerFlags), "movflags=frag_keyframe min_frag_duration=4000000");
	}
	else {
		snprintf(ffmuxerFlags, sizeof(ffmuxerFlags), "%s", muxerFlags.c_str());
	}
	return ffmuxerFlags;
}

bool CheckInput(InputParser input, Options &opt) {
	int iFPS, iMode, iBitrate, outX, outY, pUserErr;
	char path[256];
	char* pUser;
	BOOL fScreenX, fScreenY;
	size_t pUserLen;
	smatch outRegex;
	wstring directory;

	try {
		::SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE);
		fScreenX = GetSystemMetrics(SM_CXSCREEN);
		fScreenY = GetSystemMetrics(SM_CYSCREEN);
		opt.screenX = fScreenX;
		opt.screenY = fScreenY;
	}
	catch (...) {
		cout << "[Error] Cannot determine your screen resolution!" << endl;
		exit(500);
	}

	if (input.cmdOptionExists("-h") || input.cmdOptionExists("-help") || input.cmdOptionExists("--help")) {
		PrintHelp();
		exit(0);
	}

	if (input.cmdOptionExists("--fps")) {
		string fps = input.getCmdOption("--fps");
		if (fps.empty())
			return false;
		iFPS = ParseInt(fps);
		if (iFPS != 30 && iFPS != 60)
			return false;
		opt.fps = iFPS;
	}	
	
	if (input.cmdOptionExists("--mode")) {
		string mode = input.getCmdOption("--mode");
		if (mode.empty())
			return false;
		iMode = ParseInt(mode);
		if (iMode != 1 && iMode!= 2)
			return false;
		opt.mode = iMode;
	}

	if (input.cmdOptionExists("--bitrate")) {
		string bitrate = input.getCmdOption("--bitrate");
		if (bitrate.empty())
			return false;
		iBitrate = ParseInt(bitrate);
		if (iBitrate < 2500 || iBitrate > 24000)
			return false;
		opt.bitrate = iBitrate;
	}

	if (input.cmdOptionExists("--path")) {
		string path = input.getCmdOption("--path");
		if (path.empty())
			return false;
		opt.path = path;
	}

	if (input.cmdOptionExists("--muxerFlags")) {
		string muxerFlags = input.getCmdOption("--muxerFlags");
		if (muxerFlags.empty())
			return false;
		opt.muxerFlags = GetFFMuxerFlags(muxerFlags);
	}

	if (input.cmdOptionExists("--fileName")) {
		string fileName = input.getCmdOption("--fileName");
		if (fileName.empty())
			return false;
		if (fileName != "default" && fileName.find(".mp4") == string::npos && fileName.find(".mkv") == string::npos) {
			cout << "[Error] File name must contain .mp4 or .mkv extension" << endl;
			exit(500);
		}
		opt.fileName = fileName;
	}

	if (input.cmdOptionExists("--output")) {
		string output = input.getCmdOption("--output");
		if (output.empty())
			return false;
		if (output == "fullRes") {
			opt.outputX = fScreenX;
			opt.outputY = fScreenY;
		}
		else {
			try {
				regex rx("([0-9]{3,4})x([0-9]{3,4})");
				if (!regex_match(output, outRegex, rx))
					return false;
			}
			catch (...) {
				return false;
			}
			outX = ParseInt(outRegex[1]);
			outY = ParseInt(outRegex[2]);
			if (outX < 768 || outY < 768)
				return false;
			opt.outputX = outX;
			opt.outputY = outY;
		}
	}

	if (input.cmdOptionExists("-hide") || input.cmdOptionExists("--hide")) {
		HideConsole();
	}

	if (opt.path == "default") {
		pUserErr = _dupenv_s(&pUser, &pUserLen, "USERPROFILE");
		if (!pUserErr) {
			snprintf(path, sizeof path, "%s\\Videos\\CosmicPlays", pUser);
			opt.path = path;
		}
	}
	opt.filePath = GetFilePath(opt.path, opt.fileName);

	StringToWString(directory, opt.path);
	if (!createDirectoryRecursively(directory)) {
		cout << "[Error] Cannot create directory at specified path: " << opt.path << endl;
		exit(500);
	}

	return true;
}

void PrintOptions(Options &opt) {
	cout << "-- Recording options:" << endl;
	cout << "------------------------" << endl;
	cout << "- FPS: " << opt.fps << endl;
	cout << "- Mode: " << opt.mode << endl;
	cout << "- Video bitrate: " << opt.bitrate << endl;
	cout << "- Audio bitrate: " << opt.audioBitrate << endl;
	cout << "- Screen res: " << opt.screenX << "x" << opt.screenY << endl;
	cout << "- Output res: " << opt.outputX << "x" << opt.outputY << endl;
	cout << "- Path: " << opt.path << endl;
	cout << "- File: " << opt.filePath << endl;
	cout << "------------------------" << endl;
	cout << endl;
}
