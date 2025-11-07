#!/bin/bash
xhost +SI:localuser:root # O l'utente con cui gira il processo finale nel container.
                         # Se usi l'entrypoint che switcha a 'contuser' (UID 1000),
                         # e il tuo utente host 'antonio' è UID 1000, allora:
                         # xhost +SI:localuser:antonio

# 2. Comando Docker Run
docker run --rm -it \
  --network="host" \
  -e DISPLAY=$DISPLAY \
  -e REDIS_TAG=0:0 \
  -e XAUTHORITY=/tmp/.Xauthority_docker \
  -v /tmp/.X11-unix:/tmp/.X11-unix \
  -v /home/antonio:/home/contuser \
  aguagliardi/legopst:latest \
#  bash -c "echo \"REDIS_TAG interno prima del profilo: [$REDIS_TAG]\"; env | grep REDIS_TAG; exec bash -l"
  bash -c "source /home/legoroot_fedora41/.profile_legoroot && exec bash -l"
