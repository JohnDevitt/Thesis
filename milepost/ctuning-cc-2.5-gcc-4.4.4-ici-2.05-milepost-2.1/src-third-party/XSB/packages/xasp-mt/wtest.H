
:- private_foreign_pred testintret(+a,-retval) from intret(a:int):int.

:- foreign_pred testintret1(+a,-retval) from intret2(a:int):int.
