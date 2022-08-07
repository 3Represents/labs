from src.partie import *

RETRY = 'Veuillez reessayer.'

class Jeu:
    
    def __init__(self):
        self.pioche = (
            [Attaque('Accident')]                     * 3 +
            [Attaque('Crevaison')]                    * 3 +
            [Attaque("Panne d'essence")]              * 3 +
            [Attaque('Limitation de vitesse')]        * 4 +
            [Attaque('Feu rouge')]                    * 5 +
            [Defense('Reparations')]                  * 6 +
            [Defense('Roue de secours')]              * 6 +
            [Defense('Fin de limitation de vitesse')] * 6 +
            [Defense('Essence')]                      * 6 +
            [Defense('Feu vert')]                     * 14 +
            [
                Botte('As du volant'), Botte("Citerne d'essence"),
                Botte('Increvable')  , Botte('Vehicule prioritaire')
            ] +
            [Distance('Escargot')]                    * 10 +
            [Distance('Canard')]                      * 10 +
            [Distance('Papillon')]                    * 10 +
            [Distance('Lievre')]                      * 12 +
            [Distance('Hirondelle')]                  * 4
        )
        self.__nomJoueurs = None
        self.__partie = None

    def __nouvellePartie(self):
        """
        Creer une nouvelle partie à partir de la liste de noms de joueurs.
        """
        if self.__nomJoueurs:
            # Si l'on a fini au moins une partie
            while True:
                res = input('\nVoulez vous changer de joueur ([y]/n) ? ')
                if res == 'y':
                    changer = True
                    break
                elif res == 'n':
                    changer = False
                    break

                print(RETRY)

        if (not self.__nomJoueurs) or changer:
            # Commencer pour la première fois / changer de joueurs
            while True:
                try:
                    nJoueurs = int(input('\nCombien de joueurs (2 a 6) ? '))
                    if 2 <= nJoueurs <= 6:
                        break
                except:
                    pass

                print(RETRY)

            self.__nomJoueurs = []
            for i in range(nJoueurs):
                self.__nomJoueurs += [input(f'Veuillez entrer le nom du joueur {i+1} : ')]

        self.__partie = Partie(self.pioche, self.__nomJoueurs)
        
    def __chargerPartie(self):
        """
        Charger une partie sauvgardée dans ./data.

        Returns
        -------
        bool
            Si une partie est choisie et chargée.
        """
        try:
            path = os.path.join(os.getcwd(), 'data')
            listFiles = os.listdir(path)
            listFiles = [i for i in listFiles if i[-7:] == '.pickle']
            listFiles.sort()
            
            if not listFiles:
                print('Aucune partie sauvegardee, une nouvelle commencera.')
                return False
        except:
            # Si le dossier n'exise pas
            print('Aucune partie sauvegardee, une nouvelle commencera.')
            return False
        
        nFiles = len(listFiles)
        print()
        for i, file in enumerate(listFiles):
            print(f'{i} : {file}')
        print(f'{nFiles} : Commencer une nouvelle partie')
        
        while True:
            try:
                choix = int(input('\nLaquelle voulez vous choisir (un nombre) ? '))
                if 0 <= choix <= nFiles:
                    break
            except:
                pass
            
            print(RETRY)

        if choix == nFiles:
            return False
        else:
            try:
                path = os.path.join(os.getcwd(), 'data', listFiles[choix])
                with open(path, 'rb') as f:
                    self.__partie = pickle.load(f)
                    self.__nomJoueurs = self.__partie.nomJoueurs
                f.close()
                return True
            except:
                # Fichier corrompu
                return False

    def __initPartie(self):
        """
        Choisir d'initialiser un jeu à partir d'une liste de noms ou d'un fichier .pickle, ou de quitter.

        Returns
        -------
        bool
            True si une nouvelle partie va commencer.
        """
        res = 'n : Commencer une nouvelle partie'
        res = '\n' + '-' * len(res) + '\n' + res + '\n'
        res += 'c : Charger une partie sauvegardée\n'
        res += 'q : Quitter'
        print(res)

        while True:
            mode = input('\nQue souhaitez vous faire (n/c/q) ? ')
            if mode == 'c':
                # Charge une partie
                res = self.__chargerPartie()
                if not res:
                    self.__nouvellePartie()
                
                return True
            elif mode == 'n':
                # Nouvelle partie
                self.__nouvellePartie()
                return True
            elif mode == 'q':
                # Quitte le jeu
                return False
            else:
                print(RETRY)

    def jouer(self):
        """
        Jouer le jeu 1000 Bornes.
        """
        welcome = '| Bienvenue dans le jeu du 1000 bornes ! |'
        welcome = '+' + '-' * (len(welcome)-2) + '+\n' + welcome + '\n+' + '-' * (len(welcome)-2) + '+'
        print(welcome)

        while True:
            res = self.__initPartie()
            if not res:
                # Commande d'arrêt reçue
                break

            self.__partie.jouer()

        print('Au revoir.')