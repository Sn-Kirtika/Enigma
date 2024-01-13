// Déclaration et définitions des variables :

// Pins de lecture des boutons
# define BUTTON_1 5
# define BUTTON_2 4
# define BUTTON_3 3
# define BUTTON_4 2
# define BUTTON_RESET 7

// Pin buzzer
# define BUZZER 6

// Pins LED RGB
# define LED_BLUE 9
# define LED_RED 10
# define LED_GREEN 11

// Etat initial des boutons
int previous_state_1 = 0;
int previous_state_2 = 0;
int previous_state_3 = 0;
int previous_state_4 = 0;

// Nombre saisi par l'utilisateur et compteur du nombre de chiffre dans le nombre
int input_number = 0;
int digit_number = 0;

// Nombre à deviner
int number;


// ------------------------------------------------------------------------------------------------------------------------


// Fonction setup :

void setup() {
  // Boutons configurés en INPUT_PULLUP pour activer la résistance de tirage
  pinMode(BUTTON_1, INPUT_PULLUP);
  pinMode(BUTTON_2, INPUT_PULLUP);
  pinMode(BUTTON_3, INPUT_PULLUP);
  pinMode(BUTTON_4, INPUT_PULLUP);
  pinMode(BUTTON_RESET, INPUT_PULLUP);

  // Configuration des broches pour le buzzer et la LED RGB 
  pinMode(BUZZER, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);

  // Initialisation de la communication
  Serial.begin(9600);

  // Génération d'un nombre aléatoire
  random_number();
}


// ------------------------------------------------------------------------------------------------------------------------


// Fonction loop :

void loop() {

  // Lecture des états des boutons
  int current_state_1 = digitalRead(BUTTON_1);
  int current_state_2 = digitalRead(BUTTON_2);
  int current_state_3 = digitalRead(BUTTON_3);
  int current_state_4 = digitalRead(BUTTON_4);
  int current_state_reset = digitalRead(BUTTON_RESET);

  /* Vérification si changement d'état du bouton (évite erreur en cas de pression continue sur le bouton), puis vérification 
  si plusieurs boutons enfoncées simultanément.
  Mise à jour de la valeur du nombre saisi par l'utilisateur : x10 du nombre précédent + valeur du bouton afin d'ajouter à 
  droite du nombre précédemment obtenu la valeur du dernier bouton pressé.
  Mise à jour du compteur du nombre de chiffre contenu dans le nombre saisi par l'utilisateur et de l'état du bouton */

  if (current_state_1 != previous_state_1) {
    if (current_state_1 == 1 && current_state_2 == 0 && current_state_3 == 0 && current_state_4 == 0) {
      input_number = (input_number * 10) + 1;
      digit_number++;
    }
    previous_state_1 = current_state_1;
  }
  delay(100);
  
  if (current_state_2 != previous_state_2) {
    if (current_state_2 == 1 && current_state_1 == 0 && current_state_3 == 0 && current_state_4 == 0) {
      input_number = (input_number * 10) + 2;
      digit_number++;
    }
    previous_state_2 = current_state_2;
  }
  delay(100);

  if (current_state_3 != previous_state_3) {
    if (current_state_3 == 1 && current_state_1 == 0 && current_state_2 == 0 && current_state_4 == 0) {
      input_number = (input_number * 10) + 3;
      digit_number++;
    }
    previous_state_3 = current_state_3;
  }
  delay(100);

  if (current_state_4 != previous_state_4) {
    if (current_state_4 == 1 && current_state_1 == 0 && current_state_2 == 0 && current_state_3 == 0) {
      input_number = (input_number * 10) + 4;
      digit_number++;
    }
    previous_state_4 = current_state_4;
  }

  delay(100);

  /* Affichage du nombre saisi par l'utilisateur si 4 digits saisi et vérification s'il s'agit du bon nombre
  (Facultatif - Pour debuggage) Affichage des éléments nécessaires pour jouer rapidement sur le Serial Monitor */

  if (digit_number == 4) {
    int difference = abs(number - input_number);
    Serial.println("\n Différence : " + String(difference));

    // Si nombre correct, allumage LED verte et génération du son de victoire, réinitialisation du nombre aléatoire
    
    if (input_number == number) {
      analogWrite(LED_GREEN, 255);
      playWin();
      delay(1000);
      analogWrite(LED_GREEN, 0);
      random_number();

      /* Sinon, allumage LED en fonction de la proximité du nombre saisi par l'utilisateur par rapport au nombre qui était à deviner
      (rouge, orange, jaune, vert clignotant ou vert) et génération du son de mauvaise réponse */

    } else {
      if (difference >= 1500) {
        analogWrite(LED_RED, 255);
      } else if (difference < 1500 && difference >= 1000) {
        analogWrite(LED_GREEN, 15);
        analogWrite(LED_RED, 200);
      } else if (difference < 1000 && difference >= 100) {
        analogWrite(LED_GREEN, 50);
        analogWrite(LED_RED, 200);
      } else if (difference < 100) {
        for(int i=0; i<10; i++) {
          analogWrite(LED_GREEN, 150);
          delay(50);
          analogWrite(LED_GREEN, 0);
          delay(50);
        }
      }

      playFail();
     	delay(2000);
      noTone(BUZZER);
      analogWrite(LED_RED, 0);
      analogWrite(LED_GREEN, 0);
      analogWrite(LED_BLUE, 0);
    }

    // Réinitialisation du nombre saisi et du nombre de digit 
    reset();
  }


  // Reset si bouton reset pressé et lecture du son de réinitialisation
  if (current_state_reset == 1) {
    reset();
    tone(BUZZER,10,1915);
    delay(10);
    noTone(BUZZER);
  }

}


// ------------------------------------------------------------------------------------------------------------------------


// Fonction reset - Le compteur et le nombre saisi sont réinitialisés à 0.

void reset() {
  digit_number = 0;
  input_number = 0;
}


// ------------------------------------------------------------------------------------------------------------------------


// Fonction permettant de générer un nombre aléatoire :

void random_number() {
  /* Initialisation du générateur de nombre aléatoire avec une graine (et non avec une valeur par défaut) pour générer une séquence
  de chiffre aléatoire différente à chaque exécution du programme */
  randomSeed(analogRead(0));
  number = 0;

  // Génération 4 chiffres compris avec 1 et 4 pour constituer le nombre aléatoire de 4 chiffres
  for (int i = 0; i < 4; i++) {
    int random_digit = random(1, 5);
    number = number * 10 + random_digit;
  }
  Serial.println("\n Nombre à deviner : " + String(number));
}


// ------------------------------------------------------------------------------------------------------------------------


// Fonctions de lecture des musiques en cas de victoire et défaite :

void playWin() {

  /* Listes contenant la fréquence et la durée de chaque notes constituants la mélodie (dans l'ordre)
  Parcour des listes simultanément pour générer le son grace à tone */

  int frequences[] = {494, 659, 740, 1319, 988, 2637, 1480, 1976, 1319};
  int duration[] = {126, 126, 126, 126, 126, 126, 126, 126, 126};

  for (int note = 0; note < sizeof(frequences) / sizeof(int); note++)  {    
      tone(BUZZER, frequences[note], duration[note]);    
      delay(duration[note]);    
      noTone(BUZZER);
  }
}

void playFail() {

  // Lecture du son de fréquence 1000 Hz pendant 1000 ms
 	tone(BUZZER, 1000, 1000);
}
