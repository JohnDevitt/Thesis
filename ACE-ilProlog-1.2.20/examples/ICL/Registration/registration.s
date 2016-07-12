
use_package(icl).
use_package(query).
load(models).

% classes([party(yes), party(no)]).
classes([party_yes, party_no]).

icl_multi(on).

significance_level(0).
talking(4).

simplify(on).

maxhead(3).
maxbody(10).
cv_sets(2).

% old(([subscription(A)],[company_type(commercial)]))
% new(([subscription(A),course_len(A,4)],[company_type(commercial)]))
% result((subscription(A),course_len(A,4),\+company_type(commercial)))
