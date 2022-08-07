from src.carte import *

class Joueur:
    
    def __init__(self, nom, pioche):
        self.nom = nom
        self.distance = 0
        self.fois200 = 2
        self.pileBataille = 'Feu rouge'
        self.pileVitesse = False
        self.bottes = set()
        self.hand = [pioche.pop() for _ in range(6)]

    def __str__(self):
        res = f'{self.nom} ({self.distance} km, '
        for botte in self.bottes:
            res += botte + ', '
        if self.pileBataille:
            res += self.pileBataille + ', '
        if self.pileVitesse:
            res += 'Limitation de vitesse, '
        res = res[:-2] + ')'
        
        return res