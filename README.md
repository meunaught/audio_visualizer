# audio_visualizer
* Library Pre-requisites : 
  * Debian Based Distributions : ```libsdl2-dev``` ```libfftw3-dev```
  * Special Note: Make sure you ```GLIB_C2.29``` or above (comes preinstalled from ```Ubuntu_19.04``` onwards)
  * Others : Please check the requirements on their websites
  * Windows is not supported :)
* Compile Commands : 
  * ```make``` or ```g++ *.cpp -lSDL2 -lfftw3 -o musico``` 
  * ```./musico -f Song_Name.wav``` to play a certain song
  * ```./musico``` for recording and then playback
* P.S. Please make sure that you're inputing the correct filename with the correct extension (which is ```.wav``` if you are wondering :))
