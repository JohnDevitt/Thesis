

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


