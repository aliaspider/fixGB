
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <malloc.h>
#include <string.h>
#include <inttypes.h>
#include <time.h>
#include <math.h>
#include "cpu.h"
#include "input.h"
#include "ppu.h"
#include "mem.h"
#include "apu.h"

#include "../interface.h"
#define VISIBLE_DOTS 160
#define VISIBLE_LINES 144

int gbEmuLoadGame(const char *filename);
void gbEmuMainLoop(void);
void gbEmuDeinit(void);
extern uint8_t inValReads[8];
extern uint32_t textureImage[0x9A00];
extern volatile bool emuRenderFrame;
extern const char *VERSION_STRING;

void memLoadSave()
{

}
void memSaveGame()
{

}
int audioInit()
{
   return 1;
}
void audioDeinit()
{

}
void audioSleep()
{

}


void module_init(const module_init_info_t *init_info, module_info_t *module_info)
{
   if (gbEmuLoadGame(init_info->filename) != EXIT_SUCCESS)
      debug_log("load error\n");

   module_info->output_width = VISIBLE_DOTS;
   module_info->output_height = VISIBLE_LINES;
   module_info->screen_format = screen_format_ARGB8888;
   module_info->stereo = true;
   module_info->framerate = 60;
   module_info->audio_rate = (float)apuGetFrequency();

   debug_log("module init\n");
}

void module_destroy()
{
   debug_log("module destroy\n");


}

void module_run(module_run_info_t *run_info)
{

   inValReads[BUTTON_A]      = run_info->pad->buttons.A;
   inValReads[BUTTON_B]      = run_info->pad->buttons.B;
   inValReads[BUTTON_SELECT] = run_info->pad->buttons.select;
   inValReads[BUTTON_START]  = run_info->pad->buttons.start;
   inValReads[BUTTON_RIGHT]  = run_info->pad->buttons.right;
   inValReads[BUTTON_LEFT]   = run_info->pad->buttons.left;
   inValReads[BUTTON_UP]     = run_info->pad->buttons.up;
   inValReads[BUTTON_DOWN]   = run_info->pad->buttons.down;

   while(!emuRenderFrame)
      gbEmuMainLoop();

   for(int i = 0; i < VISIBLE_LINES; i++)
      memcpy(run_info->screen.u8 + run_info->pitch * i * 4, textureImage + VISIBLE_DOTS * i, VISIBLE_DOTS * sizeof(uint32_t));

   emuRenderFrame = false;
   run_info->frame_completed = true;
}

int audioUpdate()
{
#if 0
#if AUDIO_FLOAT
   static int16_t buffer[512 * 2];
   float* buffer_in = (float*)apuGetBuf();
   int16_t* out_ptr = buffer;
   int samples = apuGetBufSize() / sizeof(float);
   while (samples)
   {
#define CLAMP_16(x) (((x) > INT16_MAX)? INT16_MAX : ((x) < INT16_MIN)? INT16_MIN : (x))
      *(out_ptr++) = CLAMP_16((*buffer_in * 0x4000) - 0x2000);
      buffer_in++;
      samples--;
      if (out_ptr == &buffer[512 * 2])
      {
         audio_batch_cb(buffer, 512);
         out_ptr = buffer;
      }
   }

   if (out_ptr > buffer)
      audio_batch_cb(buffer, (out_ptr - buffer) / 2);
#else
   uint16_t* buffer_in = (uint16_t*)apuGetBuf();
   int samples = apuGetBufSize() / (2 * sizeof(uint16_t));
   while (samples > 512)
   {
     audio_batch_cb(buffer_in, 512);
     buffer_in += 1024;
     samples -= 512;
   }
   audio_batch_cb(buffer_in, samples);
#endif
#endif
   return 1;
}
