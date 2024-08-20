# 10k_keypad

QMK firmware for the [10k keypad](https://github.com/souffle17/10k-keypad-hall-sensors) with variable input hall sensors     
**Note**: This uses a different pin configuration, and will not work unchanged.

* Keyboard Maintainer: [souffle17](https://github.com/souffle17)
* Hardware Supported: [10k keypad](https://github.com/souffle17/10k-keypad-hall-sensors)
* Hardware Availability: [10k keypad](https://github.com/souffle17/10k-keypad-hall-sensors)

Make example for this keyboard (after setting up your build environment):

    make 10k_keypad:default

Flashing example for this keyboard:

    make 10k_keypad:default:flash

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

## Bootloader

Enter the bootloader in 2 ways:

* **Bootmagic reset**: Hold down the top row keys and plug in the keyboard
* **Physical reset button**: Briefly press the button on the back of the PCB
