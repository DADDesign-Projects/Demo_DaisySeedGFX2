# Demo DaisySeedGFX2

This repository contains two example projects showcasing the use of the DaisySeedGFX MK2 library for the Daisy Seed platform by Electrosmith.

## Author

DAD Design

## Overview

The examples demonstrate different ways to utilize the graphical features of the DaisySeedGFX MK2 library, focusing on fonts and images.

### Examples Provided

1. **Demo**:

   - Utilizes fonts and images encoded directly in C.
   - Fonts are generated using the TrueType-to-Bitmap-Converter utility (repository: [https://github.com/DADDesign-Projects/TrueType-to-Bitmap-Converter](https://github.com/DADDesign-Projects/TrueType-to-Bitmap-Converter)).

2. **DemoUseFlashMemory**:

   - Demonstrates the use of fonts and images stored in the Daisy Seed's flash memory.
   - Fonts and bitmaps are stored in flash memory using the **Daisy\_QSPI\_Flasher** utility (repository: [https://github.com/DADDesign-Projects/Daisy\_QSPI\_Flasher](https://github.com/DADDesign-Projects/Daisy_QSPI_Flasher)).

## Getting Started

To clone and set up the project:

1. Clone this repository with submodules recursively:

   ```bash
   git clone --recurse-submodules https://github.com/DADDesign-Projects/Demo_DaisySeedGFX2
   ```

2. Build the projects using the Daisy Seed development environment configured in VS Code. Refer to the Daisy Seed documentation for setup instructions: [https://github.com/electro-smith](https://github.com/electro-smith).

##

