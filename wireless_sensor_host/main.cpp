#include "Device.hpp"
#include "DeviceManager.hpp"

static DeviceManager m_dm;
int main(int argc, char *argv[]) {
    m_dm.start();
    while (1)
        ;
}
