

//////////////////////////
/// Display Management ///
//////////////////////////


//	printByte_Score( LEFT , RIGHT )
void printByte_Score(byte playerL[], byte playerR[])
{
	int i = 0;
	for (i = 0;i < 8;i++)
	{
		lc.setRow(0, i + !duceMode, playerR[i]); //+1 to i so we can shift the number down a row. to look symetrical vertically  UPdate: changed +1 to +!duceMode so if duec is ffalse, it still adds one. if it is in duce, it adds 0. so it doesnt shift image down by 1
		lc.setRow(1, i, playerL[i]);
		lc.setRow(2, i, playerR[i]);
		lc.setRow(3, i + !duceMode, playerL[i]);
	}
	if (!tuneplayed) {
		displayTapDot();
		lc.setRow(0, 0, 0x00);
		lc.setRow(3, 0, 0x00);
	}
}

//Left Player: 0
//Right Player: 1
void printByte_Score(int player, byte character[])
{
	int i = 0;
	for (i = 0;i < 8;i++)
	{
		if (player == 0) {
			lc.setRow(1, i, character[i]);
			lc.setRow(3, i + !duceMode, character[i]);
		}
		else {
			lc.setRow(0, i + !duceMode, character[i]); //+1 to i so we can shift the number down a row. to look symetrical vertically
			lc.setRow(2, i, character[i]);
		}
	}
	if (!tuneplayed) {
		displayTapDot();
		lc.setRow(0, 0, 0x00);
		lc.setRow(3, 0, 0x00);

	}
}



//
//     Location Position
//		Top: 0 
//		Bottom: 1  
void printByte_Info(int location, byte character[])
{
	// loc = 0 = top, loc = 1 = bottom
	int i = 0;
	for (i = 0;i < 8;i++)
	{
		if (location == 0) {
			lc.setRow(0, i, character[i]);
			lc.setRow(3, i, character[i]);
		}
		else {
			lc.setRow(1, i, character[i]);
			lc.setRow(2, i, character[i]);
		}
	}
}




//
// Super Hack here to fix glitch due to no time.
// So what happens is that if a digit is repeated on the second for loop, the bytes were already shifted so it is displayed incorrectly
// To hack it, i made a left half and right half, so that way the bits are reverted to original position
// also have to do top and bottom digit at same time with them past in as parametered bytes because i get a error if i try to pass in only the bounces and math it here
// and then initiallizieng a byte to a byte doesnt work straight up, specialy things have to be done which i dont know yet.
// one method is to use a forloop to copy the bytes but i couldnt really do it is its already a pointed byte, or at least idk how to yet. so second hack is just do the maths
// at the function call as parameter. uglyer but it works.
void printByte_TotalBounce_Left(byte charTho[], byte charHun[], byte charTen[], byte charSin[])
{
	int i = 0;
	for (i = 0;i < 3;i++)
	{
		lc.setColumn(0, i, charTho[i]);
		lc.setColumn(3, i, charTho[i]);

		lc.setColumn(1, i, charTen[i]);
		lc.setColumn(2, i, charTen[i]);
	}

	for (i = 5;i < 8;i++)
	{
		lc.setColumn(0, i, charHun[i-5]);
		lc.setColumn(3, i, charHun[i-5]);

		lc.setColumn(1, i, charSin[i-5]);
		lc.setColumn(2, i, charSin[i-5]);
	}

}


//void printByte_TotalBounce(int bounces)
//{
//	int digTho = bounces / 1000 % 10;
//	int digHun = bounces/100%10;
//	int digTen = bounces / 10 % 10;
//	int digSig = bounces  % 10;
//
//	byte character[] = AnyDigit[2];
//	for (int i = 0; i < 3;i++) {
//		characters[i] = AnyDigit[digTho[i]];
//	}
//	// loc = 0 = top, loc = 1 = bottom
//	int i = 0;
//	for (i = 0;i < 3;i++)
//	{
//		if (location == 0) {
//			lc.setColumn(0, i, character[digHun]);
//			lc.setColumn(3, i, character[i]);
//		}
//		else {
//			lc.setColumn(1, i, character[i]);
//			lc.setColumn(2, i, character[i]);
//
//		}
//	}
//}





void displayPingpong() {
	//ping
	printByte_Info(0, Pi);
	printByte_Info(1, ng);
	delay(1000);
	// --
	printByte_Score(Dash, Dash);
	delay(500);
	//pong
	printByte_Info(0, Po);
	printByte_Info(1, ng);
	delay(1000);
}
void displayDuce() {
	printByte_Info(0, Deuce1);
	printByte_Info(1, Deuce2);
}
void displayD() {
	printByte_Score(DeuceD, DeuceD);
}
void displayOneAdv() {
	printByte_Score(Adv, Dash);

}
void displayTwoAdv() {
	printByte_Score(Dash, Adv);

}
void onewin() {
	printByte_Score(smile, Blank);
}
void twowin() {
	printByte_Score(Blank, smile);
}

void displayNull() {
	printByte_Score(Blank, Blank);
}



// Flashes the current servers score
void flashServerScore() {
#if DEBUG_Flash
	Serial.println();
	Serial.print("FlashServer :: ");
	Serial.print("  FlashActive: "); Serial.print(flashActive);
	Serial.print(" Millis(): "); Serial.print(millis());
	Serial.print("  FlashTimer: "); Serial.print(flashTimer);

	Serial.println();
#endif
	if (millis() > (flashTimer + flashDelay)) {
		flashActive = !flashActive;
		flashTimer = millis();
		//blank display
		if (flashActive) {
			printByte_Score(currentServer, Blank);
		}

		//else show points
		else
		{
			if (duceMode) {
				if (ducePoint == 3) {
					if (currentServer == 0) printByte_Score(currentServer, Adv);
					else  printByte_Score(currentServer, Dash);
				}
				else if (ducePoint == 1) {
					if (currentServer == 1) printByte_Score(currentServer, Adv);
					else  printByte_Score(currentServer, Dash);
				}
				else printByte_Score(DeuceD, DeuceD);
			
			}

			else printByte_Score(digit[pOneScore], digit[pTwoScore]); // ya idk why i did that above, could just set them all without more iffs

		}
	}

}

// Flashes The total number of bounces and the final score
void flashTotalBounce() {

	if (millis() > (flashTimer + flashDelayBounce)) {
		if (flashActive == 4) flashActive = 0;
		flashTimer = millis();
		flashActive++;
		

		if (flashActive == 1) {
			//show original score
			if (!duceMode) printByte_Score(digit[pOneScore], digit[pTwoScore]);
			else if (ducePoint == 4)  printByte_Score(smile, Blank);
			else  printByte_Score(Blank, smile);
		}

		else if (flashActive == 2) {
			// show title
			printByte_Info(0, TB);
			printByte_Info(1, Blank);

		}
		//else show total bounces
		else if (flashActive == 3) {
			printByte_Score(Blank, Blank);
			printByte_TotalBounce_Left(AnyDigit[totalBounces / 1000 % 10], AnyDigit[totalBounces / 100 % 10], AnyDigit[totalBounces / 10 % 10], AnyDigit[totalBounces % 10]);
			
		}
		else {
			// show title
			printByte_Info(0, FS);
			printByte_Info(1, Blank);
		}
	}

}
