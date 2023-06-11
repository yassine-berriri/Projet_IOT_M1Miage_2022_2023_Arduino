# IOTProjectMiage


## Projet de Surveillance de Piscine basé sur IoT
Nous sommes ravis de vous présenter notre projet de surveillance de piscine basé sur l'Internet des objets (IoT). Ce projet a été conçu et développé dans le cadre du programme de MIAGE, sous la supervision de M. Gilles Menez. Son objectif principal est de démontrer l'utilisation et les avantages des technologies IoT. En mettant l'accent sur les protocoles MQTT et HTTP, l'outil de flux de données Node-RED et l'intégration avec divers services web, ce projet facilite la communication entre un dispositif ESP32 et une base de données MongoDB.

## Présentation du projet
Bienvenue dans notre projet de surveillance de piscine basé sur IoT. Il a été réalisé dans le cadre d'un cours du programme MIAGE, sous la direction de M. Gilles Menez. L'objectif principal de ce projet est de démontrer l'utilisation et les avantages des technologies de l'Internet des objets (IoT). En mettant l'accent sur les protocoles MQTT et HTTP, l'outil de flux de données Node-RED et l'intégration avec divers services web, ce projet facilite la communication entre un dispositif ESP32 et une base de données MongoDB.

## Présentation du projet
Le projet se concentre sur la surveillance d'un ensemble de piscines. Les informations relatives à chaque piscine, y compris leurs températures respectives, sont publiées sur un sujet MQTT dédié. Par la suite, ces informations sont récupérées, comparées et la piscine affichant la température la plus élevée est mise en évidence en rouge sur une carte Node-RED.

De plus, notre système intègre également des données de localisation des personnes souhaitant utiliser ces piscines. À cette fin, nous avons mis en place une application appelée OwnTracks. Cette application est configurée pour envoyer des informations de localisation à un autre sujet MQTT, qui est ensuite reçu et traité par notre dispositif ESP32.

Lorsqu'un utilisateur souhaite accéder à une piscine, une demande est envoyée via une requête GET. Cette demande est ensuite transmise à notre ESP et à notre service web respectivement via une requête POST. Le service web traite la demande et l'enregistre dans notre base de données MongoDB. De plus, l'application vérifie si la distance entre la piscine et l'utilisateur est inférieure à 100 mètres, ce qui constitue une condition préalable à l'accès à la piscine par l'utilisateur.

Parallèlement, la position de l'utilisateur est mise en évidence en jaune sur notre carte Node-RED.

## Lien vers le service Web
Pour obtenir les données : https://iot-27a4.onrender.com/get
Pour insérer des données : https://iotwebservice-kspm.onrender.com/insert

## Tableau de bord d'administration
Dans le cadre de ce projet, nous avons également développé un tableau de bord d'administration. Ce tableau de bord permet de suivre toutes les entrées et sorties de la piscine.

## Lien vers le référentiel Git du tableau de bord
https://github.com/yassine-berriri/ProjetIOT_M1MIAGE_2022_2023_DashboardAdmin

## Lien vers le référentiel Git du service Web
https://github.com/yassine-berriri/Projet_IOT_M1_Miage_2022_2023

## Lancement du projet
Pour démarrer le tableau de bord, veuillez utiliser la commande http-server.

Pour démarrer le service localement, veuillez utiliser la commande node service.js.

Pour démarrer le service Arduino, vous aurez besoin d'une carte ESP32 et d'Arduino sur votre ordinateur.

## Retour d'expérience
La réalisation de ce projet a été une expérience extrêmement enrichissante, car elle nous a permis de plonger pour la première fois dans le monde passionnant de l'IoT. Nous avons acquis une compréhension approfondie de la façon dont différents composants et technologies peuvent être combinés pour créer une solution complète qui peut avoir des applications concrètes dans le monde actuel. Cela nous a également donné un aperçu de la manière dont les dispositifs IoT peuvent être utilisés pour automatiser et simplifier certains processus quotidiens.
.
