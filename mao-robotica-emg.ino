#include <Servo.h>

#define EMG_PIN A0

#define DEFAULT_MAX 180
#define DEFAULT_MIN 0

#define TH_CLOSE 120
#define TH_OPEN  60

Servo thumb;
Servo index;
Servo middle;
Servo ring;
Servo pinky;

float filteredEnvelope = 0.0;
bool handClosed = false;

void openHand()
{
    thumb.write(DEFAULT_MIN);
    index.write(DEFAULT_MIN);
    middle.write(DEFAULT_MIN);
    ring.write(DEFAULT_MAX);
    pinky.write(DEFAULT_MIN);
}

void closeHand()
{
    thumb.write(DEFAULT_MAX);
    index.write(DEFAULT_MAX);
    middle.write(DEFAULT_MAX);
    ring.write(DEFAULT_MIN);
    pinky.write(DEFAULT_MAX);
}

void setup()
{
    Serial.begin(115200);

    thumb.attach(10);
    index.attach(8);
    middle.attach(11);
    ring.attach(6);
    pinky.attach(5);

    filteredEnvelope = analogRead(EMG_PIN);

    openHand();
}

void loop()
{
    int reading = analogRead(EMG_PIN);

    filteredEnvelope =
        (0.85f * filteredEnvelope) +
        (0.15f * reading);

    if (!handClosed && filteredEnvelope > TH_CLOSE)
    {
        closeHand();
        handClosed = true;
    }

    if (handClosed && filteredEnvelope < TH_OPEN)
    {
        openHand();
        handClosed = false;
    }

    Serial.print(reading);
    Serial.print(",");
    Serial.println(filteredEnvelope);

    delay(5);
}