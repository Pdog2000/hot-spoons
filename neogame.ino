#include <Adafruit_NeoPixel.h>
#include <FastSPI_LED2.h>
#include <SerialCommand.h>
#include "funkboxing.h"
#include "button.h"
#include "game.h"

#define SERIALCOMMAND_DEBUG 1
#define LIGHTSHOW_MODE 3

#define NUM_LEDS 240
int mode = SPECTRUM_MODE;

Button button1 = Button(A6);
Button button2 = Button(A7);
Button button3 = Button(A8);
fastspi_strip strip = fastspi_strip(NUM_LEDS);
game g = game(NUM_LEDS, strip, button1, button3);
SerialCommand sCmd;

void setup() {
  Serial.begin(115200);

  sCmd.addCommand("m",   set_mode_strip);
  sCmd.setDefaultHandler(unrecognized);

  strip.setup();

  if (mode != LIGHTSHOW_MODE) {
    g.setup(mode);
  }

  Serial.println("---SETUP COMPLETE---");
}

void loop() {
  if (button2.pressed()) {
    mode = (mode + 1) % 4;
    setup();
  }

  if (mode == LIGHTSHOW_MODE) {
    lightshow_loop();
  }
  else {
    g.loop();
  }
}

void set_mode_strip() {    //-SETS THE MODE (SOME MODES REQUIRE RANDOM STARTS TO WORK RIGHT
  char *arg;
  arg = sCmd.next();

  if (arg != NULL) {
    strip.set_mode(atoi(arg));
  }
  Serial.print("~~~***NEW MODE-");
  Serial.println(strip.ledMode);
}

//------------------MAIN LOOP------------------
void lightshow_loop() {
  if(button1.pressed()) {
    set_lightshow_mode((strip.ledMode-1) % 28);
  }
  if(button3.pressed()) {
    set_lightshow_mode((strip.ledMode+1) % 28);
  }

  sCmd.readSerial();     //-PROCESS SERIAL COMMANDS

  strip.loop();
}

void set_lightshow_mode(int mode) {
  strip.set_mode(mode);
  Serial.print("~~~***NEW MODE-");
  Serial.println(strip.ledMode);
}


// GETS CALLED BY SERIALCOMMAND WHEN NO MATCHING COMMAND
void unrecognized(const char *command) {
  Serial.println("nothin fo ya...");
}
