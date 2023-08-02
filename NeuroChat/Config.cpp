#include "Include.h"

string strMessage = "";
string strEndPoint = "";
string strModelName = "";

string strKeys[7] =
{
	"NC_IsActive", //bool
	"NC_PlayerHint", //bool
	"NC_ChatHistory", //bool
	"NC_OpenAIKey", //string
	"NC_AddStr", //string
	"NC_Temperature", //float
	"NC_Identifiers" //string
};

string strDefValue[7] =
{
	"1",
	"1",
	"0",
	"None",
	" - [Answer me like a teen].",
	"1.0",
	"-1"
};

void CreateConfig()
{
	if (CreateDirectoryA("NeuroChat", NULL))
	{
		CreateFileA("NeuroChat\\History.txt", NULL, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		for (int i = 0; i < 7; i++)
			WritePrivateProfileStringA("NeuroChat", strKeys[i].c_str(), strDefValue[i].c_str(), "NeuroChat\\Config.ini");
	}
	else
	{
		if (!g_pFileSystem->FileExists("NeuroChat\\History.txt"))
			CreateFileA("NeuroChat\\History.txt", NULL, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		for (int i = 0; i < 7; i++)
		{
			char chValue[MAX_PATH];

			if (GetPrivateProfileStringA("NeuroChat", strKeys[i].c_str(), strDefValue[i].c_str(), chValue, sizeof(chValue), "NeuroChat\\Config.ini") == NULL)
				WritePrivateProfileStringA("NeuroChat", strKeys[i].c_str(), strDefValue[i].c_str(), "NeuroChat\\Config.ini");
		}
	}
}

void LoadConfig()
{
	char chValue[7][MAX_PATH];

	for (int i = 0; i < 7; i++)
		GetPrivateProfileStringA("NeuroChat", strKeys[i].c_str(), strDefValue[i].c_str(), chValue[i], MAX_PATH, "NeuroChat\\Config.ini");

	neurochat.SetValue(atoi(chValue[0]));
	nc_playerhint.SetValue(atoi(chValue[1]));
	nc_chathistory.SetValue(atoi(chValue[2]));
	nc_setkey.SetValue(chValue[3]);
	nc_addstr.SetValue(chValue[4]);
	nc_temperature.SetValue((float)atof(chValue[5]));
	nc_ids.SetValue(chValue[6]);
}

void SaveConfig()
{
	string strValue[7] =
	{
		to_string(neurochat.GetBool()),
		to_string(nc_playerhint.GetBool()),
		to_string(nc_chathistory.GetBool()),
		nc_setkey.GetString(),
		nc_addstr.GetString(),
		to_string(nc_temperature.GetFloat()),
		nc_ids.GetString()
	};

	for (int i = 0; i < 7; i++)
		WritePrivateProfileStringA("NeuroChat", strKeys[i].c_str(), strValue[i].c_str(), "NeuroChat\\Config.ini");
}

void ResetConfig()
{
	for (int i = 0; i < 7; i++)
		WritePrivateProfileStringA("NeuroChat", strKeys[i].c_str(), strDefValue[i].c_str(), "NeuroChat\\Config.ini");
}