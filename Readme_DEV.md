# development notes
this is the first test, it doesnt work (01.08.2021)

i want to establish a socket connection from a node script to a mod running inside beatsaber

- run `qpm restore`, `.\build.ps1`
- run `adb forward tcp:3360 tcp:3360` to forward local port to port on quest
- run `.\copy.ps1` to start the game, adb logcat should output something like
    ```
    QuestHook[questshell|v0.1.0]: Starting server at port 3306
    QuestHook[questshell|v0.1.0]: Started Server
    QuestHook[questshell|v0.1.0]: Installing hooks...
    QuestHook[questshell|v0.1.0]: Installing hook: MainMenuViewController_DidActivate to offset: 0x7b34b375f4
    QuestHook[questshell|v0.1.0]: Installed all hooks!
    ```
- in /src run `node client.js` (you need nodejs [https://nodejs.dev/])
- it connects and immediately disconnects 
    ```
    Connected
    Connection closed
    ```
- somewhere there is a mistake. the expected behaviour is that adb logcat outputs something like `QuestHook[questshell|v0.1.0]: connect event!`
- i have no idea what im missing :)

# tino's ideas/plans:

- simplify the build process, so that there are
    - less files needed
    - configuration in only one file
    - less copying on windows
- simple tool that parses adb logcat and gives useful and organized information about loaded libs, maybe a button to copy needed .so files


# tino's questions

- why is build.ps "compiling" the other libs? we dont need that i think, but i really dont understand these makefiles
- could one do without qpm?
    - just put all needed header files inside this repo
    - the .so files are always the same because they are always running on a quest 2 and dont have to be built (?)
    - would this get rid of the copies of the .so files in /obj 
- there should be a default directory for the android-ndk so that there is no need for ndkpath.txt
- currently if theres a new version it has to be changed in Android.mk, right? would be better via qpm.json (or a new package.json)
