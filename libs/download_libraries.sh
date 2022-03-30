#!/bin/sh

# Automatisch Library hinzufügen
lib_download () {
    if [ ! -d "$1" ]; then                   # Nur wenn noch nicht existiert
        wget --output-document="$1".zip "$3" # Herunterladen
        unzip "$1".zip                       # Entpacken
        rm "$1".zip                          # zip löschen
        mv "$1"-"$2" "$1"                    # Version aus Ordnernamen entfernen
    fi
}

#           | Library, Version, Download-Link
# lib_download nada_assets 1.0 https://gitlab.com/chris_nada/nada_assets/-/archive/1.0/nada_assets-1.0.zip
# lib_download rang 3.2 https://github.com/agauniyal/rang/archive/refs/tags/v3.2.zip
lib_download doctest 2.4.7 https://github.com/doctest/doctest/archive/refs/tags/2.4.7.zip
lib_download cereal 1.3.2 https://github.com/USCiLab/cereal/archive/refs/tags/v1.3.2.zip
lib_download imgui-sfml 2.5 https://github.com/eliasdaler/imgui-sfml/archive/refs/tags/v2.5.zip
lib_download implot 0.13 https://github.com/epezent/implot/archive/refs/tags/v0.13.zip
lib_download imgui 1.87 https://github.com/ocornut/imgui/archive/refs/tags/v1.87.zip
lib_download enet 1.3.17 https://github.com/lsalzman/enet/archive/refs/tags/v1.3.17.zip

cp imconfig-SFML.h imgui/imconfig.h
