 #include "panel.h"
 #include "OLED.h"
 #include "key.h"
 #include "gray.h"

 typedef enum
 {
     PANEL_MENU_NONE = 0,
     PANEL_MENU_MODE_LINE = 1,
     PANEL_MENU_GRAY_CALIB = 2,
     PANEL_MENU_LINE_COLOR = 3,
     PANEL_MENU_SPEED = 4
 } PanelMenuPage;

 static PanelConfig   *s_cfg = 0;
 static PanelMenuPage  s_menu_page = PANEL_MENU_NONE;

 static void Panel_DrawStandby(void)
 {
     OLED_Clear();
     OLED_ShowString(1, 1, "Smart Car");
     OLED_ShowString(2, 1, "Standby");
     OLED_ShowString(3, 1, "K1:Menu");
     OLED_ShowString(4, 1, "K2:Run");
 }

 static void Panel_DrawLineFollow(void)
 {
     OLED_Clear();
     OLED_ShowString(1, 1, "Mode: Line");
     OLED_ShowString(2, 1, "K1:Start");
     OLED_ShowString(3, 1, "K2:Back");
 }

 static void Panel_DrawGrayCalib(void)
 {
     uint8_t values[8];
     GRAY_ReadAll(values);

     OLED_Clear();
     OLED_ShowString(1, 1, "Gray Calib");
     OLED_ShowString(2, 1, "12345678");

     for (uint8_t i = 0; i < 8; i++)
     {
         char ch = values[i] ? '1' : '0';
         OLED_ShowChar(3, 1 + i, ch);
     }

     OLED_ShowString(4, 1, "K2:Next K1:Back");
 }

 static void Panel_DrawLineColor(void)
 {
     OLED_Clear();
     OLED_ShowString(1, 1, "Line Color");
     if (s_cfg && s_cfg->line_color == PANEL_LINE_COLOR_BLACK)
     {
         OLED_ShowString(2, 1, "BLACK");
     }
     else
     {
         OLED_ShowString(2, 1, "WHITE");
     }
     OLED_ShowString(3, 1, "K1:Toggle");
     OLED_ShowString(4, 1, "K2:Next");
 }

 static void Panel_DrawSpeed(void)
 {
     int16_t speed = 0;
     if (s_cfg)
     {
         speed = s_cfg->base_speed;
     }

     OLED_Clear();
     OLED_ShowString(1, 1, "Base Speed");
     OLED_ShowSignedNum(2, 1, speed, 4);
     OLED_ShowString(3, 1, "K1:+50");
     OLED_ShowString(4, 1, "K2:Next");
 }

 static void Panel_EnterMenu(PanelMenuPage page)
 {
     s_menu_page = page;

     switch (s_menu_page)
     {
     case PANEL_MENU_MODE_LINE:
         Panel_DrawLineFollow();
         break;
     case PANEL_MENU_GRAY_CALIB:
         Panel_DrawGrayCalib();
         break;
     case PANEL_MENU_LINE_COLOR:
         Panel_DrawLineColor();
         break;
     case PANEL_MENU_SPEED:
         Panel_DrawSpeed();
         break;
     default:
         Panel_DrawStandby();
         break;
     }
 }

 static void Panel_ExitMenu(void)
 {
     s_menu_page = PANEL_MENU_NONE;
     Panel_DrawStandby();
 }

 void Panel_Init(PanelConfig *config)
 {
     if (!config)
     {
         return;
     }

     s_cfg = config;

     if (s_cfg->base_speed == 0)
     {
         s_cfg->base_speed = 400;
     }
     s_cfg->current_state = PANEL_STATE_STANDBY;
     if (s_cfg->line_color != PANEL_LINE_COLOR_BLACK &&
         s_cfg->line_color != PANEL_LINE_COLOR_WHITE)
     {
         s_cfg->line_color = PANEL_LINE_COLOR_BLACK;
     }

     Panel_DrawStandby();
 }

 void Panel_Task(PanelConfig *config)
 {
     KEY_Value key;

     if (!config)
     {
         return;
     }

     s_cfg = config;
     key = KEY_Scan(0);

     if (s_menu_page == PANEL_MENU_NONE)
     {
         if (key == KEY1_PRESS)
         {
             Panel_EnterMenu(PANEL_MENU_MODE_LINE);
         }
         else if (key == KEY2_PRESS)
         {
             s_cfg->current_state = PANEL_STATE_LINE_FOLLOW;
             OLED_Clear();
             OLED_ShowString(1, 1, "Line Follow");
             OLED_ShowString(2, 1, "Running...");
         }
     }
     else
     {
         switch (s_menu_page)
         {
         case PANEL_MENU_MODE_LINE:
             if (key == KEY1_PRESS)
             {
                 s_cfg->current_state = PANEL_STATE_LINE_FOLLOW;
                 OLED_Clear();
                 OLED_ShowString(1, 1, "Line Follow");
                 OLED_ShowString(2, 1, "Running...");
                 s_menu_page = PANEL_MENU_NONE;
             }
             else if (key == KEY2_PRESS)
             {
                 Panel_EnterMenu(PANEL_MENU_GRAY_CALIB);
             }
             break;

         case PANEL_MENU_GRAY_CALIB:
             if (key == KEY1_PRESS)
             {
                 Panel_ExitMenu();
             }
             else
             {
                 Panel_DrawGrayCalib();
                 if (key == KEY2_PRESS)
                 {
                     Panel_EnterMenu(PANEL_MENU_LINE_COLOR);
                 }
             }
             break;

         case PANEL_MENU_LINE_COLOR:
             if (key == KEY1_PRESS)
             {
                 if (s_cfg->line_color == PANEL_LINE_COLOR_BLACK)
                 {
                     s_cfg->line_color = PANEL_LINE_COLOR_WHITE;
                 }
                 else
                 {
                     s_cfg->line_color = PANEL_LINE_COLOR_BLACK;
                 }
                 Panel_DrawLineColor();
             }
             else if (key == KEY2_PRESS)
             {
                 Panel_EnterMenu(PANEL_MENU_SPEED);
             }
             break;

         case PANEL_MENU_SPEED:
             if (key == KEY1_PRESS)
             {
                 if (s_cfg->base_speed < 1000)
                 {
                     s_cfg->base_speed += 50;
                 }
                 else
                 {
                     s_cfg->base_speed = 200;
                 }
                 Panel_DrawSpeed();
             }
             else if (key == KEY2_PRESS)
             {
                 Panel_ExitMenu();
             }
             break;

         default:
             Panel_ExitMenu();
             break;
         }
     }
 }

