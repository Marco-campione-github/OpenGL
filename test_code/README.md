
# Test Code

## Struttura del progetto
- `.vscode`: cartella contenente la configurazione dell'IDE Visual Studio Code
    - `c_cpp_properties.json` configurazione del linter, contiene i percorsi alle librerie (2 configurazioni per windows e linux)
    - `extensions.json` estensioni di vscode necessarie per il debug C++, se non installate vscode mostra pop-up con le raccomandazioni
    - `launch.json` configurazione di compilazione e debug
    - `settings.json` configurazione dell'IDE
    - `tasks.json` contiene i task che eseguono il make (build del progetto) e make clean (clean del progetto)
- `freeglut, glew, glm, freeglut.dll, glew32.dll`: librerie
- `Makefile`: contiene le istruzioni per compilare il codice sorgente
- `test_code.cpp`: codice sorgente del progetto

> NOTA per Windows: se si modificano le librerie utilizzate o la loro posizione va aggiornato il file `c_cpp_properties.json`

## Uso del progetto con VS Code

Scaricate ed installate [VS Code](https://code.visualstudio.com/)

### Windows
1. scaricate [MinGW32](https://github.com/brechtsanders/winlibs_mingw/releases/download/10.2.0-11.0.0-8.0.0-r7/winlibs-i686-posix-dwarf-gcc-10.2.0-llvm-11.0.0-mingw-w64-8.0.0-r7.7z) ed estraete il contenuto dell'archivio in `C:\mingw32` in modo che gli eseguibili (es. g++) si trovino nella cartella `C:\mingw32\bin`
2. aggiungete alla variabile d'ambiente `Path` la cartella `C:\mingw32\bin`
3. aprite PowerShell e verificate che il comando `Get-Command mingw32-make | Select-Object -ExpandProperty Definition` restituisca `C:\mingw32\bin\mingw32-make.exe`
    > se così non è verificate che non ci siano riferimenti ad altre versioni di MinGW (es. MinGW64) nella variabile Path
4. con `cd <percorso>\test_code` navigate alla cartella del progetto
5. compilate il codice con il comando `mingw32-make`, provate ad eseguire `test_code.exe`

Dopo aver verificato tutti i punti precedenti

6. aprite VS Code, menu `File -> Open Folder`, scegliete la cartella `test_code`
7. in basso a destra compare un pop-up che chiede di autorizzare la modifica del terminal, consentitela (forza l'uso di PowerShell come terminal)
8. sempre in basso a destra potrebbe comparire un pop-up che suggerisce delle estensioni, accettate ed installate l'estensione C/C++
    > se non compare il pop-up verificate che l'estensione sia già installata (quinta icona dall'alto nella barra verticale di sinistra) altrimenti installatela
9. premendo `F5` compila ed esegue il progetto, il debugger si ferma automaticamente alla prima riga utile
    > [qui](https://code.visualstudio.com/docs/editor/debugging) trovate istruzioni su come usare il debugger, [qui](https://code.visualstudio.com/docs/editor/debugging#_debug-actions) le scorciatoie da tastiera per step over, step into, etc.

### Linux
> Quanto segue sono istruzioni per Ubuntu 20.04 ma sono simili per altre distro
1. installate le librerie: `sudo apt install build-essential make freeglut3-dev libglew-dev libglm-dev`
2. con `cd <percorso>/test_code` navigate alla cartella del progetto
3. compilate il codice con il comando `make`, provate ad eseguire `test_code.exe`

Dopo aver verificato tutti i punti precedenti

4. aprite VS Code, menu `File -> Open Folder`, scegliete la cartella test_code
5. in basso a destra potrebbe comparire un pop-up che suggerisce delle estensioni, accettate ed installate l'estensione C/C++
    > se non compare il pop-up verificate che l'estensione sia già installata (quinta icona dall'alto nella barra verticale di sinistra) altrimenti installatela
6. premendo `F5` compila ed esegue il progetto, il debugger si ferma automaticamente alla prima riga utile
    > [qui](https://code.visualstudio.com/docs/editor/debugging) trovate istruzioni su come usare il debugger, [qui](https://code.visualstudio.com/docs/editor/debugging#_debug-actions) le scorciatoie da tastiera per step over, step into, etc.
