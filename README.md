# Undertale_Discord_Rich_Presence

## How to use

Just drop these files in the Undertale Main Directory and execute Undertale_modded.exe to start the modded Version of Undertale. 
You can edit the JSON Configs in the data/ Folder, change or make a new languages for the locations names, etc.. 
You can also use following placeholders: 

{LV} - LV/LOVE  
{Kills_Global} - Kills in all areas  
{Kills_Ruins} - Kills in Ruins  
{Kills_Snowdin} - Kills in Snowin  
{Kills_Waterfall} - Kills in Waterfall  
{Kills_HotlandCore} - Kills in Hotland and Core   
{Location_ID} - The current Location ID ([https://pcy.ulyssis.be/undertale/rooms](https://pcy.ulyssis.be/undertale/rooms))  
{Location_Name} - The current Name of the location (editable in the lang files)  
{Area_Name} - The current Name of the area   

## How it works

The Undertale_modded.exe is a DLL Injector for the UNDERTALE_DLL.dll. 
It starts the real UNDERTALE.exe in your root folder of Undertale and a short time after it attaches the DLL to the running UNDERTALE Process.  
  
To get the needed Values UndertaleDRP reads out the memory of the running Undertale Process so it can be used via. the placeholders above.

## Disclaimer

Your Antivirus might be triggered by the DLL Injector, because this behavior is very similar like how a virus would attach to a process to not get detected. 
Nothing to worry about, but the Injector might be killed by such an Incident.
