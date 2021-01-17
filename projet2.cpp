#include <iostream>
#include <map>
#include <string>
#include <cstring>
#include <charconv>
#include <iterator>
#include <fstream> 
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <cstring>
#include <cstddef>
#include <vector>
#define DR_WAV_IMPLEMENTATION
#include <string>
#include <iostream>
#include "dr_wav.h"

void mtol()                                                     // La fonction qui traduit du morse vers le latin
{

    std::map<std::string, std::string> dicoMorse2Char;          //Un dictionnaire qui traduit depuis le morse vers le latin
    dicoMorse2Char[".-"]="a";
    dicoMorse2Char["-..."]="b";
    dicoMorse2Char["-.-."]="c";
    dicoMorse2Char["-.."]="d";
    dicoMorse2Char["."]="e";
    dicoMorse2Char["..-."]="f";
    dicoMorse2Char["--."]="g";
    dicoMorse2Char["...."]="h";
    dicoMorse2Char[".."]="i";
    dicoMorse2Char[".---"]="j";
    dicoMorse2Char["-.-"]="k";
    dicoMorse2Char[".-.."]="l";
    dicoMorse2Char["--"]="m";
    dicoMorse2Char["-."]="n";
    dicoMorse2Char["---"]="o";
    dicoMorse2Char[".--."]="p";
    dicoMorse2Char["--.-"]="q";
    dicoMorse2Char[".-."]="r";
    dicoMorse2Char["..."]="s";
    dicoMorse2Char["-"]="t";
    dicoMorse2Char["..-"]="u";
    dicoMorse2Char["...-"]="v";
    dicoMorse2Char[".--"]="w";
    dicoMorse2Char["-..-"]="x";
    dicoMorse2Char["-.--"]="y";
    dicoMorse2Char["--.."]="z";
    dicoMorse2Char["-----"]="0";
    dicoMorse2Char[".----"]="1";
    dicoMorse2Char["..---"]="2";
    dicoMorse2Char["...--"]="3";
    dicoMorse2Char["....-"]="4";
    dicoMorse2Char["....."]="5";
    dicoMorse2Char["-...."]="6";
    dicoMorse2Char["--..."]="7";
    dicoMorse2Char["---.."]="8";
    dicoMorse2Char["----."]="9";
    dicoMorse2Char[".-.-.-"]=".";
    dicoMorse2Char["--..--"]=",";
    dicoMorse2Char["-..-."]="/";
    dicoMorse2Char["..--.."]="?";


    drwav wav;
    int iSonMorse[10000], iSonMorseCumul[1000];                                         // Les listes qui donneront la longueur de chaque son
    int iSilenceMorse[10000], iSilenceMorseCumul[1000];                                 // Les listes qui donneront la longueur de chaque silence
    int iSonMin=999999, iSonMax=0;                                                      // Ces entiers se destinent à être le son le plus faible et le plus fort
    int iLongueurPoint, iLongueurTiret, iLongueurSilencePointTiret, iLongueurSilenceLettre,iLongueurSilenceMot; // Les noms me semblent suffisamment évocateurs
    iSonMorse[0] = 0;
    iSilenceMorse[0] = 0;
    int iMorseCourant = 0;              
    int iMorseCourantCumul = 0;

    char text[1000];
    std::cout << "Entrez l'adresse de votre fichier : ";   // Par exemple morse-MORSE_CODE-20wpm-600hz.wav
    std::cin.getline(text,1000);
    
    if (!drwav_init_file(&wav, text, NULL)) {
        std::cout << "-1" << std::endl;;
    }

    int32_t* Echantillons = (int32_t*)malloc((size_t)wav.totalPCMFrameCount * wav.channels * sizeof(int32_t));
    drwav_read_pcm_frames_s32(&wav, wav.totalPCMFrameCount, Echantillons);


    if (Echantillons != NULL) {
        for (int i = 0; i < wav.totalPCMFrameCount; i++) {          //Dans cette boucle, on remplit la liste iSilenceMorse : sa ième case est le ième silence, et la valeur de sa ième case est la longueur de ce silence. On remplit aussi iSonMorse avec la même logique
          if (abs(Echantillons[i]) > 0) {
            if (iSilenceMorse[iMorseCourant] > 0) {
              iMorseCourant++;
              iSonMorse[iMorseCourant] = 1;
              iSilenceMorse[iMorseCourant] = 0;
            }
            else iSonMorse[iMorseCourant]++;
          }
          else {
            if (iSonMorse[iMorseCourant] > 0) {
              iMorseCourant++;
              iSonMorse[iMorseCourant] = 0;
              iSilenceMorse[iMorseCourant] = 1;
            }
            else iSilenceMorse[iMorseCourant]++;
          }
        }

      iSonMorseCumul[0] = 0;
      iSilenceMorseCumul[0] = 0;
      for (int i=0; i<=iMorseCourant; i++) {                                        // Ici, on se débarasse des erreurs : il arrive que des tous petits sons ou des petits silences qui ne correspondent à rien apparaissent. Nous les supprimons donc des listes et sommons les cases séparées par ces erreurs
          if (iSilenceMorseCumul[iMorseCourantCumul] < 20) {                        // On fixe le seuil à 20 : si un son ou un silence a une durée inférieure à 20 frames, on considère que c'est une erreur
            iSilenceMorseCumul[iMorseCourantCumul] += iSilenceMorse[i];
            iSonMorseCumul[iMorseCourantCumul] += iSonMorse[i];
          }
          else  {
            iMorseCourantCumul++;
            iSilenceMorseCumul[iMorseCourantCumul] += iSilenceMorse[i];
            iSonMorseCumul[iMorseCourantCumul] += iSonMorse[i];
          }
      }
    }
    for (int i=0; i<=iMorseCourantCumul; i++) {
      if (iSonMorseCumul[i] < iSonMin) iSonMin = iSonMorseCumul[i];             // C'est la durée minimale d'un son : donc c'est un point
      if (iSonMorseCumul[i] > iSonMax) iSonMax = iSonMorseCumul[i];             // C'est la durée maximale d'un son : donc c'est un tiret
    }
    //Dans les prochaines lignes, on définit la longueur de chaque phonème de morse
    iLongueurPoint = iSonMin;
    iLongueurTiret = iSonMax;
    iLongueurSilencePointTiret = iSonMin; // pas nécessaire (puisque ça vient après chaque signe Morse)
    iLongueurSilenceLettre = iSonMax;
    iLongueurSilenceMot = iSonMax * 3;
    std::string sMorse = "";
    std::string e = "e";                // Ce sera l'espace entre deux lettres
    std::string g = "g";                // Ce sera l'espace entre deux mots
    for (int i=0; i<=iMorseCourantCumul; i++) {
      if ((iSonMorseCumul[i] > iLongueurPoint * 0.9) && (iSonMorseCumul[i] < iLongueurPoint * 1.1)) sMorse.append(".");         //Comme nous ne sommes pas psychorigides, nous acceptons un écart de 10% avec la longueur de son calculée
      if ((iSonMorseCumul[i] > iLongueurTiret * 0.9) && (iSonMorseCumul[i] < iLongueurTiret * 1.1)) sMorse.append("-");
      if ((iSilenceMorseCumul[i] > iLongueurSilenceLettre * 0.9) && (iSilenceMorseCumul[i] < iLongueurSilenceLettre * 1.1)) sMorse.append(e);
      if ((iSilenceMorseCumul[i] > iLongueurSilenceMot * 0.9) && (iSilenceMorseCumul[i] < iLongueurSilenceMot * 1.1)) sMorse.append(g);
    }
    //std::cout << "Chaîne Morse correspondant au fichier .WAV : \"" << sMorse << "\"\n";
    drwav_uninit(&wav);

    std::string lettre = "";                            // La lettre morse en cours de lecture, qui sera réinitialisée à chaque espace
    std::string mot = "";                               // Le mot dans l'alphabet latin
    lettre += sMorse[0];
    for(int i = 1; i<sMorse.size(); i++){               // Maintenant, traduisons dans l'alphabet latin
        if(sMorse[i] == 'e'){
            mot += dicoMorse2Char[lettre];
            lettre = "";
        }
        if(sMorse[i] == 'g'){
            mot += dicoMorse2Char[lettre];
            mot += " ";
            lettre = "";
        }
        else{
            if(sMorse[i] != 'e'){
            lettre += sMorse[i];
            }
        }
    }
    std::cout << mot << std::endl;
}

