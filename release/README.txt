RoQ Player v2.1 (now based on DreamRoQ)
=====================================

v2.1 fixes a crash on real hardware
Done by gameblabla, source code is at https://github.com/gameblabla/dreamroq.

Warning : It's worth noting that this version will reject any video files that are not exactly 320x240.
In doubt, reconvert them with the command shown below.


RoQ Player is a video player for the TI-Nspire that can playback RoQ type files,

a video format that was first used in the game the 11th Guest and later Quake 3 Arena.

!!! YOU NEED TO HAVE NDLESS INSTALLED ON YOUR TI-NSPIRE TO RUN IT !!!

Its quality is comparable to MPEG-1 while targeting slow processors like the Intel 80486.

The size isn't the best though but decoding is very fast.

Converting videos
===================

ffmpeg -i myvideo.avi -s 320x240 -aspect 320:240 -r 30 -an output.roq

"myvideo.avi" is the file that is going to be converted to RoQ.

"output.roq" is the resulting RoQ File that you can run with RoQPlayer.

"-r 30" means that the video will run at 30 FPS. Lower it if you can to save space.

Controls
==========

Menu
=====

CTRL = Choose RoQ file
Touchpad/Clickpad = Move the cursor

Lecteur 
=======

ESC = Exit the video player (It will also exit itself if video reaches the end)
