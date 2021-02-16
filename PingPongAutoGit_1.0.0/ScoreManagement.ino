

///////////////////////
//  Score Managment  //
///////////////////////


// Increase point and server count
void AddPlayerOne() {
	if (pOneScore < 21) {
		pOneScore++;
		//server++;
	}
	resetVolleyFlags();
	updateScore();
}
void SubPlayerOne() {
	if (pOneScore > 0) {
		pOneScore--;
		//server--;

	}
	resetVolleyFlags();
	updateScore();
}
void AddPlayerTwo() {
	if (pTwoScore < 21) {
		pTwoScore++;
		//server++;
	}
	resetVolleyFlags();
	updateScore();
}
void SubPlayerTwo() {
	if (pTwoScore > 0) {
		pTwoScore--;
		//server--;
	}
	resetVolleyFlags();
	updateScore();
}


//	Updates the current score and determines the current server and if anyone win
void updateScore() {
	if (SpeedMode) SpeedModeTime = millis();

	if (!duceMode) {
	/// Displays current score
	printByte_Score(digit[pOneScore], digit[pTwoScore]);

	// Check if anyone win
	
		if (((pOneScore == 21 || pTwoScore == 21) && !TwoServeEachVal) || ((pOneScore == 11 || pTwoScore == 11) && TwoServeEachVal)) {
			//playWin();
			tuneplayed = true;
			delay(1220);
			displayNull();
			delay(220);
		}



		// Anaylize if server switchies by adding two scores, divide by 5. if its whole number, switch server. divide by 2 to see if server is even or odd. that is who its serving.
		
		int whole = pOneScore + pTwoScore;
		if (!TwoServeEachVal) whole = whole / 5; // Int div int = whole number only.
		//int rem = totScore % 5; // Mod by 5 is remainder. 
		
		else whole = whole / 2; // for 2 serves each mode

		whole = whole % 2; // Check if the remaider div by 2 for even odd.

		if (whole == 0) {
			currentServer = 1;

			if (LeftServeFirst) currentServer = 0;

		}
		else {
			currentServer = 0;

			if (LeftServeFirst) currentServer = 1;
		}


		//// Shows current server by comparing values since 5 serves each
		//if ((!TwoServeEachVal && server == 20) || (TwoServeEachVal && server == 14)) { //On duece
		//	server = 10;
		//}
		//else if ((!TwoServeEachVal && server > 14) || (TwoServeEachVal && server > 11) || (!TwoServeEachVal && server < 10) || (TwoServeEachVal && server < 10)) {
		//	currentServer = 1;
		//	//we used to set an led here to siginify whose serving but now we have a flag to flash the display
		//	if (!serveBeep) {
		//		BeepPoint(400);
		//		serveBeep = !serveBeep;
		//	}
		//}
		//else if ((!TwoServeEachVal && server < 15) || (TwoServeEachVal && server < 12)) {
		//	currentServer = 0;
		//	if (serveBeep) {
		//		BeepPoint(400);

		//		serveBeep = !serveBeep;
		//	}
		//}
	}

}


// Reset game variables
void reset() {
	if ((digitalRead(buttonPin0) == LOW) && (digitalRead(buttonPin1) == LOW)) TwoServeEachVal = !TwoServeEachVal;
	pOneScore = 0;
	pTwoScore = 0;
	ducePoint = 2;
	duceMode = false;
	startgame = false;
	tuneplayed = false;

	displayPingpong();
	mario();

	/*if (TwoServeEachVal) {
	printByte_Info(0, n11);
	printByte_Info(1, PtsMode);
	delay(2000);
	}*/
	printByte_Score(n00, n00);

	volleyActive = false;
	volleyServe = true;
	netFlag = false;
	firstGameServe = true;
	flashActive = false;
	tempBounceCount = 0;
	totalBounces = 0;
	trackBall = 0;
	currentServer = 0;

}

void resetNetFlags() {
	resetVolleyFlags();
	//netFlag = false;
	//firstGameServe = false;
	//trackBall = currentServer;
	//tempBounceCount = totalBounces;
	updateScore();
}
void resetVolleyFlags() {
	volleyActive = false;
	volleyServe = true;
	netFlag = false;
	BeepPoint();
}

void BeepPoint() {
	analogWrite(SpeakerPin, 200);
	delay(200);
	digitalWrite(SpeakerPin, LOW);
}
void BeepPoint(int i) {
	analogWrite(SpeakerPin, 150);
	delay(i);
	digitalWrite(SpeakerPin, LOW);
}
