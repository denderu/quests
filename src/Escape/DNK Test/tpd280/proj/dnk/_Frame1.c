#include "hmiworks.h"
#pragma GCC diagnostic ignored "-Wunused-variable"

static tWidget *thisCanvas = (tWidget *)&CanvasFrame1;
static tContext globalContext;
static tContext *thisContext = &globalContext;

#pragma GCC diagnostic warning "-Wunused-variable"


static void ConnectWidgetVariable();
static void DisconnectWidgetVariable();

#include "__frame1.h"

Canvas(Label4, WIDGET_ROOT, 0, 0,
    HMI_DISPLAY_DRIVER_PTR, 21, 34, 271, 62,
    CANVAS_STYLE_TEXT | CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE
    | CANVAS_STYLE_TEXT_HCENTER | CV_AOPT_VISIBLE | CV_AOPT_ENABLED | CANVAS_STYLE_TEXT_OPAQUE
    , 0xFFFFFF, 0xFFFFFF, 0x000000,
    g_pFontIcpdas_cs_cyrillic20, "Ожидание ДНК...", 0, 0);
Canvas(Label5, WIDGET_ROOT, 0, 0,
    HMI_DISPLAY_DRIVER_PTR, 67, 8, 180, 20,
    CANVAS_STYLE_TEXT | CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE
    | CANVAS_STYLE_TEXT_HCENTER | CV_AOPT_VISIBLE | CV_AOPT_ENABLED | CANVAS_STYLE_TEXT_OPAQUE
    , 0xFFFFFF, 0xFFFFFF, 0x000000,
    g_pFontCm18, "", 0, 0);
RectangularButton(TextPushButton6, WIDGET_ROOT, 0,
    0, HMI_DISPLAY_DRIVER_PTR, 19, 111, 274, 100,
    (PB_STYLE_OUTLINE | PB_STYLE_TEXT_OPAQUE | PB_STYLE_TEXT | PB_AOPT_VISIBLE | PB_AOPT_ENABLED | PB_STYLE_AUTO_REPEAT | 
    PB_STYLE_FILL), 0x80FFFF, 0xFFFFFF, 0x000000, 0x000000,
    g_pFontIcpdas_cs_cyrillic20, "ЗАПУСК", 0,
    200, 100,
    _OnTextPushButton6Click, _OnTextPushButton6Release, 0,
    0, -1, 0);
TimerWidget(Timer7, WIDGET_ROOT, 0, 0, HMI_DISPLAY_DRIVER_PTR, 0, 0, 32, 32,
    0, 1000, 0, Timer7OnExecute);
TimerWidget(Timer8, WIDGET_ROOT, 0, 0, HMI_DISPLAY_DRIVER_PTR, 0, 0, 32, 32,
    0, 1000, 0, Timer8OnExecute);
Canvas(Label9, WIDGET_ROOT, 0, 0,
    HMI_DISPLAY_DRIVER_PTR, 37, 215, 84, 20,
    CANVAS_STYLE_TEXT | CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE
    | CANVAS_STYLE_TEXT_HCENTER | CV_AOPT_ENABLED | CANVAS_STYLE_TEXT_OPAQUE
    , 0xFFFFFF, 0x000000, 0x000000,
    g_pFontCm18, "Label9", 0, 0);


Canvas(CanvasFrame1, WIDGET_ROOT, 0, 0, HMI_DISPLAY_DRIVER_PTR, 0, 0,
       320, 240, (CANVAS_STYLE_APP_DRAWN | CV_AOPT_VISIBLE), 0, 0, 0, 0, 0, 0, OnFrame12Paint);

void renderFrame12Vector(tContext *pContext, int ox, int oy) {
}
void OnFrame12Paint(tWidget *pWidget, tContext *pContext)
{
hmi_SetForeground(pContext, 0xFFFFFF);
hmi_FillRect(pContext, 0, 0, 320, 240);
renderFrame12Vector(pContext, 0, 0);
}

static void InitGlobalContext()
{
  GrContextInit(thisContext, thisCanvas->pDisplay);
  GrContextClipRegionSet(thisContext, &(thisCanvas->sPosition));
}
void InitWidget1()
{
 Frame12OnCreate();
    WidgetAdd(WIDGET_ROOT, (tWidget *)(&CanvasFrame1));
    WidgetAdd(WIDGET_ROOT, (tWidget *)&Label4);
    WidgetAdd(WIDGET_ROOT, (tWidget *)&Label5);
    WidgetAdd(WIDGET_ROOT, (tWidget *)&TextPushButton6);
    WidgetAdd(WIDGET_ROOT, (tWidget *)&Timer7);
    WidgetAdd(WIDGET_ROOT, (tWidget *)&Timer8);
    WidgetAdd(WIDGET_ROOT, (tWidget *)&Label9);
    InitGlobalContext();
    ConnectWidgetVariable();
}

void FreeWidget1()
{
    DisconnectWidgetVariable();
    WidgetRemove((tWidget *)(&CanvasFrame1));
    WidgetRemove((tWidget *)(&Label4));
    WidgetRemove((tWidget *)(&Label5));
    WidgetRemove((tWidget *)(&TextPushButton6));
    WidgetRemove((tWidget *)(&Timer7));
    WidgetRemove((tWidget *)(&Timer8));
    WidgetRemove((tWidget *)(&Label9));
 Frame12OnDestroy();
}


void _OnTextPushButton6Release(tWidget *pWidget)
{
}

void _OnTextPushButton6Click(tWidget *pWidget)
{
  TextPushButton6OnClick(pWidget);
}


void OnTimer7Execute(tWidget *pWidget)
{
  Timer7OnExecute(pWidget);
}


void OnTimer8Execute(tWidget *pWidget)
{
  Timer8OnExecute(pWidget);
}


static void ConnectWidgetVariable()
{
  if (_CurrentFrame != 1) return;
}

static void DisconnectWidgetVariable()
{
}

void _Frame1()
{
  var_table_init();
  FreeCurrentFrame();
  _CurrentFrame = 1;
  InitWidget1();
}

