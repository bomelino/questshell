# development notes

- onMessage starts a duktape javascript interpreter, evaluates the message.
- native c functions can be bound into the javascript context (see main.cpp:native_log)

# todo

- keep the javascript context between messages
- add a function that changes something in the game

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
