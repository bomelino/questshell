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

// Called later on in the game loading - a good time to install function hooks
extern "C" void load() {
    il2cpp_functions::Init();

    getLogger().info("Starting server at port 3306");
    SocketHandler& socketHandler = SocketHandler::getCommonSocketHandler();

    ServerSocket* serverSocket = socketHandler.createServerSocket(3306);
    serverSocket->bindAndListen();
    getLogger().info("Started Server");

    serverSocket->registerConnectCallback(&connectEvent);

    /*
    serverSocket.registerListenCallback([this](Channel& client, const Message& message){
        listenOnEvents(client, message);
    });*/


    getLogger().info("Installing hooks...");
    // Install our hooks (none defined yet)
    INSTALL_HOOK(
    getLogger(),
    MainMenuViewController_DidActivate
    );
    getLogger().info("Installed all hooks!");
}