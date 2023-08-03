#include "Include.h"

string strMessage = "";
string strEndPoint = "";
string strModelName = "";

string strKeys[8] =
{
	"NC_IsActive", //bool
	"NC_ShowHint", //bool
	"NC_ShowError", //bool
	"NC_ChatHistory", //bool
	"NC_OpenAIKey", //string
	"NC_AddStr", //string
	"NC_Temperature", //float
	"NC_Identifiers" //string
};

string strDefValue[8] =
{
	"1",
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

		for (int i = 0; i < ARRAYSIZE(strKeys); i++)
			WritePrivateProfileStringA("NeuroChat", strKeys[i].c_str(), strDefValue[i].c_str(), "NeuroChat\\Config.ini");
	}
	else
	{
		if (!g_pFileSystem->FileExists("NeuroChat\\History.txt"))
			CreateFileA("NeuroChat\\History.txt", NULL, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		for (int i = 0; i < ARRAYSIZE(strKeys); i++)
		{
			char chValue[MAX_PATH];

			if (GetPrivateProfileStringA("NeuroChat", strKeys[i].c_str(), strDefValue[i].c_str(), chValue, sizeof(chValue), "NeuroChat\\Config.ini") == NULL)
				WritePrivateProfileStringA("NeuroChat", strKeys[i].c_str(), strDefValue[i].c_str(), "NeuroChat\\Config.ini");
		}
	}
}

void LoadConfig()
{
	char chValue[8][MAX_PATH];

	for (int i = 0; i < ARRAYSIZE(strKeys); i++)
		GetPrivateProfileStringA("NeuroChat", strKeys[i].c_str(), strDefValue[i].c_str(), chValue[i], MAX_PATH, "NeuroChat\\Config.ini");

	neurochat.SetValue(atoi(chValue[0]));
	nc_showhint.SetValue(atoi(chValue[1]));
	nc_showerror.SetValue(atoi(chValue[2]));
	nc_chathistory.SetValue(atoi(chValue[3]));
	nc_setkey.SetValue(chValue[4]);
	nc_addstr.SetValue(chValue[5]);
	nc_temperature.SetValue((float)atof(chValue[6]));
	nc_ids.SetValue(chValue[7]);
}

void SaveConfig()
{
	string strValue[8] =
	{
		to_string(neurochat.GetBool()),
		to_string(nc_showhint.GetBool()),
		to_string(nc_showerror.GetBool()),
		to_string(nc_chathistory.GetBool()),
		nc_setkey.GetString(),
		nc_addstr.GetString(),
		to_string(nc_temperature.GetFloat()),
		nc_ids.GetString()
	};

	for (int i = 0; i < ARRAYSIZE(strKeys); i++)
		WritePrivateProfileStringA("NeuroChat", strKeys[i].c_str(), strValue[i].c_str(), "NeuroChat\\Config.ini");
}

void ResetConfig()
{
	for (int i = 0; i < ARRAYSIZE(strKeys); i++)
		ResetToDefault(strKeys[i]);
}

void ResetToDefault(string strKeyName)
{
	char chValue[MAX_PATH];

	for (int i = 0; i < ARRAYSIZE(strKeys); i++)
	{
		if (strstr(strKeyName.c_str(), strKeys[i].c_str()))
		{
			WritePrivateProfileStringA("NeuroChat", strKeys[i].c_str(), strDefValue[i].c_str(), "NeuroChat\\Config.ini");
			break;
		}
	}
}