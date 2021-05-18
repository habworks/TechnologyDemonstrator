/*
 * COMPANY: IMR Engineering
 * PROJECT: Technology Demonstrator
 * ENGINEER: Hab Collector
 * VER: 1.0
 * 
 * IDE: Arduino 1.8.13
 * 
 * DESCRIPTION:
 * This is code - written as an Arduino Sketch to support the ESP8266 Wifi Module on the Tech Demo REV 2 Hardware
 * The code has push buttons to turn on and off the RED / GREEN LEDs of LED2 on the hardware - .  It also 
 * accepts inputs from the hardware.  These inputs are displayed as "UI UPDATE 1" and "UI UPDATE 2".  All
 * interface between the Tech Demo Hardware occur via the Serial Tx and Rx pins of the ESP8266 and UART 2
 * of the Hardware.  The simple serial porotocol is explained below.  
 * 
 * NOTES:
 * The website is stored as a String var.  Begins with <!DOCTYPE html> and ends with </html>
 * To program the ESP8266 installed with the hardware install: JA, JB, JC and program via J11 and the Arduino IDE
 * To operate within the Tech Demo hardware Remove JA, JB, JC - note board must be reset after programming
 */

#include <ESP8266WiFi.h>
// VERY SIMPLE MSG PROTOCOL: Allways FourBytes, First Byte always Header Byte, Last Byte always simple sum
#define HEADER_BYTE           0xF0
#define HEADER_INDEX          0
#define DATA_SELECT_INDEX_1   1       
#define DATA_VALUE_INDEX_2    2
#define SIMPLE_CHECKSUM_INDEX 3
// FOR LED STATE TRANSMISSION
#define RED_LED               1     // For use in Data Select Index 1
#define GREEN_LED             2
#define LED_ON                1     // For use in Data Value Index 2
#define LED_OFF               0
// FOR UI UPDATE RECEPTION
#define UI_UPDATE_0           1     // For UI channel Select Index
#define UI_UPDATE_1           2


WiFiServer server(80);
const char WiFiPassword[] = "Connext123";
const char AP_NameChar[] = "IMR_TechDemo";
uint8_t OutputMsgBuffer[4] = {0};
uint8_t InputMsgBuffer[4] = {0};
uint8_t Update_UI_Value1 = 0;
uint8_t Update_UI_Value2 = 0;
String TmpString1 = "";
String TmpString2 = "";
String request = "";
String header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
String WebSite = R"=====(
<!DOCTYPE html>
<html>

<head>
    <meta http-equiv="refresh" content="2">
    <meta name='viewport' content='width=device-width, initial-scale=1.0' />
    <meta charset='utf-8'>

    <style>
        formg {
            /* position: static; */
            position: relative;
            bottom: 57px;
            left: 140px;
            text-align: left;
        }

        .buttonRed {
            width: 100px;
            background-color: #ff0000;
            /* Red */
            border: none;
            color: white;
            padding: 15px 15px;
            text-align: center;
            text-decoration: none;
            display: inline-block;
            font-size: 16px;
            font: Iceland;
            margin: 4px 2px;
            cursor: pointer;
        }

        .buttonGreen {
            width: 100px;
            background-color: #00ff00;
            /* Green */
            border: none;
            color: white;
            padding: 15px 15px;
            text-align: center;
            text-decoration: none;
            display: inline-block;
            font-size: 16px;
            font: Consolas;
            margin: 4px 2px;
            cursor: pointer;
        }

        .buttonBlack {
            background-color: #555555;
        }

        /* Black */
    </style>

    <title>Auto Update Example Using Javascript</title>
</head>



<body>

    <!-- <div class=WordSection1> -->

    <body lang=EN-US style='word-wrap:break-word'>

        <p class=MsoNormal style='margin-bottom:0in'><b><span style='font-size:48.0pt;
