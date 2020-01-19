# Eingebettete Ampelschaltung WS19/20

Symbolisiert eine tertiäre Ampel und erlaubt einen eingangsgesteuerten Phasenablauf
Dieses Projekt verwendet die [Sipeed-Longan-Nano-Traffic-Light-Library](https://github.com/ChococookieOS/Sipeed-Longan-Nano-Traffic-Light-Library)
und die dort beschriebenen Verkabelung wurde beibehalten.

## Modi
Die Ampel läuft im wesentlichen in drei verschiedenen Modi. 
Nach dem Einschalten wird immer der Standby-Modus ausgewählt.

### Standby-Modus (Nachtmodus)
Im Standby-Modus blinkt lediglich die gelbe Lampe der Ampel.
Erfolgt keine physische Eingabe schaltet sich die Ampel nach zehn Blinkzyklen in den Schlafmodus.
Erfolgt eine physische Eingabe schaltet sich die Ampel in den Aktiv-Modus.

### Aktiv-Modus
Im Aktiv-Modus werden zyklisch nacheinander folgende Leuchtphasen angestuert:
Rot
Rot-Gelb
Grün
Gelb
Rot

Durch eine kurze physische Eingabe wird der Zyklus pausiert und hält in seiner aktuellen Phase an.
Eine weitere kurzee physische Eingabe setzt den Zyklus fort.
Eine kurze physische Eingabe wird ab einer Dauer von 100ms erkannt.

Durch eine lange phyische Eingabe wird die Ampel in den Standby-Modus versetzt.
Eine lange phyische Eingabe wird ab einer Dauer von 2000ms erkannt 

### Schlaf-Modus
Im Schlaf-Modus sind alle Lampen ausgeschaltet.
Durch eine phyische Eingabe wird die Ampel reaktiviert und in den Standby-Modus versetzt.
Eine phyische Eingabe wird ab einer Dauer von 100ms erkannt.