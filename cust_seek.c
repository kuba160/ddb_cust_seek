/*
    Custom Seek Hotkeys Plugin for DeaDBeeF
    Copyright (C) 2019 Jakub Wasylków <kuba_160@protonmail.com>

    This software is provided 'as-is', without any express or implied
    warranty.  In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.

    3. This notice may not be removed or altered from any source distribution.
*/
#include <deadbeef/deadbeef.h>
#include <stdio.h>

DB_misc_t plugin;
//#define trace(...) { deadbeef->log ( __VA_ARGS__); }
#define trace(...) { deadbeef->log_detailed (&plugin.plugin, 0, __VA_ARGS__); }
#define trace_err(...) { deadbeef->log ( __VA_ARGS__); }

DB_functions_t *deadbeef;

static int
cust_seek_start () {
    return 0;
}

static int
cust_seek_stop () {
    return 0;
}

static int
seek_pres (int preset_num, unsigned char forward) {
    char conf_m[] = "cust_seek.preset1";
    char conf_v[] = "cust_seek.preset1_val";
    char conf_def = 0;
    signed char sign = -1;
    if (preset_num == 2) {
        (conf_m + strlen(conf_m) - 1)[0] = '2';
        (conf_v + strlen("cust_seek.preset"))[0] = '2';
        conf_def = 1;
    }
    if (forward) {
        sign = 1;
    }

    int percentage_mode = deadbeef->conf_get_int (conf_m, conf_def);
    trace ("%s - percentage_mode = %d", conf_m, percentage_mode);

    uint32_t seekval = deadbeef->streamer_get_playpos() * 1000;
    if (percentage_mode) {
        DB_playItem_t *pl = deadbeef->streamer_get_playing_track ();
        if (pl) {
            float amount = deadbeef->pl_get_item_duration (pl) * deadbeef->conf_get_int(conf_v, 3) / 100;
            //trace ("amount %.4f\n", amount);
            seekval += + sign * amount * 1000;
            deadbeef->pl_item_unref (pl);
        }
    }
    else {
        seekval += sign * deadbeef->conf_get_int(conf_v, 3) * 1000;
    }
    trace ("resulted seek (approx.): %d ms\n", (int) seekval - (int) deadbeef->streamer_get_playpos() * 1000);
    deadbeef->sendmessage (DB_EV_SEEK, (uintptr_t) NULL, seekval, 0);

    return 0;
}

static int
cust_seek_preset1a (DB_plugin_action_t *act, int ctx) {
    return seek_pres (1, 1);
}

static int
cust_seek_preset2a (DB_plugin_action_t *act, int ctx) {
    return seek_pres (2, 1);
}

static int
cust_seek_preset1b (DB_plugin_action_t *act, int ctx) {
    return seek_pres (1, 0);
}

static int
cust_seek_preset2b (DB_plugin_action_t *act, int ctx) {
    return seek_pres (2, 0);
}


static DB_plugin_action_t preset2b_action = {
    .name = "cust_seek_preset2b",
    .title = "Playback/Seek Backward Preset 2",
    .flags = DB_ACTION_COMMON,
    .callback2 = cust_seek_preset2b,
    .next = NULL
};

static DB_plugin_action_t preset1b_action = {
    .name = "cust_seek_preset1b",
    .title = "Playback/Seek Backward Preset 1",
    .flags = DB_ACTION_COMMON,
    .callback2 = cust_seek_preset1b,
    .next = &preset2b_action
};

static DB_plugin_action_t preset2a_action = {
    .name = "cust_seek_preset2",
    .title = "Playback/Seek Forward Preset 2",
    .flags = DB_ACTION_COMMON,
    .callback2 = cust_seek_preset2a,
    .next = &preset1b_action
};

static DB_plugin_action_t preset1a_action = {
    .name = "cust_seek_preset1a",
    .title = "Playback/Seek Forward Preset 1",
    .flags = DB_ACTION_COMMON,
    .callback2 = cust_seek_preset1a,
    .next = &preset2a_action
};

static DB_plugin_action_t *
cust_seek_get_actions (DB_playItem_t *it) {
    if (!it)
        return &preset1a_action;
    return NULL;
}


static const char settings_dlg[] =
    "property \"Preset 1: Seconds/Percentage\" checkbox cust_seek.preset1 0;\n"
    "property \"Preset 1 value:\" entry cust_seek.preset1_val 3;\n"
    "property \"Preset 2: Seconds/Percentage\" checkbox cust_seek.preset2 1;\n"
    "property \"Preset 2 value:\" entry cust_seek.preset2_val 3;\n";

DB_misc_t plugin = {
    .plugin.api_vmajor = 1,
    .plugin.api_vminor = 10,
    .plugin.type = DB_PLUGIN_MISC,
    .plugin.version_major = 1,
    .plugin.version_minor = 0,
    .plugin.id = "cust_seek",
    .plugin.name ="Custom Seek Hotkeys Plugin",
    .plugin.descr = "This plugin allows you to define custom seek actions that "
                    "can be used as hotkeys.\n"
                    "You can choose between time or percentage mode.\n"
                    "2 presets are available\n",
    .plugin.copyright =
        "Custom Seek Hotkeys Plugin for DeaDBeeF\n"
        "Copyright (C) 2019 Jakub Wasylków <kuba_160@protonmail.com>\n"
        "\n"
        "This software is provided 'as-is', without any express or implied\n"
        "warranty.  In no event will the authors be held liable for any damages\n"
        "arising from the use of this software.\n"
        "\n"
        "Permission is granted to anyone to use this software for any purpose,\n"
        "including commercial applications, and to alter it and redistribute it\n"
        "freely, subject to the following restrictions:\n"
        "\n"
        "1. The origin of this software must not be misrepresented; you must not\n"
        " claim that you wrote the original software. If you use this software\n"
        " in a product, an acknowledgment in the product documentation would be\n"
        " appreciated but is not required.\n"
        "\n"
        "2. Altered source versions must be plainly marked as such, and must not be\n"
        " misrepresented as being the original software.\n"
        "\n"
        "3. This notice may not be removed or altered from any source distribution.\n",
    .plugin.website = "http://github.com/kuba160/ddb_cust_seek",
    .plugin.start = cust_seek_start,
    .plugin.stop = cust_seek_stop,
    .plugin.configdialog = settings_dlg,
    .plugin.get_actions = cust_seek_get_actions
};

DB_plugin_t *
cust_seek_load (DB_functions_t *ddb) {
    deadbeef = ddb;
    return DB_PLUGIN(&plugin);
}
