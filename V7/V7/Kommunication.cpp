#include <cstdint> 
#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <cstring>
#include <array>
#include <b15f/b15f.h>

using std::cin;
using std::endl;
using std::cout;
using std::string;
using std::vector;
using std::bitset;

class Kommunication{
  public:
    const int D1 = 80; // Verzögerungskonstante
    const int D2 = 40; // Handshake-Verzögerung
    std::vector<char> CharSpeicherung; //Vektor zum speichern
    char EndKommunikation = static_cast<char>(0b00010111); // Ende der Kommunikation in ASCII

    Kommunication() = default;//Konstruktor ohne Agar, um zu verhindern, dass der Compiler die Übertragung zweimal initialisiert

  // PcNummer= true --> Pc2
    void setPCnr(bool p) {
      PcNummer = p;
      setDatenSenderMask();
      setRichtungSenderMask();
      setDatenEmpfaengerMask();
    }



// 1. Aufgabe
   void charKommuniktion() {
      B15F& drv = B15F::getInstance();
      drv.setRegister(&DDRA, static_cast<uint8_t>(senderRichtungMask.to_ulong()));

      char user_input = userInput();

      bool bereit = verbindung(drv);
      drv.delay_ms(D2);

      if (bereit) { // wenn Verbindung hergestellt
           // In Bitset konvertieren
          bitset<8> user_input_bit(user_input); // casting der char zu binary (ASCII)
          cout << user_input << " zu binaer: " << user_input_bit << endl;
          sendMassge(user_input_bit, drv);
      }
  }

// 2. Aufgabe
    void stringKommunication() {
      char message[100] = {}; /// was in echo darin ist!
      B15F& drv = B15F::getInstance();
      // Verbindung herstellen
      drv.delay_ms(D2);
      bool bereit = verbindung(drv);
      // wenn Verbindung hergestellt
      if (bereit) {
        while(fgets(message, sizeof(message), stdin) != nullptr) { // während wir den String lesen (!nullptr)
          int message_len = static_cast<int>(strlen(message));
          for(int i = 0; i < static_cast<int>(strlen(message)) ; i++) {
            message_len --;
            if (message_len> 0) {
              SendEmpfangSpeichern(i, message, drv);
            } else {
              sendungEnde(drv);
            }
          }
        }
      }
    }

// 3. Aufgabe
    void DateiKommuniktion() {
      FILE *inputFileStream = stdin;
      B15F& drv = B15F::getInstance();
      drv.setRegister(&DDRA, static_cast<uint8_t>(senderRichtungMask.to_ulong()));

      bool bereit = verbindung(drv);
      bool erfolgreich = false;

      if (bereit) {
        while (!feof(inputFileStream)) {
          char buf[10];
          auto read_file = fgets(buf, sizeof(buf), inputFileStream);

          if (feof(inputFileStream) == 0) {
            std::vector<char> vec(buf, buf + 10);

            printAktuellSendung(vec);

            for (int i = 0; i < vec.size(); i++) {
              std::bitset <8> as_bit (vec[i]);
              if (as_bit != 0b00000000) sendFile(i, vec, drv);
            }
          }
        }
      }
      cout << "Die Sendung ist beendet !" << endl;
    }

  private:

    bool PcNummer;
    bitset <8> datenSenderMask;
    bitset <8> senderRichtungMask;
    bitset <8> datenEmpfaengerMask;


    // Setter
    void setDatenSenderMask() { // Absender Daten maske abhängig von den Peers festlegen (PC1 oder PC2)
      datenSenderMask = PcNummer ? 0b00000111 : 0b11100000;
    }

    void setRichtungSenderMask() { // Richtung maske abhängig von den Peers festlegen (PC1 oder PC2)
      senderRichtungMask = PcNummer ? 0b00001111 : 0b11110000;
    }

    void setDatenEmpfaengerMask() { // empfänger Daten maske abhängig von den Peers festlegen (PC1 oder PC2)
      datenEmpfaengerMask = PcNummer ? 0b11100000 : 0b00000111;
    }


    // Functions

 /////// Byte Aufteiler
    auto ByteAufteilenPc2(bitset<8> bit, int part) -> bitset<8> {
      bitset<8> mask(0b11100000 >> (part * 3));
      bitset<8> result(bit & mask);
      switch (part) {
        case 0:
          result = result >> 5;
          break;
        case 1:
          result = result >> 2;
          break;
        case 2:
          result = result << 1;
          result[0] = true;
          break;
      }
      return result;
    }

