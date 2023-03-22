#! /bin/bash
find assets/audio -type f -name "*.wav" -o -name "*.mp3" | grep "music" > assets/asset_lists/music_list.txt
find assets/audio -type f -name "*.wav" -o -name "*.mp3" | grep -v "music" > assets/asset_lists/sound_list.txt
find assets/fonts -type f -name "*.ttf" > assets/asset_lists/font_list.txt
find assets/images/tga -type f -name "*.tga" | grep -v "/env/" > assets/asset_lists/image_list.txt
find assets/images/tga/env -type f -name "*.tga" > assets/asset_lists/image_env_list.txt
find assets/objects -type f -name "*.obj" | grep -v "/animations/" > assets/asset_lists/object_list.txt
find assets/objects -type f -name "*.mtl" > assets/asset_lists/material_list.txt
find assets/objects/animations -type f -name "*.obj" | sort -V > assets/asset_lists/anim_list.txt