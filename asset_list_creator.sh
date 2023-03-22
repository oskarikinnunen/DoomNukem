#! /bin/bash
find assets/audio -type f -name "*.wav" -o -name "*.mp3" | grep "music" > assets/.music_list.txt
find assets/audio -type f -name "*.wav" -o -name "*.mp3" | grep -v "music" > assets/.sound_list.txt
find assets/fonts -type f -name "*.ttf" > assets/.font_list.txt
find assets/images/tga -type f -name "*.tga" | grep -v "/env/" > assets/.image_list.txt
find assets/images/tga/env -type f -name "*.tga" > assets/.image_env_list.txt
find assets/objects -type f -name "*.obj" | grep -v "/animations/" > assets/.object_list.txt
find assets/objects -type f -name "*.mtl" > assets/.material_list.txt
find assets/objects/animations -type f -name "*.obj" | sort -V > assets/.anim_list.txt