# audio_visualizer
* Library Pre-requisites : 
  * Unix Operating Systems:
    * Debian Based Distributions : ```libsdl2-dev``` ```libfftw3-dev```
    * Special Note: Make sure you have ```GLIB_C2.29``` or above (comes preinstalled from ```Ubuntu_19.04``` onwards)
    * Others : Please check the requirements on their websites
  * Windows 10: (is only required for compilation)
    * [```minGW```](https://winlibs.com/) (```GCC 8.4.0 64-bit``` or above is recommended)
* Compile Commands : 
  * Unix Operating Systems:
    * ```make``` or ```g++ src/*.cpp -lSDL2 -lfftw3 -o musico``` 
    * ```./musico -f Path_to_wav_file``` to play a certain song
    * ```./musico``` for recording and then playback
  * Windows 10:
    * run the batch file called ```minGW_compile.bat```
    * similarly run ```musico.exe``` available in the ```winexe``` folder
    * Special note: Notice that the windows executable is inside a subfolder so you may have to use proper directories
* ```P.S.``` Please make sure that you're inputing the correct filename with the correct extension (which is ```.wav``` if you are wondering :))
