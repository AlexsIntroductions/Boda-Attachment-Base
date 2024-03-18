# Boda Attachment Base
A Raspberry Pi Pico (rp2040) based project to coincide with the Boda capstone project. 

## Description
The Boda is a robotic arm that has interchangeable attachments that can be designed for a users use-case. The attachments are based on an rp2040 (Raspberry Pi Pico) that has been programmed using this repo.
The repo includes a pre-formatted framework which contains the functionality to communicate with a Boda base arm and 

### System Requirements
- cmake
- Pi Pico C/C++ SDK

**Windows/iMac**

VSCode and A valid instance of WSL/WSL2; development used WSL2: Ubuntu

## Build Instructions
1. Complete the Getting started with Raspberry Pi Pico to properly download and install the rp2040 C/C++ SDK (if not installed) :

   https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf?_gl=1*r2v8m9*_ga*MTcxOTc2MzgwNi4xNzA3MjYzODA5*_ga_22FD70LWDS*MTcwNzYwOTU4Mi4xLjAuMTcwNzYwOTU4Mi4wLjAuMA..

2. Fork this repository then go to step 2 OR if you plan on making your own repository later skip to the next step.

3. Clone Git Repo to a local folder.

4. In the top level project directory, edit the `CMakeLists.txt` locate line 3:
```
set(PICO_SDK_PATH "/{FILE_PATH}/pico-sdk")
```
&emsp; and change the file path where the locally installed `pico-sdk` folder is located.

4. In the VSCode Terminal, create a build directory:
```
mkdir build
```

5. Enter the build directory:
```
cd build
```

6. Generate the Build files using cmake by running:
```
cmake .. 
```

7. Generate `main.uf2` to upload to the Raspberry Pi Pico:
```
make
```

8. Connect Pi Pico in bootsel mode by holding the `BOOTSEL` button while inserting the USB.
9. Drag resulting `main.uf2` file in the build directory into pi pico's drive to load it.

## Development Instructions:
### Attachment Library Description:
The attachment library, `Attachment.h & Attachment.c`, contain the default attachment settings to communicate with the Boda arm. The attachment struct instance at `line 6: Attachment a;` is already created and initialized in `line 13: initAttachment(&a);`.
It uses the Pi Pico's `spi_default` to communicate with the Boda arm and automatically sets an IRQ to receive inputs and commands. Commands are decoded and automatically sent back to the arm without user intervention and inputs are stored in a status byte in the Attachment struct. Inputs are checked by calling `pollButtonFunctions(Attachment* a);` which is called in the program's infinite loop. This function will check if a button's bit has been set in the Attachment's status byte and then call its respective button function and clear the bit. These functions are seen below the main loop and are titled `buttonOneFunction() -> buttonEightFunction()` and are where each button's functionality goes.

### Updating the Version:
The version of your attachment library is located in `src/Attachment.h`. If the version in your project differs from this repo and you want to update, FIRST make sure to copy your user set Identifiers and Settings as noted in the next section **THEY ARE NOT AUTOMATICALLY COPIED**, download the `Attachment.h` and `Attachment.c` files and replace the ones in your project with the newly downloaded ones. Then replace the default user set Identifiers and Settings with the ones you copied.

### Editing Attachment Identifiers and Settings:
In `Attachment.c` at the top of the file there are several user changeable settings and identifiers to set when creating an attachment including: what button activates each 'button{num}Function', WHO_AM_I identifier, locations of sensor Data.
Our current implementation relies on our PlayStation 2 Controller interface, thus the options for buttons are based on the PS2 Controllers'.

### Editing Functionality:
Each button has its own function:
1. Figure out which button's function you want to edit. If you haven't set them yourself, check in `Attachment.c` for the `buttons[8]` array, get the index of the button you want to add functionality to, add 1, and go to its button function in `main.c`.
2. In the function add your code before the return statement to give the button functionality.

## Attachment Commands:
| Command | Description |
| ------- | ----------- |
| 0x01 | Get Buttons to Trigger buttonOneFunction and buttonTwoFunction | 
| 0x02 | Get Buttons to Trigger buttonThreeFunction and buttonFourFunction | 
| 0x03 | Get Buttons to Trigger buttonFiveFunction and buttonSixFunction | 
| 0x04 | Get Buttons to Trigger buttonSevenFunction and buttonEightFunction | 
| 0x64 | Read WHO_AM_I register | 
