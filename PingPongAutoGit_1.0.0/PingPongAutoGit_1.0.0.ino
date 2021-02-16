 /*
Ping Pong Scoreboard - Extreme
UIC Senior Design
created by raphael codrean
credit to the multi-click button code guy

Timeline for updating to automatic sensors
3-24-17 replace duel display with led matrix
4-8-17 add debug info
and idk after that. look at the github
8-9-17 finally synced this to github

maybe disabe 3 taps during speed mode so it doesnt boucing balls after point dont accidentally start


*/
#define D false
#define DEBUG false
#define DEBUG_Flash false
#define D_Noise false
#define D_bounce false
#define D_Net false
#define D_Miss false
#define brightness 4
int SpeakerPin = 5;
#define NOTE_GS4 415
#define NOTE_AS4 466
#define NOTE_C5  523
#define NOTE_G5  784
#define NOTE_A5  880
#define NOTE_E6  1319
#define NOTE_G6  1568


//button pin should be connected to LOW

int buttonPin0 = 4; // Left Player //old ping pong used analog pins as digital.
int buttonPin1 = 2;  // Right Player
int buttonPin2 = 3;  // Menu/Trigger button
int buttonPin[3] = { buttonPin0,  buttonPin1,  buttonPin2 };




//initial settings
boolean serveBeep = false;  // Beeps on serve switch
int currentServer = 0;		// Left player = 0 ; Right Player = 1
boolean tuneplayed = false; // Whether the tune was played so it wont repeat
boolean TwoServeEachVal = false;  //Whether the game is 2 serves each, up to 11 mode
boolean startgame = false; // Records whether game is active or not.
boolean duceMode = false;  // Whether game is in duece

int ducePoint = 2;
int pOneScore = 0;
int pTwoScore = 0;
//idk why i did this server 10 thing the first time, but i guess it works.
int server = 10; // x < 5 player one servers.  x > 4 player two server reset at 9 or 10 i guess.
//now ten is default. so less than 15 is player one, and more than 14 is player 2. reset when hits 20 back to 10


//newstuff
int initPiezo[3] = { 0,0,0 }; //value to calibrate sensor to
int sensorLeft = A0;
int sensorRight = A1;
int sensorNet = A2;
int sensorValue[3] = { 0,0,0 };        // value read from the piezo
int sensorPin[3] = { sensorLeft, sensorRight, sensorNet };
boolean volleyActive = false; // Whether actively recording the volley
boolean volleyServe = true;  // First server of a volley
boolean netFlag = false;    // Whether net sensor was triggered during first serve
boolean firstGameServe = true;  // First serve of the game
boolean LeftServeFirst = false; //To manage who current server is
int flashActive = 0; //if flashing script should be on
long flashTimer = 0;
int flashDelay = 300;
int flashDelayBounce = 1000;
int trackBall = 0; // Value to track which side the ball is on
int tapCount = 0;  // Number user tapped the table to initiate serve
int tapEvent = 0;  // Value of event for the tap.
int totalBounces = 0; //Total count of bounces during the game for stats
int tempBounceCount = 0;  //Stores totalBounces at first serve so we can evalute net sensor.
int samples = 300; //how many samples to smooth out signal from power noise`
int NetMax = 2; //When sensor goes past this value, indicates net was triggered // default 1 on my net
//int NetMin = -2;
int BallMin = 15 ; // default 5
int BallMax = 1300;  //dont really need a max, but ill leave it since this code is a duplicate of tapDector()
int TapMin = 5;  //default 5
int TapMax = 150;    // should be ~120. but now woith 300 samples, typical reading is ~2,3,7, max 60
int ballDebounce = 80;          // ms debounce period to prevent flickering when pressing or releasing the button
long ballHangTime = 0; // The time when the ball last hit the table
int ballHangDelay = 2800; //Time for ball to stay in air
boolean SpeedMode = true; // Speed mode flag. Auto Starts serve in few seconds
long SpeedModeTime = 0; // The time when the ball last hit the table
int SpeedModeDelay = 3000; //Time for ball to stay in air
int pointCoolDown = 2000; // Time to wait before starting begining to anaylze any future bounces. In case it repeatedly bounces on table preventing start of auto serve
// or perhaps a better way without causing delays everywhere is to put a check on tapCheck or bounce detect to not work untill time is longer than delay time + time point was scored. then no halting the processor
//some day make spike counts

///////////////////////////////////////////////////
//////////////////////////////////////////////////

#include <LedControlMS.h>
byte* digit[22];  // Digits 00-21

