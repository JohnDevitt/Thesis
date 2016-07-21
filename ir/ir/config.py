
## Make sure beebs and the spreadsheet are in the right place for now, but maybe it's more suitable to
## supply these as command line args.
source_directory = "/home/john/Thesis/beebs/src"
flag_database = "/home/john/Thesis/ir/ir/arm-none-eabi-gcc-4.8.2-flags.csv"


run_iterative_compilation = False
iteative_compilation_depth = 1000


training_data_directory = "/home/john/Thesis/training-data"


optimisation_settings = """

load(models).
prune_rules(false).
use_packs(ilp).

classes([one, two, three]).


use_packs(0).
resume(off).
sampling_strategy(none).

warmode(method(+-obj)).
warmode(basic_block(+-obj)).
warmode(in(+-obj, +-obj)).
warmode(directed_edge(+-obj, +-obj)).
warmode(method_call(+-obj, +-obj)).

load_package(query).
log_queries([prettypacks]).

minfreq(0.5).
warmr_maxdepth(4).
warmode(method_call(+-obj, +-obj)).


"""



binary_settings = """

load(models).
prune_rules(false).
use_packs(ilp).

classes([true, false]).


use_packs(0).
resume(off).
sampling_strategy(none).

warmode(method(+-obj)).
warmode(basic_block(+-obj)).
warmode(in(+-obj, +-obj)).
warmode(directed_edge(+-obj, +-obj)).
warmode(method_call(+-obj, +-obj)).

load_package(query).
log_queries([prettypacks]).

minfreq(0.5).
warmr_maxdepth(4).
warmode(method_call(+-obj, +-obj)).


"""

runner_script = """

#!/usr/bin/env bash
 
SCRIPT_DIR="$( cd "$( dirname "$0" )" && pwd )"
echo "$SCRIPT_DIR"
cd $SCRIPT_DIR

echo "tilde" | $ACE_ILP_ROOT/bin/ace
echo "warmr" | $ACE_ILP_ROOT/bin/ace


"""