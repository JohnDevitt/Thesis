dlab_template('
	0-2:	[	job(c_job),
			company_type(c_company_type),
			subscription(_S), 
			course_len(_S,c_course_len),
			course_type(_S,c_course_type)
		] 
<-- 
	0-len:	[	job(c_job),
			company_type(c_company_type),
			subscription(_S), 
			course_len(_S,c_course_len),
			course_type(_S,c_course_type)
		]
').

dlab_variable(c_job, 1-1, [researcher,manager,president]).
dlab_variable(c_company_type, 1-1, [commercial,university]).
dlab_variable(c_course_len, 1-1, [2,3,4]).
dlab_variable(c_course_type, 1-1, [introductory,advanced]).

