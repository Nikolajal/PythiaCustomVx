#!/bin/bash
#
source ~/.bashrc
g++ main_custom.cc -std=c++2b `pythia8308-config --ldflags --cxxflags` `hepmc3-config --ldflags --cppflags`
install_name_tool -add_rpath /Users/nrubini/alice/sw/osx_arm64/HepMC3/3.2.5-local2/lib/ ./a.out
./a.out
exit 1
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
g++ main41.cc -std=c++2b `pythia8308-config --ldflags --cxxflags` `hepmc3-config --ldflags --cppflags`
install_name_tool -add_rpath /Users/nrubini/alice/sw/osx_arm64/HepMC3/3.2.5-local2/lib/ ./a.out