typedef struct  WAV_HEADER {                //Il s'agit de l'entête d'un fichier.wav
  uint8_t       RIFF[4];
  uint32_t      ChunkSize;
  uint8_t       WAVE[4];
  uint8_t       fmt[4];
  uint32_t      Subchunk1Size;
  uint16_t      AudioFormat;
  uint16_t      NumOfChan;
  uint32_t      SamplesPerSec;
  uint32_t      bytesPerSec;
  uint16_t      blockAlign;
  uint16_t      bitsPerSample;
  uint8_t       Subchunk2ID[4];
  uint32_t      dwDataLength;
} __attribute__((packed)) entete_WAV;








int ChargeFichierWAV(char *sNomFichier, entete_WAV *enteteWAV, char **sDonnees) {   //Cette fonction charge un fichier.wav et en renvoie la longueur des données, qui est indispensable pour que l'on puisse concaténer correctement les différents fichiers audios par la suite
  int lgEntete = sizeof(entete_WAV);
  int lgFichier, lgData;
  FILE *fFichier;
  fFichier = fopen(sNomFichier, "rb");                                      
  fread(enteteWAV, lgEntete, 1, fFichier);                                       //On lit l'entête pour obtenir sa longueur, mais notons bien qu'on se destine à utiliser la même entête pour tous les fichiers
  lgFichier = enteteWAV->ChunkSize;                                              //La taille du fichier
  lgData = (unsigned long)enteteWAV->dwDataLength;                               //La taille des données réelles, c'est à dire du fichier privé de son entête, c'est la taille que l'on renseignera pour concaténer les fichiers, car on ne concatène pas les entêtes, juste le contenu
  *sDonnees = (char *)malloc(lgData * sizeof(char));                             //On alloue la bonne taille dans la mémoire pour le fichier (privé de son entête)
  fread(*sDonnees, sizeof(char), (size_t)lgData, fFichier);                      //Et le pointeur créé à la ligne précédente pointe vers notre fichier
  fclose(fFichier);
  return(lgData);
}

