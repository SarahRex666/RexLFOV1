#include <Wire.h>
#include <Adafruit_MCP4725.h>
Adafruit_MCP4725 dac;
#define DAC_RESOLUTION    (12)
int sineledPin = 5;
int triangleledPin = 1;
int squareledPin = 2;
int randomledPin = 3;
const int potPin = 20;
int modeSelector = 0;
const int buttonPin = 0;
unsigned long previousMillis = 0;
unsigned long lfoInterval = 0;
unsigned long randomChangeInterval = 0;

const int squareWaveTable[1024] = {
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 
4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 
4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 
4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 
4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 
4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 
4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 
4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 
4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 
4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 
4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 
4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 
4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 
4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 
4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 
4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095

};


const int triangleWaveTable[512] = 
{
16,32,48,64,80,96,112,128,
144,160,176,192,208,224,240,256,
272,288,304,320,336,352,368,384,
400,416,432,448,464,480,496,512,
528,544,560,576,592,608,624,640,
656,672,688,704,720,736,752,768,
784,800,816,832,848,864,880,896,
912,928,944,960,976,992,1008,1024,
1040,1056,1072,1088,1104,1120,1136,1152,
1168,1184,1200,1216,1232,1248,1264,1280,
1296,1312,1328,1344,1360,1376,1392,1408,
1424,1440,1456,1472,1488,1504,1520,1536,
1552,1568,1584,1600,1616,1632,1648,1664,
1680,1696,1712,1728,1744,1760,1776,1792,
1808,1824,1840,1856,1872,1888,1904,1920,
1936,1952,1968,1984,2000,2016,2032,2048,
2063,2079,2095,2111,2127,2143,2159,2175,
2191,2207,2223,2239,2255,2271,2287,2303,
2319,2335,2351,2367,2383,2399,2415,2431,
2447,2463,2479,2495,2511,2527,2543,2559,
2575,2591,2607,2623,2639,2655,2671,2687,
2703,2719,2735,2751,2767,2783,2799,2815,
2831,2847,2863,2879,2895,2911,2927,2943,
2959,2975,2991,3007,3023,3039,3055,3071,
3087,3103,3119,3135,3151,3167,3183,3199,
3215,3231,3247,3263,3279,3295,3311,3327,
3343,3359,3375,3391,3407,3423,3439,3455,
3471,3487,3503,3519,3535,3551,3567,3583,
3599,3615,3631,3647,3663,3679,3695,3711,
3727,3743,3759,3775,3791,3807,3823,3839,
3855,3871,3887,3903,3919,3935,3951,3967,
3983,3999,4015,4031,4047,4063,4079,4095,
4079,4063,4047,4031,4015,3999,3983,3967,
3951,3935,3919,3903,3887,3871,3855,3839,
3823,3807,3791,3775,3759,3743,3727,3711,
3695,3679,3663,3647,3631,3615,3599,3583,
3567,3551,3535,3519,3503,3487,3471,3455,
3439,3423,3407,3391,3375,3359,3343,3327,
3311,3295,3279,3263,3247,3231,3215,3199,
3183,3167,3151,3135,3119,3103,3087,3071,
3055,3039,3023,3007,2991,2975,2959,2943,
2927,2911,2895,2879,2863,2847,2831,2815,
2799,2783,2767,2751,2735,2719,2703,2687,
2671,2655,2639,2623,2607,2591,2575,2559,
2543,2527,2511,2495,2479,2463,2447,2431,
2415,2399,2383,2367,2351,2335,2319,2303,
2287,2271,2255,2239,2223,2207,2191,2175,
2159,2143,2127,2111,2095,2079,2063,2048,
2032,2016,2000,1984,1968,1952,1936,1920,
1904,1888,1872,1856,1840,1824,1808,1792,
1776,1760,1744,1728,1712,1696,1680,1664,
1648,1632,1616,1600,1584,1568,1552,1536,
1520,1504,1488,1472,1456,1440,1424,1408,
1392,1376,1360,1344,1328,1312,1296,1280,
1264,1248,1232,1216,1200,1184,1168,1152,
1136,1120,1104,1088,1072,1056,1040,1024,
1008,992,976,960,944,928,912,896,
880,864,848,832,816,800,784,768,
752,736,720,704,688,672,656,640,
624,608,592,576,560,544,528,512,
496,480,464,448,432,416,400,384,
368,352,336,320,304,288,272,256,
240,224,208,192,176,160,144,128,
112,96,80,64,48,32,16,0,
};

