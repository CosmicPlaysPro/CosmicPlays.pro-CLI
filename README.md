# CosmicPlays.pro-CLI
 
Source code of CosmicPlays.pro-CLI video recorder based on [OBS.Studio](https://github.com/obsproject/obs-studio)

Get the app here: [CosmicPlays.pro](https://cosmicplays.pro)

## Running

```bash
CosmicPlaysCLI.exe --help
```

## Usage

```bash
------------------------
--   CosmicPlaysCLI   --
------------------------
- Website: https://CosmicPlays.pro
- Partner: https://CosmicBoost.pro
- Powered by: OBS Studio (https://github.com/obsproject/obs-studio)


Usage: CosmicPlaysCLI.exe <parameters>

Available parameters:
--fps      <30/60> Change FPS of recorded video (default: 30)
--output   <1280x768/1920x1080/fullRes> Change FPS of recorded video (default: fullRes)
--bitrate  <2500/9000/24000> Change video Bitrate (default: 2500)
--path     <D:\Recordings> Change recording location (default: C:\Users\<user>\Videos)
--fileName <default/filename[.mp4/.mkv]> Change recording file name (default: rec-<datetime>.mp4)
--mode <1/2> Change recording mode: 1 - ffmuxer, 2 - ffmpeg (default: 1)
--muxerFlags <flags> Change ffmuxer flags (default: movflags=frag_keyframe min_frag_duration=4000000)
```