    auto byteAufteilenPc1(bitset<8> bit, int part) -> bitset<8> {
 bitset<8> mask(0b11100000 >> (part * 3));
 bitset<8> result(bit & mask);

 switch (part) {
   case 0:
     result = result;
     break;
   case 1:
     result= result << 3;
     break;
   case 2:
     result <<= 1;
     result[0] = true;
     result = result << 5;

     break;
 }
 return result;
}

///// Byte Aufbuer
    auto byteAufbauenPc2(vector <bitset<8> > halter) -> bitset<8> {
     bitset <8> result = 0b00000000 ;
     int i = 0;
     for (auto &byte : halter) {
       switch(i) {
           case 0:
               result |= byte;
               break;
           case 1:
               result |= (byte >> 3);
               break;
           case 2:
               if (byte[5] == 1) result |= (byte >> 6);
               break;
       }
       i++;
     }
     std::cout << "PC2 empfaengt : " << result << std::endl;
     return result;
}

    auto byteAufbauenPc1(vector <bitset<8>> holder) -> bitset<8> {
    bitset <8> result = 0b00000000;
    int i = 0;
    for (auto &byte : holder) {
        switch(i) {
            case 0:
                result |= byte;
                break;
            case 1:
                result = (result << 3) | byte;
                break;
            case 2:
                if (byte[0] == 1) result = ((result << 3) | byte) >> 1;
                break;
        }
        i++;
    }
   std::cout << "Pc1 empfaengt : " << result << std::endl;
    return result;
}

////// empfangen und senden
    auto sendMassge(bitset<8> userInputByte, B15F& drv) -> char{
        bitset<8> aufgeteiltUserByte;
        bitset<8> finalByte;
        vector<bitset<8>> BytesEmpfang;
        for (int i = 0; i < 3; i++){
          // senden
        aufgeteiltUserByte = PcNummer ? ByteAufteilenPc2(userInputByte, i) : byteAufteilenPc1(userInputByte, i);
        drv.setRegister(&PORTA, static_cast<uint8_t>((aufgeteiltUserByte & datenSenderMask).to_ulong()));
        drv.delay_ms(D1);

          //empfangen
        bitset<8> empfang(drv.getRegister(&PINA));
        empfang &= datenEmpfaengerMask;
        BytesEmpfang.push_back(empfang);
        if (i == 2){
            finalByte = PcNummer ? byteAufbauenPc2(BytesEmpfang) : byteAufbauenPc1(BytesEmpfang);
            auto ch = static_cast<char>(finalByte.to_ulong());
            CharSpeicherung.push_back(ch);
            cout<<"Character Empfangen: " << ch <<endl;
            return ch;
            }
          }
        }

//////// Syncronisation
    auto verbindung(B15F& drv) -> bool { // Methode zum Herstellen der Verbindung zwischen PC1 und PC2.
      bool bereit = false;

      bitset<8> sender_bestaetigung(PcNummer ? 0b00001111 : 0b11110000); // Absender Bestätigt, dass der Absender senden möchte
      bitset<8> empfaenger_bestaetigug(PcNummer ? 0b11110000 : 0b00001111); /// Empfängerbestätigung, um zu wissen, wo gelesen werden soll
      bitset<8> stabilitaet_bestaetigung(PcNummer ? 0b00001111 : 0b11110000); // um zu wissen, dass der andere Peer gelesen wird (letzter Schritt im Handshake)
      bitset<8> herstellung_verbindung(PcNummer ? 0b11100000 : 0b00000111); // der erste Schritt zum Handshake sende 111

      vector <bitset <8>> buffer_empfang; //zum speichern, was empfangen wird

      while (true) { //nach einem Partner suchen, mit dem eine Verbindung aufgebaut werden kann
        cout << "Stelle Verbindung her..." << endl;
        // send
        drv.setRegister(&PORTA, static_cast<uint8_t>((senderRichtungMask & datenSenderMask).to_ulong())); // casting des Bitsatzes in ulong und dann in uint8_t
        drv.delay_ms(D2);

        // received
        bitset<8> gelesen(drv.getRegister(&PINA));
        gelesen &= datenEmpfaengerMask;// Ich bekomme, was ich lesen möchte, nicht das ganze Stück, weshalb ich es getan habe, und mit einer Maske, um zu lesen, was wahr ist

        if (gelesen == herstellung_verbindung) buffer_empfang.push_back(gelesen);//Wenn das empfangene Byte = ist, fügen Sie es auf die Rückseite des Vektors ein, um eine Verbindung herzustellen
        if (buffer_empfang.size() % 3 == 0 && buffer_empfang.size() != 0) {// Wir haben das Senden von 00000111 oder 11100000 abgeschlossen (es wird in 3 Teilen gesendet)

          int i = 0;
          vector <bitset <8>> Puffer_Empfang_Bestätigung;

          for (auto &frame : buffer_empfang) {
            if (frame == herstellung_verbindung) i++;  // 111 Baumzeiten senden

            if (i == 3) {
              while(true) {
                // send
                drv.setRegister(&PORTA, static_cast<uint8_t>((senderRichtungMask & sender_bestaetigung).to_ulong())); // 1111 senden
                drv.delay_ms(D2);

                // received
                bitset<8> received_ack(drv.getRegister(&PINA)); //receive 1111
                received_ack &= empfaenger_bestaetigug;

                if (received_ack == empfaenger_bestaetigug) Puffer_Empfang_Bestätigung.push_back(received_ack);

                if (Puffer_Empfang_Bestätigung.size() % 3 == 0 && Puffer_Empfang_Bestätigung.size() != 0) {
                  i = 0;
                  for (auto &frame : Puffer_Empfang_Bestätigung) {
                    if (frame == empfaenger_bestaetigug) i++;

                    if (i == 3) return true;
                  }
                }
              }
            }
          }
        }
      }
      return bereit;
    }
//// print des aus der Datei gesendeten Teils
    void printAktuellSendung(std::vector<char> &msg) {
      cout<<"sendet das part----->  " ;
      for (auto &ch : msg) cout << ch << ' ';
      cout << endl;
    }

//// Absender/Empfänger/Speicherr der Nachricht
    void SendEmpfangSpeichern(int &i, char message[100], B15F& drv) { // Senden und Empfangen einer Nachricht mit denselben Methoden wie beim Senden und Empfangen eines Zeichens
      drv.setRegister(&DDRA, static_cast<uint8_t>(senderRichtungMask.to_ulong()));
     bitset<8> byte_send(static_cast<int>(message[i]));
       sendMassge(byte_send, drv);
        drv.delay_ms(D2);
        //Speichern in einer Datei
      std::string s(CharSpeicherung.begin(), CharSpeicherung.end());
      saveStringToFile(s,"echo.txt");
      if (CharSpeicherung.at(CharSpeicherung.size() - 1) == EndKommunikation)  {
        drv.delay_ms(D2);
        cout << "end sending" << endl;
        exit(0);
      }
    }

