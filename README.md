# Pomogotchi (Work in progress)

Pomogotchi is a small STM32-based device aimed to be a company while completing daily tasks. Inspired by the [Pomodoro technique](https://en.wikipedia.org/wiki/Pomodoro_Technique), a simple but useful technique to improve productivity by setting focus and rest periods.
Pomogotchi is a simple implementation of the Pomodoro technique without the need of setting alarms in distracting devices like a smartphone.

When executing, Pomogotchi has three states: 
- Focus State: 25 minutes period of focused work
- Short Rest State: 5 minutes period of resting
- Long Rest State: 15 minutes period of resting after 4 focus periods

Pomogotchi is designed wih a [bare-metal](https://en.wikipedia.org/wiki/Bare_machine) approach, implementing just what is needed. A [2.13inch e-paper display](https://www.waveshare.com/product/displays/e-paper/2.13inch-e-paper-hat-plus.htm) is used as the display interface since it can retain an image without any energy supplied. These traits make Pomogotchi a simple device with low current consumption.


![Alt text](media/pomogotchiDisplay.jpeg)


# Disclaimer!

To design Pomogotchi, only CMSIS libraries were used (no HAL layer nor e-ink paper libraries). Instead, all displaying functions, images and fonts are developed from scratch, since the goal of Pomogotchi isn't to create a visually appealing device but to serve as a hands-on project.


# Table of Contents

1. [Set up and Requirements](#set-up-and-requirements)
    - [Software Requirements](#software-requirements)
    - [Hardware Requirements](#hardware-requirements)
    - [Set up Steps](#set-up-steps)
2. [Project file structure](#project-file-structure)
3. [Software structure](#software-structure)
4. [Makefile phony targets](#makeifle-phony-targets)


# Set up and Requirements

### Software Requirements:
- GNU make
- [CMSIS6](https://github.com/ARM-software/CMSIS_6) header files
- [CMSIS device for STM32 F4 devices](https://github.com/STMicroelectronics/cmsis_device_f4) header files
- ARM gcc cross-toolchain
- OpenOCD
- gdb-multiarch
- clang-format-15

### Harware Requirements:
- 2.13 e-Paper HAT
- STM32F4 Board

### Set up steps:
Since this project is developed without any IDE, third party libraries need to be cloned on the environment. This next procedure is encouraged to build with the provided Makefile:

- Set the ENVIRONMENT_HOME_DIR variable with the name of the path where Pomogotchi repository is clone in the .bashrc file:

```
echo "export ENV_HOME_DIR="<your_path>\"" >> ~/.bashrc
```
> [!IMPORTANT]
> Use >> instead of > to append instead of overwritting the .bashrc file.

- clone the CMSIS files. In this project:
    - create a directory called cmsis_header_files on the root directory
    - clone CMSIS6 project (Core definitions)
    - clone CMSIS device for STM32F4 devices (Board definitions)

These steps can be followed but there are not mandatory. Instead, you can define your own path where header files are stored, just CMSIS_CORE and CMSIS_DEVICE variables in the Makefile need to be changed. 

# Project file structure

- bsp: e-ink display functionality
- build:
    - bin: executable file loaded in the MCU
    - debug: .log file (created with OpenOCD)
    - map: .map file
    - obj: object files
- media: images and videos
- user: task and main file
- util: miscellaneous files (start up file, linked script, system calls, dockerfile, OpenOCD config file)

# Software architecture

![Alt text](media/softwareStructure.png)


# Makefile phony targets

### all

This project is build using the GNU Makefile tool, to build everything just type on a terminal in the project directory:
```
make all
```
Or just ```make```. This creates the build/obj directory which contains all the objects of the project. This folder creates one directory for every layer (bsp, drivers, usr, util), each object file is placed in the corresponding directory. Alongside, an executable file named "executable.elf" is placed in a newly created directory called build/bin. This phony target can be ommited since other rules build entirely the project too. This phony target is used when no further action is required.

### load

Builds the project and then loads the executable file into the board using OpenOCD utilities using the openocd.cfg file, which is a config file for the target board. It do not halt the target CPU.

### debug

Builds the project and then opens a OpenOCD session loading the executable file and halting the CPU waiting for another terminal to open a gdb-multiarch sesion to start debugging (this can be done manually, or with another provided makeifle phony target).

### gdb_debug

Opens a gdb-multiarch session and halts the CPU in the main function, useful for manually debugging the project.

![Manually debugging with gdb_debug](media/debug.gif)

### gdb_log

Opens a gdb-multiarch session and executes a gdb script, which prints to a log file placed in build/debug directory. The gdb script is in the util directory.

![Creating log file with gdb_log rule](media/debugLog.gif)

### clean

Cleans the project, removes all the previously created objects.


# How Pomogotchi is designed

Every second, a timer triggers an interruption which calls an scheduler. The scheduler is function which manages the next state (pomodoro period) to execute. It defines whether a time period has elapsed or if the screen counter has to be updated. While there is no display update operation, the scheduler invokes the idle task, which reduces the amount of current consumed:

![Current consumption](media/currentConsumption.gif)

To turn on/off the device, the built-in button is used. This sets the CPU in sleep mode and waits until the next button press, which will re-initialaze the Pomogotchi and run a new session.
