load_package(rrl).
load_package(irl).
load_package(ribc).
load_package(blocks).
		
rrl_Qlearner(ribc).

rrl_numberOfEpisodes(1000).
rrl_saveQFunctions(1000).
rrl_testFrequency(50).
rrl_numberOfTests(100).

rib_maxdiff(0.1).  % dit moet er bij voor ribc, dan snoeit hij en ist veel
rib_maxlndiff(0.105360515657826).
rib_safe_lowerbound(yes).

%rrl_action_after_end_dangerous(yes).

%%%%%%% ACE BULLSHIT %%%%%%%%%%

rmode_key(model_info(MI)).