line-height:107%;font-family:"Arial",sans-serif;color:#92D050'>IMR</span></b><b><span
                    style='font-size:48.0pt;line-height:107%;font-family:"Arial",sans-serif'> </span></b><span
                style='font-size:48.0pt;line-height:107%;font-family:"Arial",sans-serif'>Engineering</span>

            <br class=MsoNormal style='margin-bottom:0in'><span style='font-family:"Arial",sans-serif'>Ideas
                Made Real</span> </br>

            <!-- <p class=MsoNormal style='margin-bottom:0in'><span style='font-family:"Arial",sans-serif'>&nbsp;</span></p> -->

        <p class=MsoNormal style='margin-bottom:0in'><span style='font-family:"Arial",sans-serif'>&nbsp;</span></p>

        <p class=MsoNormal style='margin-bottom:0in'><b><span style='font-size:28.0pt;
line-height:107%;font-family:Iceland;color:red'>Technology Demonstrator</span></b>

            <br class=MsoNormal style='margin-bottom:0in'><span style='font-size:12.0pt;
line-height:107%;font-family:"Arial",sans-serif;color:red'>WiFi Access Point</span>

            <br class=MsoNormal style='margin-bottom:0in'><span style='font-size:12.0pt;
line-height:107%;font-family:"Arial",sans-serif;color:red'>SOFT Ver:     1.0</span>

            <br class=MsoNormal style='margin-bottom:0in'><span style='font-size:12.0pt;
line-height:107%;font-family:"Arial",sans-serif;color:red'>Engineer:       Hab
                Collector</span>
        </p>

        <p class=MsoNormal style='margin-bottom:0in'><b><span style='font-size:22.0pt;
line-height:107%;font-family:Iceland;color:#0070C0'>&nbsp;</span></b></p>

        <p class=MsoNormal style='margin-bottom:0in'><b><span style='font-size:18.0pt;
line-height:107%;font-family:"Arial",sans-serif;color:#0070C0'>USER BOARD
                    CONTROLS</span></b>

            <br class=MsoNormal style='margin-bottom:0in'><span style='font-family:"Arial",sans-serif'>UI
                UPDATED 1:     %UI_Value1%</span>

            <br class=MsoNormal style='margin-bottom:0in'><span style='font-family:"Arial",sans-serif'>UI
                UPDATED 2:     %UI_Value2%</span>
        </p>

        <p class=MsoNormal style='margin-bottom:0in'><span style='font-family:"Arial",sans-serif'>&nbsp;</span></p>

        <!--<p class=MsoNormal style='margin-bottom:0in'><span style='font-family:"Arial",sans-serif'>&nbsp;</span></p> -->

        <p class=MsoNormal style='margin-bottom:0in'><b><span style='font-size:18.0pt;
line-height:107%;font-family:"Arial",sans-serif;color:#0070C0'>USER WEB CONTROLS</span></b></p>



        <div id='main'>

            <form id='F1' action='LEDON1'>
                <input class='buttonRed' type='submit' value='LED ON '>
            </form>

            <form id='F2' action='LEDOFF1'>
                <input class='buttonRed' type='submit' value='LED OFF'>
            </form>

            <form id='F3' action='LEDON2'>
                <input class='buttonGreen' type='submit' value='LED ON '>
            </form>

            <form id='F4' action='LEDOFF2'>
                <input class='buttonGreen' type='submit' value='LED OFF '>
            </form>

        </div>

    </body>

</html>
)=====";



void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  Serial.println();

  boolean conn = WiFi.softAP(AP_NameChar, WiFiPassword);
  server.begin();

} // void setup()

