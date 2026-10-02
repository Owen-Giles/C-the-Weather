#ifndef WEATHER_H  
#define WEATHER_H

typedef struct 
{
   int temp,
       feelsLike,
       humidity;
    float percentOfPrecipitation;
    char conditions[21];
} weatherTemp;

weatherTemp findWeather(char *city, char *state, char *apiKey);

#endif
