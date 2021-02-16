

/////////////////////
///Tune Management///
/////////////////////

int marioMelody[] = {
	NOTE_E6, NOTE_E6, 0, NOTE_E6, 0, NOTE_A5, NOTE_E6, NOTE_G6, 0, NOTE_G5, 0
};
// note durations: 4 = quarter note, 8 = eighth note, etc.:
int marionoteDurations[] = {
	8, 8, 8, 8, 8, 8, 4, 4, 4, 4, 2
};

int ffvictoryMelody[] = {
	NOTE_C5, NOTE_C5, NOTE_C5, NOTE_C5, NOTE_GS4, NOTE_AS4, 0, NOTE_C5, 0, NOTE_AS4, NOTE_C5
};
int ffvictorynoteDurations[] = {
	8, 8, 8, 3, 3, 4, 64, 6, 32, 8, 1.5
};

void mario() {
	for (int thisNote = 0; thisNote < 12; thisNote++) {
		// to calculate the note duration, take one second 
		// divided by the note type.
		//e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
		int noteDuration = 1000 / marionoteDurations[thisNote];
		tone(SpeakerPin, marioMelody[thisNote], noteDuration);
		int pauseBetweenNotes = noteDuration * 1.3;
		delay(pauseBetweenNotes);
		noTone(SpeakerPin);
	}
}

void victory() {
	for (int thisNote = 0; thisNote < 12; thisNote++) {
		int noteDuration = 1000 / ffvictorynoteDurations[thisNote];
		tone(SpeakerPin, ffvictoryMelody[thisNote], noteDuration);
		int pauseBetweenNotes = noteDuration * 1.28;
		delay(pauseBetweenNotes);
		noTone(SpeakerPin);
	}

}

void playWin() {
	if (!tuneplayed) {
		victory();
		//delay(1);
		tuneplayed = true;
	}
}
