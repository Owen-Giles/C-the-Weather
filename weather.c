// Dont use AI, it fixed this then ruined it so ill have to add comments later
#include <curl/curl.h>
#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "weather.h"

typedef struct 
{
   char *data;
   size_t size;
} Buffer;

size_t weatherWriteCallback(char *response, size_t itemSize, size_t itemCount, void *userdata)
{
   size_t totalBytes = itemSize * itemCount;
   Buffer *buf = (Buffer *)userdata;

   buf->data = realloc(buf->data, buf->size + totalBytes + 1);
   memcpy(buf->data + buf->size, response, totalBytes);
   buf->size += totalBytes;
   buf->data[buf->size] = '\0';

   return totalBytes;
}

weatherTemp findWeather(char *city, char *state, char *apiKey)
{
   weatherTemp data = {0};
   Buffer buf = {0};

   char url[256];

   CURL *weatherAPI = curl_easy_init();

   char *cityName = curl_easy_escape(weatherAPI, city, 0);
   snprintf(url, sizeof(url),
         "http://api.openweathermap.org/data/2.5/forecast?q=%s,%s,US&units=imperial&APPID=%s",
         cityName, state, apiKey);
   curl_free(cityName);

   curl_easy_setopt(weatherAPI, CURLOPT_URL, url);
   curl_easy_setopt(weatherAPI, CURLOPT_WRITEFUNCTION, weatherWriteCallback);
   curl_easy_setopt(weatherAPI, CURLOPT_WRITEDATA, &buf);

   CURLcode res = curl_easy_perform(weatherAPI);

   curl_easy_cleanup(weatherAPI);

   cJSON *root        = cJSON_Parse(buf.data);
   cJSON *list        = cJSON_GetObjectItem(root, "list");
   cJSON *entry       = cJSON_GetArrayItem(list, 0);
   cJSON *main        = cJSON_GetObjectItem(entry, "main");
   cJSON *weatherArr  = cJSON_GetObjectItem(entry, "weather");
   cJSON *weatherItem = cJSON_GetArrayItem(weatherArr, 0);

   cJSON *temp      = cJSON_GetObjectItem(main, "temp");
   cJSON *feelsLike = cJSON_GetObjectItem(main, "feels_like");
   cJSON *humidity  = cJSON_GetObjectItem(main, "humidity");
   cJSON *pop       = cJSON_GetObjectItem(entry, "pop");
   //clearer description in main
   cJSON *description = cJSON_GetObjectItem(weatherItem, "main");

   if(temp) data.temp                  = (int)temp->valuedouble;
   if(feelsLike) data.feelsLike        = (int)feelsLike->valuedouble;
   if(humidity) data.humidity          = humidity->valueint;
   if(pop) data.percentOfPrecipitation = (float)pop->valuedouble;
   if(description) snprintf(data.conditions, sizeof(data.conditions), "%s", description->valuestring);

   cJSON_Delete(root);
   free(buf.data);

   return data;
}
