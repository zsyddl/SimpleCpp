This is a FFmpeg Win32 shared build by Kyle Schwarz.

Zeranoe's FFmpeg Builds Home Page: <http://ffmpeg.zeranoe.com/builds/>

This build was compiled on: Jan  2 2013, at: 20:17:35

FFmpeg version: 1.0.1
  libavutil      51. 73.101 / 51. 73.101
  libavcodec     54. 59.100 / 54. 59.100
  libavformat    54. 29.104 / 54. 29.104
  libavdevice    54.  2.101 / 54.  2.101
  libavfilter     3. 17.100 /  3. 17.100
  libswscale      2.  1.101 /  2.  1.101
  libswresample   0. 15.100 /  0. 15.100
  libpostproc    52.  0.100 / 52.  0.100

This FFmpeg build was configured with:
  --disable-static
  --enable-shared
  --enable-gpl
  --enable-version3
  --disable-w32threads
  --enable-avisynth
  --enable-bzlib
  --enable-fontconfig
  --enable-frei0r
  --enable-gnutls
  --enable-libass
  --enable-libbluray
  --enable-libcaca
  --enable-libfreetype
  --enable-libgsm
  --enable-libmp3lame
  --enable-libopencore-amrnb
  --enable-libopencore-amrwb
  --enable-libopenjpeg
  --enable-libopus
  --enable-librtmp
  --enable-libschroedinger
  --enable-libtheora
  --enable-libtwolame
  --enable-libvo-aacenc
  --enable-libvo-amrwbenc
  --enable-libvorbis
  --enable-libvpx
  --enable-libx264
  --enable-libxavs
  --enable-libxvid
  --enable-zlib

This build was compiled with the following external libraries:
  bzip2 1.0.6 <http://bzip.org/>
  Fontconfig 2.10.2 <http://freedesktop.org/wiki/Software/fontconfig>
  Frei0r 20121203-git-f4bac51 <http://frei0r.dyne.org/>
  GnuTLS 3.1.5 <http://gnutls.org/>
  libass 0.10.1 <http://code.google.com/p/libass/>
  libbluray 0.2.3 <http://videolan.org/developers/libbluray.html>
  libcaca 0.99.beta18 <http://caca.zoy.org/wiki/libcaca>
  FreeType 2.4.10 <http://freetype.sourceforge.net/>
  GSM 1.0.13-4 <http://packages.debian.org/source/squeeze/libgsm>
  LAME 3.99.5 <http://lame.sourceforge.net/>
  OpenCORE AMR 0.1.3 <http://sourceforge.net/projects/opencore-amr/>
  OpenJPEG 1.5.1 <http://openjpeg.org/>
  Opus 1.0.2 <http://opus-codec.org/>
  RTMPDump 20121109-git-19d3636 <http://rtmpdump.mplayerhq.hu/>
  Schroedinger 1.0.11 <http://diracvideo.org/>
  Theora 1.1.1 <http://theora.org/>
  TwoLAME 0.3.13 <http://twolame.org/>
  VisualOn AAC 0.1.2 <https://github.com/mstorsjo/vo-aacenc>
  VisualOn AMR-WB 0.1.2 <https://github.com/mstorsjo/vo-amrwbenc>
  Vorbis 1.3.3 <http://vorbis.com/>
  vpx 1.2.0 <http://webmproject.org/>
  x264 20121108-git-1cffe9f <http://videolan.org/developers/x264.html>
  XAVS svn-r55 <http://xavs.sourceforge.net/>
  Xvid 1.3.2 <http://xvid.org/>
  zlib 1.2.7 <http://zlib.net/>

The source code for this FFmpeg build can be found at: <http://ffmpeg.zeranoe.com/builds/source/>

This build was compiled on Debian 6.0.6 (64-bit): <http://www.debian.org/>

GCC 4.7.2 was used to compile this FFmpeg build: <http://gcc.gnu.org/>

This build was compiled using the MinGW-w64 toolchain: <http://mingw-w64.sourceforge.net/>

Licenses for each library can be found in the 'licenses' folder.


0.8.2  mjpeg debug 失败
1.0.1  mjpeg release 失败  |－》原因  vs 设置－》 连接－》优化－》引用 －》 “保留未引用数据库”
1.0.1  mjpeg release ->成功 
1.1.1  mjpeg release 失败  |
