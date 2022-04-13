import sys
import os
import numpy as np
import matplotlib.pyplot as plt
#-------------------------------------------------------------------------------------------
# ETAPE 1 : RECUPERER LE NOM DU FICHIER DE DONNEES PASSE EN ARGUMENT DE LA LIGNE DE COMMANDE
# 1.1. on verifie qu'on a le bon nombre d'arguments
if len(sys.argv) != 2:
    print("il faut un parametre en ligne de commande : le nom de l'instance")
    sys.exit()

#1.2. on recupère le nom du fichier de données
nomFic = sys.argv[1]

#---------------------------------------------
# ETAPE 2 : EXTRAIRE LES DONNEES DU FICHIER

#2.1. on vérifie que le chemin vers le fichier est correct
if not os.path.exists(nomFic):
    print("ERREUR Ouverture fichier " + nomFic)
    sys.exit()

#2.2. on stocke le contenu du fichier dans une matrice
M = np.loadtxt(nomFic)

#---------------------------------------------
# ETAPE 3 : DESSINER ET SAUVEGARDER LE DESSIN
#3.1. on dessine le nuage de points
for i in range(1, M.shape[1]):
    plt.scatter(M[:, 0], M[:, i])

 #3.2. on sauvegarde le dessin en png
plt.savefig(nomFic+".png")
#3.3. facultatif : affiche le dessin à l'écran (ne pas mettre avant savefig)
plt.show()

