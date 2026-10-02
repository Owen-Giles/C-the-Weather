                                C the Weather
===============================================================================
Description:
   This program is written in C using the raylib library to take care of the UI
   and GUI. It uses an API to find your IP address to locate where you are then
   uses a weather API (uses your key) to give you the weather of your current
   location. You can also search for a location almost anywhere, but this 
   program was designed to find weather in the US (the API will take care of 
   most national locations).

   * To compile on MacOS:
      `gcc weatherApp.c location.c weather.c cJSON.c -o "C the weather" -lcurl $(pkg-config --libs --cflags raylib)`

   * To compile on Linux:
      `Work in progress`

   * To compile on Windows
      `Not working in progress`

===============================================================================
Credits:
   * Program written by Owen Giles

   * Built with:
      - C: programming language
      - raylib: graphics, window, and UI
      - libcurl: HTTP requests
      - cJSON: JSON parsing

   * Weather information provided by OpenWeatherMap(https://openweathermap.org)

   * IP location information provided by going to http://ip-api.com/json

   * Special thanks to all the open-source authors and maintainers of raylib, 
      libcurl, and cJSON

===============================================================================
Directions:
   1. Make sure raylib is downloaded
      * https://www.raylib.com/

   2. Get an API key from OpenWeatherMap (it can take some time)

   3. Run the given compile and run command 

   4. Look at you current location weather or look up a different location's 
         current weather
