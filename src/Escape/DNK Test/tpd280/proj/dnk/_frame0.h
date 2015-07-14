#ifndef ___FRAME0_H
#define ___FRAME0_H
extern void _frame0();
extern int _CurrentFrame;
extern void FreeCurrentFrame();
extern void hmi_GotoFrame(int nFrame);
extern void hmi_TimerTrigger();
extern void hmi_TurnBacklight(int bOn);
extern volatile unsigned long g_TimerTicksCount;

extern tCanvasWidget Label4;
extern tCanvasWidget Label5;
extern tPushButtonWidget TextPushButton6;
extern void _OnTextPushButton6Paint(tWidget *, unsigned long);
extern void _OnTextPushButton6Click(tWidget *pWidget);
extern void _OnTextPushButton6Release(tWidget *pWidget);

extern tTimerWidget Timer7;
extern tTimerWidget Timer8;
extern tCanvasWidget Label9;
extern void OnFrame12Paint(tWidget *pWidget, tContext *pContext);

extern tCanvasWidget CanvasFrame1;

extern void InitWidget1();

extern void FreeWidget1();


extern void _Frame1();

#endif
