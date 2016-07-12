
output_options([c45,c45c,c45e,lp,prolog]).
%talking(4).
use_packs(ilp).
transform_queries(once).

% load_package(oo_prolog).

% load_package(cplogic).

write_predictions([testing]).

warmr_maxdepth(3).

predict(bongard(+B,-C)).

warmr_assoc([warmr_rules,warmr_rules_min_confidence(0.5),warmr_rules_min_support(0.1)]).
warmr_assoc_output_options([assoc_pred]).

rmode(5: triangle(+P,+-S)).
rmode(5: square(+P,+-S)).
rmode(5: circle(+P,+-S)).
rmode(5: in(+P,+S1,+-S2)).
rmode(5: config(+P,+S,up)).
rmode(5: config(+P,+S,down)).

typed_language(yes).
type(triangle(pic,obj)).
type(square(pic,obj)).
type(circle(pic,obj)).
type(in(pic,obj,obj)).
type(config(pic,obj,conf)).
type(bongard(pic,class)).

max_lookahead(1).
lookahead(triangle(X,Y), in(X,Y,-Z)).
%lookahead(triangle(X,Y), in(X,-Z,Y)).
lookahead(square(X,Y), in(X,Y,-Z)).
%lookahead(square(X,Y), in(X,-Z,Y)).
lookahead(circle(X,Y), in(X,Y,-Z)).
%lookahead(circle(X,Y), in(X,-Z,Y)).

lookahead(in(X,Y,Z), triangle(X,Z)).
lookahead(in(X,Y,Z), square(X,Z)).
lookahead(in(X,Y,Z), circle(X,Z)).

% report_timings(on).

% load_package(query).
% log_queries([queries, examples]).

% execute(loofl([1])).
% execute(quit).

write_predictions([testing, distribution]).

% execute(tq(all, ((bongard(X), a(X,Y)), b(X,Z)))).
% execute(tqs(all, 'tilde/bongard.logqueries')).

combination_rule([product, sum]).

% execute(nfold(bagging(tilde,2),[[]],5,5)).
% execute(quit).

% tilde_version('2.2').

% talking(4).

%query_sample_probability(0.5).

% execute(nfoldt(10)).
% execute(q).

% roulette_wheel_selection(yes).

execute(t).
execute(nfold(tilde,3)).
execute(w).
execute(q).

% execute(cplogic_read('shop.cpl')).
% execute(cplogic_query(spaghetti, [])).
% execute(cplogic_read_fo('dice.cpl')).
% execute(cplogic_query(on(8,1), [])).
% execute(quit).

