#include <obs-module.h>
#include <obs-frontend-api.h>
#include <QMainWindow>
#include <QAction>
#include <QMessageBox>
#include "chat-aggregator.hpp"
#include "websocket-server.hpp"

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE("multi-platform-chat", "en-US")

// Global instances
ChatAggregator *g_chatAggregator = nullptr;
WebSocketServer *g_webSocketServer = nullptr;

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
    // Create chat aggregator
    g_chatAggregator = new ChatAggregator();
    if (!g_chatAggregator) {
        return false;
    }

    // Create WebSocket server
    g_webSocketServer = new WebSocketServer(8765);
    if (!g_webSocketServer) {
        delete g_chatAggregator;
        return false;
    }
    
    g_webSocketServer->setChatAggregator(g_chatAggregator);

    // TODO: Add OBS UI integration when obs.lib is available
    
    return true;
}

// Unload module
void obs_module_unload(void)
{
    if (g_webSocketServer) {
        delete g_webSocketServer;
        g_webSocketServer = nullptr;
    }

    if (g_chatAggregator) {
        g_chatAggregator->disconnectAll();
        delete g_chatAggregator;
        g_chatAggregator = nullptr;
    }
}

// Post-load callback
void obs_module_post_load(void)
{
    // Empty for now
}
