


/////////////////////////////////////
///////////Button Script/////////////
////////////////Epic/////////////////
/////////////////////////////////////


//=================================================
//  MULTI-CLICK:  Many Buttons, Multiple Events

// Button timing variables
int debounce = 20;          // ms debounce period to prevent flickering when pressing or releasing the button
int DCgap = 350;            // max ms between clicks for a double click event
int holdTime = 1000;        // ms hold period: how long to wait for press+hold event
int longHoldTime = 1200;    // ms long hold period: how long to wait for press+hold event

							// Button variables
boolean buttonVal[] = { HIGH , HIGH, HIGH };   // value read from button
boolean buttonLast[] = { HIGH , HIGH, HIGH };  // buffered value of the button's previous state
boolean DCwaiting[] = { false, false, false };  // whether we're waiting for a double click (down)
boolean longHoldEventPast[] = { false, false,false }; // whether or not the long hold event happened already
boolean DConUp[] = { false, false,false };      // whether to register a double click on next release, or whether to wait and click
boolean singleOK[] = { true, true,true }; ;    // whether it's OK to do a single click
long downTime[] = { -1,-1,-1 };         // time the button was pressed down
long upTime[] = { -1,-1,-1 };           // time the button was released
boolean ignoreUp[] = { false,false,false };   // whether to ignore the button release because the click+hold was triggered
boolean waitForUp[] = { false, false,false }; // when held, whether to wait for the up event
boolean holdEventPast[] = { false, false,false };     // whether or not the hold event happened already

													  ///////////////////////////////////////
													  //		This functions allows multiple buttons to be single clicked, double clicked, and click-and-hold.
													  //		Originally, it was duplicated per button, but by setting each variable to an array, i enabled it to be used for multiple buttons.
													  //
													  //		i: position in array
													  //		
													  ///////////////////////////////////////
int checkButton(int i) {
	buttonVal[i] = digitalRead(buttonPin[i]);
	//digitalWrite(ledPin, buttonVal[i]);
	int event = 0;
	// Button pressed down
	if (buttonVal[i] == LOW && buttonLast[i] == HIGH && (millis() - upTime[i]) > debounce)
	{
		downTime[i] = millis();
		ignoreUp[i] = false;
		waitForUp[i] = false;
		singleOK[i] = true;
		holdEventPast[i] = false;
		longHoldEventPast[i] = false;
		if ((millis() - upTime[i]) < DCgap && DConUp[i] == false && DCwaiting[i] == true)  DConUp[i] = true;
		else  DConUp[i] = false;
		DCwaiting[i] = false;
	}
	// Button released
	else if (buttonVal[i] == HIGH && buttonLast[i] == LOW && (millis() - downTime[i]) > debounce)
	{
		if (!ignoreUp[i])
		{
			upTime[i] = millis();
			if (DConUp[i] == false) DCwaiting[i] = true;
			else
			{
				event = 2;
				DConUp[i] = false;
				DCwaiting[i] = false;
				singleOK[i] = false;
			}
		}
	}
	// Test for normal click event: DCgap expired
	if (buttonVal[i] == HIGH && (millis() - upTime[i]) >= DCgap && DCwaiting[i] == true && DConUp[i] == false && singleOK[i] == true && event != 2)
	{
		event = 1;
		DCwaiting[i] = false;
	}
	// Test for hold
	if (buttonVal[i] == LOW && (millis() - downTime[i]) >= holdTime) {
		// Trigger "normal" hold
		if (!holdEventPast[i])
		{
			event = 3;
			waitForUp[i] = true;
			ignoreUp[i] = true;
			DConUp[i] = false;
			DCwaiting[i] = false;
			//downTime = millis();
			holdEventPast[i] = true;
		}
		// Trigger "long" hold
		if ((millis() - downTime[i]) >= longHoldTime)
		{
			if (!longHoldEventPast[i])
			{
				event = 4;
				longHoldEventPast[i] = true;
			}
		}
	}
	buttonLast[i] = buttonVal[i];
	return event;
}