    //Sender/Empfänger/Speicherr der Datei
    void sendFile(int &i, std::vector<char> &message, B15F& drv) {
      bitset<8> byte_send(static_cast<int>(message[i])); // cast der char zu byte
      cout << "send " << message[i] << " as " << byte_send << endl; // print char und Ascii
      sendMassge(byte_send, drv);
      drv.delay_ms(D2);
      std::string s(CharSpeicherung.begin(), CharSpeicherung.end()); ///  die phrase
      saveStringToFile(s,"text.txt"); // in einer Datei speichern
    }

// // Speichern in Datei
    void saveStringToFile(const std::string& str, const std::string& filepath) {
    std::ofstream file(filepath);
    if (file.is_open()) {
        file << str;
        file.close();
    } else {
        std::cout << "Datei nicht gefunden: " << filepath << std::endl;
    }
}

// Hole die Benutzereingaben
    char  userInput(){ // Methode zum Abrufen der Benutzereingabe (nur ein Zeichen)
        char user_input;
        cout << "Gebe einen Charcter zu senden: ";
        cin >> user_input;
        return user_input;
}

    // Senden von ende der Kommunikation
    void sendungEnde(B15F& drv) { // Wenn der Versand beendet ist, senden wir NOTHINGBYTE, um anzuzeigen, dass der Versand beendet ist
      int z = 0;
     while(true){
        SendEmpfangSpeichern(z, &EndKommunikation, drv);
      }
    }
};
