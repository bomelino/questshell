# development notes


## test 1 - socket_lib

creates a socket server on the quest, client.js connects to it, receives and sends a message.


- run `qpm restore`, `.\build.ps1`
- run `adb forward tcp:9007 tcp:9007` to forward local port to port on quest

- run `.\copy.ps1` to start the game, adb logcat should output something like
    ```
    QuestHook[questshell|v0.1.0]: Starting server at port 9007
    QuestHook[questshell|v0.1.0]: Started Server
    QuestHook[questshell|v0.1.0]: Installing hooks...
    QuestHook[questshell|v0.1.0]: Installing hook: MainMenuViewController_DidActivate to offset: 0x7b34b375f4
    QuestHook[questshell|v0.1.0]: Installed all hooks!
    ```
- in /src run `node client.js` (you need nodejs [https://nodejs.dev/])
- it connects and prints
    ```
    Connected
    Received: hi!
    ```
- adb logcat prints 
    ```
    QuestHook[questshell|v0.1.0]: connect event
    QuestHook[questshell|v0.1.0]: Message received: Hello, server! Love, Client.
    ```

## test 2 - by hand with sys/socket


- call test2() in load
- run `adb reverse tcp:9007 tcp:9007`
- run `node server.js` in src

# tino's ideas/plans:

- simplify the build process, so that there are
    - less files needed
    - configuration in only one file
    - less copying on windows
- simple tool that parses adb logcat and gives useful and organized information about loaded libs, maybe a button to copy needed .so files


# tino's questions

- how to do configuration in one place? (for example the port number)
- in this test the module creates a socketserver but i think it would be better if the server was on the pc to allow multiple connections from multiple games (which always run the same code)
- why is build.ps "compiling" the other libs? we dont need that i think, but i really dont understand these makefiles
- could one do without qpm?
    - just put all needed header files inside this repo
    - the .so files are always the same because they are always running on a quest 2 and dont have to be built (?)
    - would this get rid of the copies of the .so files in /obj 
- there should be a default directory for the android-ndk so that there is no need for ndkpath.txt
- currently if theres a new version it has to be changed in Android.mk, right? would be better via qpm.json (or a new package.json)
