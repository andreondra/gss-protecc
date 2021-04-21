# Shrnutí

Tento dokument je shrnutím, zprávou a závěrem celé práce.

Semestrální práci jsem dokončil úspěšně. Byly implementovány všechny plánované  funkce a navíc ještě nějaké další. Po specifikaci zadání jsem projekt upřesnil v teoretické přípravě. Na jejím základě jsem sestavil kostru projektu a začal sestavovat jednotlivé bloky. 

Nejprve jsem se zaměřil na funkce zajišťující komunikaci s hardwarem, především tedy čtečkou karet. Bylo nutné vymyslet způsob načítání karet (pomocí MFRC522) a způsob evidence. Jasnou volbou byla vestavěná EEPROM, kde omezený počet zápisů nevadí, jelikož se karty z a do evidence tak často přidávat nebudou.

Dále bylo nutné vymyslet uživatelské rozhraní. Nakonec, jak již zaznělo v teoretické přípravě, zvítězila implementace pomocí FSM. Rozhraní je rozčleněno do obrazovek. Některé obrazovky mají i podobrazovky, ale většina obrazovek je neblokujících. K prodlevě dojde pouze při situacích potvrzení naskenování, kdy se stejně jiné operace neprovádějí.

Posledním, neméně důležitým krokem, bylo sestavení nejen projektové dokumentace, ale i uživatelského manuálu.

Během projektu jsem si osvěžil znalosti Arduina a vyzkoušel zajímavý způsob implementace uživatelského rozhraní. Projekt tedy byl prospěšný z vícero důvodů.
