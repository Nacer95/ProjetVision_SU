import sys
import os
import numpy as np
import matplotlib.pyplot as plt

dict_focntion = {
    'max': ["cpp_max_basic", "cpp_max_reg", "cpp_max_rot", "cpp_max_red", "cpp_max_ilu3_red", "cpp_max_elu2_red",
            "cpp_max_elu2_red_factor", "cpp_max_ilu3_elu2_red", "cpp_max_ilu3_elu2_red_factor"],
    'fusion': ["cpp_basic", "cpp_fusion", "cpp_fusion_ilu5_red", "cpp_fusion_ilu5_elu2_red",
               "cpp_fusion_ilu5_elu2_red_factor", "cpp_fusion_ilu15_red"],
    'pipeline': ["cpp_basic","cpp_pipeline_basic","cpp_pipeline_red","cpp_pipeline_ilu3_red",
                 "cpp_pipeline_elu2_red","cpp_pipeline_elu2_red_factor","cpp_pipeline_ilu3_elu2_red","cpp_pipeline_ilu3_elu2_red_factor"],
    'swp_max': ["cpp_max_basic","cpp_SWP_max_rot_uint8","cpp_SWP_max_rot_uint16","cpp_SWP_max_rot_uint32"]

}

repertoire_GRAPHIQUE = "bench_graphiques/"
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
plt.xlabel("dimension image n*n")
plt.ylabel("cycles/point")
plt.legend()

# 3.2. on sauvegarde le dessin en png
plt.savefig(repertoire_GRAPHIQUE + sys.argv[2] + ".png")
# 3.3. facultatif : affiche le dessin à l'écran (ne pas mettre avant savefig)
# plt.show()
