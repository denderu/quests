#define HMI_WDT_ENABLE 0
#define HMI_WDT_TIMEOUT 4000
#define HMI_WDT_CLEAR 1000
#define HMI_STARTUP_LED 0
#define HMI_STARTUP_BEEP 1
#define HMI_STARTUP_DELAY 100
#define HMI_BEEP_FREQ 800
#define HMI_BEEP_MS 25
#define HMI_LCD_AUTO_OFF 600
#define HMI_LCD_ON_BEEP 1
#define HMI_LCD_OFF_BEEP 1
#define HMI_LCD_BRIGHTNESS 180
#define HMI_TOUCH_BEEP 1
#define HMI_TCP_TIMEOUT_BEEP 0


#include "hmiworks.h"

#define TICKS_PER_SECOND 100
#define MS_PER_TICK      (1000 / TICKS_PER_SECOND)
#define APPLICATION_TITLE "TouchPAD-ICPDAS"

extern volatile unsigned char g_ucLCDIdleUserCallbackStatus;   // 0 = nothing, 1=Idle callback, 2 = wake up callback
extern PFN_VOIDCALLBACK pfnLCDIdleUserCallback;       // Gary @ Mar.29, 2012
extern PFN_VOIDCALLBACK pfnLCDWakeupUserCallback;

volatile tBoolean g_bFirmwareUpdate = false;
unsigned long g_ulLastClearWatchdog = 0;
unsigned long g_ulLastHSCallback = 0;

void SysTickIntHandler(void)
{
   hmi_TimerTrigger();
}

int main(void)
{
	hmi_DeviceInit(640);

   if ( HMI_STARTUP_DELAY )
      hmi_DelayUS( HMI_STARTUP_DELAY * 1000 );  

   if ( HMI_WDT_ENABLE )  // Enable Watchdog 
      hmi_WdtEnable( HMI_WDT_TIMEOUT );
        
   SoundInit();
   HMI_INIT(O_LANDSCAPE);
   hmi_BacklightSet( HMI_LCD_BRIGHTNESS );

   if ( HMI_WDT_ENABLE )
   {  
      g_ulLastClearWatchdog = hmi_GetTickCount();
      hmi_WdtClear();  // Don't reboot
   }

   if ( HMI_STARTUP_BEEP )
      hmi_Beep();

   TouchScreenInit();
   TouchScreenCallbackSet(WidgetPointerMessage);
   
   uart_Init();
    
   if ( HMI_TOUCH_BEEP )
      hmi_TurnTouchBeep(1);

   if ( HMI_LCD_AUTO_OFF ) 
   {
      hmi_LCDIdleSetTimeout( HMI_LCD_AUTO_OFF * 1000 );
      hmi_LCDIdleSetBeep(HMI_LCD_OFF_BEEP, HMI_LCD_ON_BEEP);
   }

   _frame0();

   while(!g_bFirmwareUpdate)
   {
      WidgetMessageQueueProcess();
        
      if ( HMI_WDT_ENABLE )
      {
         if ( hmi_GetTickCount() - g_ulLastClearWatchdog > HMI_WDT_CLEAR )
         {
            g_ulLastClearWatchdog = hmi_GetTickCount();
            hmi_WdtClear();  // Don't reboot
         }
      }
        
      if (g_TimerTicksCount != g_ulLastHSCallback) 
      {
         // Process High Speed Callback Functions at 10 ms
         hmi_ProcessHSCallback();
         g_ulLastHSCallback = g_TimerTicksCount;            
      }

      // callback for LCD idle or wake up
      if ( 1 == g_ucLCDIdleUserCallbackStatus && pfnLCDIdleUserCallback )
      {
         g_ucLCDIdleUserCallbackStatus = 0;
         pfnLCDIdleUserCallback();
      }
      else if ( 2 == g_ucLCDIdleUserCallbackStatus && pfnLCDWakeupUserCallback )
      {
         g_ucLCDIdleUserCallbackStatus = 0;
         pfnLCDWakeupUserCallback();
      }
   }

   WidgetMessageQueueProcess();
   hmi_DelayUS(500000);
   //SoftwareUpdateBegin();
   while(1) { }
}



