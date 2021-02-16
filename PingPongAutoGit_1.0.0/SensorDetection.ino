



////////////////////////////
///   Sensor Detection   ///
////////////////////////////



///////////////////////
// For Ball bounces //
/////////////////////

// Timing variables

// Bounce variables
boolean ballActive[2] = { false, false };	// Whether activly counting taps, otherwise set event = 0.
int ballButtonLast[2] = { 0,0 };		   // Buffered value of the button's previous state
long ballDownTime[2] = { -1,-1 };         // Time the button was pressed down
										  //long ballUpTime[2] = { -1,-1 };          // Time the button was released
long ballUpTime = -1;          // Time the button was released
long ballStartTime[2] = { 0,0 };	    // Time first tap was triggered
int readAvg0 = 0;
int readAvg1 = 0;
int readAvg = 0;

// Average 5 readings of the incoming signal.
//	i: position of pin number in array
int readAverage(int i, int samp) {
	for (int x = 0; x < samp; x++)
	{
		readAvg = readAvg + analogRead(sensorPin[i]);
	}
	readAvg = abs(round(readAvg / samp - initPiezo[i]));
	return readAvg;
}
void readAverageDual() {
	for (int x = 0; x < samples; x++)
	{
		readAvg0 = readAvg0 + analogRead(sensorPin[0]);
		readAvg1 = readAvg1 + analogRead(sensorPin[1]);
	}
	readAvg0 = abs(round(readAvg0 / samples - initPiezo[0]));
	readAvg1 = abs(round(readAvg1 / samples - initPiezo[1]));

	//return readAvg;
}


void bounceDetect() {

	// read the analog in value:
	readAverageDual();
	sensorValue[0] = readAvg0;
	sensorValue[1] = readAvg1;

	//sensorValue[i] = analogRead(sensorPin[i]) - initPiezo[i];  //As of now, sensorValue overlaps with tapDector(), but these 2 functions are ran seperately, and will never overlap during operation
	int i = 0;
	// Check which sensor read higher ("meaning the ball bounces on that side more likely than the other table, cause cross contamination)
	if (sensorValue[0] > sensorValue[1]) i = 0;
	else if (sensorValue[1] > sensorValue[0]) i = 1;
	//Serial.print("Sensor 0 "); Serial.print(sensorValue[0]);  Serial.print(" at time "); Serial.println(millis());
	//Serial.print("Sensor 1 "); Serial.print(sensorValue[1]);  Serial.print(" at time "); Serial.println(millis());


	// Ball bounced down
	if ((sensorValue[i] > BallMin) && (sensorValue[i] < BallMax) && (ballButtonLast[i] <= BallMin) && (millis() - ballDownTime[i]) > ballDebounce)
	{
		// Left table bounce
		if (i == 0) {
			trackBall++;

		}

		//Right table bounce
		else if (i == 1) {
			trackBall--;
		}
		ballDownTime[i] = millis();
		if (volleyActive) {
			ballHangTime = millis();
			totalBounces++;
#if D_bounce
			Serial.print("Sensor TRACK "); Serial.print(i); Serial.print(" measured "); Serial.print(sensorValue[i]);  Serial.print(" at time "); Serial.println(millis());
#endif
		}



	}
	// Button released 
	//originally is elsif, but i change to if, so it can write ball up time, so since it read
	//technically this part doesnt even matter as it never got triggered before i changed above code, which still makes this part pointless
	else if ((sensorValue[i] <= BallMin) && (ballButtonLast[i] > BallMin) && (ballButtonLast[i] < BallMax) && (millis() - ballDownTime[i]) > ballDebounce)
	{
		ballUpTime = millis();
	}
	ballButtonLast[0] = sensorValue[0];
	ballButtonLast[1] = sensorValue[1];

}



////////////////////////////
// Triple tap detector   //
//////////////////////////

//Actually, tapping is only going to valid for 1 person at a time, so i dont need to create arrays per sensor
// Timing variables
int tapDebounce = 60;          // ms debounce period to prevent flickering when pressing or releasing the button
int TCgap = 1000;			  // max ms between clicks for a triple tap event

							  // Bounce/tap variables
boolean tapActive = false;	  // whether activly counting taps, otherwise set event = 0.
int tapButtonLast = 0;			 // buffered value of the button's previous state
long tapDownTime = -1;         // time the button was pressed down
long tapUpTime = -1;          // time the button was released
long tapStartTime = 0;		  // time first tap was triggered

							  //int tapEvent = 0;

int tapDetector(int i) {

	// read the analog in value:
	sensorValue[i] = readAverage(i, samples);
	//sensorValue[i] = analogRead(sensorPin[i]) - initPiezo[i];

#if D_Noise
	Serial.println();
	Serial.print("TapSensorNoise #-");
	Serial.print(i); Serial.print(" : ");
	Serial.println(sensorValue[i]);
#endif

	if (tapActive) {
		if (millis() > tapStartTime + TCgap) {  //if current time is more than start+time allowed for 3 taps, then reset to 0 and save new time
			tapStartTime = millis();
			tapEvent = 0;
			tapActive = false;
		}
	}

	// Button pressed down
	if ((sensorValue[i] > TapMin) && (sensorValue[i] < TapMax) && (tapButtonLast <= TapMin) && (millis() - tapUpTime) > tapDebounce)
	{
		if (tapEvent == 0) {  //for first click since power up, set start time
			tapStartTime = millis();
			tapActive = true;
		}
		if (millis() > tapStartTime + TCgap) {  //if current time is more than start+time allowed for 3 taps, then reset to 0 and save new time
			tapStartTime = millis();
			tapEvent = 0;
			tapActive = true;
		}
		tapDownTime = millis();
		tapEvent++;

#if DEBUG_Noise
		//Serial.print("now: "); Serial.print(millis());
		//Serial.print("\t uptime: "); Serial.print(UpTime);
		//Serial.print("\t Starttime: "); Serial.print(startTime);
		//Serial.print("\t Diff: "); Serial.println(millis() - startTime);

		Serial.println();
		Serial.print("SensorTap #-");
		Serial.print(i); Serial.print(" : ");
		Serial.println(sensorValue[i]);
#endif
	}
	// Button released 
	else if ((sensorValue[i] <= TapMin) && (tapButtonLast > TapMin) && (tapButtonLast < TapMax) && (millis() - tapDownTime) > tapDebounce)
	{
		tapUpTime = millis();
	}
	tapButtonLast = sensorValue[i];
	return tapEvent;
}




////////////////////////////
///  Sensor Calibration  ///
////////////////////////////

// Calibrates default sensor value to 0.
void calib(int sensor, int samp)
{
	for (int i = 0; i < samp; i++)
	{
		initPiezo[sensor] = initPiezo[sensor] + analogRead(sensorPin[sensor]);
		//delay(100);
	}
	initPiezo[sensor] = round(initPiezo[sensor] / samp);
#if DEBUG
	Serial.print("\n Calibrate sensor to = ");
	Serial.println(initPiezo[sensor]);
#endif 
}
