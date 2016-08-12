
import os

## Make sure beebs and the spreadsheet are in the right place for now, but maybe it's more suitable to
## supply these as command line args.
root_directory = "/home/john/Thesis"
beebs_directory = os.path.join(root_directory, "beebs", "src")
output_directory = os.path.join(root_directory, "output")
database_directory = os.path.join(root_directory, "database")
database_path = os.path.join(database_directory, "arm-none-eabi-gcc-4.8.2-flags.csv")

run_iterative_compilation = True
iterative_compilation_depth = 30



## ACE settings 
optimisation_settings = """

load(models).
prune_rules(false).
use_packs(ilp).

classes([two, three]).



rmode(method(+-S)).
rmode(basic_block(+-S)).
rmode(in(+-S1,+-S2)).
rmode(directed_edge(+-S1,+-S2)).
rmode(conditional_edge(+-S1,+-S2)).
rmode(method_call(+-S1,+-S2)).
rmode(return_statement(+-S1,+-S2)).


typed_language(yes).
type(method(obj)).
type(basic_block(obj)).
type(in(obj,obj)).
type(directed_edge(obj,obj)).
type(conditional_edge(obj,obj)).
type(method_call(obj,obj)).
type(return_statement(obj,obj)).



load_package(query).
log_queries([prettypacks]).

minfreq(0.5).
warmr_maxdepth(4).

warmr_assoc([horn_clauses,
assoc_min_confidence(0.4),
assoc_min_support(0.2)]).
warmr_valid_func(assoc_std).
warmr_assoc_output_options([assoc_pred]).


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
