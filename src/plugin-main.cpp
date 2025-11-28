#include <obs-module.h>
#include <obs-frontend-api.h>
#include <QMainWindow>
#include <QAction>
#include <QMessageBox>
#include "chat-dock.hpp"
#include "chat-aggregator.hpp"
#include "websocket-server.hpp"
#include "browser-source.hpp"

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE("multi-platform-chat", "en-US")

// Global instances
ChatAggregator *g_chatAggregator = nullptr;
WebSocketServer *g_webSocketServer = nullptr;
ChatDock *g_chatDock = nullptr;

// Module description
MODULE_EXPORT const char *obs_module_description(void)
{
    return "Multi-platform live chat aggregator supporting 19+ streaming platforms";
}

// Module name
MODULE_EXPORT const char *obs_module_name(void)
{
    return "Multi-Platform Chat";
}

// Module author
MODULE_EXPORT const char *obs_module_author(void)
{
    return "Multi-Platform Chat Contributors";
}

// Load module
bool obs_module_load(void)
{
    blog(LOG_INFO, "================================================");
    blog(LOG_INFO, "Multi-Platform Chat OBS Plugin Loading...");
    blog(LOG_INFO, "Version: 1.0.0");
    blog(LOG_INFO, "================================================");

    // Create chat aggregator
    g_chatAggregator = new ChatAggregator();
    if (!g_chatAggregator) {
        blog(LOG_ERROR, "Failed to create ChatAggregator");
        return false;
    }
    blog(LOG_INFO, "✓ Chat aggregator created");

    // Create WebSocket server
    g_webSocketServer = new WebSocketServer(8765);
    if (!g_webSocketServer) {
        blog(LOG_ERROR, "Failed to create WebSocketServer");
        delete g_chatAggregator;
        return false;
    }
    
    g_webSocketServer->setChatAggregator(g_chatAggregator);
    
    if (g_webSocketServer->isRunning()) {
        blog(LOG_INFO, "✓ WebSocket server running on port %d", 
             g_webSocketServer->port());
    } else {
        blog(LOG_WARNING, "⚠ WebSocket server failed to start");
    }

    // Get OBS main window
    QMainWindow *mainWindow = static_cast<QMainWindow*>(
        obs_frontend_get_main_window());
    
    if (!mainWindow) {
        blog(LOG_ERROR, "Failed to get OBS main window");
        delete g_webSocketServer;
        delete g_chatAggregator;
        return false;
    }

    // Create chat dock
    obs_frontend_push_ui_translation(obs_module_get_string);
    
    g_chatDock = new ChatDock(mainWindow);
    if (!g_chatDock) {
        blog(LOG_ERROR, "Failed to create ChatDock");
        obs_frontend_pop_ui_translation();
        delete g_webSocketServer;
        delete g_chatAggregator;
        return false;
    }
    
    g_chatDock->setChatAggregator(g_chatAggregator);
    g_chatDock->setObjectName("multiPlatformChatDock");
    g_chatDock->setFloating(false);
    g_chatDock->setVisible(false);
    
    obs_frontend_add_dock(g_chatDock);
    blog(LOG_INFO, "✓ Chat dock added to OBS");
    
    obs_frontend_pop_ui_translation();

    // Register browser source
    register_chat_browser_source();
    blog(LOG_INFO, "✓ Browser source registered");

    blog(LOG_INFO, "================================================");
    blog(LOG_INFO, "✓ Multi-Platform Chat Plugin Loaded Successfully!");
    blog(LOG_INFO, "================================================");
    blog(LOG_INFO, "");
    blog(LOG_INFO, "Usage:");
    blog(LOG_INFO, "  1. Open 'Multi-Platform Chat' dock from View menu");
    blog(LOG_INFO, "  2. Click 'Add Platform' to connect to streaming platforms");
    blog(LOG_INFO, "  3. Add 'Multi-Platform Chat Overlay' browser source to scene");
    blog(LOG_INFO, "");
    blog(LOG_INFO, "Supported Platforms:");
    blog(LOG_INFO, "  YouTube, Twitch, Kick, Rumble, Gettr, Odysee,");
    blog(LOG_INFO, "  Pilled.net, Bigo Live, Instagram, Facebook,");
    blog(LOG_INFO, "  NimoTV, Retake.tv, SOOP, FC2 Live, Bitchute,");
    blog(LOG_INFO, "  DLive, Twitter/X, VK, Locals");
    blog(LOG_INFO, "");
    blog(LOG_INFO, "WebSocket Server: ws://localhost:8765");
    blog(LOG_INFO, "================================================");

    return true;
}

// Unload module
void obs_module_unload(void)
{
    blog(LOG_INFO, "================================================");
    blog(LOG_INFO, "Multi-Platform Chat Plugin Unloading...");
    blog(LOG_INFO, "================================================");

    // Clean up in reverse order
    if (g_chatDock) {
        blog(LOG_INFO, "Removing chat dock...");
        // obs_frontend_remove_dock() doesn't exist - just delete the widget
        // The dock will be automatically removed when the widget is destroyed
        delete g_chatDock;
        g_chatDock = nullptr;
    }

    if (g_webSocketServer) {
        blog(LOG_INFO, "Shutting down WebSocket server...");
        delete g_webSocketServer;
        g_webSocketServer = nullptr;
    }

    if (g_chatAggregator) {
        blog(LOG_INFO, "Disconnecting all platforms...");
        g_chatAggregator->disconnectAll();
        delete g_chatAggregator;
        g_chatAggregator = nullptr;
    }

    blog(LOG_INFO, "================================================");
    blog(LOG_INFO, "✓ Multi-Platform Chat Plugin Unloaded");
    blog(LOG_INFO, "================================================");
}

// Post-load callback
void obs_module_post_load(void)
{
    blog(LOG_INFO, "Multi-Platform Chat: Post-load complete");
}
