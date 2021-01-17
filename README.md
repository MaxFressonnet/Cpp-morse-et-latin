Documentation utilisateur

Le code est le fichier projet2.cpp, mais nous utilisons aussi dr_wav.h, pour lire les fichiers .wav : ce n'est pas moi qui l'ai écrite, elle est disponible sur Internet

Pour compiler : g++ projet.cpp -o projet
Pour exécuter : ./projet

Il vous sera demandé : Voulez vous traduire depuis le morse ou depuis le latin ?
En répondant morse (ou n'importe quel mot qui commence par m), vous signalez que vous souhaitez traduire depuis le morse audio vers le latin écrit
	Il vous sera alors demandé : Entrez le nom de votre fichier : 
	Vous y entrerez le nom du fichier.wav, déposé dans le répertoire (par exemple morse-MORSE_CODE-20wpm-600hz.wav)
	Le texte traduit sera alors affiché dans le terminal
En répondant latin (ou n'importe quel mot qui ne commence pas par m), vous signalez que vous souhaitez traduire depuis le latin écrit vers le morse audio
	Il vous sera alors demandé : Entrez votre texte en français : 
	Vous y entrerez le texte que vous souhaitez traduire
	Le fichier audio sera alors disponible sous le nom sortie.wav
	
Les exemples :
	Le fichier le_projet.wav est le résultat obtenu après avoir traduit le texte "le projet" en morse
	Le fichier Code_morse.JPG est l'image de ce qu'on obtient après avoir traduit le fichier morse-MORSE_CODE-20wpm-600hz.wav