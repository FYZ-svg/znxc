#ifndef EEPROM_H
#define EEPROM_H

extern int Pm;
extern int key1_status;
extern int key2_status;
extern int key3_status;
extern int key4_status;
extern int key5_status;
extern int key1_last_status;
extern int key2_last_status;
extern int key3_last_status;
extern int key4_last_status;
extern int key5_last_status;

void Key_Init(void);
void Switch_Init(void);
uint8 Key_Scan_Deal(void);
void Strategy_Slect(void);
void UI(void);


void DisplayCursor(void);
void UI_ContentKey(void);
void UI_Content(void);
void UI_Datapage(void);
void UI_DatapageKey(void);

uint8 pagenumup(void);
uint8 pagenumdown(void);

void DisplayMain(void);
void MotorPara(void);
void TrackPara(void);
void SpeedPara(void);
void BiZhang(void);
void L_CirPara(void);
void R_CirPara(void);
void DisplayBarrierPara(void);
void DisplayPodaoPara(void);




#endif