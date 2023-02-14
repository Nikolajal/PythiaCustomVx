#!/bin/bash
#
# Options
PYTHIAVERSION=""
VERBOSE=FALSE
RIVETBUILD=FALSE
while getopts ':V-:' OPTION; do
    case "$OPTION" in
        ## Complex options
        -)
            case "$OPTARG" in
                ## Set verbose
                verbose)
                    VERBOSE=TRUE
                    ;;
                ## Set verbose
                dryrun)
                    DRYRUN=TRUE
                    ;;
                ## Set version
                version)
                    CURRENTVAL="${!OPTIND}"; OPTIND=$(( $OPTIND + 1 ))
                    PYTHIAVERSION="$CURRENTVAL"
                    PYTHIASHORTV_="${PYTHIAVERSION:0:${#PYTHIAVERSION}-2}"
                    ;;
                version=*)
                    CURRENTVAL=${OPTARG#*=}
                    PYTHIAVERSION="$CURRENTVAL"
                    PYTHIASHORTV_="${PYTHIAVERSION:0:${#PYTHIAVERSION}-2}"
                    ;;
                ## enable Rivet build
                Rivet)
                    CURRENTVAL="${!OPTIND}"; OPTIND=$(( $OPTIND + 1 ))
                    PYTHIAVERSION="$CURRENTVAL"
                    PYTHIASHORTV_="${PYTHIAVERSION:0:${#PYTHIAVERSION}-2}"
                    ;;
                Rivet=*)
                    CURRENTVAL=${OPTARG#*=}
                    PYTHIAVERSION="$CURRENTVAL"
                    PYTHIASHORTV_="${PYTHIAVERSION:0:${#PYTHIAVERSION}-2}"
                    ;;
                *)
                    if [ "$OPTERR" = 1 ] && [ "${optspec:0:1}" != ":" ]; then
                        echo "Unknown option --${OPTARG}" >&2
                        echo "use -h to see list of available commands. abort"
                        exit 1
                    fi
                    ;;
            esac;;
            ## Verbose
        V)
            VERBOSE=TRUE
            ;;
            ## Helper
        *)
            echo    "[HELP] Custom pythia builer, this script build locally "
            echo    "any available version of pythia version. A number of   "
            echo -e "possible arguments can be given:                       \n"
            echo    "-V          Verbose mode                               "
            echo    "--version  Set pythia version to build                 "
            echo    "--dryrun   Only compile code                           "
            echo    "--Rivet    Only compile code                           "
            exit 1
            ;;
  esac
done
shift "$(($OPTIND -1))"
#
if [ -z "$PYTHIAVERSION" ]
    then
    echo "[ERROR] A version must be given --version"
    exit 1
fi
#
mkdir -p Pythia_vX
cd Pythia_vX
#
if $RIVETBUILD
    then
    RIVET_DIR="$ALIBUILD_WORK_DIR/$(aliBuild architecture)/Rivet/latest/"
FASTJET_ROOT="$ALIBUILD_WORK_DIR/$(aliBuild architecture)/fastjet/latest/"
HEPMC2_ROOT="/$ALIBUILD_WORK_DIR/$(aliBuild architecture)/HepMC/latest/"
HEPMC3_ROOT="/$ALIBUILD_WORK_DIR/$(aliBuild architecture)/HepMC3/latest/"
LHAPDF_ROOT="/$ALIBUILD_WORK_DIR/$(aliBuild architecture)/lhapdf/latest/"
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
                --cxx-common="-O2 -std=c++17 -pedantic -W -Wall -Wshadow -fPIC -pthread"
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

