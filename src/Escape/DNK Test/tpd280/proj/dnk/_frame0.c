#include "hmiworks.h"
BYTE ladder_main(BYTE);
static BYTE r_out(BYTE);

int _CurrentFrame = 0;

char *g_FrameNames[] = 
{
  "Frame1",
};

const int g_FrameCount = (sizeof(g_FrameNames)/sizeof(char *));

#define MAGIC_NUM_TO_CON		472882027



volatile unsigned long g_TimerTicksCount = 0;
unsigned long g_ulTickTmrEvtTrig = 0;
void hmi_TimerTrigger()
{
  g_TimerTicksCount++;
  if (table_init_flag && (g_TimerTicksCount%(100/10)) == 0)
  { g_ulTickTmrEvtTrig = g_TimerTicksCount;
    AddQueueMessage(0);}
  
  switch(_CurrentFrame) {
    case 1: {
     TimerUpdate(&Timer7, 10);
     TimerUpdate(&Timer8, 10);
    break;
    }
  }
}

void FreeCurrentFrame()
{
  switch (_CurrentFrame)
  {
	case 1: FreeWidget1(); break;
  }
}

void hmi_GotoFrame(int nFrame)
{
  WidgetMessageQueueProcessEx(WIDGET_MSG_PAINT);

  switch(nFrame)
  {
	case 1: _Frame1(); break;
  }

  WidgetMessageQueueClear();
  WidgetPaint(WIDGET_ROOT);
}

int hmi_IndexOfFrame(const char *frame_name)
{
    int i;
    for (i=0; i<g_FrameCount; i++)
       if (strcmp(frame_name, g_FrameNames[i]) == 0) return i+1;
    return 0;
}


void hmi_GotoFrameByName(const char *frame_name)
{
    hmi_GotoFrame(hmi_IndexOfFrame(frame_name));
}

long default_timer_proc(tWidget *pWidget, unsigned long ulMessage,
                     unsigned long ulParam1, unsigned long ulParam2)

{
  ladder_main(1);
  passDataThruBindingOnChange();
  r_out(1);
  return 1;
}

static BYTE r_out(BYTE en)
{
  default_output(1);

  default_input(1);

  return 0;
}

void _frame0()
{
  tContext sContext;
  
  RegisterNullWidgetMsgProc(default_timer_proc);
  
  hmi_gr_lib_init();
  GrContextInit(&sContext, HMI_DISPLAY_DRIVER_PTR);
  hmi_GotoFrame(1);
  GrFlush(&sContext);
}
