/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* this implements a GPS hardware library for the Android emulator.
 * the following code should be built as a shared library that will be
 * placed into /system/lib/hw/gps.goldfish.so
 *
 * it will be loaded by the code in hardware/libhardware/hardware.c
 * which is itself called from android_location_GpsLocationProvider.cpp
 */


#include <errno.h>
#include <fcntl.h>

#define  LOG_TAG  "nfc_qemu"
#include <cutils/log.h>
#include <hardware/nfc.h>
#include <hardware/qemud.h>

/* the name of the qemud-controlled socket */
#define  QEMU_CHANNEL_NAME  "nfc"

#define  NFC_DEBUG  0

#if NFC_DEBUG
#  define  D(...)   LOGD(__VA_ARGS__)
#else
#  define  D(...)   ((void)0)
#endif

typedef struct {
    hw_device_t common;
} nfc_goldfish_device_t;

/* Close an opened goldfish device instance */
static int
dev_close(hw_device_t *dev)
{
    nfc_goldfish_device_t *gdev = (nfc_goldfish_device_t*)dev;

    free(dev);
    return 0;
}

static int
module_open(const struct hw_module_t* module,
            char const* name,
            struct hw_device_t** device)
{
    if (strcmp(name, NFC_PN544_CONTROLLER)) {
        return -EINVAL;
    }

    nfc_goldfish_device_t *dev = calloc(1, sizeof(nfc_goldfish_device_t));

    dev->common.tag = HARDWARE_DEVICE_TAG;
    dev->common.version = 0;
    dev->common.module = (struct hw_module_t*)module;
    dev->common.close = dev_close;

    *device = (struct hw_device_t*)dev;
    return 0;
}


static struct hw_module_methods_t nfc_module_methods = {
    .open = module_open
};

const struct nfc_module_t HAL_MODULE_INFO_SYM = {
    .common = {
        .tag = HARDWARE_MODULE_TAG,
        .version_major = 1,
        .version_minor = 0,
        .id = NFC_HARDWARE_MODULE_ID,
        .name = "Goldfish NFC Module",
        .author = "Vicamo Yang",
        .methods = &nfc_module_methods,
    },
};
