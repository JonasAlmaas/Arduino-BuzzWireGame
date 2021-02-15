<!-- HEADER -->
# Arduino - BuzzWireGame
Buzz wire game for Arduino with a grove backlight LCD

<!-- Setup -->
## Guide
- If you don't already have the rgb_lcd library download it below.
- The handle should be connected to 5V.
- Make sure you are using a 220ish Ohm resistor for each of the input signals as demonstrated below. (Red is the 5V, black is ground and orange is the signal wire)
![alt text](https://i.imgur.com/dJbQWGH.png "Preview image")

<!-- I/O List -->
## I/O List
| Signal | Pin |
| ------ |:---:|
| Start  | 10 |
| End    | 11 |
| Touch  | 13 |
| Buzzer | 3 (D3) |
| LCD    | And of the I2C plugs |

<!-- Resources -->
## Resources
[Grove LCD RGB backlight documentation](https://wiki.seeedstudio.com/Grove-LCD_RGB_Backlight/)

[rgb_lcd library download link](https://github.com/Seeed-Studio/Grove_LCD_RGB_Backlight/archive/master.zip)
