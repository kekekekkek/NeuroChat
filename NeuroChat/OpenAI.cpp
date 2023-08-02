#include "Include.h"

bool bWait = false;
future <string> ftResult;

//LoL, im too lazy to use a JSON library to do that
string ReceiveMessage(string strResult)
{
	if (IndexOf(strResult, "Failed") != -1
		|| strResult.empty())
		return strResult;

	if (IndexOf(strResult, "content") == -1)
		strResult = "Failed to extract the message.";
	else
	{
		for (int i = (IndexOf(strResult, "content") + 11); i < strResult.length(); i++)
		{
			if (strResult[i] == 34)
			{
				strResult.erase(0, (IndexOf(strResult, "content") + 11));
				strResult.erase(IndexOf(strResult, "\""), strResult.length());

				break;
			}
		}
	}

	return strResult;
}

//You can use this function for additional information
int GetPromptTokens(string strResult)
{
	int iTokens = -1;

	if (IndexOf(strResult, "Failed") != -1
		|| IndexOf(strResult, "prompt_tokens") == -1
		|| strResult.empty())
		return iTokens;

	for (int i = (IndexOf(strResult, "prompt_tokens") + 16); i < strResult.length(); i++)
	{
		if (strResult[i] == 44)
		{
			strResult.erase(0, (IndexOf(strResult, "prompt_tokens") + 16));
			strResult.erase(IndexOf(strResult, ","), strResult.length());

			iTokens = atoi(strResult.c_str());
			break;
		}
	}

	return iTokens;
}

//And this function too
int GetCompTokens(string strResult)
{
	int iTokens = -1;

	if (IndexOf(strResult, "Failed") != -1
		|| IndexOf(strResult, "completion_tokens") == -1
		|| strResult.empty())
		return iTokens;

	for (int i = (IndexOf(strResult, "completion_tokens") + 20); i < strResult.length(); i++)
	{
		if (strResult[i] == 44)
		{
			strResult.erase(0, (IndexOf(strResult, "completion_tokens") + 20));
			strResult.erase(IndexOf(strResult, ","), strResult.length());

			iTokens = atoi(strResult.c_str());
			break;
		}
	}

	return iTokens;
}

//We need to know the amount of tokens that were used to execute the request in order to calculate the amount of money spent
int GetTotalTokens(string strResult)
{
	return ((GetPromptTokens(strResult) + GetCompTokens(strResult) > 0) ? GetPromptTokens(strResult) + GetCompTokens(strResult) : -1);
}

//"fTemperature" is a parameter that sets the creativity of your neural network, the higher the value, the more interesting the answers will be
//You can also specify in the message how the model should behave when answering your query, for example (strMessage + " - [Answer me like a teen]."). ("NC_AddStr" parameter)
//But remember, the longer your message is, the more tokens the neural network consumes
/* --- fTemperature ---
  * Default value: 0.7f;
  * Minimum value: 0.0f;
  * Maximum value: 1.0f.
   ---------------------*/

string SendRequest(string strAPIKey, string strModelName, string strEndPoint, string strMessage, float fTemperature = 0.7f)
{
	string strResult = "";
	HINTERNET hConnect = InternetConnectA(InternetOpenA(NULL, NULL, NULL, NULL, NULL), "api.openai.com", INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, NULL, NULL);

	if (hConnect)
	{
		//You can use "INTERNET_FLAG_ASYNC" flag and "InternetSetStatusCallback" function instead "std::async" to make an asynchronous call
		HINTERNET hRequest = HttpOpenRequestA(hConnect, "POST", strEndPoint.c_str(), NULL, NULL, NULL, (INTERNET_FLAG_KEEP_CONNECTION | INTERNET_FLAG_SECURE), NULL);

		if (hRequest)
		{
			string strSetHeader = ("Content-Type: application/json\r\nAuthorization: Bearer " + strAPIKey);
			string strSetData = ("{\"model\": \"" + strModelName + "\", \"messages\": [{\"role\": \"user\", \"content\": \"" + strMessage + "\"}], \"temperature\": " + to_string(fTemperature) + "}");

			if (HttpSendRequestA(hRequest, strSetHeader.c_str(), strSetHeader.length(), (void*)strSetData.c_str(), strSetData.length()))
			{
				//Buffer size = maximum number of input tokens
				//For "gpt-3.5-turbo" its 4096 tokens

				char chContent[4096];
				DWORD dwBytesRead = NULL;

				if (InternetReadFile(hRequest, chContent, sizeof(chContent), &dwBytesRead))
					strResult = string(chContent);
				else
					strResult = "Failed to read file data!";
			}
			else
				strResult = "Failed to execute request!";

			InternetCloseHandle(hConnect);
			InternetCloseHandle(hRequest);
		}
		else
			strResult = "Failed to open request!";
	}
	else
		strResult = "Failed to connect!";

	return strResult;
}