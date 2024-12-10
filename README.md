# ENSEA_2A_TFTP_HILMI_HAAS

## Objectifs : Réaliser un client TFTP, à l’aide des RFC et de captures wireshark.


### Q1) Nous avons testé les trois arguments : Le programme envoi deux arguments en ligne de commande qui sont l'adresse du serveur 
et le nom du fichier à télécharger ou téléverser, comme vous pouvez le voir dans les captures ci-dessous : 

images/Q1).code.jpg

### Q2) Ensuite, nous avons fait appel à getaddrinfo afin d'obtenir l'adresse du serveur et nous avons remarqué que cela marche bien 
car nous avons bien vu l'affichage de "succes" sans erreur comme indiqué ci-dessous : 

images/Q2_result.png


### Q3) Puis, nous avons crée un socket UDP sachant que le protocole TFTP utilise UDP, car il est léger et rapide, et ensuite nous l'avons testé 
et nous avons remarqué que cela marche bien en recevant encore "success Q3", comme vous le remarquer dans les captures ci-dessous : 

images/Q3_code.png
images/Q3_result.png

### Q4) Pour gettftp :
#### a) Nous avons construit une requête en lecture (RRQ) correctement formée, et nous l'avons envoyé par la suite  au
serveur.


### b) Nous avons reçu un fichier constitué d’un seul paquet de données (DAT) et son acquittement (ACK) et nous avons regardé ensuite
