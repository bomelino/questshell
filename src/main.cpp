#include "main.hpp"
#include "GlobalNamespace/MainMenuViewController.hpp"

#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "UnityEngine/GameObject.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"

#include "beatsaber-hook/shared/config/config-utils.hpp"

  
#include <functional>

#include <cstdio>
#include <iostream>
#include <csignal>
#include "socket_lib/SocketHandler.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "duktape.h"

using namespace SocketLib;

static ModInfo modInfo; // Stores the ID and version of our mod, and is sent to the modloader upon startup


MAKE_HOOK_MATCH(MainMenuViewController_DidActivate, &GlobalNamespace::MainMenuViewController::DidActivate, void,
  GlobalNamespace::MainMenuViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling
) {
  
  MainMenuViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);
  auto solo_button = UnityEngine::GameObject::Find(il2cpp_utils::createcsstr("SoloButton"));
  //auto solo_text = solo_button->GetComponentInChildren<HMUI::CurvedTextMeshPro>();
  //solo_text.set_text(il2cpp_utils::createcsstr("Hello World"));
}


// Loads the config from disk using our modInfo, then returns it for use
Configuration& getConfig() {
    static Configuration config(modInfo);
    config.Load();
    return config;
}

// Returns a logger, useful for printing debug messages
Logger& getLogger() {
    static Logger* logger = new Logger(modInfo);
    return *logger;
}

// Called at the early stages of game loading
// i think this is why we need modloader.hpp
// ID and VERSION seem to get set via Andriod.mk:  
//    LOCAL_CFLAGS += -I'extern/libil2cpp/il2cpp/libil2cpp' -DID='"testmod"' -DVERSION='"0.1.0"'
// this could be done automatically via a central config
extern "C" void setup(ModInfo& info) {
    info.id = ID;
    info.version = VERSION;
    modInfo = info;
	
    getConfig().Load(); // Load the config file
    getLogger().info("questshell Completed setup!");
}


/* example: function that can get called inside javascript
log a number
*/
static duk_ret_t native_log(duk_context *ctx) {
  int i = duk_to_number(ctx,0); // argument
  getLogger().info("this got called inside javascript with the parameter: %i",i);
  return 0;  /* no return values */
}


/*
  // int is client descriptor, true if connect, false disconnected
    using ConnectCallback = std::function<void(Channel&, bool)>;
    using ListenCallback = std::function<void(Channel&, const Message&)>;
*/
void connectEvent(Channel& c, bool connected){
  getLogger().info("connect event!");

  if (connected) {
        c.queueWrite(Message("hi!\n"));
    }
}

void onMessage(Channel& client, const Message& message){
  getLogger().info("Message received: %s",message.toString().c_str());

  /* try to execute >:) */
  duk_context *ctx = duk_create_heap_default();
  // import native function into javascript context
  duk_push_c_function(ctx, native_log, 1 /*nargs*/);
  duk_put_global_string(ctx, "log");

  // evaluate message string
  duk_eval_string(ctx, message.toString().c_str());
  int result =  (int) duk_get_int(ctx, -1);
  getLogger().info("eval result:%i",result);

}

/* use socket_lib to start a socket server */
void test1(){

    getLogger().info("Starting server at port 9007");
    SocketHandler& socketHandler = SocketHandler::getCommonSocketHandler();

    ServerSocket* serverSocket = socketHandler.createServerSocket(9007);
    serverSocket->bindAndListen();
    getLogger().info("Started Server");

    serverSocket->registerConnectCallback(&connectEvent);
    serverSocket->registerListenCallback(&onMessage);
   
}

/**
 * blocking call to a socket server in node.js
 * call this in load() and run node server.js in /src
 * you also have to run `adb reverse tcp:9007 tcp:9007` instead of adb forward
 */
void test2(){
 int sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9006); // port
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1"); //INADDR_ANY;

    int status = connect(sock, (struct sockaddr * ) &server_address, sizeof(server_address));

    if(status == -1){
      getLogger().info("error connecting");
    } else {
      getLogger().info("connected successfully!");
      char response[256];

      // this is a blocking call
      recv(sock,&response,sizeof(response),0);

      getLogger().info("%s",response);
    }
}

// Called later on in the game loading - a good time to install function hooks
extern "C" void load() {
    il2cpp_functions::Init();

    test1();    

    /* try javascript interpreter */
    duk_context *ctx = duk_create_heap_default();
    duk_eval_string(ctx, "1+2");
    int result =  (int) duk_get_int(ctx, -1);
    getLogger().info("js result:%i",result);
    

    getLogger().info("Installing hooks...");
    // Install our hooks (none defined yet)
    INSTALL_HOOK(
    getLogger(),
    MainMenuViewController_DidActivate
    );
    getLogger().info("Installed all hooks!");
}