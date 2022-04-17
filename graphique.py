import sys
import os
import numpy as np
import matplotlib.pyplot as plt

dict_focntion = {
    'max': ["max_basic", "max_reg", "max_rot", "max_red", "max_ilu3_red", "max_elu2_red",
            "max_elu2_red_factor", "max_ilu3_elu2_red", "max_ilu3_elu2_red_factor"],
    'fusion': ["basic", "fusion", "fusion_ilu5_red", "fusion_ilu5_elu2_red",
               "fusion_ilu5_elu2_red_factor", "fusion_ilu15_red"],
    'pipeline': ["basic","pipeline_basic","pipeline_red","pipeline_ilu3_red",
                 "pipeline_elu2_red","pipeline_elu2_red_factor","pipeline_ilu3_elu2_red","pipeline_ilu3_elu2_red_factor"],
    'swp_max': ["max_basic","SWP_max_rot_uint8","SWP_max_rot_uint16","SWP_max_rot_uint32"],
    'swp_pipeline' : ["ouverture_basic","SWP_pipeline_rot_uint8","fusion_ilu5_elu2_red_factor","pipeline_ilu3_elu2"],
    'best_ouverture' : ["min_max_elu3_ilu2_red_factor","fusion_ilu5_elu2_red_factor","pipeline_ilu3_elu2"]

}

dict_legende = {
    'cpp': ["dimension image n*n", "cycles/point"],
    's':   [" dimension image n*n", "temps en seconde"]

}

repertoire_GRAPHIQUE = "bench_graphiques/"+sys.argv[4]+"/"
# -------------------------------------------------------------------------------------------
# ETAPE 1 : RECUPERER LE NOM DU FICHIER DE DONNEES PASSE EN ARGUMENT DE LA LIGNE DE COMMANDE
# 1.1. on verifie qu'on a le bon nombre d'arguments
if len(sys.argv) < 2:
    print("il faut un parametre en ligne de commande : le nom de l'instance")
    sys.exit()

# 1.2. on recupère le nom du fichier de données
nomFic = sys.argv[1]

# ---------------------------------------------
# ETAPE 2 : EXTRAIRE LES DONNEES DU FICHIER

# 2.1. on vérifie que le chemin vers le fichier est correct
if not os.path.exists(nomFic):
    print("ERREUR Ouverture fichier " + nomFic)
    sys.exit()

# 2.2. on stocke le contenu du fichier dans une matrice
M = np.loadtxt(nomFic)

# ---------------------------------------------
# ETAPE 3 : DESSINER ET SAUVEGARDER LE DESSIN
# 3.1. on dessine le nuage de points
for i in range(1, M.shape[1]):
    plt.scatter(M[:, 0], M[:, i], s=5, label=dict_focntion[sys.argv[3]][i - 1])

plt.title(sys.argv[2])
plt.xlabel(dict_legende[sys.argv[4]][0])
plt.ylabel(dict_legende[sys.argv[4]][1])
plt.legend()

# 3.2. on sauvegarde le dessin en png
plt.savefig(repertoire_GRAPHIQUE + sys.argv[2] + ".png")
# 3.3. facultatif : affiche le dessin à l'écran (ne pas mettre avant savefig)
# plt.show()
