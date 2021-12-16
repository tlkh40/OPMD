#include <Arduino.h>

#include <Adafruit_NeoPixel.h>
#define PIN 2
#define NUMPIXELS 40

// Random variables for doing things later on
Adafruit_NeoPixel NeoPixel = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
bool hasRanOnce = false;
uint32_t Red = NeoPixel.Color(255, 0, 0);
uint32_t White = NeoPixel.Color(255, 255, 255);


// Struct that will be used to print / display the color on the display
struct RawInstruction
{
  int led;
  uint32_t color;
  bool isInit = false;
};

// Struct for making "RawInstructions" easier to do
struct instruction
{
  Position led;
  uint32_t color;
  bool isInit = false;
};


void setup()
{
  NeoPixel.begin();
}

// Utility class for getting a position on the grid easier
class Position
{
public:
  int X;
  int Y;

  Position()
  {
  }

  struct Position setX(int x)
  {
    X = x;
    return *this;
  }

  struct Position setY(int y)
  {
    Y = y;
    return *this;
  }

  int transform()
  {
    return X + (Y * 8);
  }
};


// class userd for storing / getting all the chracter information.
class Character
{
private:
  bool isSet = false;

public:
  int first = 0;
  int second = 1;
  int third = 2;
  instruction instructions[18] = {};

  Character() {}

  void set(String character, bool isCapatilized = false)
  {
    String TLC = character;
    TLC.trim();
    TLC.toLowerCase();
    if (TLC == "h")
      setFirstCharacterAsH(Red,isCapatilized);
  }

  void setFirstCharacterAsH(uint32_t color = Red, bool isCapital = false)
  {
    if (isCapital)
    {
      setInstruction(0, Position().setX(0).setY(0), color);
      setInstruction(1, Position().setX(0).setY(1), color);
      setInstruction(2, Position().setX(0).setY(2), color);
      setInstruction(3, Position().setX(0).setY(3), color);
      setInstruction(4, Position().setX(0).setY(4), color);
      setInstruction(6, Position().setX(2).setY(1), color);
      setInstruction(7, Position().setX(2).setY(0), color);
      setInstruction(8, Position().setX(1).setY(2), color);
      setInstruction(9, Position().setX(2).setY(2), color);
      setInstruction(10, Position().setX(2).setY(3), color);
      setInstruction(11, Position().setX(2).setY(4), color);
    }
    else
    {
      setInstruction(0, Position().setX(0).setY(0), color);
      setInstruction(1, Position().setX(0).setY(1), color);
      setInstruction(2, Position().setX(0).setY(2), color);
      setInstruction(3, Position().setX(0).setY(3), color);
      setInstruction(4, Position().setX(0).setY(4), color);
      setInstruction(5, Position().setX(1).setY(2), color);
      setInstruction(6, Position().setX(2).setY(3), color);
      setInstruction(7, Position().setX(2).setY(4), color);
    }
    isSet = true;
  }

  void setInstruction(int i, struct Position post, uint32_t color)
  {
    instructions[i].led = post;
    instructions[i].color = color;
    instructions[i].isInit = true;
  }
};

// class for turning "Position" structs into "Rawinstruction" structs
class Grid
{
public:
  struct Position StartFrom;
  struct Position EndAt;
  RawInstruction ri;
  Grid()
  {
  }

  int setStartFrom(struct Position startFrom)
  {
    StartFrom = startFrom;
    return 0;
  }

  int setEndAt(struct Position endAt)
  {
    EndAt = endAt;
    return 0;
  }

  struct RawInstruction toRawInstruction(struct instruction instruction, struct Character character)
  {
    int first = StartFrom.transform();
    int second = StartFrom.transform() + 1;
    int third = StartFrom.transform() + 2;
    int charFirst = character.first;
    int charSecond = character.second;
    int charThird = character.third;
    int charX = instruction.led.X;
    int X;
    int Y = instruction.led.Y * 8;
    if (charX == charFirst)
    {
      X = first;
    }
    if (charX == charSecond)
    {
      X = second;
    }
    if (charX == charThird)
    {
      X = third;
    }
    RawInstruction ri;
    ri.color = instruction.color;
    ri.led = X + Y;
    ri.isInit = true;
    return ri;
  }
};

void loop()
{
  if(!hasRanOnce) printH();
}

void printH()
{
  Character c;
  c.set("h",false);
  Grid grid;
  grid.setStartFrom(Position().setX(1).setY(0));
  grid.setEndAt(Position().setX(7).setY(0));
  RawInstruction ri[18];
  for (int i = 0; i < 18; i++)
  {
    if (c.instructions[i].isInit)
    {
      ri[i] = grid.toRawInstruction(c.instructions[i], c);
    }
  }
  for (int i = 0; i < 18; i++)
  {
    if (ri[i].isInit)
    {
      NeoPixel.setPixelColor(ri[i].led, ri[i].color);
      NeoPixel.show();
    }
  }
  hasRanOnce = true;
}