#!/bin/bash
#
# Options
PYTHIAVERSION=""
while getopts 'hv:' OPTION; do
    case "$OPTION" in
        h)
            echo "[HELP] Helper to download and install Pythia versions"
            echo "Pythia version should be given in format: 8308 for Pythia 8.308"
            exit 1
            ;;
        v)
            PYTHIAVERSION="$OPTARG"
            PYTHIASHORTV_="${PYTHIAVERSION:0:${#PYTHIAVERSION}-2}"
            echo "[INFO] Trying to download and install Pythia $PYTHIAVERSION"
            ;;
        ?)
            echo "[HELP] Helper to download and install Pythia versions"
            echo "Pythia version should be given in format: -v 8308 for Pythia 8.308"
            exit 1
            ;;
  esac
done
shift "$(($OPTIND -1))"
#
if [ -z "$PYTHIAVERSION" ]
    then
    echo "[ERROR] A version must be given with -v"
    exit 1
fi
#
mkdir -p Pythia_vX
cd Pythia_vX
#
FASTJET_ROOT="/Users/nrubini/alice/sw/osx_arm64/fastjet/latest/"
HEPMC2_ROOT="/Users/nrubini/alice/sw/osx_arm64/HepMC/latest/"
HEPMC3_ROOT="/Users/nrubini/alice/sw/osx_arm64/HepMC3/latest/"
LHAPDF_ROOT="/Users/nrubini/alice/sw/osx_arm64/lhapdf/latest/"
#
# Download pythia code
mkdir -p tars
if ! [ -f tars/pythia$PYTHIAVERSION.tar ]; then
    echo "Version $PYTHIAVERSION not cached, downloading"
    curl -sS https://pythia.org/download/pythia$PYTHIASHORTV_/pythia$PYTHIAVERSION.tar > tars/pythia$PYTHIAVERSION.tar
fi
echo "Extracting $PYTHIAVERSION"
tar -xf tars/pythia$PYTHIAVERSION.tar
echo "tar -xf tars/pythia$PYTHIAVERSION.tar"
cd pythia$PYTHIAVERSION
#
if [ $PYTHIASHORTV_ = "82" ]
    then
    ./configure ${HEPMC2_ROOT:+--with-hepmc2="$HEPMC2_ROOT"} \
                ${LHAPDF_ROOT:+--with-lhapdf6="$LHAPDF_ROOT"} \
                ${FASTJET_ROOT:+--with-fastjet3="$FASTJET_ROOT"} \
                --cxx-common="-O2 -std=c++2b -pedantic -W -Wall -Wshadow -fPIC -pthread"
fi
if [ $PYTHIASHORTV_ = "83" ]
    then
    ./configure ${HEPMC3_ROOT:+--with-hepmc3="$HEPMC3_ROOT"} \
                ${LHAPDF_ROOT:+--with-lhapdf6="$LHAPDF_ROOT"} \
                ${FASTJET_ROOT:+--with-fastjet3="$FASTJET_ROOT"} \
                --cxx-common="-O2 -std=c++2b -pedantic -W -Wall -Wshadow -fPIC -pthread"
fi
make -j40
#
# make alias zshrc
if grep -q "pythia$PYTHIAVERSION-config" ~/.zshrc
    then
    echo "[INFO] Skip making alias, as one is already in ~/.zshrc"
else
    # exit status of grep is non-zero: the pattern DOES NOT MATCH the file
    echo "[INFO] Set pythia8-config alias in ~/.zshrc: pythia$PYTHIAVERSION-config"
    echo -e "alias pythia$PYTHIAVERSION-config=\"$(pwd)/bin/pythia8-config\"\n" >> ~/.zshrc
fi
#
# make alias bashrc
if grep -q "pythia$PYTHIAVERSION-config" ~/.bashrc
    then
    echo "[INFO] Skip making alias, as one is already in ~/.bashrc"
else
    # exit status of grep is non-zero: the pattern DOES NOT MATCH the file
    echo "[INFO] Set pythia8-config alias in ~/.bashrc: pythia$PYTHIAVERSION-config"
    echo -e "alias pythia$PYTHIAVERSION-config=\"$(pwd)/bin/pythia8-config\"\n" >> ~/.bashrc
fi

