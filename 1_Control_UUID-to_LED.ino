/** A la lecture d'un tag NFC allume ledPinOK ou ledPinNO si la valeur est comprise ou non dans la liste de tags identifiés **/

/** include library */
#include "nfc.h"
 
/** define a nfc class */
NFC_Module nfc; 

int ledPinOK =12;
int ledPinNO =13;

String valid_UUID = "77105186124";
 
void setup(void)
{
  
  pinMode(ledPinOK, OUTPUT);    
  pinMode(ledPinNO, OUTPUT); 
  Serial.begin(9600);
  nfc.begin();
  Serial.println("NFC MF1S50 Reader!");
 
  uint32_t versiondata = nfc.get_version();
  /*if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }*/
 
  // Affiche la version de la puce!
  Serial.print("Puce PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
 
  /** Set normal mode, and disable SAM */
  nfc.SAMConfiguration();
}
 
void loop(void)
{
  u8 buf[32],sta;

  String UUID; /* ajoutee */
  /** Polling the mifar card, buf[0] is the length of the UID */
  sta = nfc.InListPassiveTarget(buf);
 
  digitalWrite(ledPinOK, LOW);
  digitalWrite(ledPinNO, LOW);

  /** check state and UID length */
  if(sta && buf[0] == 4){
    /** the card may be Mifare Classic card, try to read the block */ 
    
    Serial.println(); 
    Serial.print("UUID length:");
    Serial.print(buf[0], DEC);
    Serial.println();
    Serial.print("UUID:");
    nfc.puthex(buf+1, buf[0]);
    Serial.print("Longueur :");
    Serial.println(buf[0],DEC);
    for (int i=1; i<=buf[0];i++){
        if (buf[i]<0x10){
            Serial.print("0");
        }
        Serial.print(buf[i]);
        UUID.concat(buf[i]);
    }
    Serial.println();
    
    // Verification du badge
    if(UUID == valid_UUID){
      Serial.print("OK");
      digitalWrite(ledPinOK, HIGH);
    }
    else{
      Serial.print("NO");
      digitalWrite(ledPinNO, HIGH);
    }      
    
    Serial.println();
   
  }
}
