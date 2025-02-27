#include "MachineSettings.h"

u8 enabled_gcodes[CUSTOM_GCODES_COUNT];
u8 gcode_num;
u8 gc_page_count;
u8 gc_cur_page = 0;

CUSTOM_GCODES * customcodes = NULL;

LISTITEMS customItems = {
// title
LABEL_CUSTOM,
// icon                 ItemType      Item Title        item value text(only for custom value)
{
  {ICONCHAR_BACKGROUND, LIST_LABEL,  LABEL_BACKGROUND, LABEL_BACKGROUND},
  {ICONCHAR_BACKGROUND, LIST_LABEL,  LABEL_BACKGROUND, LABEL_BACKGROUND},
  {ICONCHAR_BACKGROUND, LIST_LABEL,  LABEL_BACKGROUND, LABEL_BACKGROUND},
  {ICONCHAR_BACKGROUND, LIST_LABEL,  LABEL_BACKGROUND, LABEL_BACKGROUND},
  {ICONCHAR_BACKGROUND, LIST_LABEL,  LABEL_BACKGROUND, LABEL_BACKGROUND},
  {ICONCHAR_PAGEUP,     LIST_LABEL,  LABEL_BACKGROUND, LABEL_BACKGROUND},
  {ICONCHAR_PAGEDOWN,   LIST_LABEL,  LABEL_BACKGROUND, LABEL_BACKGROUND},
  {ICONCHAR_BACK,       LIST_LABEL,  LABEL_BACKGROUND, LABEL_BACKGROUND},}
};

//
//load values on page change and reload
//
void loaditemsCustomGcode()
{
  for (uint32_t i = 0; i < LISTITEM_PER_PAGE; i++)
  {
    uint32_t item_index = gc_cur_page*LISTITEM_PER_PAGE + i;
    if (item_index < gcode_num){
      customItems.items[i].icon = ICONCHAR_CODE;
      customItems.items[i].titlelabel.index = LABEL_DYNAMIC;
      setDynamicLabel(i,customcodes->name[item_index]);
    }
    else{
      customItems.items[i].icon = ICONCHAR_BACKGROUND;
      customItems.items[i].titlelabel.index = LABEL_BACKGROUND;
    }
  }
  // set page up down button according to page count and current page
  if (gcode_num <= LISTITEM_PER_PAGE)
  {
    customItems.items[5].icon = ICONCHAR_BACKGROUND;
    customItems.items[6].icon = ICONCHAR_BACKGROUND;
  }
  else
  {
    if(gc_cur_page == 0){
      customItems.items[5].icon = ICONCHAR_BACKGROUND;
      customItems.items[6].icon = ICONCHAR_PAGEDOWN;
    }
    else if(gc_cur_page == (gc_page_count-1)){
      customItems.items[5].icon = ICONCHAR_PAGEUP;
      customItems.items[6].icon = ICONCHAR_BACKGROUND;
    }
    else
    {
      customItems.items[5].icon = ICONCHAR_PAGEUP;
      customItems.items[6].icon = ICONCHAR_PAGEDOWN;
    }
  }
}

void menuCustom(void)
{
  //load custom codes
  CUSTOM_GCODES tempcodes;
  customcodes = &tempcodes;
  W25Qxx_ReadBuffer((u8*)&tempcodes,CUSTOM_GCODE_ADDR,sizeof(CUSTOM_GCODES));
  gcode_num = customcodes->count;

  gc_page_count = (gcode_num+LISTITEM_PER_PAGE-1)/LISTITEM_PER_PAGE;

  KEY_VALUES key_num = KEY_IDLE;

  loaditemsCustomGcode();
  menuDrawListPage(&customItems);

  while(infoMenu.menu[infoMenu.cur] == menuCustom)
  {
    key_num = menuKeyGetValue();

    if (key_num < LISTITEM_PER_PAGE)
    {
      uint32_t item_index = gc_cur_page * LISTITEM_PER_PAGE + key_num;
      if (item_index < gcode_num)
      {
        storeCmd(customcodes->gcode[item_index]);
      }
    }
    switch(key_num)
    {
    case KEY_ICON_5:
      if (gc_page_count > 1)
      {
        if (gc_cur_page > 0)
        {
          gc_cur_page--;
          loaditemsCustomGcode();
          menuRefreshListPage();
        }
      }
      break;
    case KEY_ICON_6:
      if (gc_page_count > 1)
      {
        if (gc_cur_page < gc_page_count - 1)
        {
          gc_cur_page++;
          loaditemsCustomGcode();
          menuRefreshListPage();
        }
      }
      break;
    case KEY_ICON_7:
      gc_cur_page = 0;
      infoMenu.cur--;
      break;
    default:
      break;
    }
    loopProcess();
  }
}



