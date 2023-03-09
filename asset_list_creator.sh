#! /bin/bash
find assets/audio -type f -name "*.wav" -o -name "*.mp3" | grep "music" > assets/music_list.txt
find assets/audio -type f -name "*.wav" -o -name "*.mp3" | grep -v "music" > assets/sound_list.txt