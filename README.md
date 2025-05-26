# 🚦 StrikeUnit

**StrikeUnit** ist ein modulares, Arduino-basiertes Spielmodul für Airsoft- und Paintball-Szenarien.  
Das System unterstützt mehrere Spielmodi (💣 Search & Destroy, 🛠️ Sabotage, 🏁 Domination, ✂️ Cut the Wire)  
und bietet zahlreiche Features wie LCD-Anzeige, Keypad, LED-Signalisierung, Sirene und MOSFET-gesteuerte Ausgänge.

---
![Wokwi Simulation](screenshot.png)
---

## ✨ Features

- 💣 Mehrere Spielmodi (Search & Destroy, Sabotage, Domination, Cut the Wire)
- 🕹️ Bedienung über Tastenfeld & LCD-Display
- 🔐 Passwortschutz und flexible Code-Eingabe
- 🚨 LED-, Piezo- und Sirenen-Ausgänge
- 🧨 MOSFET-Steuerung für Pyro-Effekte oder externe Geräte
- 🧩 Modulare, gut strukturierte Codebasis
- 🔄 Einfache Erweiterung und Anpassung

---

## 🛠️ Hardware-Voraussetzungen

- 🧠 **Arduino** (Nano, Uno, Pro Mini empfohlen)
- 🖥️ **LCD-Display** (I2C oder parallel)
- ⌨️ **4x4 Keypad**
- 🔴 **LEDs**, **Piezo**, ggf. **Sirene**
- ⚡ **MOSFETs** für externe Ausgänge
- 🔊 **Lautsprecher** (optional)
- 🪛 Diverse Kabel und Kleinteile

---

## 🚀 Installation

1. 📥 Repository herunterladen oder klonen.
2. 📦 Abhängige Libraries installieren (z. B. Keypad, LiquidCrystal).
3. 🛠️ Hardware gemäß Schaltplan anschließen.
4. 💻 Code in Arduino IDE oder Visual Studio Code (mit PlatformIO) öffnen.
5. 🔌 Sketch auf den Arduino flashen.

---

## 🧪 Live-Demo

Teste das Projekt direkt im Browser mit Wokwi:  