byte n00[8] = { 0x00,0xE7,0xA5,0xA5,0xA5,0xE7,0x00,0x00 };
byte n01[8] = { 0x00,0xE2,0xA6,0xA2,0xA2,0xE7,0x00,0x00 };
byte n02[8] = { 0x00,0xE7,0xA1,0xA7,0xA4,0xE7,0x00,0x00 };
byte n03[8] = { 0x00,0xE7,0xA1,0xA7,0xA1,0xE7,0x00,0x00 };
byte n04[8] = { 0x00,0xE5,0xA5,0xA7,0xA1,0xE1,0x00,0x00 };
byte n05[8] = { 0x00,0xE7,0xA4,0xA7,0xA1,0xE7,0x00,0x00 };
byte n06[8] = { 0x00,0xE7,0xA4,0xA7,0xA5,0xE7,0x00,0x00 };
byte n07[8] = { 0x00,0xE7,0xA1,0xA1,0xA1,0xE1,0x00,0x00 };
byte n08[8] = { 0x00,0xE7,0xA5,0xA7,0xA5,0xE7,0x00,0x00 };
byte n09[8] = { 0x00,0xE7,0xA5,0xA7,0xA1,0xE7,0x00,0x00 };
byte n10[8] = { 0x00,0x47,0xC5,0x45,0x45,0xE7,0x00,0x00 };
byte n11[8] = { 0x00,0x42,0xC6,0x42,0x42,0xE7,0x00,0x00 };
byte n12[8] = { 0x00,0x47,0xC1,0x47,0x44,0xE7,0x00,0x00 };
byte n13[8] = { 0x00,0x47,0xC1,0x47,0x41,0xE7,0x00,0x00 };
byte n14[8] = { 0x00,0x45,0xC5,0x47,0x41,0xE1,0x00,0x00 };
byte n15[8] = { 0x00,0x47,0xC4,0x47,0x41,0xE7,0x00,0x00 };
byte n16[8] = { 0x00,0x47,0xC4,0x47,0x45,0xE7,0x00,0x00 };
byte n17[8] = { 0x00,0x47,0xC1,0x41,0x41,0xE1,0x00,0x00 };
byte n18[8] = { 0x00,0x47,0xC5,0x47,0x45,0xE7,0x00,0x00 };
byte n19[8] = { 0x00,0x47,0xC5,0x47,0x41,0xE7,0x00,0x00 };
byte n20[8] = { 0x00,0xE7,0x25,0xE5,0x85,0xE7,0x00,0x00 };
byte n21[8] = { 0x00,0xE2,0x26,0xE2,0x82,0xE7,0x00,0x00 };
//byte Deuce1[8] = { 0x00,0xCE,0xA8,0xAE,0xA8,0xCE,0x00,0x00 }; // D E
//byte Deuce2[8] = { 0xAE,0xA8,0xEE,0x00,0xE7,0xE4,0x82,0xEE }; // U C E S
byte Deuce1[8] = { 0x00,0xCE,0xA8,0xAC,0xA8,0xCE,0x00,0xAE };
byte Deuce2[8] = { 0xA8,0xEE,0x00,0xE7,0xE4,0x82,0xEE,0x00 };
byte DeuceD[8] = { 0x00,0x38,0x24,0x24,0x24,0x24,0x38,0x00 };

byte Adv[8] = { 0x00,0x18,0x18,0x7E,0x7E,0x18,0x18,0x00 }; // plus symbol for advantage, blank for other player
byte Fill[8] = { 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF };
byte Blank[8] = { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };
byte Dash[8] = { 0x00,0x00,0x00,0xE7,0xE7,0x00,0x00,0x00 };
////////////////
byte Net1[8] = { 0x88,0xC8,0xE8,0xA8,0xB8,0x98,0x00,0x0F };
byte Net2[8] = { 0x08,0x0E,0x08,0x0F,0xE0,0x40,0x40,0x40 };
byte Pi[8] = { 0xE0,0xA0,0xE0,0x8E,0x84,0x04,0x0E,0x00 };
byte Po[8] = { 0xE0,0xA0,0xE0,0x8E,0x8A,0x0A,0x0E,0x00 };
byte ng[8] = { 0x00,0xE0,0xA0,0xAE,0x0A,0x0E,0x02,0x0E };
byte TB[8] = { 0x00,0xEC,0x4A,0x4C,0x4A,0x4C,0x00,0x00 }; // Total Bounce
byte FS[8] = { 0x00,0xE6,0x88,0xEC,0x82,0x8E,0x00,0x00 }; //Final Score
byte Speed1[8] = { 0x70,0x87,0x65,0x17,0xE4,0x04,0xE4,0x80 }; //Speed mode
byte Speed2[8] = { 0xCE,0x88,0xEC,0x08,0xCE,0xA0,0xA0,0xC0 };


