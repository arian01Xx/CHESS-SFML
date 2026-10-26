#!/bin/bash

echo "Iniciando tareas..."
g++ -o m main.cpp -lsfml-graphics -lsfml-window -lsfml-system && ./m
echo "¡Tareas completadas!"
