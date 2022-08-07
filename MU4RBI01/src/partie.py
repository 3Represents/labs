from src.joueur import *
from datetime import datetime
from random import shuffle
import os
import pickle

class Partie:

    def __init__(self, pioche, nomJoueurs):
        self.__pioche     = pioche.copy()
        shuffle(self.__pioche)
        self.nomJoueurs   = nomJoueurs
        self.__nJoueurs   = len(nomJoueurs)
        self.__joueurs    = [Joueur(nom, self.__pioche) for nom in nomJoueurs]
        self.__nActuel    = 0 # Numero du joueur actuel
        self.__nDistMax   = 0 # #(joueurs) sur laquelle plus aucune carte de distance ne peut être utilisée.
        self.__nMainVide  = 0
        self.__gameover   = False
        self.__coupFourre = False
        self.__saveQuit   = False

    def __afficherJoueurs(self):
        """
        Affichage de joueurs au debut d'une partie.
        """
        print('\n' + '-' * 9 + '\nJoueurs :')
        for j in self.__joueurs:
            print(j)

    def __piocher(self, handAct):
        """
        Pioche de carte.s.

        Parameters
        ----------
        handAct : liste de Cartes
            Main du joueur actuel.
        """
        if self.__saveQuit:
            # Revient d'une partie sauvegardée, ne pioche pas 
            self.__saveQuit = False
            return

        if self.__pioche:
            handAct += [self.__pioche.pop()]

        if self.__coupFourre:
            if self.__pioche:
                handAct += [self.__pioche.pop()]
            self.__coupFourre = False

    def __afficherOptions(self, joueurAct, nCartes):
        """
        Afficher les options disponibles d'un tour.

        Parameters
        ----------
        joueurAct : Joueur
            Joueur actuel.
        nCartes   : int
            Nombre de cartes dans la main.
        """
        for i in range(nCartes):
            print(f'{i} : {joueurAct.hand[i]}')
        print(f'{i+1} : Defausser une carte')
        print(f'{i+2} : Sauvegarder et quitter')

    def __selecteurAction(self, nCartes):
        """
        Sélecteur d'action.

        Parameters
        ----------
        nCartes   : int
            Nombre de cartes dans la main.

        Returns
        -------
        indAction : int
            Numero d'action à effectuer.
        """
        while True:
            try:
                indAction = int(input('\nQue souhaitez vous faire (un nombre) ? '))
                if 0 <= indAction <= nCartes+1:
                    break
            except:
                pass

            print('Veuillez reessayer.')

        return indAction

    def __selecteurCible(self, joueurAct):
        """
        Sélecteur du joueur à attaquer.

        Parameters
        ----------
        joueurAct : Joueur
            Joueur actuel.

        Returns
        -------
        indCible  : int
            Numero du joueur à attaquer.
        """
        options = [] # Joueurs attaquables
        for i in range(self.__nJoueurs):
            if self.__joueurs[i] is not joueurAct:
                options += [i]
                print(f'{i} : {self.__joueurs[i]}')

        while True:
            try:
                indCible = int(input('\nQui voulez vous attaquer (un nombre) ? '))
                if indCible in options:
                    break
            except:
                pass

            print('Veuillez reessayer.')

        return indCible

    def __gestionDefausse(self, joueurAct, nCartes):
        """
        Gestion de la défausse d'une carte.

        Parameters
        ----------
        joueurAct : Joueur
            Joueur actuel.
        nCartes   : int
            Nombre de cartes dans la main.

        Returns
        -------
        bool
            Si une carte est bien defausee.
        """
        while True:
            try:
                indDefausse = int(input('\nQuelle carte souhaitez vous defausser (un nombre) ? '))
                if 0 <= indDefausse < nCartes:
                    joueurAct.hand.pop(indDefausse)
                    return True
            except:
                pass

            print('Veuillez reessayer.')

    def __gestionCF(self, carteChoisie, indCible, joueurCible):
        """
        Gestion du coup-fourre.

        Parameters
        ----------
        carteChoisie : Attaque
            Carte Attaque choisie.
        indCible     : int
            Numero du joueur à attaquer.
        joueurCible  : Joueur
            Joueur à attaquer.
        """
        for c in joueurCible.hand:
            if isinstance(c, Botte) and c.nom in att2botdef[carteChoisie.nom]:
                self.__coupFourre = True
                self.__nActuel = indCible
                c.action(joueurCible)
                joueurCible.hand.remove(c)
                break

    def __updateJoueur(self):
        """
        Mise à jour de l'indice de joueur qui va jouer ensuite.
        """
        self.__nActuel += 1
        self.__nActuel %= self.__nJoueurs

    def __gestionGO(self, joueurAct):
        """
        Gerer si la partie doit être terminé.

        Parameters
        ----------
        joueurAct : Joueur
            Joueur actuel.
        """
        if joueurAct.distance == 1000:
            self.__gameover = True
        elif 1000 - joueurAct.distance < 25:
            self.__nDistMax += 1
            if self.__nDistMax == self.__nJoueurs:
                self.__gameover = True

    def __sauvegarder(self):
        """
        Gestion de la sauvegarde de la partie dans un dichier .pickle.

        Returns
        -------
        bool
            Si la partie est bien sauvgardée.
        """
        try:
            self.__saveQuit = True
            filename = datetime.now().strftime("%y%m%d_%H%M%S") + '.pickle'
            folderpath = os.path.join(os.getcwd(), 'data')
            if not os.path.exists(folderpath):
                os.makedirs(folderpath)

            filepath = os.path.join(os.getcwd(), 'data', filename)
            with open(filepath, 'wb') as f:
                pickle.dump(self, f)
            f.close()

            # Active le fanion qui n'est pas enregistré dans le fichier,
            # afin qu'il n'y ait pas besoin de le désactiver lors du rechargement du jeu.
            self.__gameover = True
            return True
        except:
            return False

    def __tour(self):
        """
        Jouer un tour.
        """
        joueurAct = self.__joueurs[self.__nActuel]
        handAct = joueurAct.hand

        if not handAct:
            self.__nMainVide += 1
            if self.__nMainVide == self.__nJoueurs:
                # Tout le monde n'a pas de cartes => pioche vide, partie terminee
                self.__gameover = True

            msg = 'Pas de cartes a jouer pour ' + str(joueurAct) + '.'
            msg = '-' * len(msg) + '\n' + msg
            print(msg)

            self.__updateJoueur()
            return
        else:
            if self.__coupFourre:
                print('\nCoup-Fourre !')

            msg = "C'est a " + str(joueurAct) + ' de jouer :'
            msg = '\n' + '-' * len(msg) + '\n' + msg
            print(msg)

        self.__piocher(handAct)
        nCartes = len(handAct)
        self.__afficherOptions(joueurAct, nCartes)

        tourOK = False
        while not tourOK:
            indAction = self.__selecteurAction(nCartes)

            if 0 <= indAction < nCartes:
                # Utilisation d'une carte
                carteChoisie = handAct[indAction]
                
                if isinstance(carteChoisie, Attaque):
                    indCible = self.__selecteurCible(joueurAct)
                    joueurCible = self.__joueurs[indCible]
                    tourOK = carteChoisie.action(joueurCible)
                    if tourOK:
                        self.__gestionCF(carteChoisie, indCible, joueurCible)
                        if not self.__coupFourre:
                            self.__updateJoueur()
                elif isinstance(carteChoisie, Distance):
                    tourOK = carteChoisie.action(joueurAct)
                    if tourOK:
                        self.__gestionGO(joueurAct)
                        self.__updateJoueur()
                elif isinstance(carteChoisie, Botte):
                    tourOK = carteChoisie.action(joueurAct)
                elif isinstance(carteChoisie, Defense):
                    tourOK = carteChoisie.action(joueurAct)
                    if tourOK:
                        self.__updateJoueur()
            elif indAction == nCartes:
                # Defausse d'une carte
                tourOK = self.__gestionDefausse(joueurAct, nCartes)
                if tourOK:
                        self.__updateJoueur()
            elif indAction == nCartes + 1:
                # Sauvegrade de la partie
                tourOK = self.__sauvegarder()

            if tourOK:
                print('Action realisee.')
                if indAction < nCartes:
                    # S'il ne s'agit pas de sauvgarde
                    handAct.pop(indAction)
            else:
                print('Action non realisee, veuillez reessayer.')

    def jouer(self):
        """
        Jouer une partie.
        """
        self.__afficherJoueurs()
        while not self.__gameover:
            self.__tour()
        
        if self.__saveQuit:
            print('Partie sauvegardee.')
        elif self.__gameover:
            res = [(i, self.__joueurs[i].distance) for i in range(self.__nJoueurs)]
            res.sort(key = lambda x: -x[1])

            msg = '-' * 20 + '\n'
            msg += str(self.__joueurs[res[0][0]]) + ' a gagne !\n'
            msg += 'Voici le classement de joueurs:'
            print(msg)
            for i in res:
                print(str(self.__joueurs[i[0]]))