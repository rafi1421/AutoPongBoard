
void AutoGame() {
	if (!volleyActive) {
		tempBounceCount = totalBounces;

		


		// Flash score only after selecting first server
		if (!firstGameServe && !tuneplayed) flashServerScore();

		// When you reach 3 taps, start recording volley
		if (SpeedMode) {
			if ((millis() >(SpeedModeTime + SpeedModeDelay))) {
				delay(10);

				// Stop flashing. see note in flashServerScore()
				if (currentServer == 0)
				{

					if (!duceMode) printByte_Score(digit[pOneScore], digit[pTwoScore]);
					trackBall = 0;
				}
				else {
					if (!duceMode) printByte_Score(digit[pOneScore], digit[pTwoScore]);
					trackBall = 1;
				}
				
				tapCount = 0;
				BeepPoint(150);
				volleyActive = true;
			}
		}

		if (tapCount < 3) {

			// Check number of tap and update the display with # of taps
			tapCount = tapDetector(currentServer);

			displayTapDot(); //gonna try in display part also, so it hopefully wont blink. sucess!
		}

		
		else {

			// So only after first point i enable the taps to start the game. otherwise press the c button to start game
			volleyActive = true;
			tapCount = 0;


			// Stop flashing. see note in flashServerScore()
			if (currentServer == 0)
			{
				if (!duceMode) printByte_Score(digit[pOneScore], digit[pTwoScore]);
				trackBall = 0;
			}
			else {
				if (!duceMode) printByte_Score(digit[pOneScore], digit[pTwoScore]);
				trackBall = 1;
			}
			delay(500); //delay just to prevent some debouncing? might be helpful


		}

	}
	//////////////////////////////////////
	//////  Automatic Game Logics  //////
	else {
		//next thing to do is add a timing variable for if the ball has not been sensed in a few seconds. probably valid whaen tempBounceCount +1

		// Check bounces on each table, then evalute
		bounceDetect();

		//ATM this code isnt used 
		// First serve of the game, so player that has sensor hit on thier side first will set which side has first serve
		if (firstGameServe) {

			if (trackBall > 0) {
				currentServer = 0;
				firstGameServe = false;
			}
			else if (trackBall < 0) {
				// Trackball is set to 0 on boot. So if RIGHT player serves, first, then on ball return it will be -1, 
				// which will make him loose a point automatically. so we need to immediately fix to correct value
				trackBall = 0;
				currentServer = 1;
				firstGameServe = false;

			}
		}


		// Check if ball missed the table or couldnt return ball 
		if ((totalBounces > tempBounceCount) && (millis() > (ballHangTime + ballHangDelay))) {
			// After first bounce  and current time is more than hang delay allowed
#if D_Miss 
			Serial.println("Ball missed the table!");
#endif
			if (trackBall == 1) {
				//LEFT player missed table
				// Give point to right
				resetVolleyFlags();

				if (duceMode) {
					ducePoint--;
					currentServer = !currentServer;
				}
				else AddPlayerTwo();

				delay(pointCoolDown);
			}
			if (trackBall == 0) {
				resetVolleyFlags();


				if (duceMode) {
					currentServer = !currentServer;
					ducePoint++;
				}
				else AddPlayerOne();

				delay(pointCoolDown);
			}
		}


		// Check net on server
		// VolleyServe : the first serve of a volley, when true, checks net sensor
		if (volleyServe) {
			//sensorValue[2] = analogRead(sensorPin[2]) - initPiezo[2];
			sensorValue[2] = readAverage(2, 100);  //read average of net sensor

			if (currentServer == 0 && trackBall == -1 && tempBounceCount + 1 == totalBounces) {
				// Hit wrong side of table on serve

				resetVolleyFlags();

				AddPlayerTwo();
				if (duceMode) {
					currentServer = !currentServer;
					ducePoint--;
				}
				delay(pointCoolDown);
				return;
			}
			if (currentServer == 1 && trackBall == 2 && tempBounceCount + 1 == totalBounces) {
				// Hit wrong side of table on serve

				resetVolleyFlags();
				AddPlayerOne();

				if (duceMode) {
					currentServer = !currentServer;
					ducePoint++;
				}
				delay(pointCoolDown);
				return;
			}

			if (sensorValue[2] > NetMax) { //Now its absolute value so dont need negative || (sensorValue[2] < NetMin)) {
				netFlag = true;
#if D_Net
				Serial.print("Sensor net "); Serial.print(sensorValue[2]);  Serial.print(" at time "); Serial.println(millis());
#endif

#if DEBUG
				Serial.print("NetFlag ON");
#endif
			}

			if (currentServer == trackBall && tempBounceCount + 2 == totalBounces) {
				//means that the ball bounces on opposite side of the table AND it triggered the net
				if (netFlag == true) {
#if DEBUG
					Serial.print("RESTART VOLLEY");
#endif
					printByte_Info(0, Net1);
					printByte_Info(1, Net2);

					//sensorValue[2] =0;  // since the net doesnt continuously scan, it needs to be reset to 0 manually, otyherwise the net is always on
					//server--;  //need to remove a serve. otherwise it miscalculates
					BeepPoint();
					delay(200);
					BeepPoint();
					delay(1500); // time to show net

					resetNetFlags();
					delay(pointCoolDown);
				}
				else volleyServe = false;  // After second bounce, we dont need to check for net anymore so we can bypass the whole thing.	
			} // end two bounces

		} // volley Serve




		 //Continuously track ball
		 //Left table bounces will always increase trackBall, Right table bounces decrease trackBall
		 //Seems to be that WHENEVER trackball is 2, then the RIGHT player scored a point, and when trackBall is -1, then LEFT player got a point

		
			if (trackBall == 1) {
				// set led on left
				lc.setLed(0, 0, 7, 1);
				lc.setLed(1, 7, 7, 0);
				lc.setLed(2, 7, 0, 1);
				lc.setLed(3, 0, 0, 0);

			}
			if (trackBall == 0) {
				//set led on right
				lc.setLed(0, 0, 7, 0);
				lc.setLed(1, 7, 7, 1);
				lc.setLed(2, 7, 0, 0);
				lc.setLed(3, 0, 0, 1);
			}
			if (trackBall > 1) {
				// Player 1 "LEFT"  Scored
				resetVolleyFlags();
				if (duceMode) {
					ducePoint--;
					currentServer = !currentServer;
				}
				else AddPlayerTwo();

				delay(pointCoolDown);
			}

			if (trackBall < 0) {
				//Player 2 "RIGHT" scored
				resetVolleyFlags();
				if (duceMode) {
					ducePoint++;
					currentServer = !currentServer;
				}
				else AddPlayerOne();

				delay(pointCoolDown);
			}

		}
	}

	void displayTapDot() {
		// Reflect dots to # of taps
		if (currentServer == 0) {
			lc.setRow(1, 7, tapCount);
		}
		if (currentServer == 1) {
			lc.setRow(2, 7, tapCount);
		}
	}
