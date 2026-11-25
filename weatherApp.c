// gcc weatherApp.c location.c weather.c cJSON.c -o "C the weather" -lcurl $(pkg-config --libs --cflags raylib)
// ./"C the weather"
// Maybe oneday I'll come back and make everything into functions to make it clean
#include "raylib.h"
#include <string.h>
#include "location.h"
#include "weather.h"
#include <stdlib.h>
#include <stdio.h>


#define TITLE_FONT_SIZE 25

bool pressButton(Rectangle box, const char *label, Color color);

void readAPI(char api[]);

typedef struct
{
   Rectangle box1;
} box;

int main()
{
   //CLEAN THIS UP PLEASE
   char place[20] = {0},
        textLabel[6][15] = {"Location:", "Condition:", "Temperature:", "Feels like:",
                              "Precipitation:", "Humidity:"},
        state[3],
        chosenState[3] = {0},
        chosenCity[26] = {0};
   int index,
       screenWidth  = 800,
       screenHeight = 450,
       xTextLocation[6] = {0},
       headerTextYLocation[6] = {20, 90, 160, 230, 300, 370},
       informationTextLocation[6] = {0}; //not yet
   int key,
       cityLetterCount = 0,
       stateLetterCount = 0;
               //loc x loc y size x size y
   box exitButton =   {{0, 0, 48, 48}};
   box locationDisplay = {{575, -100, 200, 600}};
   box chosenLocationDisplay = {{300, -100, 200, 600}};
   box cityText = {{300, 0, 200, 24}};
   box stateText = {{400, 24, 100, 24}};
   
   char apiKey[51];

   //Name and make window
   InitWindow(screenWidth, screenHeight, "C The Weather");
   SetTargetFPS(15);

   Texture2D backgroundImage = LoadTexture("Background.jpg");

   //Finds location
   locations locationVar = location();
   strcpy(place, locationVar.city);
   strcpy(state, locationVar.state);

   //API key read
   readAPI(apiKey);

   //Weather function for finding locations weather
   weatherTemp weather = findWeather(place, state, apiKey);
   weatherTemp chosenLocation = {0};

   //Math loop for centering text on locationDisplay and chosenLocationDisplay
   for(index = 0; index < 6; index++)
   {
      xTextLocation[index] = (locationDisplay.box1.width - MeasureText(textLabel[index], 
         TITLE_FONT_SIZE)) / 2;
   }

   //Main display loop
   while (!WindowShouldClose()) 
   {
      BeginDrawing();
      ClearBackground(BLANK);
      DrawTexture(backgroundImage, 0, 0, WHITE);

      //Draw info display box
      DrawRectangleRounded(locationDisplay.box1, 1, 64, Fade(BLACK, 0.4f));
      DrawRectangleRounded(chosenLocationDisplay.box1, 1, 64, Fade(BLACK, 0.4f));

      //Draw text box
      DrawRectangleRec(cityText.box1, Fade(RAYWHITE, 0.5f));
      DrawRectangleRec(stateText.box1, Fade(RAYWHITE, 0.5f));

      //Draw text on display box
      for(index = 0; index < 6; index++)
      {
         DrawText(TextFormat("%s", textLabel[index]), locationDisplay.box1.x + 
            xTextLocation[index], headerTextYLocation[index], TITLE_FONT_SIZE, 
            WHITE);
         
         //Determines when to print separating lines
         if(index < 5) 
         {  
            DrawLine(575, headerTextYLocation[index] + 60, 775, 
               headerTextYLocation[index] + 60, RAYWHITE);   
            DrawLine(300, headerTextYLocation[index] + 60, 500, 
               headerTextYLocation[index] + 60, RAYWHITE);
         }
               
         //Risky!! But I do not want to print index 0 ("Location:) for this part.
         index++;
         DrawText(TextFormat("%s", textLabel[index]), chosenLocationDisplay.box1.x + 
            xTextLocation[index], headerTextYLocation[index], TITLE_FONT_SIZE, 
            WHITE);
         index--;
      }

      //Draws disclaimer
      DrawText("Based on IP address", locationDisplay.box1.x + 
         (locationDisplay. box1. width - MeasureText("Based on IP address", 5)) 
         / 2, 42, 5, WHITE);

      //Draw location information
      DrawText(TextFormat("- %s", place), 580, 55, 20, WHITE);
      DrawText(TextFormat("- %s", weather.conditions), 580, 125, 20, WHITE);
      DrawText(TextFormat("- %d°F", weather.temp), 580, 195, 20, WHITE);
      DrawText(TextFormat("- %d°F", weather.feelsLike), 580, 265, 20, WHITE);
      DrawText(TextFormat("- %d%%", (int)(weather.percentOfPrecipitation * 100)), 
         580, 335, 20, WHITE);
      DrawText(TextFormat("- %d%%", weather.humidity), 580, 405, 20, WHITE);

      key = GetCharPressed();

      //I know there is an easier way but its 12:00a.m.
      //City box selected
      if(CheckCollisionPointRec(GetMousePosition(), cityText.box1))
      {
         DrawRectangleLines(300, 0, 200, 24, RED);
         DrawRectangleLines(400, 25, 100, 24, BLACK);

         SetMouseCursor(MOUSE_CURSOR_IBEAM);

         while (key > 0)
         {
            if ((key >= 32) && (key <= 125) && (cityLetterCount < 25))
            {
               chosenCity[cityLetterCount] = (char)key;
               chosenCity[cityLetterCount+1] = '\0';
               cityLetterCount++;
            }

            key = GetCharPressed();
         }

         if (IsKeyPressed(KEY_BACKSPACE))
         {
            cityLetterCount--;
            if (cityLetterCount < 0) cityLetterCount = 0;
            chosenCity[cityLetterCount] = '\0';
         }
      }
      //State box selected 
      else if(CheckCollisionPointRec(GetMousePosition(), stateText.box1))
      {
         DrawRectangleLines(300, 0, 200, 24, BLACK);
         DrawRectangleLines(400, 25, 100, 24, RED);

         SetMouseCursor(MOUSE_CURSOR_IBEAM);

         while (key > 0)
         {
            if ((key >= 32) && (key <= 125) && (stateLetterCount < 2))
            {
               chosenState[stateLetterCount] = (char)key;
               chosenState[stateLetterCount+1] = '\0';
               stateLetterCount++;
            }

            key = GetCharPressed();
         }

         if (IsKeyPressed(KEY_BACKSPACE))
         {
            stateLetterCount--;
            if (stateLetterCount < 0) stateLetterCount = 0;
           chosenState[stateLetterCount] = '\0';
         }
      }
      //No box selected
      else
      {
         DrawRectangleLines(300, 0, 200, 24, BLACK);
         DrawRectangleLines(400, 25, 100, 24, BLACK);

         SetMouseCursor(MOUSE_CURSOR_DEFAULT);
      }

      //Draws what is in text box
      DrawText(chosenCity, 303, 6, 19, DARKGRAY);
      DrawText(chosenState, 403, 30, 20, DARKGRAY);

      //Draw chosen location information
      DrawText("Region:", 315, 25, 18, BLACK);
      DrawText("-Works best when using US locations", 305, 60, 10, WHITE);
      DrawText(TextFormat("- %s", chosenLocation.conditions), 305, 125, 20, WHITE);
      DrawText(TextFormat("- %d°F", chosenLocation.temp), 305, 195, 20, WHITE);
      DrawText(TextFormat("- %d°F", chosenLocation.feelsLike), 305, 265, 20, WHITE);
      DrawText(TextFormat("- %d%%", (int)(chosenLocation.percentOfPrecipitation * 100)), 
         305, 335, 20, WHITE);
      DrawText(TextFormat("- %d%%", chosenLocation.humidity), 305, 405, 20, WHITE);

      //x button
      if(pressButton(exitButton.box1, " x", YELLOW))
      {
         break;
      }

      //Gives parameters to findWeather function
      if(IsKeyPressed(KEY_ENTER) && cityLetterCount > 0 && stateLetterCount > 0)
      {
         chosenLocation = findWeather(chosenCity, chosenState, apiKey);
      }

      EndDrawing();
   }
   
   UnloadTexture(backgroundImage);
   CloseWindow();
   return 0;
}

/*****************************************************************************
| Exit button logic.
******************************************************************************/
bool pressButton(Rectangle box, const char *label, Color color)
{
   Vector2 mouse = GetMousePosition();
   bool hovering = CheckCollisionPointRec(mouse, box);

   //Color tint
   DrawRectangleRec(box, hovering ? Fade(GRAY, 0.6f) : Fade(color, 0.8f));

   DrawText(label, box.x + 10, box.y + 10, 20, WHITE);
   return hovering && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

/*****************************************************************************
| Gets API key.
******************************************************************************/
void readAPI(char apiKey[])
{
   FILE *apiKeyFile = fopen("apiKey.txt", "r");
   int index = 0,
       fileCharacters;

   if (apiKeyFile == NULL)
   {
      printf("Error opening file!");
   }

   while((fileCharacters = fgetc(apiKeyFile)) != EOF)
   {
      apiKey[index] = fileCharacters;
      index++;
   }

   fclose(apiKeyFile);

   return;
}