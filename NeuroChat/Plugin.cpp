#include "Include.h"

CNeuroChat g_NeuroChatPlugin;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR(CNeuroChat, IClientPlugin, CLIENT_PLUGIN_INTERFACE_VERSION, g_NeuroChatPlugin);

ConVar neurochat("neurochat", "1", FCVAR_CLIENTDLL, "Allows you to enable or disable this plugin.");
ConVar nc_showhint("nc_showhint", "1", FCVAR_CLIENTDLL, "Includes a hint for other players that their request is being processed.");
ConVar nc_showerror("nc_showerror", "1", FCVAR_CLIENTDLL, "Outputs an error message to chat if the request was not processed correctly.");
ConVar nc_chathistory("nc_chathistory", "0", FCVAR_CLIENTDLL, "Logs the request and its response to a text file.");
ConVar nc_setkey("nc_setkey", "None", FCVAR_CLIENTDLL, "API key for requests to the OpenAI server.");
ConVar nc_addstr("nc_addstr", " - [Answer me like a teen].", FCVAR_CLIENTDLL, "Parameter that adds additional text at the end of the request.");
ConVar nc_temperature("nc_temperature", "1.0", FCVAR_CLIENTDLL, "Neural network temperature.");
ConVar nc_ids("nc_ids", "-1", FCVAR_CLIENTDLL, "Identifiers of players whose requests should be processed.");

CON_COMMAND(nc_savecfg, "Save current configuration.")
{
	SaveConfig();
	ConColorMsg(Color(40, 255, 0, 255), "[NeuroChat] Configuration successfully saved!\n");
}

CON_COMMAND(nc_loadcfg, "Load current configuration.")
{
	LoadConfig();
	ConColorMsg(Color(40, 255, 0, 255), "[NeuroChat] Configuration successfully loaded!\n");
}

CON_COMMAND(nc_resetcfg, "Reset current configuration.")
{
	ResetConfig();
	ConMsg("[NeuroChat] Configuration has been reset.\n");
}

api_version_s CNeuroChat::GetAPIVersion()
{
	return SVENMOD_API_VER;
}

bool CNeuroChat::Load(CreateInterfaceFn pfnSvenModFactory, ISvenModAPI* pSvenModAPI, IPluginHelpers* pPluginHelpers)
{
	BindApiToGlobals(pSvenModAPI);
	ConVar_Register();

	CreateConfig();
	LoadConfig();

	ConColorMsg(Color(40, 255, 0, 255), "[NeuroChat] Successfully loaded!\n");
	return true;
}

void CNeuroChat::PostLoad(bool bGlobalLoad)
{
	dhHandle = Hooks()->HookUserMessage("SayText", (UserMsgHookFn)SayTextHook, &OrigSayTextHook);
}

void CNeuroChat::Unload(void)
{
	SaveConfig();

	Hooks()->UnhookUserMessage(dhHandle);
	ConVar_Unregister();
}

bool CNeuroChat::Pause(void)
{
	return true;
}

void CNeuroChat::Unpause(void)
{

}

void CNeuroChat::OnFirstClientdataReceived(client_data_t* pcldata, float flTime)
{

}

void CNeuroChat::OnBeginLoading(void)
{

}

void CNeuroChat::OnEndLoading(void)
{

}

void CNeuroChat::OnDisconnect(void)
{
	ResetToDefault("NC_Identifiers");
	LoadConfig();
}

void CNeuroChat::GameFrame(client_state_t state, double frametime, bool bPostRunCmd)
{
	if (bWait)
	{
		//If our thread ended with the result "future_status::ready"
		if (ftResult.wait_for(chrono::seconds(0)) == future_status::ready)
		{
			//Receive a message
			string strMsg = ReceiveMessage(ftResult.get());

			//LoL, dont ask the neural network to write this xd
			if (IndexOf(strMsg, "Failed to extract the message.") != -1)
			{
				if (!nc_showerror.GetBool())
				{
					ConColorMsg(Color(255, 0, 0, 255), (("[NeuroChat] " + strMsg + "\n").c_str()));

					bWait = false;
					return;
				}
			}

			//Get the number of lines which we want to send to chat (120 = max chat msg length)
			int iLines = (strMsg.length() / 120);
			int iErase = 0;

			if (nc_chathistory.GetBool())
				WriteChatHistory(("[NeuroChat - Response] " + strMsg + "\n"));

			if (iLines > 0)
			{
				for (int i = 0; i < iLines; i++)
				{
					//Erase the next 120 characters and print the string
					strMsg.erase(0, iErase);
					g_pEngineFuncs->ClientCmd(("say " + strMsg).c_str());

					iErase += 120;
				}
			}
			else
				g_pEngineFuncs->ClientCmd(("say " + strMsg).c_str());

			bWait = false;
		}
	}
}

void CNeuroChat::Draw(void)
{

}

void CNeuroChat::DrawHUD(float time, int intermission)
{

}

const char* CNeuroChat::GetName(void)
{
	return "NeuroChat";
}

const char* CNeuroChat::GetAuthor(void)
{
	return "kek";
}

const char* CNeuroChat::GetVersion(void)
{
	return "0.2";
}

const char* CNeuroChat::GetDescription(void)
{
	return "Allows you to use a neural network as a answer to player messages.";
}

const char* CNeuroChat::GetURL(void)
{
	return "https://github.com/kekekekkek/NeuroChat";
}

const char* CNeuroChat::GetDate(void)
{
	return SVENMOD_BUILD_TIMESTAMP;
}

const char* CNeuroChat::GetLogTag(void)
{
	return "NEUROCHAT";
}