byte PtsMode[8] = { 0x00,0x00,0xE7,0xA2,0xE2,0x82,0x82,0x00 };
byte Star[8] = { 0x18,0x18,0x3C,0xFF,0x3C,0x3C,0x66,0x81 };
byte Smile[8] = { 0x00,0x24,0x24,0x00,0x00,0x42,0x3C,0x00 };  //Smile no circle face
byte smile[8] = { 0x3C,0x42,0xA5,0x81,0xA5,0x99,0x42,0x3C };  // Smiley face with circle
///////////////
byte* AnyDigit[10];  // Array for display any digit as 3 bytes 0-9

byte ad0[3] = { 0x7C,0x44,0x7C };
byte ad1[3] = { 0x24,0x7C,0x04 };
byte ad2[3] = { 0x5C,0x54,0x74 };
byte ad3[3] = { 0x54,0x54,0x7c };
byte ad4[3] = { 0x70,0x10,0x7c };
byte ad5[3] = { 0x74,0x54,0x5c };
byte ad6[3] = { 0x7c,0x54,0x5c };
byte ad7[3] = { 0x40,0x40,0x7c };
byte ad8[3] = { 0x7c,0x54,0x7c };
byte ad9[3] = { 0x74,0x54,0x7c };





//LedControl(dataPin,clockPin,csPin,numDevices)
//and create the LedControl 
LedControl lc = LedControl(12, 10, 11, 4);



///////////////////////////////////////////
///////////////////////////////////////////
void setup() {
#if D
	Serial.begin(115200);
#endif
	//Serial.begin(115200);
	//wake up the MAX72XX from power-saving mode 
	lc.shutdown(0, false);
	lc.shutdown(1, false);
	lc.shutdown(2, false);
	lc.shutdown(3, false);
	//set a medium brightness for the Leds
	lc.setIntensity(0, brightness);
	lc.setIntensity(1, brightness);
	lc.setIntensity(2, brightness);
	lc.setIntensity(3, brightness);

	pinMode(SpeakerPin, OUTPUT);

	// Set button input pin

	pinMode(buttonPin0, INPUT);
	pinMode(buttonPin1, INPUT);
	pinMode(buttonPin2, INPUT);
	digitalWrite(buttonPin0, HIGH); //turn on internal pull up
	digitalWrite(buttonPin1, HIGH);
	digitalWrite(buttonPin2, HIGH);



	// Set Array to refrence


	digit[0] = n00;
	digit[1] = n01;
	digit[2] = n02;
	digit[3] = n03;
	digit[4] = n04;
	digit[5] = n05;
	digit[6] = n06;
	digit[7] = n07;
	digit[8] = n08;
	digit[9] = n09;
	digit[10] = n10;
	digit[11] = n11;
	digit[12] = n12;
	digit[13] = n13;
	digit[14] = n14;
	digit[15] = n15;
	digit[16] = n16;
	digit[17] = n17;
	digit[18] = n18;
	digit[19] = n19;
	digit[20] = n20;
	digit[21] = n21;
	////////////////
	AnyDigit[0] = ad0;
	AnyDigit[1] = ad1;
	AnyDigit[2] = ad2;
	AnyDigit[3] = ad3;
	AnyDigit[4] = ad4;
	AnyDigit[5] = ad5;
	AnyDigit[6] = ad6;
	AnyDigit[7] = ad7;
	AnyDigit[8] = ad8;
	AnyDigit[9] = ad9;


	////////////////
	printByte_Info(0, Pi); // turn on led to create noise on signal before we calibrate
	printByte_Info(1, ng); // turn on led to create noise on signal before we calibrate

	delay(1000);  //delay to allow time for sensors to settle before calibration

	calib(0, samples);
	calib(1, samples);
	calib(2, 100);


	reset();
	

}