void ltom(){                                    // La fonction qui traduit du latin vers le morse
    std::map<std::string, std::string> dico;    //on créé un dictionnaire qui associe sa traduction en morse à chaque lettre
    dico["a"] = "pt";                           //p est le point, et t le tiret
    dico["b"] = "tppp";
    dico["c"] = "tptp";
    dico["d"] = "tpp";
    dico["e"] = "p";
    dico["f"] = "pptp";
    dico["g"] = "ttp";
    dico["h"] = "pppp";
    dico["i"] = "pp";
    dico["j"] = "pttt";
    dico["k"] = "tpt";
    dico["l"] = "ptpp";
    dico["m"] = "tt";
    dico["n"] = "tp";
    dico["o"] = "ttt";
    dico["p"] = "pttp";
    dico["q"] = "ttpt";
    dico["r"] = "ptp";
    dico["s"] = "ppp";
    dico["t"] = "t";
    dico["u"] = "ppt";
    dico["v"] = "pppt";
    dico["w"] = "ptt";
    dico["x"] = "tppt";
    dico["y"] = "tptt";
    dico["z"] = "ttpp";
    dico["1"] = "ptttt";
    dico["2"] = "ppttt";
    dico["3"] = "ppptt";
    dico["4"] = "ppppt";
    dico["5"] = "ppppp";
    dico["6"] = "tpppp";
    dico["7"] = "ttppp";
    dico["8"] = "tttpp";
    dico["9"] = "ttttp";
    dico["0"] = "ttttt";
    dico["."] = "ptptpt";
    dico[","] = "ttpptt";
    dico["/"] = "tpptp";
    dico["?"] = "ppttpp";

    char text[1000];                            //On initialise un texte, qui se destine à être la phrase que l'on cherche à traduire
    std::string g = "g";                        //g est est l'espace entre deux mots : c'est un silence qui dure un point et deux tirets
    std::string e = "e";                        //e est l'espace entre deux lettres : c'est un silence qui dure un tiret
    std::string s1 = "";                        //On initialise encore un texte, qui se destine cette fois-ci à être la traduction en morse de notre phrase
    std::cout << "Entrez votre texte en français : ";   //On demande la phrase à l'utilisateur
    std::cin.getline(text,1000);
    for(int i = 0;i<strlen(text);i++){          //On parcourt le texte
    if(text[i] != ' '){                         //Pour chaque lettre, on ajoute sa traduction en morse puis un petit espace
        char b = text[i];
        std::string z;
        z.push_back(b);
        s1 = s1 + dico[z] + e;
    }
    else{                                       //Pour chaque espace, on ajoute un grand espace
        s1 = s1 + g;
    }
    }
    std::cout << s1 << std::endl;

    //On a alors traduit notre phrase en morse 'écrit', s1 est une chaîne de caractères composée de points, de tirets et d'espaces (que j'ai préféré écrire avec des lettres)

    //Plusieurs fichiers audios ont utilisés : "point.wav", "tiret.wav", "silence_lettre.wav", "silence_mot.wav". Les noms me semblent suffisamment clairs pour que je n'explique pas leur contenu, mais si un doute persiste, vous pouvez les écouter

    entete_WAV enteteWAV;                                                                 //On initialise (sans toutefois encore leur donner de valeur) tout plein de choses dans les cinq prochaines lignes : une entête, des pointeurs vers les fichiers audios à charger, les tailles de l'entête et des données, et des pointeurs vers les données qui nous permettrons de créer un .WAV avec le bon son
    FILE *fTiret, *fPoint, *fSilencePointTiret, *fSilenceLettre, *fSilenceMot, *fSortie;
    int lgEntete = sizeof(entete_WAV);
    int lgFichier;
    int lgDonneesTiret, lgDonneesPoint, lgDonneesSilencePointTiret, lgSilenceLettre, lgSilenceMot;
    char *sDonneesTiret, *sDonneesPoint, *sDonneesSilencePointTiret, *sDonneesSilenceLettre, *sDonneesSilenceMot;
    lgDonneesPoint = ChargeFichierWAV((char *)"/mnt/c/users/max/c++/cpp-morse-et-latin/point.wav", &enteteWAV, &sDonneesPoint);     //Et on charge les fichiers audios
    lgSilenceLettre = ChargeFichierWAV((char *)"/mnt/c/users/max/c++/cpp-morse-et-latin/silence_lettre.wav", &enteteWAV, &sDonneesSilenceLettre);
    lgSilenceMot = ChargeFichierWAV((char *)"/mnt/c/users/max/c++/cpp-morse-et-latin/silence_mot.wav", &enteteWAV, &sDonneesSilenceMot);
    lgDonneesSilencePointTiret = ChargeFichierWAV((char *)"/mnt/c/users/max/c++/cpp-morse-et-latin/silence_point-tiret.wav", &enteteWAV, &sDonneesSilencePointTiret); //Fichier pas encore détenu
    lgDonneesTiret = ChargeFichierWAV((char *)"/mnt/c/users/max/c++/cpp-morse-et-latin/tiret.wav", &enteteWAV, &sDonneesTiret);

    fSortie = fopen("/mnt/c/users/max/c++/cpp-morse-et-latin/sortie.wav", "wb");      //On va écrire le fichier audio
    fwrite(&enteteWAV, lgEntete, 1, fSortie);
    for (std::string::size_type i = 0; i < s1.size(); i++) {        //On parcourt la phrase traduite
          switch (s1[i]) {
            case 'p':                                           //si le i° caractère est un point
              enteteWAV.dwDataLength += lgDonneesSilencePointTiret + lgDonneesPoint;   //On n'oublie pas de préciser la taille des données
              enteteWAV.ChunkSize += lgDonneesSilencePointTiret + lgDonneesPoint + 36;
              fwrite(sDonneesSilencePointTiret, lgDonneesSilencePointTiret, 1, fSortie);  //Et on remplit le fichier
              fwrite(sDonneesPoint, lgDonneesPoint, 1, fSortie);
              break;
            case 't':
              enteteWAV.dwDataLength += lgDonneesSilencePointTiret + lgDonneesTiret;   
              enteteWAV.ChunkSize += lgDonneesSilencePointTiret + lgDonneesTiret + 36;
              fwrite(sDonneesSilencePointTiret, lgDonneesSilencePointTiret, 1, fSortie);  
              fwrite(sDonneesTiret, lgDonneesTiret, 1, fSortie);
              break;
            case 'e':
              enteteWAV.dwDataLength += lgSilenceLettre;   //On a choisit une longueur de silence plus courte pour que l'on obtienne la bonne durée en concaténant avec SilencePointTiret
              enteteWAV.ChunkSize += lgSilenceLettre + 36;
              fwrite(sDonneesSilenceLettre, lgSilenceLettre, 1, fSortie);
              break;
            case 'g':
              enteteWAV.dwDataLength += lgSilenceMot;   //On a choisit une longueur de silence plus courte pour que l'on obtienne la bonne durée en concaténant avec SilencePointTiret
              enteteWAV.ChunkSize += lgSilenceMot + 36;
              fwrite(sDonneesSilenceMot, lgSilenceMot, 1, fSortie);
              break;
          }
    }
    fseek(fSortie, 0, SEEK_SET);
    fwrite(&enteteWAV, lgEntete, 1, fSortie);
    fclose(fSortie);

}

int main(){

    char text[100];
    std::cout << "Voulez vous traduire depuis le morse ou depuis le latin ? ";
    std::cin.getline(text,1000);
    if(text[0] == 'm'){
        mtol();
    }
    else{
        ltom();
    }

    return 0;
}