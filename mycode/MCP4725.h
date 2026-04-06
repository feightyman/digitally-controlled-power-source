#ifndef __MCP4725_H
#define __MCP4725_H


void MCP4725_SetVoltage_Fast(uint16_t voltage);
uint16_t MCP4725_ReadStatus(void);
void MCP4725_SetVoltage_EEPROM(uint16_t voltage);




#endif
