int TastZu=2;      // Taster_Tuer_Zu
int TastAuf=3;      // Taster_Tuer_Auf
int EndZu=5;      // Endschalter_Tur_Zu
int EndAuf=4;     // Endschalter_Tur_Auf
int VenZu=7;      // Ventil_Tuer_Zu
int VenAuf=8;     // Ventil_Tuer_Auf
int VenZuf=6;     // Ventil_Tuer_Zufuhr
int StatZu;       // Tasterstatus_Tuer_Zu
int StatAuf;      // Tasterstatus_Tuer_Auf
int SchZu;        // Schalterstatur_Endschalter_Tuer_Zu
int SchAuf;       // Schalterstatur_Endschalter_Tuer_Auf
const int Piep=11;        // Warnton
unsigned long lastMillis;


void setup() 
  {
    Serial.begin(9600);
    // set up pinModes and initial values
    // TODO ersetze Zahlen durch Variablen
    pinMode(2, INPUT);
    pinMode(3, INPUT);
    pinMode(4, INPUT);
    pinMode(5, INPUT);
    pinMode(7, OUTPUT);
    pinMode(6, OUTPUT);
    pinMode(8, OUTPUT);
    pinMode(13, OUTPUT);
    pinMode(Piep, OUTPUT);
    digitalWrite(6,HIGH);
    digitalWrite(7,HIGH);
    digitalWrite(8,HIGH);
    StatAuf=LOW;
    StatZu=LOW;
    SchAuf=LOW;
    SchZu=LOW; 
    lastMillis=millis();


  } 

void loop() 
  {
    // Tasterstatus pruefen

    StatZu=digitalRead(2);
    StatAuf=digitalRead(3);
    
    SchAuf=digitalRead(5);
    
    SchZu=digitalRead(4);
  
  
  //Warnton
  
  
  if(StatZu==HIGH or StatAuf==HIGH or (SchZu!=HIGH and SchAuf!=HIGH))
    {
      if((millis()-lastMillis)>=1000)
        {
    
          tone(Piep,4000,500);
        
          //digitalWrite(Piep, !digitalRead(Piep));
          lastMillis=millis();
        }
    }
  else
    {
    //notone(11);
    //digitalWrite(Piep, LOW);
    } 
  

  // RESET

  if(StatAuf==HIGH and StatZu==HIGH)
    {
      digitalWrite(6,HIGH);
      digitalWrite(7,HIGH);
      digitalWrite(8,HIGH);
      StatAuf=LOW;
      StatZu=LOW;
      SchAuf=LOW;
      SchZu=LOW;
  
      Serial.println("----------RESET----------");
      Serial.println("----------5s Warten!----------");
      delay(5000);
    }

  // Tuer schließen

  if(StatZu==HIGH)
    {
      digitalWrite(6,LOW);
      digitalWrite(7,LOW);
      Serial.println("Tuer geht zu");
      delay(1000);
      SchAuf=LOW;
    }
    

  if(SchZu==HIGH)
    {
      digitalWrite(6,HIGH);
      digitalWrite(7,HIGH);
      Serial.println("Tuer ist geschlossen");
    }
  
  // Tuer oeffnen
  
  if(StatAuf==HIGH)
    {
      digitalWrite(6,LOW);
      digitalWrite(8,LOW);
      Serial.println("Tuer geht auf");
      delay(1000);
    }

  if(SchAuf==HIGH)
    {
      digitalWrite(6,HIGH);
      digitalWrite(8,HIGH);
      Serial.println("Tuer ist geoeffnet");
    }

  
  Serial.println("AUS");

}
