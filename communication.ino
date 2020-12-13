/*
======ALL SNIPPETS ARE MINIMAL CODES PERFORMING A CERTAIN FUNCTIONALITY=========

This code snippet is for testing if the Arduino and Pi are able to communicate with each other in order to time excecution of serial writting and reading commands.
*/

void setup()
{
    Serial.begin(115200);
}

void loop()
{
    // check if py is ready
    int pi_response=Serial.read();
    if (pi_response==2)
    {
        // pi is ready
        // while new values have not yet been measured arduino can try again to
        // send values
        // signal pi that now values get send
        Serial.write(2);
        // send values to pi
        Serial.write(4);
        Serial.write(5);
        Serial.write(6);
    }
}