void loop()
{

  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client)
  {
    return;
  }

  // Create a response to button request based on user pressed
  request = client.readStringUntil('\r');
  OutputMsgBuffer[HEADER_INDEX] = HEADER_BYTE;
  if (request.indexOf("LEDON1") > 0)
  {
    OutputMsgBuffer[DATA_SELECT_INDEX_1] = RED_LED;
    OutputMsgBuffer[DATA_VALUE_INDEX_2] = LED_ON;
    calculateSimpleCheckSum(OutputMsgBuffer, sizeof(OutputMsgBuffer));
    Serial.write(OutputMsgBuffer, sizeof(OutputMsgBuffer));
  }
  else if (request.indexOf("LEDOFF1") > 0)
  {
    OutputMsgBuffer[DATA_SELECT_INDEX_1] = RED_LED;
    OutputMsgBuffer[DATA_VALUE_INDEX_2] = LED_OFF;
    calculateSimpleCheckSum(OutputMsgBuffer, sizeof(OutputMsgBuffer));
    Serial.write(OutputMsgBuffer, sizeof(OutputMsgBuffer));
  }
  else if (request.indexOf("LEDON2") > 0)
  {
    OutputMsgBuffer[DATA_SELECT_INDEX_1] = GREEN_LED;
    OutputMsgBuffer[DATA_VALUE_INDEX_2] = LED_ON;
    calculateSimpleCheckSum(OutputMsgBuffer, sizeof(OutputMsgBuffer));
    Serial.write(OutputMsgBuffer, sizeof(OutputMsgBuffer));
  }
  else if (request.indexOf("LEDOFF2") > 0)
  {
    OutputMsgBuffer[DATA_SELECT_INDEX_1] = GREEN_LED;
    OutputMsgBuffer[DATA_VALUE_INDEX_2] = LED_OFF;
    calculateSimpleCheckSum(OutputMsgBuffer, sizeof(OutputMsgBuffer));
    Serial.write(OutputMsgBuffer, sizeof(OutputMsgBuffer));
  }

  // Check for Serial Data from TD Hardware
  inputSerialParser();
  
  // Create web page with info and display
  client.flush();
  client.print(header);
  TmpString1 = WebSite;
  TmpString1.replace("%UI_Value1%", String(Update_UI_Value1));
  TmpString2 = TmpString1;
  TmpString2.replace("%UI_Value2%", String(Update_UI_Value2));
  client.print(TmpString2);

  // Flash the ESP8266 oN board LED
  digitalWrite(LED_BUILTIN, LOW);
  delay(250);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(250);

  // The client will actually be disconnected when the function returns and 'client' object is detroyed

} // void loop()




void calculateSimpleCheckSum(uint8_t *Buffer, uint8_t Size)
{
  uint8_t SimpleSum = 0;
  for (uint8_t Index = 0; Index < Size - 1; Index++)
  {
    SimpleSum = SimpleSum + Buffer[Index];
  }
  Buffer[Size - 1] = SimpleSum;
}


void inputSerialParser(void)
{
  uint8_t InByte;
  static uint8_t State = 0;

  while (Serial.available() > 0)
  {
    InByte = Serial.read();
    switch(State)
    {
      case 0:
      {
        if (InByte == HEADER_BYTE)
        {
          InputMsgBuffer[HEADER_INDEX] = HEADER_BYTE;
          State++;
        }
        break;
      }

      case 1:
      {
        if ((InByte == 1) || (InByte == 2))
        {
          InputMsgBuffer[DATA_SELECT_INDEX_1] = InByte;
          State++;
        }
        else
        {
          State = 0;
        }
        break;
      }

      case 2:
      {
        InputMsgBuffer[DATA_VALUE_INDEX_2] = InByte;
        State++;
        break;
      }

      case 3:
      {
        calculateSimpleCheckSum(InputMsgBuffer, sizeof(InputMsgBuffer));
        if (InputMsgBuffer[SIMPLE_CHECKSUM_INDEX] == InByte)
        {
          if (InputMsgBuffer[DATA_SELECT_INDEX_1] == 1)
            Update_UI_Value1 = InputMsgBuffer[DATA_VALUE_INDEX_2];
          if (InputMsgBuffer[DATA_SELECT_INDEX_1] == 2)
            Update_UI_Value2 = InputMsgBuffer[DATA_VALUE_INDEX_2];
        }
        State = 0;
        break;
      }

      default:
      {
        State = 0;
        break;
      }
    } // END OF SWITCH
  } // END OF CHECK FOR SERIAL DATA
}