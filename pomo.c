#include "raylib.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
// describes break or studying state
typedef struct{
  float total_time; //total time of the timer (50 mins, 10 mins)
  char header[30]; // display study or break
  bool is_study; // checking if the current state is the study state
  int cycles; // number of study-break cycles
}State;

int nc = 3; //number of cycles
// toggles states (study to break and vice versa)
void toggle_state(State *state)
{
  state->is_study = !state->is_study;
  if(state->is_study)
  {
    state->cycles-=1;
    state->total_time = 50*60; //user defined
    strcpy(state->header, "study :<");
  }
  else
  {
    state->total_time = 10*60; //user defined
    if(state->cycles==1){
      state->total_time *= (nc-1); //can be user defined
    }
    strcpy(state->header, "brek >:3");
  }
}
int main(void)
{
    State state = {50*60, "study :<", true, nc}; 
    InitWindow(240, 210, "raylib example - basic window");
    InitAudioDevice();   
    if(IsAudioDeviceReady())
      printf("yuh \n");
    SetMasterVolume(.5);
    printf("get master volume: %.2f", GetMasterVolume());
    SetTargetFPS(60); 
    SetWindowPosition(1659, 45);

    Sound done = LoadSound("your_timer_sound_mp3_file_directory_here");
    
    int laps=0;
    int secs=0;
    bool running=false;
    bool is_paused=false;
    bool is_switch=false;
    int mins=0;
    float start_time=0.0f;
    float pause_time=0.0f;

    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(RAYWHITE);
              for(int i=0; i<240; i+=30)
                DrawLine(i,0,i,240, LIGHTGRAY);
              for(int i=0; i<240; i+=30)
                DrawLine(0,i,240,i, LIGHTGRAY);
        // if user presses t, timer runnings, if user presses y, timer pauses
        if (IsKeyPressed(KEY_T)) {
            if(laps == 0)
            start_time = GetTime();
        if(is_paused && !is_switch) {
          pause_time = GetTime() - pause_time;
          start_time+=pause_time;
          printf("%0.0f \n", pause_time);
        }
          is_paused = false;
          running = true;
          is_switch = false;
        } 
        if (state.cycles==0){
            running = false;
//          toggle_state(&state);
            state.cycles = nc;
            pause_time=0.0f;
            start_time=0.0f;
            is_paused = false;
            is_switch = false;
            laps=0;
        }
        if(IsKeyPressed(KEY_S)){
          running = false;
          is_paused = false;
          toggle_state(&state);
          start_time = 0;
          pause_time = 0;
          is_switch=true;
          laps = 0;
        }
        if (IsKeyPressed(KEY_Y) && running){
          running = false;
          is_paused = true;
          is_switch = false;
          pause_time = GetTime();
        }
        
        if (running && is_paused==false) {
          laps=GetTime()-start_time;
          DrawText("Press y to stop !", 40, 130, 20, MAROON);
        }
        else {
        DrawText("Press t to start !", 40, 130, 20, MAROON);
        }
        //if(mins<=0 && secs<=0)
          //DrawText(TextFormat("%s\n00:00\n",state.header), 80, 60, 30 , MAROON); 
        mins = (int)(state.total_time - laps)/60;
        secs = ((int)state.total_time - laps)%60;

        if(round(state.total_time)<=laps) { //changed to <= from ==
          PlaySound(done);
          toggle_state(&state);
          start_time=GetTime();
          pause_time = 0;
        } if (laps>=1){
              //printf("laps: %d\n", laps);
              //printf("mins: %d\n", mins);
              //printf("secs: %d\n", secs);
              //printf("pause_time: %.f\n", pause_time);
              //printf("is_switch: %d\n", is_switch);
              //printf("is_paused: %d\n", is_paused);
              //printf("running: %d\n", running);
        }
        if(mins<=0 && secs<=0)
          DrawText(TextFormat("%s\n00:00\n",state.header), 80, 60, 30 , MAROON);       
        else
          if(is_switch)
          DrawText(TextFormat("%s\n%d:00\n",state.header,(int)state.total_time/60,secs), 80, 60, 30 , MAROON);   
          else
          DrawText(TextFormat("%s\n%d:%d\n",state.header,mins,secs), 80, 60, 30 , MAROON);   

       EndDrawing();
    }
    UnloadSound(done); // adding to prevent resource leak
    CloseAudioDevice();
    CloseWindow();

    return 0;
}
