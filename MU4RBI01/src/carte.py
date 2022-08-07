from abc import ABC, abstractmethod

nom2dist = {
    'Escargot'  : 25,
    'Canard'    : 50,
    'Papillon'  : 75,
    'Lievre'    : 100,
    'Hirondelle': 200
}

att2botdef = {
    'Accident'             : ['As du volant'        , 'Reparations'],
    'Crevaison'            : ['Increvable'          , 'Roue de secours'],
    'Feu rouge'            : ['Vehicule prioritaire', 'Feu vert'],
    'Limitation de vitesse': ['Vehicule prioritaire', 'Fin de limitation de vitesse'],
    "Panne d'essence"      : ["Citerne d'essence"   , 'Essence']
}

class Carte(ABC):

    def __init__(self, nom):
        self.nom = nom

    def __str__(self):
        pass

    @abstractmethod
    def action(self, joueur):
        pass


class Attaque(Carte):

    def __init__(self, nom):
        super().__init__(nom)

    def __str__(self):
        return f'Attaque : {self.nom}'

    def action(self, joueur):
        """
        Effet qu'une carte Attaque peut avoir sur un joueur.

        Parameters
        ----------
        joueur : Joueur

        Returns
        -------
        bool
            Si l'action a été effectuée avec succès.
        """
        for botte in joueur.bottes:
            if botte in att2botdef[self.nom]:
                # S'il y a un Botte qui peut annuler cette attaque
                return False

        if (self.nom == 'Limitation de vitesse') and (not joueur.pileVitesse):
            joueur.pileVitesse = True
        elif not joueur.pileBataille:
            joueur.pileBataille = self.nom
        else:
            return False

        return True


class Distance(Carte):

    def __init__(self, nom):
        super().__init__(nom)
        self.dist = nom2dist[self.nom]

    def __str__(self):
        return f'Distance : {self.nom}, {self.dist}'

    def action(self, joueur):
        """
        Ajouter la distance à un joueur.

        Parameters
        ----------
        joueur : Joueur

        Returns
        -------
        bool
            Si l'action a été effectuée avec succès.
        """
        if joueur.pileBataille:
            # Joueur attaque
            return False
        if (self.dist > 50) and joueur.pileVitesse:
            # Vitesse de joueur limitee
            return False
        if 1000 - joueur.distance >= self.dist:
            if self.dist == 200:
                if joueur.fois200:
                    joueur.fois200 -= 1
                else:
                    return False
            joueur.distance += self.dist
            return True

        return False


class Defense(Carte):

    def __init__(self, nom):
        super().__init__(nom)

    def __str__(self):
        return f'Defense : {self.nom}'
        
    def action(self, joueur):
        """
        Défense contre une attaque.

        Parameters
        ----------
        joueur : Joueur

        Returns
        -------
        bool
            Si l'action a été effectuée avec succès.
        """
        if (self.nom == 'Fin de limitation de vitesse') and joueur.pileVitesse:
            joueur.pileVitesse = False
        elif joueur.pileBataille and (self.nom in att2botdef[joueur.pileBataille]):
            joueur.pileBataille = None
        else:
            return False

        return True


class Botte(Carte):
    
    def __init__(self, nom):
        super().__init__(nom)

    def __str__(self):
        return f'Botte : {self.nom}'
        
    def action(self, joueur):
        """
        Des actions peuvent être faites de la Botte

        Parameters
        ----------
        joueur : Joueur

        Returns
        -------
        bool
            Si l'action a été effectuée avec succès.
        """
        if self.nom in joueur.bottes:
           return False

        joueur.bottes.add(self.nom)
        if self.nom == 'Vehicule prioritaire':
            joueur.pileVitesse = False
        if joueur.pileBataille and (self.nom in att2botdef[joueur.pileBataille]):
            joueur.pileBataille = None
        return True