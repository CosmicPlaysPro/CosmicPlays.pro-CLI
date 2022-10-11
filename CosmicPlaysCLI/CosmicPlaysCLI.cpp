#include "Header.h"

extern "C" __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
extern "C" __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;

void signalHandler(int signum) {
	cout << "Interrupt signal (" << signum << ") received.\n";
	exit(signum);
}

int main(int argc, char** argv) {
	system("cls");
	signal(SIGTERM, signalHandler);
	PrintHeader();
	Options opt;
	InputParser input(argc, argv);
	if (!CheckInput(input, opt)) {
		cout << "[Error] Wrong parameters provided!" << endl;
		return 0;
	}
	PrintOptions(opt);

	StreamMgr mgr(opt);
	if (!mgr.Init()) {
		cout << "OBS failed to start!" << endl;
		mgr.ShutDown();
		exit(500);
	}
	else {
		cout << "OBS started Successfully." << endl;
	}
	mgr.StartRecording();
	string command;
	while (command != "stop") {
		cout << "Type 'stop' to stop recording" << endl;
		cin >> command;
	}
	mgr.StopRecording();
	return 0;
}
