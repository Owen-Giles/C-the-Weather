#include <curl/curl.h>
#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "location.h"

size_t write_callback(char *response, size_t item_size, size_t item_count, void *userdata)
{
   size_t total_bytes = item_size * item_count;

   cJSON *json   = cJSON_Parse(response);
   cJSON *city   = cJSON_GetObjectItem(json, "city");
   cJSON *region = cJSON_GetObjectItem(json, "region");

   //Memory alocation
   if(city)    snprintf(((locations *)userdata)->city,  50, "%s", city->valuestring);
   if(region)  snprintf(((locations *)userdata)->state, 50, "%s", region->valuestring);

   cJSON_Delete(json);
   return total_bytes;
}

locations location()
{
   CURL *locationAPI = curl_easy_init();
   locations locationResult = {0};
   
   curl_easy_setopt(locationAPI, CURLOPT_URL, "http://ip-api.com/json");
   curl_easy_setopt(locationAPI, CURLOPT_WRITEFUNCTION, write_callback);
   curl_easy_setopt(locationAPI, CURLOPT_WRITEDATA, &locationResult);

   curl_easy_perform(locationAPI);
   curl_easy_cleanup(locationAPI);

   //Caller must free() this
   return locationResult;  
}