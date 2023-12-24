/*
 * DeviceManager.h
 *
 *  Created on: 26 mei 2017
 *      Author: André van Schoubroeck
 */

#ifndef SRC_DEVICEMANAGER_H_
#define SRC_DEVICEMANAGER_H_

// Include libusb before C++ includes, otherwise Windows build fails
extern "C" {
#include "libusb.h"
}

using namespace std;

// C++ Standard Libraries
#include <condition_variable>
#include <map>
#include <queue> // std::queue
#include <string>
#include <thread> // std::thread
#include <vector>

#include "Device.hpp"

typedef struct {
    struct libusb_context *ctx;
    struct libusb_device *dev;
    libusb_hotplug_event event;
} libusb_hotplug_event_t;

#include "IDeviceManager.hpp"

class DeviceManager {
  public:
    // DeviceManager();
    virtual ~DeviceManager();

    void start();

    //    IDevice* getDevice(uint32_t id)  {
    //        return mapSerial2Device[id];
    //    }
  private:
    libusb_context *ctx = nullptr;
    static DeviceManager *dm;

    void addController(Device *device);
    void eraseController(Device *device);

    bool libusb_hotplug_callback_thread_running = true;
    static void libusb_hotplug_callback_thread_code(DeviceManager *dm);
    thread libusb_hotplug_callback_thread;
    condition_variable libusb_hotplug_callback_cv;
    mutex libusb_hotplug_callback_mutex;

    libusb_hotplug_callback_handle hotplug_handle;

    static int LIBUSB_CALL libusb_hotplug_callback(struct libusb_context *ctx, struct libusb_device *dev,
                                                   libusb_hotplug_event event, void *user_data);

    queue<libusb_hotplug_event_t> libusb_hotplug_event_queue;

    thread libusb_handle_events_thread;
    bool libusb_handle_events_thread_running = true;
    static void libusb_handle_events_thread_code(DeviceManager *dm);

    static int debug_printf(const char *format, ...);

    map<std::string, Device *> mapSerial2Device;
    map<libusb_device *, Device *> mapUsb2Device;
};

#endif /* SRC_DEVICEMANAGER_H_ */
