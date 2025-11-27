#pragma once

#include <obs-module.h>
#include <QString>

// Browser source registration
void register_chat_browser_source();

// Browser source structure
struct chat_browser_source {
    obs_source_t *source;
    QString url;
    uint32_t width;
    uint32_t height;
    bool shutdown_on_hide;
};