void menuRGBSettings(void)
{
  MENUITEMS RGBItems = {
  // title
  LABEL_RGB_SETTINGS,
  // icon                       label
  {{ICON_RGB_RED,              LABEL_RED},
    {ICON_RGB_GREEN,            LABEL_GREEN},
    {ICON_RGB_BLUE,             LABEL_BLUE},
    {ICON_RGB_WHITE,            LABEL_WHITE},
    {ICON_RGB_OFF,              LABEL_OFF},
    {ICON_BACKGROUND,           LABEL_BACKGROUND},
    {ICON_BACKGROUND,           LABEL_BACKGROUND},
    {ICON_BACK,                 LABEL_BACK},}
  };

  KEY_VALUES key_num = KEY_IDLE;

  menuDrawPage(&RGBItems);

  while(infoMenu.menu[infoMenu.cur] == menuRGBSettings)
  {
    key_num = menuKeyGetValue();
    switch(key_num)
    {
      case KEY_ICON_0:  //Red
        storeCmd("M150 R255 U0 B0 P255\n");
        break;

      case KEY_ICON_1:  //Green
        storeCmd("M150 R0 U255 B0 P255\n");
        break;

      case KEY_ICON_2:  //Blue
        storeCmd("M150 R0 U0 B255 P255\n");
        break;

      case KEY_ICON_3:  //White
        storeCmd("M150 R255 U255 B255 P255\n");
        break;

      case KEY_ICON_4:  //Turn Off
        storeCmd("M150 R0 U0 B0 P0\n");
        break;

      case KEY_ICON_7:
        infoMenu.cur--;
        break;

      default:
        break;
    }
    loopProcess();
  }
}


void menuMachineSettings(void)
{

  MENUITEMS machineSettingsItems = {
  // title
  LABEL_MACHINE_SETTINGS,
  // icon                       label
   {{ICON_PARAMETER,            LABEL_PARAMETER_SETTING},
    {ICON_GCODE,                LABEL_TERMINAL},
    {ICON_EEPROM_SAVE,          LABEL_EEPROM_SAVE},
    {ICON_EEPROM_RESET,         LABEL_EEPROM_RESET},
    {ICON_RGB_SETTINGS,         LABEL_RGB_SETTINGS},
    {ICON_CUSTOM,               LABEL_CUSTOM},
    {ICON_BACKGROUND,           LABEL_BACKGROUND},
    {ICON_BACK,                 LABEL_BACK},}
  };

  KEY_VALUES key_num = KEY_IDLE;
  menuDrawPage(&machineSettingsItems);

  while(infoMenu.menu[infoMenu.cur] == menuMachineSettings)
  {
    key_num = menuKeyGetValue();
    switch(key_num)
    {
      case KEY_ICON_4:
        infoMenu.menu[++infoMenu.cur] = menuRGBSettings;
        break;

      case KEY_ICON_0:
        infoMenu.menu[++infoMenu.cur] = menuParameterSettings;
        break;

      case KEY_ICON_5:
        infoMenu.menu[++infoMenu.cur] = menuCustom;
        break;

      case KEY_ICON_1:
        infoMenu.menu[++infoMenu.cur] = menuSendGcode;
        break;
/*
      case KEY_ICON_6:
        storeCmd("M81\n");
        break;
*/
      case KEY_ICON_3:
        infoMenu.menu[++infoMenu.cur] = menuEepromReset;
        break;

      case KEY_ICON_2:
        infoMenu.menu[++infoMenu.cur] = menuEepromSave;
        break;

      case KEY_ICON_7:
        infoMenu.cur--;
        break;

      default:
        break;
    }

    loopProcess();
  }
}

void menuEepromSave(void)
{
  if(infoMachineSettings.EEPROM == 0)  // if EEPROM is not supported by printer
  {
    infoMenu.cur--;

    return;
  }

  u16 key_num = IDLE_TOUCH;

  popupDrawPage(DIALOG_TYPE_ALERT, bottomDoubleBtn, textSelect(LABEL_WARNING), textSelect(LABEL_EEPROM_SAVE_INFO), textSelect(LABEL_CONFIRM), textSelect(LABEL_CANCEL));

  while(infoMenu.menu[infoMenu.cur] == menuEepromSave)
  {
    key_num = KEY_GetValue(2, doubleBtnRect);
    switch(key_num)
    {
      case KEY_POPUP_CONFIRM:
        storeCmd("M500\n");
        infoMenu.cur--;
        break;

      case KEY_POPUP_CANCEL:
        infoMenu.cur--;
        break;
    }

    loopProcess();
  }
}

void menuEepromReset(void)
{
  if(infoMachineSettings.EEPROM == 0)  // if EEPROM is not supported by printer
  {
    infoMenu.cur--;

    return;
  }

  u16 key_num = IDLE_TOUCH;

  popupDrawPage(DIALOG_TYPE_ALERT, bottomDoubleBtn, textSelect(LABEL_WARNING), textSelect(LABEL_EEPROM_RESET_INFO), textSelect(LABEL_CONFIRM), textSelect(LABEL_CANCEL));

  while(infoMenu.menu[infoMenu.cur] == menuEepromReset)
  {
    key_num = KEY_GetValue(2, doubleBtnRect);
    switch(key_num)
    {
      case KEY_POPUP_CONFIRM:
        storeCmd("M502\n");
        storeCmd("M500\n");
        infoMenu.cur--;
        break;

      case KEY_POPUP_CANCEL:
        infoMenu.cur--;
        break;
    }

    loopProcess();
  }
}
