#pragma once

extern UserMsgHookFn OrigSayTextHook;
extern DetourHandle_t dhHandle;

extern ConVar neurochat;
extern ConVar nc_playerhint;
extern ConVar nc_chathistory;
extern ConVar nc_setkey;
extern ConVar nc_addstr;
extern ConVar nc_temperature;
extern ConVar nc_ids;

extern void WriteChatHistory(string strText);
extern int IndexOf(string strText, string strValue);
extern int SayTextHook(const char* pszName, int iSize, void* pBuffer);