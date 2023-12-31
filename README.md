# NeuroChat (Concept)
Well, is it time to turn the 1998 game into the 2077 game? :)<br>

You most likely have known about such technology as ChatGPT and neural networks in general for a long time. But what if there was more variety in your favorite game? For example, if the game was connected to neural networks. Sounds interesting, doesnt it?<br>

This plugin was created in order to demonstrate how fun it is to play with neural networks while in such an old game.<br>
You can watch the [video](https://youtu.be/9-A0KLn1jPc) to see how this plugin works.<br>

I apologize if there are any errors in the text. I hope you will understand how to work with this plugin.

# Installation
The entire installation stage of this plugin will consist of the following steps:<br>

1. Go to the official [OpenAI](https://openai.com/) website;
2. Click `Log In` if you already have an account or click `Sign Up` if you dont;
3. After registering or logging into your account, go to this [page](https://platform.openai.com/account/api-keys);
4. You will see the heading `API keys` in front of you, and below that you will see a gray `Create new secret key` button. Click on it;
5. A window will appear in front of you where you will have to name your key and click on the green `Create secret key` button;
6. After you click on the green `Create secret key` button you will see a new window with the following text: `Please save this secret key somewhere safe and accessible. For security reasons, you won't be able to view it again through your OpenAI account. If you lose this secret key, you'll need to generate a new one.`;
7. You will have to copy your key. It will look something like this: `sk-7pgTYp1fYhDqGRTTU3xET5BlbkFJ3EoSRAsJCn125MjjZQXJ`;
8. Keep the key in a safe place as we will need it later;
9. Now, download the [release](https://github.com/kekekekkek/NeuroChat/releases/tag/v0.2) version of the plugin;
10. Place all files and the main `NeuroChat.dll` module in the game directory according to these [instructions](https://github.com/sw1ft747/svenmod#svenmod) for SvenMod and its plugins;
11. After completing all the steps, launch your SvenMod launcher;
12. If you have installed the plugin correctly, then after launching SvenMod, you will be able to see the following messages in the console: `[NeuroChat] Successfully loaded!` and `[SvenMod] Loaded plugin "NeuroChat"`;
13. If you made it through all the steps and got exactly the same result as described in these instructions, keep it up! You have done the most routine part of installing this plugin;
14. Next, you need to familiarize yourself with the commands and principles of work of this plugin. And also, below you will be told what you need to do with your OpenAI key.

# Console Variables
After starting the game, you will have the following commands at your disposal:
| ConVar | MinValue | MaxValue | DefValue | Type | Description |
| -------| -------- | -------- | -------- | ---- | ----------- |
| neurochat | 0 | 1 | 1 | bool | Allows you to enable or disable this plugin. |
| nc_showhint | 0 | 1 | 1 | bool | Includes a hint for other players that their request is being processed. |
| nc_showerror | 0 | 1 | 1 | bool | Outputs an error message to chat if the request was not processed correctly. |
| nc_chathistory | 0 | 1 | 0 | bool | Logs the request and its response to a text file. |
| nc_setkey | - | - | "None" | string | API key for requests to the OpenAI server. |
| nc_addstr | - | - | " - [Answer me like a teen]." | string | Parameter that adds additional text at the end of the request. |
| nc_temperature | 0.0 | 1.0 | 1.0 | float | Neural network temperature. |
| nc_ids | - | - | "-1" | string | Identifiers of players whose requests should be processed. |

Also, after launching the game, a directory named `NeuroChat` will appear in the root folder of Sven Co-op. In it you will find files such as `Config.ini` and file `History.txt`.<br>

* The file `Config.ini` will store all the values of the registered console commands.
* The file `History.txt ` will store all user requests and responses to them.

P.S: Also, im so stupid because i made configuration system in this plugin lol. You just can use `.cfg` file with specified console variables like that: `exec NeuroChat.cfg`. I didnt think about the `.cfg` files when i did it.

Now that your game is running, you will need to enter your OpenAI key. To do this, you need to open the game console and enter the following command in it: `nc_setkey + %OpenAIKey%`.<br><br>
Your end result should look something like this: `nc_setkey sk-7pgTYp1fYhDqGRTTU3xET5BlbkFJ3EoSRAsJCn125MjjZQXJ`. Also, you can change your key at any other time without starting the game. You just need to open the file `../NeuroChat/Config.ini` and edit the parameter `NC_OpenAIKey`.

If you specify an invalid OpenAI key or you dont have one, the `ReceiveMessage` function will return the error message `Failed to extract the message.`. Also, this function may return an error message if your `nc_temperature` parameter was greater than `1.0` or less than `0.0`.

After you have specified the OpenAI key, you will need to connect to any server with players. After connecting to the server, in order not to overload the neural network (since there are restrictions on the number of requests for a certain period of time), you will need to specify the `Index` of the player whose requests the neural network will have to process. To get started, you will need to enter the `status` command, which will display information about the players, after which you will see the following result:<br>
```
#      name userid uniqueid frag time ping loss adr
# 1 "Player1" 123 STEAM_0:0:000000000   0 00:00    0    0
# 2 "Player2" 125 STEAM_0:0:000000000   0 00:00    0    0
# 3 "Player3" 127 STEAM_0:0:000000000   0 00:00    0    0
# 4 "Player4" 129 STEAM_0:0:000000000   0 00:00    0    0
# 5 "Player5" 131 STEAM_0:0:000000000   0 00:00    0    0
5 users
```
The digit after the `#` symbol is the player `Index`. Now, all you have to do is specify the `Index` of the player whose requests you want to process. You will have to enter the command `nc_ids` and the end result will look something like this: `nc_ids "5"`. If you want to process requests from several players at once, you will need to list their `Indexes` separated by commas, like this: `nc_ids "1, 3, 5"`.

But **REMEMBER**, it is necessary to specify identifiers in quotation marks, since this string. If you dont use quotation marks in this command or other string commands, then the result of these variables will always be the first character specified. Now, ask the specified player to write a message to the chat, and enjoy the result.<br>

Also **REMEMBER** that the maximum length of a chat message is `120` characters, so in the `nc_addstr` command you can also tell the neural network that the final message should contain no more than `120` characters. If the response text of the neural network is longer than `120` characters, the response will be output in multiple lines.

And **REMEMBER**, the longer your message is, the more tokens you will spend. To calculate the number of tokens spent, you can call the `GetTotalTokens` function, which is not yet used by this plugin.

And **REMEMBER** one more thing, after you specify the player `Indexes`, if you disconnect from the server or connect to another one, the `nc_ids` value will be set by default (`-1`).

# Console Commands
In addition, after starting the game, you will have other equally important commands registered:
| ConCommand | Description |
| ---------- | ----------- |
| nc_savecfg | Saves the current command values to a file. `..\NeuroChat\Config.ini`. |
| nc_loadcfg | Loads the current values of commands from a file. `..\NeuroChat\Config.ini`. |
| nc_resetcfg | Resets all values to default values. |

After you have changed any variable values in the console, you will need to enter the `nc_savecfg` command to save their values. In addition, you can fill in the `Config.ini` file manually. Also, after exiting the game, your settings will be saved automatically.

# Additional Information
* `nc_showhint` - I recommend turning off this function right away, as it will display annoying text every time that the player's request has started processing;
* `nc_showerror` - Also, I recommend you to turn off this function as well, because error message or request processing message will be output to the console when `nc_showhint` or `nc_showerror` parameters are turned off;
* `nc_addstr` - This is an additional text that will be added at the end of the player's message. For example, if a player writes a message: "**Hello, how are you?**", then the contents of the variable `nc_addstr` will be automatically added to his message. The final message will be: "**Hello, how are you? - [Answer me like a teen].**". This allows you to set additional features for the response from the neural network, such as the behavior model;
* `nc_temperature` - Allows you to set the creativity of the neural network. The higher this value, the more creative the message you will receive in the response.

# Known Issues
* The game may crash if you specify the wrong value for certain parameters;
* Also, there are no checks in the plugin for those things that can cause malfunctions in the game;
* Failures can also occur if you fill in the `Config.ini` file incorrectly or specify incorrect values in it;
* The interval between requests should be around 2-5 seconds. Or, you will receive the error `Failed to extract the message`.

# Compile
Build type: `Release/x86`<br>
Also dont forget to read about [SvenMod](https://github.com/sw1ft747/svenmod#svenmod) and add an additional dependency `WinInet.lib` to your project.

# OpenAI
You can read more information about neural networks and models from OpenAI here:
1. https://openai.com/pricing;
2. https://platform.openai.com/docs/api-reference;
3. https://platform.openai.com/docs/models/overview;
4. https://platform.openai.com/docs/models/how-we-use-your-data;
5. https://platform.openai.com/docs/models/model-endpoint-compatibility.

# Screenshots
Also, you can write messages not only in English, but also in other languages that the neural network supports.
* English<br><br>
![Screenshot_1](https://github.com/kekekekkek/NeuroChat/blob/main/Images/Screenshot_1.png)
* Russian<br><br>
![Screenshot_2](https://github.com/kekekekkek/NeuroChat/blob/main/Images/Screenshot_2.png)

# Conclusion
Perhaps i missed something and forgot to write about it, but here i have outlined the basic things that you will need to know about this plugin. You can also use a neural network on your local computer, if you have a powerful enough one, and link it to this plugin. This is just a concept of how fun it is to combine a neural network with different digital things :)
