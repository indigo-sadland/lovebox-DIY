 LoveBox 
 <img src="https://github.com/indigo-sadland/lovebox-DIY/assets/37074372/6e121b9b-96d2-448a-8905-ba36c68b041c" align="left" width="95px"/>
 
The project is my dive into programming microcontrollers and all that soldering stuff. Many thanks to [julisa99](https://github.com/julisa99) for sharing the knowledge.


## Materials
1. [WeMos D1 Mini](https://www.amazon.de/dp/B01N9RXGHY) 
2. [Servo motor](https://www.amazon.de/dp/B07236KYVC)
3. [Light sensor](https://www.amazon.de/dp/B00SWO73DS)
4. [10kΩ resistor](https://www.amazon.de/dp/B072BHDBDG)
5. [OLED display](https://www.amazon.de/dp/B074NJMPYJ)
6. [Perfboard](https://www.amazon.de/dp/B00VL1KHJQ)
7. [Breakaway PCB Connector](https://www.amazon.de/dp/B01M69EA9O)
8. [1mm acryl](https://www.amazon.de/dp/B07T4JSXYR) (to cut the display)
9. [Darkening foil/dazzle strip](https://www.amazon.de/dp/B001CC6VYO)
10. Wooden Ring Box (of your choice)
11. Some Wooden Hearts for crafts
12. A Wall Plug O_O (something like [this](https://www.amazon.co.uk/White-Plugs-Plastic-Expansion-Screw/dp/B09SG7SNYK/ref=sr_1_20?keywords=plastic+board+wall+plugs&qid=1687377704&sr=8-20). You need to choose size mannualy.)

## Composition 
Below you can see where the OLED display, the light sensor with resistor and the servo motor are connected to the WeMos D1 Mini. The second photo is the soldered assembly. As you can see the micro-USB connector of the microcotroller board is at the ground. On top of the microcontroller a perfboard is soldered. Now the servo motor is added and also soldered to the perfboard. Notice the resistor and the light sensor that are also soldered to the board, with the light sensor pointing up. The display is soldered to another perfboard. The board provides a flat surface to hold the display in place. It is put on the motor and is connected to the other perfboard by using a Breakaway PCB Connector. It serves as a spacer to hold the other board above the motor.

![](https://github.com/julisa99/Lovebox/blob/master/docs/images/composition.JPG)

After finishing the electronics, it's time to take care of the case where you are planning to place the assembled board. At this moment you have two options:
1. Cut the case yourself using [the lasercut files](https://github.com/julisa99/Lovebox/tree/master/lasercut_files) containing the cutting plan.
2. Buy some wooden ring box and small wooden hearts [*].

Since I don't have proper tools for laser cutting I went with searching and buying the wooden box that fitted my assembled board. Then I've made a hole at the back of the box for the microUSB port and at the front to be able to connect servo with the wooden heart. Because walls of my box is much wider then the length of the servo motor's shaft, I used a wall plug to be able to anchor it with the heart (*you could see how it looks in the image on the right*)

<p float="left">
  <img src="https://github.com/indigo-sadland/lovebox-DIY/assets/37074372/6f3a9809-b765-43d2-9993-d4e07d4ba2ef" width="30%" />
  <img src="https://github.com/indigo-sadland/lovebox-DIY/assets/37074372/37d857d3-db0c-47b2-aa39-a3a4b30ebee9" width="42.5%"" /> 
</p>

## How it works

For programming I used the [Arduino IDE](https://www.arduino.cc/en/Main/Software). The cool thing about the Wemos D1 Mini is the built in ESP8266, so you can connect to the internet. Messages like pictures and texts can be sent to the Lovebox via the internet. In order to control the ESP8266, the following address must be added in the Arduino IDE under `File->Preferences->Additional Board Manager URLs`: http://arduino.esp8266.com/stable/package_esp8266com_index.json. Then you can type "esp8266" in the search field under `Tools->Board->Boards Manager` and click on "Install". Now the WeMos D1 Mini appears under the boards and can be selected.

Then you can open the ["Lovebox.ino"](https://github.com/julisa99/Lovebox/blob/master/Lovebox.ino) and upload it to the board, just connect it to your computer with a micro usb cable. You should first enter your WLAN, password and the path to your gist file in the ["credentials.h"](https://github.com/julisa99/Lovebox/blob/master/credentials.h). The gist file is a text document, which you create online as "private" and then almost always leave your message there, which the microcontroller can then retrieve.

The microcontroller will then always call your text document on gist.github and see if the first number in the first line is a different one. This serves as an identifier to determine if a new message is retrievable.
If this is different from the previous one, then the algorithm looks into the second line, if it contains a "t", then the microcontroller expects a text and shows the message on the display. If there is another character, e.g. a "b", then an image is expected. For the picture I use the website: https://www.dcode.fr/binary-image. There you can upload your black and white image and set the target width to 128 pixels. The image will then be converted to "0" and "1" only, so each black pixel becomes a 0 and each white pixel a 1. You can then simply enter this string in the third line of your text document on gist.github.

## Known Issues
The original source code didn't work for me so I had to modify it and now:
- [ ] Binary images are not displaying 