void loop() {
	int a = checkButton(0);
	int b = checkButton(1);
	int c = checkButton(2);

#if DEBUG
	Serial.println();
	Serial.print("StartG: "); Serial.print(startgame);
	Serial.print("   Duece: "); Serial.print(duceMode);
	Serial.print("   V_Active: "); Serial.print(volleyActive);
	Serial.print("   V_Serve: "); Serial.print(volleyServe);
	Serial.print("   TrakBall: "); Serial.print(trackBall);
	Serial.print("   CServer: "); Serial.print(currentServer);
	Serial.print("   Net: "); Serial.print(netFlag);
	Serial.print("   TempBal: "); Serial.print(tempBounceCount);
	Serial.print("   FGServe: "); Serial.print(firstGameServe);
	Serial.print("   flash: "); Serial.print(flashActive);
	//Serial.print("   tapActive: "); Serial.print(tapActive);
	Serial.print("   tapCount: "); Serial.print(tapCount);
	Serial.println();
#endif

	// Get button event and act accordingly
	if ((a == 4) || (b == 4)) {
		reset();
	}
	if (c == 4) {
		SpeedMode = !SpeedMode;
		BeepPoint(300);
		delay(100);
		BeepPoint(150);
		delay(100);
		BeepPoint(300);
	}
	//tune played indicates that the game is over and we can show stats
	if (tuneplayed) {
		
		flashTotalBounce();

	
	}


	//if game is off, the first button push decides who serves first
	else if (!startgame ) {  
		if (a == 1) {
			server = 10;
			startgame = true;
			serveBeep = true;
			firstGameServe = false;
			LeftServeFirst = true;
			currentServer = 0;
		}
		if (b == 1) {
			if (!TwoServeEachVal) { server = 15; }
			//sets to 5 to declare that player 2 is first.
			else if (TwoServeEachVal) { server = 12; }
			startgame = true;
			serveBeep = false;
			firstGameServe = false;
			LeftServeFirst = false;

			currentServer = 1;
		}
		// feature nmot really working. lwets just leave it
		/*if (c == 1)
		  {
			 resetVolleyFlags(); // wouldnt hurt to put it here too
			 volleyActive = true;
		   }*/

		else if (TwoServeEachVal) {

			printByte_Info(0, n11);
			printByte_Info(1, PtsMode);
		}
		else if (SpeedMode) {
			printByte_Info(0, Speed1);
			printByte_Info(1, Speed2);
		}
	}
	else {
		//assuming the game is on, checks if theres a duce, otherwise just adds points
		if (!duceMode) {
			if (a == 1) {
				BeepPoint();
				AddPlayerOne();
				
			}
			if (a == 2) SubPlayerOne();
			if (b == 1) {
				BeepPoint();
				AddPlayerTwo();
				
			}
			if (b == 2) SubPlayerTwo();
			if (c == 1)
			{
				resetVolleyFlags(); // wouldnt hurt to put it here too
				volleyActive = true;
				tapCount = 0;


				// Stop flashing. see note in flashServerScore()
				if (currentServer == 0)
				{
					trackBall = 0;

					if (!duceMode) printByte_Score(currentServer, digit[pOneScore]);
				}
				else {
					trackBall = 1;
					if (!duceMode) printByte_Score(currentServer, digit[pTwoScore]);

				}
				delay(500); //delay just to prevent some debouncing? might be helpful

			}

			if (a == 0 && b == 0 && c == 0) { //so only when everything is 0. can probably make it an else { instead
				AutoGame();
			}


		}

		//////////////duce part//////////////////
		if (((pOneScore == 20 && pTwoScore == 20 && !TwoServeEachVal) || (pOneScore == 10 && pTwoScore == 10 && TwoServeEachVal)) && (duceMode == false)) {
			delay(1000); //so a visible 20-20 can be shown then onto duce.
			displayDuce();
			//currentServer = !currentServer; // Flip server, unless we dont need to because it flips at update score?
			delay(1500); //display duece for a few seconds
			duceMode = true;

		}

		//now it understands its in duce, and the script will come here first instead of the other script because duce is no longer false.
		else if (duceMode == true) {
			if (a == 1) {  // advantages are done thru comparasion. more than 2 for player on and less than 2 for two. 2 for tied.
				ducePoint++;  //maybe can be changed to true and false because that probably takes less memory.
				currentServer = !currentServer;
			}  //something like player1adv = true, and then if p1avd is true and a = 1 then he wins.
			if (b == 1) {
				ducePoint--;
				currentServer = !currentServer;
			}
			if (c == 1)
			{
				resetVolleyFlags(); // wouldnt hurt to put it here too
				volleyActive = true;
				tapCount = 0;


				// Stop flashing. see note in flashServerScore()
				if (currentServer == 0)
				{
					printByte_Score(Blank, Blank);
					trackBall = 0;
				}
				else {
					printByte_Score(Blank, Blank);
					trackBall = 1;
				}
				delay(500); //delay just to prevent some debouncing? might be helpful

			}
			if (a == 0 && b == 0 && c == 0) { //so only when everything is 0. can probably make it an else { instead
				AutoGame();
			}
			if (ducePoint == 2)        displayD();
			if (ducePoint == 3)   displayOneAdv();
			if (ducePoint == 1)   displayTwoAdv();
			else if (ducePoint > 3) {
				//if (a > 0 || b > 0) { // i dont think this is nesseary cause noob programmer
					playWin();
					onewin();
				}
			
			else if (ducePoint < 1) {
				//if (a > 0 || b > 0) {

					playWin();
					twowin();
				}
			
			
			

		} ///end duce here
	}
}