👉 [StrikeUnit online testen (Wokwi.com)](https://wokwi.com/projects/431928489736753153)

---

## 🎮 Spielmodi Übersicht

- **💣 Search & Destroy (Sabotage):**  
  Zwei Teams treten gegeneinander an: Das angreifende Team muss die Bombe am Zielort scharf machen (entweder durch Passworteingabe oder längeres Tastendrücken). Sobald die Bombe gelegt ist, läuft ein Countdown bis zur Explosion. Das verteidigende Team muss die Bombe innerhalb dieser Zeit entschärfen – je nach Einstellung ebenfalls per Passwort oder Tastendruck. Gelingt die Entschärfung, kann die Bombe erneut aktiviert werden, solange die Spielzeit läuft. Läuft der Countdown ab, gewinnt das Angreifer-Team. LEDs, Display und Sounds geben jederzeit Rückmeldung zum Spielstatus.

- **🛠️ Sabotage:**  
  In diesem Modus muss das Angreifer-Team eine Bombe an einem Zielort scharf machen, während das Verteidiger-Team versucht, dies zu verhindern.  
  Nach dem Start läuft eine Gesamtspielzeit. Die Angreifer können die Bombe entweder durch Passworteingabe oder längeres Drücken einer Taste scharf machen. Ist die Bombe scharf, startet ein separater Countdown (Explosions-Timer).  
  Die Verteidiger müssen nun die Bombe entschärfen, bevor der Countdown abläuft. Auch die Entschärfung kann – je nach Einstellung – per Passwort oder Tastendruck erfolgen.  
  Gelingt die Entschärfung, startet das Spiel von vorn und die Angreifer können einen neuen Versuch unternehmen, solange die Gesamtspielzeit nicht abgelaufen ist.  
  Läuft der Countdown aus, bevor entschärft wurde, explodiert die Bombe und die Angreifer gewinnen die Runde. LEDs und Sounds signalisieren den aktuellen Status. Das Display zeigt Zeit und Statusmeldungen an.  


- **🏁 Domination:**  
  Im Domination-Modus kämpfen zwei Teams (Rot & Grün) um die Kontrolle einer zentralen Zone.  
  Das Spiel läuft über eine festgelegte Zeit.  
  Beide Teams können die Zone erobern, indem sie eine Taste gedrückt halten (Team Rot = Taste D, Team Grün = Taste C), solange die Zone neutral ist. Sobald ein Team die Zone eingenommen hat, beginnt für dieses Team ein Timer zu laufen – je länger das Team die Zone hält, desto mehr Punkte (Zeit) sammelt es.  
  Die Zone kann jederzeit vom anderen Team neutralisiert werden, indem eine Taste erneut gedrückt gehalten wird. Danach kann die Zone von einem Team neu besetzt werden.  
  Das Display zeigt abwechselnd die verbleibende Spielzeit sowie den aktuellen Status der Zone (neutral, rot oder grün) und wie lange das Team die Zone gehalten hat. LEDs und Sound geben zusätzlich Feedback.  
  Am Ende gewinnt das Team, das die Zone insgesamt am längsten gehalten hat. Das Ergebnis wird angezeigt, danach kann eine neue Runde gestartet oder zum Menü zurückgekehrt werden.

- **✂️ Cut the Wire:**  
  In diesem Modus wird eine Bombe zunächst scharf gemacht (durch längeres Drücken einer Taste). Nach dem Scharfschalten läuft ein Countdown – die Spieler müssen herausfinden und den richtigen Draht durchtrennen, um die Bombe zu entschärfen.  
  Es sind vier Drähte vorhanden, aber nur einer davon ist korrekt (die richtige Farbe/Klemme ist vorher festgelegt). Wird ein Draht durchtrennt, prüft das System:  

  - **Richtiger Draht:** Die Bombe wird sofort entschärft, das Team gewinnt.

  - **Falscher Draht:** Die Zeit läuft schneller ab (Speed-Faktor erhöht sich), das Spiel wird hektischer. Nach mehreren Fehlversuchen läuft die Zeit extrem schnell.
  Wenn die Zeit abläuft, bevor der richtige Draht durchtrennt wurde, explodiert die Bombe und das Spiel ist verloren.  
  LEDs und Soundeffekte begleiten die Aktion, das Display zeigt die verbleibende Zeit im Sekunden- und Millisekunden-Format.


---

## 📝 Anpassung

Alle Texte und Menüeinträge findest du zentral in `lang.h`.  
Die Spielmodi sind modular als eigene `.ino`-Dateien organisiert – ideal zum Erweitern.

---

## ✅ ToDo / Roadmap

- ✅ Grundfunktionen: Spielmodi „Search & Destroy“, „Sabotage“, „Domination“, „Cut the Wire“
- 🟡 Fehlerbeseitigung: Cut the Wire Modus
- ⏳ Mehrsprachigkeit / Sprachdateien
- ⏳ Teileliste (vollständige Übersicht aller benötigten Komponenten)
- ⏳ Anschlussplan (Schaltplan und Aufbauanleitung)
- ⏳ Code-Bereinigung & Vereinfachung
- ⏳ Modularisierung der Spielmodi (bessere Trennung im Code)
- ⏳ Detaillierte Dokumentation der Funktionen im Code
- ⏳ Screenshots / Fotos vom fertigen Aufbau
- ⏳ Erweiterung: Zusätzliche Spielmodi (z.B. King of the Hill, Rush, etc.)
- ⏳ Optionale Bluetooth-/WLAN-Anbindung für Steuerung/Monitoring
- ⏳ Sound- und LED-Effekte verbessern/erweitern
- ⏳ Logging & Analytics (z.B. Speicherung der Spielergebnisse)
- ⏳ API-Anbindung für externe Tools/Scraper
- ⏳ Web- oder App-Frontend für Einstellungen/Live-Monitoring


🟡 in Arbeit | 🔄 in Überarbeitung | 🧪 in Testphase | ⚠️ bekanntes Problem | ⏳ geplant | ⬜️ Noch offen | ✅ Erledigt


---

## 🙏 Credits & Originalprojekt

Dieses Projekt basiert auf [yinbot/Airsoft-BombPro](https://github.com/yinbot/Airsoft-BombPro).  
**Vielen Dank** an das Originalprojekt und die ursprünglichen Entwickler!

---

## 📝 Lizenz

MIT License

---

## 👤 Autor

D.  
GitHub: [s0n11c39]  
