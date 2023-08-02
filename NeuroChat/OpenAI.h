#pragma once

//Why do we need a class and many instances?
extern bool bWait;
extern future <string> ftResult;

extern int GetPromptTokens(string strResult);
extern int GetCompTokens(string strResult);
extern int GetTotalTokens(string strResult);
extern string ReceiveMessage(string strResult);
extern string SendRequest(string strAPIKey, string strModelName, string strEndPoint, string strMessage, float fTemperature);