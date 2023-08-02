#include "Include.h"

UserMsgHookFn OrigSayTextHook;
DetourHandle_t dhHandle = NULL;

vector<int> iIdentifiers(MAX_PLAYERS);

int IndexOf(string strText, string strValue)
{
	int iIndex = strText.find(strValue);

	if (iIndex >= 0 && iIndex != string::npos)
		return iIndex;

	return -1;
}

void ParseIdentifiers(vector<int>& iIdentifiers, string strIdentifiers)
{
	string strNum = "";

	iIdentifiers.clear();
	strIdentifiers.append(",");

	for (int i = 0; i < strIdentifiers.length(); i++)
	{
		if (strIdentifiers[i] == 44)
		{
			iIdentifiers.push_back(atoi(strNum.c_str()));
			strNum.clear();

			continue;
		}

		if (strIdentifiers[i - 1] == 45
			&& isalnum(strIdentifiers[i]))
		{
			strNum += strIdentifiers[i - 1];
			strNum += strIdentifiers[i];

			continue;
		}

		if (isalnum(strIdentifiers[i]))
			strNum += strIdentifiers[i];
	}
}

void WriteChatHistory(string strText)
{
	HANDLE hFile = CreateFileA("NeuroChat\\History.txt", FILE_APPEND_DATA, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwBytes = NULL;

		WriteFile(hFile, strText.c_str(), strText.length(), &dwBytes, NULL);
		CloseHandle(hFile);
	}
}

int SayTextHook(const char* pszName, int iSize, void* pBuffer)
{
	CMessageBuffer cmbBuffer(pBuffer, iSize, true);
	cmbBuffer.BeginReading();

	//Reading data
	int iEntIndex = cmbBuffer.ReadByte();
	string strText = cmbBuffer.ReadStringLine();

	if (iEntIndex != g_pClient->GetPlayerIndex() && neurochat.GetBool() && !bWait)
	{
		//Parses the specified identifiers
		ParseIdentifiers(iIdentifiers, nc_ids.GetString());

		for (int i = 0; i < MAX_PLAYERS; i++)
		{
			if (iEntIndex == iIdentifiers[i])
			{
				//Fill the "hud_player_info_t" structure by received entity index
				hud_player_info_t hpitPlayerInfo;
				g_pEngineFuncs->GetPlayerInfo(iEntIndex, &hpitPlayerInfo);

				//Ignore some garbage messages
				if (IndexOf(strText, "has joined the game.") != -1
					|| IndexOf(strText, "has left the game.") != -1)
					break;

				//If the name is not empty
				if (strlen(hpitPlayerInfo.name) > 0)
					//Removing unnecessary characters from the string including player name so that only the received message remains
					strText.erase(IndexOf(strText, hpitPlayerInfo.name) - 1, strlen(hpitPlayerInfo.name) + 2);

				//Sending a hint to the players
				if (nc_playerhint.GetBool())
					g_pEngineFuncs->ClientCmd("say Request in process, please wait...");

				//Write chat history in txt file
				if (nc_chathistory.GetBool())
					WriteChatHistory(("[NeuroChat - Request] " + strText + "\n"));

				//Here you can set a personality for the neural network to make its answers more creative. In this case, i chose the behavior of a teenager
				strMessage = (strText + nc_addstr.GetString()); // (strText + " - [Answer me like a teen].")

				//Setting the current model. This can be the "text-davinci-003" or "text-davinci-002" model or any other model. In this case i have chosen the model "gpt-3.5-turbo"
				strModelName = "gpt-3.5-turbo";

				//Endpoint for referencing the model
				strEndPoint = "/v1/chat/completions";

				/*You can read more about the models and their endpoints here:
				--- Link ---
				* Resource: https://platform.openai.com/docs/models/model-endpoint-compatibility
				------------*/

				//Call the function asynchronously using "std::async", because this way its easier for me to control the result of execution
				ftResult = async(SendRequest, nc_setkey.GetString(), strModelName, strEndPoint, strMessage, nc_temperature.GetFloat());

				//Checked in "CNeuroChat::GameFrame"
				bWait = true;
				break;
			}
		}
	}

	return OrigSayTextHook(pszName, iSize, pBuffer);
}