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
}__attribute__((packed)) entete_WAV;

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


int main(){
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
    lgDonneesPoint = ChargeFichierWAV((char *)"point.wav", &enteteWAV, &sDonneesPoint);     //Et on charge les fichiers audios
    lgSilenceLettre = ChargeFichierWAV((char *)"silence_lettre.wav", &enteteWAV, &sDonneesSilenceLettre);
    lgSilenceMot = ChargeFichierWAV((char *)"silence_mot.wav", &enteteWAV, &sDonneesSilenceMot);
    lgDonneesSilencePointTiret = ChargeFichierWAV((char *)"silence_point-tiret.wav", &enteteWAV, &sDonneesSilencePointTiret); //Fichier pas encore détenu
    lgDonneesTiret = ChargeFichierWAV((char *)"tiret.wav", &enteteWAV, &sDonneesTiret);

    fSortie = fopen("sortie.wav", "wb");      //On va écrire le fichier audio
    char s[500000] = "";
    for (std::string::size_type i = 0; i < s1.size(); i++) {        //On parcourt la phrase traduite
          switch (s1[i]) {
            case 'p':                                           //si le i° caractère est un point
              enteteWAV.dwDataLength += lgDonneesSilencePointTiret + lgDonneesPoint;   //On n'oublie pas de préciser la taille des données
              enteteWAV.ChunkSize += lgDonneesSilencePointTiret + lgDonneesPoint + 36;
              strcat(s,sDonneesSilencePointTiret);
              strcat(s,sDonneesPoint);
              //fwrite(sDonneesSilencePointTiret, lgDonneesSilencePointTiret, 1, fSortie);  //Et on remplit le fichier
              //fwrite(sDonneesPoint, lgDonneesPoint, 1, fSortie);
              std::cout << "p : " << lgDonneesSilencePointTiret << " + " << lgDonneesPoint << std::endl;
              break;
            case 't':
              enteteWAV.dwDataLength += lgDonneesSilencePointTiret + lgDonneesTiret;   
              enteteWAV.ChunkSize += lgDonneesSilencePointTiret + lgDonneesTiret + 36;
              strcat(s,sDonneesSilencePointTiret);
              strcat(s,sDonneesTiret);
              //fwrite(sDonneesSilencePointTiret, lgDonneesSilencePointTiret, 1, fSortie);  
              //fwrite(sDonneesTiret, lgDonneesTiret, 1, fSortie);
              std::cout << "t : " << lgDonneesSilencePointTiret << " + " << lgDonneesTiret << std::endl;
              break;
            case 'e':
              enteteWAV.dwDataLength += lgSilenceLettre;   //On a choisit une longueur de silence plus courte pour que l'on obtienne la bonne durée en concaténant avec SilencePointTiret
              enteteWAV.ChunkSize += lgSilenceLettre + 36;
              strcat(s,sDonneesSilenceLettre);
              //fwrite(sDonneesSilenceLettre, lgSilenceLettre, 1, fSortie);
              std::cout << "e : " << lgSilenceLettre << std::endl;  
              break;
            case 'g':
              enteteWAV.dwDataLength += lgSilenceMot;   //On a choisit une longueur de silence plus courte pour que l'on obtienne la bonne durée en concaténant avec SilencePointTiret
              enteteWAV.ChunkSize += lgSilenceMot + 36;
              strcat(s,sDonneesSilenceMot);
              //fwrite(sDonneesSilenceMot, lgSilenceMot, 1, fSortie);
              std::cout << "g : " << lgSilenceMot << std::endl;   
              break;
          }
          std::cout << enteteWAV.dwDataLength << "+" << enteteWAV.ChunkSize << std::endl; 
          fwrite(&enteteWAV, lgEntete, 1, fSortie);
          fwrite(s, enteteWAV.ChunkSize, 1, fSortie);
    }
    fclose(fSortie);


    /*
    //Plusieurs fichiers audios ont utilisés : "point.wav", "tiret.wav", "silence_lettre.wav", "silence_mot.wav". Les noms me semblent suffisamment clairs pour que je n'explique pas leur contenu, mais si un doute persiste, vous pouvez les écouter
    std::ifstream wavPoint("point.wav",std::ios_base::binary);      //On va manipuler les fichiers audios, et notamment les concaténer
    wavPoint.seekg(0, std::ios::end);                               //On commence à lire au temps 0
    std::ifstream::pos_type sizePoint = wavPoint.tellg();           //On mesure la taille du fichier
    char* bufPoint = new char[sizePoint];
    wavPoint.read(bufPoint, sizePoint);                             //On stocke le fichier audio de taille sizePoint dans la variable bufPoint
    std::ifstream wavTiret("tiret.wav",std::ios_base::binary);      //On recommence avec "tiret.wav", puis avec les autres fichiers
    wavTiret.seekg(0, std::ios::end);
    std::ifstream::pos_type sizeTiret = wavTiret.tellg();
    char* bufTiret = new char[sizeTiret];  
    wavTiret.read(bufTiret, sizeTiret);
    std::ifstream wavSilence("silence_lettre.wav",std::ios_base::binary);
    wavSilence.seekg(0, std::ios::end);
    std::ifstream::pos_type sizeSilence = wavSilence.tellg();
    char* bufSilence = new char[sizeSilence];  
    wavSilence.read(bufSilence, sizeSilence);
    std::ifstream wavgSilence("silence_mot.wav",std::ios_base::binary);
    wavgSilence.seekg(0, std::ios::end);
    std::ifstream::pos_type sizegSilence = wavgSilence.tellg();
    char* bufgSilence = new char[sizegSilence];  
    wavgSilence.read(bufgSilence, sizegSilence);
    std::ofstream wavFichierSortie("message_traduit_en_morse.wav",std::ios_base::binary);
    for (std::string::size_type i = 0; i < s1.size(); i++) {        //On parcourt la phrase traduite
          switch (s1[i]) {
            case 'p':                                           //A chaque point, on concatène le fichier "point.wav" avec le fichier audio final, et de même pour les autres 'lettres'
              wavFichierSortie.write(bufPoint, sizePoint);
              std::cout << "Point" << std::endl;
              break;
            case 't':
              wavFichierSortie.write(bufTiret, sizeTiret);
              std::cout << "Tiret" << std::endl;
              break;
            case 'e':
              wavFichierSortie.write(bufSilence, sizeSilence);
              std::cout << "Silence lettre" << std::endl;
              break;
            case 'g':
                wavFichierSortie.write(bufgSilence, sizegSilence);
                std::cout << "Silence mot" << std::endl;
              break;
          }
    } */

    std::map<std::string, std::string> dicoMorse2Char;
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


    return 0;
}