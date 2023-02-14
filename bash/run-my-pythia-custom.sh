#!/bin/bash
#
# Options
PYTHIAVERSION=8308
PYTHIASHORTV_=83
VERBOSE=FALSE
COLLISIONENERGY=7000
BEAMPARTICLEA=2212
BEAMPARTICLEB=2212
NEVENTS=10000
SEED=12345
RUNOPTION=0
FILENAME="out"
DRYRUN=FALSE
RUNCARD=""
RUNCOMMAND=""
while getopts ':V-:' OPTION; do
    case "$OPTION" in
        ## Complex options
        -)
            case "$OPTARG" in
                ## Set verbose
                verbose)
                    VERBOSE=TRUE
                    ;;
                ## Set dryrun option
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
                ## Set energy
                energy)
                    CURRENTVAL="${!OPTIND}"; OPTIND=$(( $OPTIND + 1 ))
                    COLLISIONENERGY="$CURRENTVAL"
                    ;;
                energy=*)
                    CURRENTVAL=${OPTARG#*=}
                    COLLISIONENERGY="$CURRENTVAL"
                    ;;
                ## Beam id A
                beamidA)
                    CURRENTVAL="${!OPTIND}"; OPTIND=$(( $OPTIND + 1 ))
                    BEAMPARTICLEA="$CURRENTVAL"
                    ;;
                beamidA=*)
                    CURRENTVAL=${OPTARG#*=}
                    BEAMPARTICLEA="$CURRENTVAL"
                    ;;
                ## Beam id B
                beamidB)
                    CURRENTVAL="${!OPTIND}"; OPTIND=$(( $OPTIND + 1 ))
                    BEAMPARTICLEB="$CURRENTVAL"
                    ;;
                beamidB=*)
                    CURRENTVAL=${OPTARG#*=}
                    BEAMPARTICLEB="$CURRENTVAL"
                    ;;
                ## Number of events
                nevents)
                    CURRENTVAL="${!OPTIND}"; OPTIND=$(( $OPTIND + 1 ))
                    NEVENTS="$CURRENTVAL"
                    ;;
                nevents=*)
                    CURRENTVAL=${OPTARG#*=}
                    NEVENTS="$CURRENTVAL"
                    ;;
                ## Generation seed
                seed)
                    CURRENTVAL="${!OPTIND}"; OPTIND=$(( $OPTIND + 1 ))
                    SEED="$CURRENTVAL"
                    ;;
                seed=*)
                    CURRENTVAL=${OPTARG#*=}
                    SEED="$CURRENTVAL"
                    ;;
                ## Filename output
                outfile)
                    CURRENTVAL="${!OPTIND}"; OPTIND=$(( $OPTIND + 1 ))
                    FILENAME="$CURRENTVAL"
                    ;;
                outfile=*)
                    CURRENTVAL=${OPTARG#*=}
                    FILENAME="$CURRENTVAL"
                    ;;
                ## Option
                option)
                    CURRENTVAL="${!OPTIND}"; OPTIND=$(( $OPTIND + 1 ))
                    RUNOPTION="$CURRENTVAL"
                    ;;
                option=*)
                    CURRENTVAL=${OPTARG#*=}
                    RUNOPTION="$CURRENTVAL"
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
            echo    "[HELP] Custom pythia runner, this script will run      "
            echo    "the file src/main_custom.cc with the required pythia   "
            echo -e "version. A number of possible arguments can be given:  \n"
            echo    "-V          Verbose mode                               "
            echo    "--version  Set pythia version to run                   "
            echo    "--energy   Set energy in GeV                           "
            echo    "--beamidA  Set beam A particle ID                      "
            echo    "--beamidB  Set beam B particle ID                      "
            echo    "--nevents  Set number of events to simulate            "
            echo    "--seed     Set seed for generation                     "
            echo    "--outfile  Set outputfile                              "
            echo    "--option   Set generation option                       "
            echo    "--dryrun   Only compile code                           "
            echo    "--RUNCARD      Additional runcard    (TBI)             "
            echo    "--RUNCOMMAND   Additional runcommand (TBI)             "
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
if [ ! -d "$(pwd)/Pythia_vX/pythia$PYTHIAVERSION/bin/" ]
    then
    if $DRYRUN
    then
        echo "[WARNING] Aborting, pythia not available: without proper Pythia version compilation will fail..."
        echo "[INFO] To install needed version run ./bash/build-my-pythia.sh -v $PYTHIAVERSION"
        exit 1
    else
        echo "[WARNING] Pythia not available, trying to fetch it"
        ./bash/build-my-pythia.sh -v $PYTHIAVERSION
    fi
fi
#
#   Explicit set-up
if $VERBOSE
    then
    echo    "[HELP] Custom pythia runner, this script will run      "
    echo    "[INFO] This script will try to run Pythia version $PYTHIAVERSION"
    echo    "with collisions $BEAMPARTICLEA - $BEAMPARTICLEB at $COLLISIONENERGY GeV energy"
fi
#
mkdir -p exe
g++ src/main_custom.cc -o exe/main_custom -std=c++2b `$(pwd)/Pythia_vX/pythia$PYTHIAVERSION/bin/pythia8-config --ldflags --cxxflags` `root-config --cflags --ldflags --libs --glibs --evelibs`
if $DRYRUN
then
    echo    "[INFO] Done! Dryrun completed   "
else
    ./exe/main_custom $FILENAME $NEVENTS $SEED $RUNOPTION $COLLISIONENERGY $BEAMPARTICLEA $BEAMPARTICLEB
fi
