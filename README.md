# IOTProjectMiage
# Projet IoT de Surveillance de Piscine

Ce projet a été proposé par Monsieur Gilles Menez dans le cadre du cursus de MIAGE. Il vise à appliquer et à démontrer la puissance des technologies de l'Internet des objets (IoT), en se focalisant spécifiquement sur les protocoles MQTT et HTTP, l'utilisation de Node-RED, et l'intégration avec des services web pour permettre la communication entre un dispositif ESP32 et une base de données MongoDB.

Le projet consiste à surveiller un ensemble de piscines, dont les informations sont publiées sur un certain topic MQTT. Les informations de chaque piscine, y compris leur température, sont ensuite comparées. Si la température de ma piscine est la plus élevée, elle est affichée en rouge sur une carte Node-RED.

De plus, ce projet est conçu pour intégrer les données de localisation des personnes qui souhaitent utiliser les piscines. Pour cela, nous utilisons l'application OwnTracks, qui a été configurée pour envoyer les données de localisation à un autre topic MQTT, qui est ensuite récupéré par notre ESP32.

Lorsqu'une personne souhaite utiliser une piscine, une demande d'ouverture peut être effectuée via une requête GET. Cette demande est alors envoyée à un service web via une requête POST, qui enregistre alors la demande dans la base de données MongoDB. En conséquence, la localisation de la personne est affichée en jaune sur la carte Node-RED.