const int sineWaveTable[512] =
{
2048,2073,2098,2123,2148,2173,2198,2223,
2248,2273,2298,2323,2348,2373,2398,2422,
2447,2472,2496,2521,2545,2569,2594,2618,
2642,2666,2690,2714,2737,2761,2784,2808,
2831,2854,2877,2900,2923,2946,2968,2990,
3013,3035,3057,3078,3100,3122,3143,3164,
3185,3206,3226,3247,3267,3287,3307,3327,
3346,3366,3385,3404,3423,3441,3459,3477,
3495,3513,3530,3548,3565,3581,3598,3614,
3630,3646,3662,3677,3692,3707,3722,3736,
3750,3764,3777,3791,3804,3816,3829,3841,
3853,3865,3876,3888,3898,3909,3919,3929,
3939,3949,3958,3967,3975,3984,3992,3999,
4007,4014,4021,4027,4034,4040,4045,4051,
4056,4060,4065,4069,4073,4076,4080,4083,
4085,4087,4089,4091,4093,4094,4094,4095,
4095,4095,4094,4094,4093,4091,4089,4087,
4085,4083,4080,4076,4073,4069,4065,4060,
4056,4051,4045,4040,4034,4027,4021,4014,
4007,3999,3992,3984,3975,3967,3958,3949,
3939,3929,3919,3909,3898,3888,3876,3865,
3853,3841,3829,3816,3804,3791,3777,3764,
3750,3736,3722,3707,3692,3677,3662,3646,
3630,3614,3598,3581,3565,3548,3530,3513,
3495,3477,3459,3441,3423,3404,3385,3366,
3346,3327,3307,3287,3267,3247,3226,3206,
3185,3164,3143,3122,3100,3078,3057,3035,
3013,2990,2968,2946,2923,2900,2877,2854,
2831,2808,2784,2761,2737,2714,2690,2666,
2642,2618,2594,2569,2545,2521,2496,2472,
2447,2422,2398,2373,2348,2323,2298,2273,
2248,2223,2198,2173,2148,2123,2098,2073,
2048,2022,1997,1972,1947,1922,1897,1872,
1847,1822,1797,1772,1747,1722,1697,1673,
1648,1623,1599,1574,1550,1526,1501,1477,
1453,1429,1405,1381,1358,1334,1311,1287,
1264,1241,1218,1195,1172,1149,1127,1105,
1082,1060,1038,1017,995,973,952,931,
910,889,869,848,828,808,788,768,
749,729,710,691,672,654,636,618,
600,582,565,547,530,514,497,481,
465,449,433,418,403,388,373,359,
345,331,318,304,291,279,266,254,
242,230,219,207,197,186,176,166,
156,146,137,128,120,111,103,96,
88,81,74,68,61,55,50,44,
39,35,30,26,22,19,15,12,
10,8,6,4,2,1,1,0,
0,0,1,1,2,4,6,8,
10,12,15,19,22,26,30,35,
39,44,50,55,61,68,74,81,
88,96,103,111,120,128,137,146,
156,166,176,186,197,207,219,230,
242,254,266,279,291,304,318,331,
345,359,373,388,403,418,433,449,
465,481,497,514,530,547,565,582,
600,618,636,654,672,691,710,729,
749,768,788,808,828,848,869,889,
910,931,952,973,995,1017,1038,1060,
1082,1105,1127,1149,1172,1195,1218,1241,
1264,1287,1311,1334,1358,1381,1405,1429,
1453,1477,1501,1526,1550,1574,1599,1623,
1648,1673,1697,1722,1747,1772,1797,1822,
1847,1872,1897,1922,1947,1972,1997,2022,
};

void setup(void) {
  dac.begin(0x62);
  pinMode(sineledPin, OUTPUT);
  pinMode(triangleledPin, OUTPUT);
  pinMode(squareledPin, OUTPUT);
  pinMode(randomledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
}


void loop() {
  int potValue = analogRead(potPin);
  int lfoRate = map(potValue, 0, 1023, 5, 3000); // Map pot value to adjust LFO rate (slower max rate)
  int buttonState = digitalRead(buttonPin);

  if (buttonState == LOW) {
    modeSelector = (modeSelector + 1) % 4;
    delay(200);
  }

    // Turn off sineled when not in Sine wave mode
  if (modeSelector != 0) {
    analogWrite(sineledPin, 0);
  }

    if (modeSelector != 3) {
    analogWrite(randomledPin, 0);
  }

  switch (modeSelector) {
    case 0:
      // Sine wave
      for (int i = 0; i < 512; i++) {
        dac.setVoltage(sineWaveTable[i], false);
        int mappedValue = map(sineWaveTable[i], 0, 4095, 0, 50);
        analogWrite(sineledPin, mappedValue); // Adjusted amplitude control for 10k pot
        delayMicroseconds(lfoRate);  // Use delayMicroseconds within the loop
      }
      break;

    case 1:
      // Triangle wave
        for (int i = 0; i < 512; i++) {
          dac.setVoltage(triangleWaveTable[i], false);
          int mappedValue = map(triangleWaveTable[i], 0, 4095, 0, 50);
          analogWrite(triangleledPin, mappedValue);
          delayMicroseconds(lfoRate);  // Use delayMicroseconds within the loop
        }
      break;

    case 2:
      // Square wave
        for (int i = 0; i < 512; i++) {
          if (i < 256) {
            dac.setVoltage(4095, false);
            analogWrite(squareledPin, 50);
          } else {
            dac.setVoltage(0, false);
            analogWrite(squareledPin, 0);
          }
          delayMicroseconds(lfoRate);  // Use delayMicroseconds within the loop
        }
      break;

    case 3:
      // Random voltage
      {
        int randomNumber = random(4095);
        dac.setVoltage(randomNumber, false);
        int mappedValue = map(randomNumber, 0, 4095, 0, 50);
        analogWrite(randomledPin, mappedValue);
        delay(lfoRate);  // Use delay with millis within the loop
      }
      break;

    default:
      // Reset modeSelector to 0 if it goes beyond 3
      modeSelector = 0;
      break;
  }
}
