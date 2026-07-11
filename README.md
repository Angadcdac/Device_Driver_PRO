# Linux Device Drivers

This repository contains my Linux Device Driver learning projects and hands-on implementations developed while exploring Linux kernel programming and embedded Linux. It includes examples of kernel modules, character device drivers, GPIO, interrupts, workqueues, timers, sysfs, procfs, I2C, USB drivers, and other Linux kernel programming concepts.

## Topics Covered

* Linux Kernel Modules
* Character Device Drivers
* Device File Operations
* GPIO Driver
* Interrupt Handling
* Kernel Timers
* Workqueues
* Tasklets
* Sysfs
* Procfs
* IOCTL
* Platform Drivers
* I2C Device Drivers
* USB Device Drivers
* Kernel Synchronization
* Kernel Logging (`printk`)
* Module Parameters
* Linked Lists in Kernel
* Memory Allocation (`kmalloc`, `kfree`)
* Wait Queues
* Polling Mechanisms

## Repository Structure

```text
Device_Driver_PRO/
├── 01_Kernel_Module/
├── 02_Character_Driver/
├── 03_GPIO/
├── 04_Interrupt/
├── 05_Kernel_Timer/
├── 06_Workqueue/
├── 07_Tasklet/
├── 08_Sysfs/
├── 09_Procfs/
├── 10_IOCTL/
├── 11_I2C/
├── 12_USB/
├── 13_Platform_Driver/
├── 14_Wait_Queue/
├── README.md
```

## Development Environment

* Ubuntu Linux
* BeagleBone Black
* Linux Kernel 5.x
* GCC
* GNU Make
* Vim
* Git

## Build

Compile a kernel module:

```bash
make
```

Load the module:

```bash
sudo insmod module_name.ko
```

Verify:

```bash
lsmod
dmesg | tail
```

Unload the module:

```bash
sudo rmmod module_name
```

## Learning Objectives

* Understand Linux kernel architecture.
* Develop Linux kernel modules and character device drivers.
* Learn driver development for embedded Linux systems.
* Implement communication between user space and kernel space.
* Gain practical experience with Linux kernel APIs and debugging techniques.

## Author

**Angad Chauhan**

Embedded Firmware Engineer | Embedded C | Linux | Device Drivers | Embedded Linux

