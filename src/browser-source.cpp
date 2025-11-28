#include "browser-source.hpp"
#include <obs-module.h>
#include <util/platform.h>
#include <util/dstr.h>

// Get source name
static const char *chat_browser_source_get_name(void *unused)
{
    UNUSED_PARAMETER(unused);
    return obs_module_text("Multi-Platform Chat Overlay");
}

// Create source instance
static void *chat_browser_source_create(obs_data_t *settings, obs_source_t *source)
{
    struct chat_browser_source *context = 
        (struct chat_browser_source *)bzalloc(sizeof(struct chat_browser_source));
    
    context->source = source;
    context->width = (uint32_t)obs_data_get_int(settings, "width");
    context->height = (uint32_t)obs_data_get_int(settings, "height");
    context->shutdown_on_hide = obs_data_get_bool(settings, "shutdown");
    
    // Get plugin data directory
    char *plugin_data_path = obs_module_file("ui/chat-overlay.html");
    if (plugin_data_path) {
        context->url = QString("file:///") + QString(plugin_data_path).replace("\\", "/");
        bfree(plugin_data_path);
    } else {
        context->url = "http://localhost:8765/overlay";
    }
    
    blog(LOG_INFO, "Chat browser source created: %s", 
         context->url.toUtf8().constData());
    
    return context;
}

// Destroy source instance
static void chat_browser_source_destroy(void *data)
{
    struct chat_browser_source *context = (struct chat_browser_source *)data;
    
    if (context) {
        bfree(context);
    }
}

// Update source settings
static void chat_browser_source_update(void *data, obs_data_t *settings)
{
    struct chat_browser_source *context = (struct chat_browser_source *)data;
    
    if (context) {
        context->width = (uint32_t)obs_data_get_int(settings, "width");
        context->height = (uint32_t)obs_data_get_int(settings, "height");
        context->shutdown_on_hide = obs_data_get_bool(settings, "shutdown");
    }
}

// Set default settings
static void chat_browser_source_defaults(obs_data_t *settings)
{
    obs_data_set_default_int(settings, "width", 400);
    obs_data_set_default_int(settings, "height", 600);
    obs_data_set_default_bool(settings, "shutdown", false);
    obs_data_set_default_bool(settings, "restart_when_active", false);
}

// Get source properties
static obs_properties_t *chat_browser_source_properties(void *data)
{
    UNUSED_PARAMETER(data);
    
    obs_properties_t *props = obs_properties_create();
    
    obs_properties_add_int(props, "width", 
                          obs_module_text("Width"), 
                          100, 3840, 10);
    
    obs_properties_add_int(props, "height", 
                          obs_module_text("Height"), 
                          100, 2160, 10);
    
    obs_properties_add_bool(props, "shutdown",
                           obs_module_text("Shutdown source when not visible"));
    
    obs_properties_add_bool(props, "restart_when_active",
                           obs_module_text("Refresh browser when source becomes active"));
    
    obs_properties_add_text(props, "info",
                           obs_module_text("Note: Browser source shows chat overlay from WebSocket server (localhost:8765)"),
                           OBS_TEXT_INFO);
    
    return props;
}

// Get source width
static uint32_t chat_browser_source_get_width(void *data)
{
    struct chat_browser_source *context = (struct chat_browser_source *)data;
    return context ? context->width : 400;
}

// Get source height
static uint32_t chat_browser_source_get_height(void *data)
{
    struct chat_browser_source *context = (struct chat_browser_source *)data;
    return context ? context->height : 600;
}

// Show source
static void chat_browser_source_show(void *data)
{
    struct chat_browser_source *context = (struct chat_browser_source *)data;
    UNUSED_PARAMETER(context);
    blog(LOG_INFO, "Chat browser source shown");
}

// Hide source
static void chat_browser_source_hide(void *data)
{
    struct chat_browser_source *context = (struct chat_browser_source *)data;
    UNUSED_PARAMETER(context);
    blog(LOG_INFO, "Chat browser source hidden");
}

// Activate source
static void chat_browser_source_activate(void *data)
{
    struct chat_browser_source *context = (struct chat_browser_source *)data;
    UNUSED_PARAMETER(context);
    blog(LOG_INFO, "Chat browser source activated");
}

// Deactivate source
static void chat_browser_source_deactivate(void *data)
{
    struct chat_browser_source *context = (struct chat_browser_source *)data;
    UNUSED_PARAMETER(context);
    blog(LOG_INFO, "Chat browser source deactivated");
}

// Source info structure - Fixed order to match obs_source_info struct
static struct obs_source_info chat_browser_source_info = {
    .id = "multi_platform_chat_browser_source",
    .type = OBS_SOURCE_TYPE_INPUT,
    .output_flags = OBS_SOURCE_ASYNC_VIDEO | OBS_SOURCE_DO_NOT_DUPLICATE,
    .get_name = chat_browser_source_get_name,
    .create = chat_browser_source_create,
    .destroy = chat_browser_source_destroy,
    .update = chat_browser_source_update,
    .activate = chat_browser_source_activate,      // Moved up
    .deactivate = chat_browser_source_deactivate,  // Moved up
    .show = chat_browser_source_show,              // Moved down
    .hide = chat_browser_source_hide,              // Moved down
    .get_width = chat_browser_source_get_width,
    .get_height = chat_browser_source_get_height,
    .get_defaults = chat_browser_source_defaults,
    .get_properties = chat_browser_source_properties,
};

// Register the browser source
void register_chat_browser_source()
{
    obs_register_source(&chat_browser_source_info);
    blog(LOG_INFO, "Multi-Platform Chat browser source registered");
}
