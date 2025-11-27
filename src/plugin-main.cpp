#include <obs-module.h>
#include <obs-frontend-api.h>

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE("multi-platform-chat", "en-US")

bool obs_module_load(void)
{
    blog(LOG_INFO, "Multi-Platform Chat Plugin loaded!");
    return true;
}

void obs_module_unload(void)
{
    blog(LOG_INFO, "Multi-Platform Chat Plugin unloaded");
}

MODULE_EXPORT const char *obs_module_description(void)
{
    return "Multi-platform live chat aggregator";
}

MODULE_EXPORT const char *obs_module_name(void)
{
    return "Multi-Platform Chat";
}
