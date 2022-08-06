#include "ioc.h"
#include <iostream>
#include <cstring>
#include <sys/ioctl.h>
#include <fcntl.h>    /* For O_RDWR */
#include <unistd.h>   /* For open(), creat() */
#include "policy.h"

#define DEV_NAME "/dev/hellfire"

#define HF_IOC_MAGIC 0x73 // 'S'
#define HF_IOC_POL_FLUSH _IO(HF_IOC_MAGIC, 1)
#define HF_IOC_POL_LIST  _IOWR(HF_IOC_MAGIC, 2, char*)
#define HF_IOC_POL_DEL   _IOWR(HF_IOC_MAGIC, 3, char*)

IOCDevice::IOCDevice() {
    fd = open(DEV_NAME, O_RDWR);
}

IOCDevice::~IOCDevice() {
    close(fd);
}

void IOCDevice::sendTo(std::string_view pol) const {
    std::cout << "Policy: " << pol << std::endl;
    if ((write(fd, pol.data(), pol.size())) == -1) {
        std::cerr << DEV_NAME << " ioctl: Cannot write the device " << std::endl;
    }
}

void IOCDevice::read(std::string_view query) {
    std::strcpy(buf, query.data());
    if (ioctl(fd, static_cast<unsigned long>(HF_IOC_POL_LIST), buf) == -1) {
        std::cerr << DEV_NAME << " ioctl: HF_IOC_POL_LIST Error" << std::endl;
    }
    try {
        Policy pol{buf};
        std::cout << pol << std::endl;    
    } catch (const std::exception& e) {
        std::cerr << DEV_NAME << " ioctl: Error " << e.what() << std::endl;
    }
    
}

void IOCDevice::flush() const {
    if (ioctl(fd, static_cast<unsigned long>(HF_IOC_POL_FLUSH)) == -1) {
        std::cerr << DEV_NAME << " ioctl: HF_IOC_POL_FLUSH Error" << std::endl;
    }
    std::cout << "Flushed the policy table" << std::endl;
}

void IOCDevice::del(std::string_view query) {
    std::strcpy(buf, query.data());
    if (ioctl(fd, static_cast<unsigned long>(HF_IOC_POL_DEL), buf) == -1) {
        std::cerr << DEV_NAME << " ioctl: HF_IOC_POL_DEL Error" << std::endl;
    }
}


