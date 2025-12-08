 #ifndef PANEL_H
 #define PANEL_H

 #include "stdint.h"

 typedef enum
 {
     PANEL_STATE_STANDBY = 0,
     PANEL_STATE_LINE_FOLLOW,
     PANEL_STATE_GRAY_CALIB,
     PANEL_STATE_LINE_COLOR,
     PANEL_STATE_SPEED_ADJUST
 } PanelState;

 typedef enum
 {
     PANEL_LINE_COLOR_BLACK = 0,
     PANEL_LINE_COLOR_WHITE = 1
 } PanelLineColor;

 typedef struct
 {
     PanelState      current_state;
     PanelLineColor  line_color;      
     int16_t         base_speed;      
 } PanelConfig;

 void Panel_Init(PanelConfig *config);
 void Panel_Task(PanelConfig *config);

 #endif

