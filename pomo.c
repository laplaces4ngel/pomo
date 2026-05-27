#include "raylib.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
// describes break or studying state
typedef struct{
  float total_time;
  char header[30];
  bool is_study;
  int cycles;
}State;

int nc = 3;

void toggle_state(State *state)
{
  state->is_study = !state->is_study;
  if(state->is_study)
  {
    state->cycles-=1;
    state->total_time = 45*60; //user defined
    strcpy(state->header, "study :<");
  }
  else
  {
    state->total_time = 15*60; //user defined
    if(state->cycles==1){
      state->total_time *= nc; //4 can be user defined
    }
    strcpy(state->header, "brek >:3");
  }
}
// testing to see if path replacement worke
int main(void)
{
    State state = {45*60, "study :<", true, nc}; //first and last values should be user defined, 3 is number of cycles
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
            //DrawGrid(10, 12.0f);
              for(int i=0; i<240; i+=30)
                DrawLine(i,0,i,240, LIGHTGRAY);
              for(int i=0; i<240; i+=30)
                DrawLine(0,i,240,i, LIGHTGRAY);
        // if user presses t, timer runnings, if user presses y, timer pauses
        if (IsKeyPressed(KEY_T)) {
            if(laps == 0)
            start_time = GetTime();
        if(is_paused) {
          if(is_switch)
            pause_time = 0;
          //pause_time = GetTime() - pause_time;
          start_time+=GetTime() - pause_time;
          printf("%0.0f \n", pause_time);
        }
          is_paused = false;
          running = true;
          is_switch = false;
        } 
        if (state.cycles==0){
            running = false;
            state.cycles = 3;
        }
        if(IsKeyPressed(KEY_S)){
          running = false;
          is_paused = true;
          toggle_state(&state);
          start_time = GetTime();
          pause_time = GetTime();
          is_switch=true;
          laps = 0;
        }
        if (IsKeyPressed(KEY_Y)){
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

        mins = (int)(state.total_time - laps)/60;
        secs = ((int)state.total_time - laps)%60;

        if(round(state.total_time)==laps) {
          PlaySound(done);
          toggle_state(&state);
          start_time=GetTime();
          pause_time = 0;
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
    CloseAudioDevice();
    CloseWindow();

    return 0;
}
