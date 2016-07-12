load(models).
prune_rules(false).
use_packs(ilp).

classes([pos,neg]).

rmode(triangle(+-S)).
rmode(square(+-S)).
rmode(circle(+-S)).
rmode(in(+S1,+-S2)).
rmode(config(+S,up)).
rmode(config(+S,down)).

typed_language(yes).
type(triangle(obj)).
type(square(obj)).
type(circle(obj)).
type(in(obj,obj)).
type(config(obj,conf)).

/*
warmode(triangle(+-obj)).
warmode(square(+-obj)).
warmode(circle(+-obj)).
warmode(in(+obj,+-obj)).
warmode(config(+obj,up)).
warmode(config(+obj,down)).
*/

load_package(query).
log_queries([prettypacks]).

minfreq(0.5).
warmr_maxdepth(4).

