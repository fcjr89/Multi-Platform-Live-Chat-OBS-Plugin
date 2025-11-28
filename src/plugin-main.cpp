#include "chat-aggregator.hpp"
#include "websocket-server.hpp"

// Don't include obs headers - we'll provide minimal stubs
#define OBS_DECLARE_MODULE()

// Global instances
ChatAggregator *g_chatAggregator = nullptr;
WebSocketServer *g_webSocketServer = nullptr;

// Minimal OBS module interface (no actual OBS integration)
extern "C" {

__declspec(dllexport) const char *obs_module_description(void)
{
    return "Multi-platform live chat aggregator";
}

__declspec(dllexport) const char *obs_module_name(void)
{
    return "Multi-Platform Chat";
}

__declspec(dllexport) const char *obs_module_author(void)
{
    return "Multi-Platform Chat Contributors";
}

__declspec(dllexport) bool obs_module_load(void)
{
    g_chatAggregator = new ChatAggregator();
    if (!g_chatAggregator) {
        return false;
    }

    g_webSocketServer = new WebSocketServer(8765);
    if (!g_webSocketServer) {
        delete g_chatAggregator;
        return false;
    }
    
    g_webSocketServer->setChatAggregator(g_chatAggregator);
    return true;
}

__declspec(dllexport) void obs_module_unload(void)
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

__declspec(dllexport) void obs_module_post_load(void)
{
}

__declspec(dllexport) void obs_module_set_locale(const char *locale)
{
    (void)locale;
}

__declspec(dllexport) void obs_module_free_locale(void)
{
}

__declspec(dllexport) const char *obs_module_text(const char *val)
{
    return val;
}

} // extern "C"
